/*
 * Copyright (C) 2015, Patrick Seemann
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */



/*
 * Implementation of "Diverging Color Maps for Scientific Visualization"
 * https://cfwebprod.sandia.gov/cfdocs/CompResearch/docs/ColorMapsExpanded.pdf
 */

#pragma once

#include <iostream>
#include <cmath>
#include <cassert>

/********************* Definitions *******************************************/

#define REF_X 0.95047
#define REF_Y 1.000
#define REF_Z 1.08883

struct XYZ;
struct RGB;
struct CIELAB;
struct MSH;

struct XYZ
{
    XYZ(double x, double y, double z) : _x(x), _y(y), _z(z) {}

    RGB toRGB();
    CIELAB toCIELAB();
    friend std::ostream& operator<< (std::ostream& os, XYZ const& xyz);

    double _x, _y, _z;
};

struct RGB
{
    RGB (){}
    RGB (double r, double g, double b) : _r(r), _g(g), _b(b) {}

    XYZ toXYZ();
    MSH toMSH();

    friend std::ostream& operator<< (std::ostream& os, RGB const& rgb);

    double _r, _g, _b;
};

struct CIELAB
{
    CIELAB(double l, double a, double b) : _l(l), _a(a), _b(b) {}

    XYZ toXYZ();
    MSH toMSH();

    friend std::ostream& operator<< (std::ostream& os, CIELAB const& lab);

    double _l, _a, _b;
};

struct MSH
{
    MSH() {}
    MSH(double m, double s, double h) : _m(m), _s(s), _h(h) {}

    CIELAB toCIELAB();
    RGB toRGB();

    void print()
    {
        std::cout << "m s h: " << _m << "  " << _s << " " << _h << std::endl;
    }

    double _m, _s, _h;
};

/****************** Implementations XYZ **************************************/

RGB XYZ::toRGB()
{
    double r, g, b;
    r = _x * 3.24063 + _y * -1.53721 + _z * -0.498629;
    g = _x * -0.968931 + _y * 1.87576 + _z * 0.0415175;
    b = _x * 0.0557101 + _y * -0.204021 + _z * 1.0570;

    /* perform a gamma correction specified by sRGB color space */
    if (r > 0.0031308)
        r = 1.055 * std::pow(r, (1.0/2.4)) - 0.055;
    else
        r *= 12.92;

    if (g > 0.0031308)
        g = 1.055 * std::pow(g, (1.0/2.4)) - 0.055;
    else
        g *= 12.92;

    if (b > 0.0031308)
        b = 1.055 * std::pow(b, (1.0/2.4)) - 0.055;
    else
        b *= 12.92;

    /* clamp values if they are too small or too large */
    double max = std::max(r, std::max(g, b));
    if (max > 1.0)
    {
        r /= max;
        g /= max;
        b /= max;
    }
    std::max(0.0, r);
    std::max(0.0, g);
    std::max(0.0, b);

    return RGB(r, g, b);
}

CIELAB XYZ::toCIELAB()
{
    double x, y, z;
    x = _x / REF_X;
    y = _y / REF_Y;
    z = _z / REF_Z;

    if (x > 0.008856)
        x = std::pow(x, 1.0/3.0);
    else
        x = (7.787 * x) + (16.0 / 116.0);

    if (y > 0.008856)
        y = std::pow(y, 1.0/3.0);
    else
        y = (7.787 * y) + (16.0 / 116.0);

    if (z > 0.008856)
        z = std::pow(z, 1.0/3.0);
    else
        z = (7.787 * z) + (16.0 / 116.0);

    double l, a, b;
    l = (116.0 * y) - 16.0;
    a = 500.0 * (x - y);
    b = 200.0 * (y - z);

    return CIELAB(l, a, b);
}

std::ostream&
operator<< (std::ostream& os, XYZ const& xyz)
{
    os << xyz._x << " " << xyz._y << " " << xyz._z;
    return os;
}


/****************** Implementations RGB **************************************/
XYZ RGB::toXYZ()
{
    double r, g, b;

    r = _r;
    g = _g;
    b = _b;

    if (r > 0.04045)
        r = std::pow((r + 0.055) / 1.055, 2.4);
    else
        r /= 12.92;

    if (g > 0.04045)
        g = std::pow((g + 0.055) / 1.055, 2.4);
    else
        g /= 12.92;

    if (b > 0.04045)
        b = std::pow((b + 0.055) / 1.055, 2.4);
    else
        b /= 12.92;

    /* Observer = 2 deg, Illuminant = D65 */
    double x, y, z;
    x = r * 0.4124 + g * 0.3576 + b * 0.1805;
    y = r * 0.2126 + g * 0.7152 + b * 0.0722;
    z = r * 0.0193 + g * 0.1192 + b * 0.9505;

    return XYZ(x, y, z);
}

MSH RGB::toMSH()
{
    return (this)->toXYZ().toCIELAB().toMSH();
}

std::ostream&
operator<< (std::ostream& os, RGB const& rgb)
{
    os << rgb._r << " " << rgb._g << " " << rgb._b;
    return os;
}


/****************** Implementations CIELAB ***********************************/
XYZ CIELAB::toXYZ()
{
    double x, y, z;
    y = (_l + 16.0) / 116.0;
    x = _a / 500.0 + y;
    z = y - _b / 200.0;

    if (std::pow(y, 3.0) > 0.008856)
        y = std::pow(y, 3.0);
    else
        y = (y - 16.0 / 116.0) / 7.787;

    if (std::pow(x, 3.0) > 0.008856)
        x = std::pow(x, 3.0);
    else
        x = (x - 16.0 / 116.0) / 7.787;

    if (std::pow(z, 3.0) > 0.008856)
        z = std::pow(z, 3.0);
    else
        z = (z - 16.0 / 116.0) / 7.787;

    x *= REF_X;
    y *= REF_Y;
    z *= REF_Z;

    return XYZ(x, y, z);
}

MSH CIELAB::toMSH()
{
    double m, s, h;
    s = h = 0.0;
    m = std::sqrt(_l*_l + _a*_a + _b*_b);
    if (m > 0.001)
        s = std::acos(_l/m);
    if (s > 0.001)
        h = std::atan2(_b, _a);

    return MSH(m, s, h);
}

std::ostream&
operator<< (std::ostream& os, CIELAB const& lab)
{
    os << lab._l << " " << lab._a << " " << lab._b;
    return os;
}


/****************** Implementations MSH ***********************************/

CIELAB MSH::toCIELAB()
{
    double l, a, b;

    l = _m * std::cos(_s);
    a = _m * std::sin(_s) * std::cos(_h);
    b = _m * std::sin(_s) * std::sin(_h);

    return CIELAB(l, a, b);
}

RGB MSH::toRGB()
{
    return (this)->toCIELAB().toXYZ().toRGB();
}


/*****************************************************************************/

/************************ Helper Functions ***********************************/

/* Provides an adjusted hue when interpolating to an unsaturated
 * color in msh space
 */
double
adjustHue(MSH const& msh, double unsaturatdM)
{
    if (msh._m >= unsaturatdM)
        return msh._h;
    else
    {
        double hSpin = (msh._s * std::sqrt(unsaturatdM*unsaturatdM
            - msh._m * msh._m)) / (msh._m * std::sin(msh._s));
        if (msh._h > -M_PI/3.0)
            return msh._h + hSpin;
        else
            return msh._h - hSpin;
    }
}

/*****************************************************************************/


/*
 * struct for creating diverging color maps
 */
struct Diverging
{
    /* Constructs a diverging colormap given the min and max of the data
       to be mapped. The midpoint of the colormap is initialized to half
       of the given data range. */
    Diverging(double min, double max)
        : _min(min), _max(max), _midPoint((max + min) / 2.0)
    {
    }

    /* RGB color of the low end of the colormap */
    void
    setLow(double r, double g, double b)
    {
        RGB rgb(r, g, b);
        _low = rgb.toMSH();
    }

    /* RGB color of the high end of the colormap */
    void
    setHigh(double r, double g, double b)
    {
        RGB rgb(r, g, b);
        _high = rgb.toMSH();
    }

    /* Midpoint of the colormap (i.e. midpoint of your data) */
    void
    setMidpoint(double midpoint)
    {
        _midPoint = midpoint;
    }


    /*
     * Maps the given value using the colormap.
     */
    RGB
    colormap (double value)
    {
        MSH low = _low;
        MSH high = _high;
        // TODO: angle diff
        if (low._s > 0.05 && high._s > 0.05)
        {
            double mMid = std::max(88.0, std::max(_low._m, _high._m));
            if (value < _midPoint)
            {
                high._m = mMid;
                high._s = 0.0;
                high._h = 0.0;
            }
            else
            {
                low._m = mMid;
                low._s = 0.0;
                low._h = 0.0;
            }
        }

        /* adjust hue of unsaturated colors */
        if (low._s < 0.05 && high._s > 0.05)
            low._h = adjustHue(high, low._m);
        else if (high._s < 0.05 && low._s > 0.05)
            high._h = adjustHue(low, high._m);

        if (value < _midPoint)
            value = (value - _min) / (_midPoint - _min);
        else
            value = (value - _midPoint) / (_max - _midPoint);

        value = std::max(0.0, std::min(1.0, value));

        /* linear interpolation on adjusted control points */
        MSH result;
        result._m = value * high._m + (1.0 - value) * low._m;
        result._s = value * high._s + (1.0 - value) * low._s;
        result._h = value * high._h + (1.0 - value) * low._h;

        return result.toRGB();
    }

    MSH _low;
    MSH _high;
    double _min, _max, _midPoint;
};
