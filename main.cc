/*
 * Copyright (C) 2015, Patrick Seemann
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#include <iostream>

#include "libcolormap/colormap.h"

void
print_pgfplotsset_yellowblue()
{
    Diverging cmap(0.0, 1.0);
    cmap.setLow(1.0, 1.0, 0.0);
    cmap.setHigh(0.0, 0.0, 1.0);
    cmap.setMidpoint(0.5);

    std::cout << "\\pgfplotsset{" << std::endl;
    std::cout << "   colormap={yellowblue}{" << std::endl;
    for (std::size_t i = 0; i <= 32; ++i)
    {
        RGB rgb = cmap.colormap(i / 32.0);
        std::cout << "    rgb=(" << rgb._r << "," << rgb._g << "," << rgb._b
            << ")" << std::endl;
    }
    std::cout << "   }" << std::endl;
    std::cout << "}" << std::endl;

}

void
print_pgfplotsset_coolwarm()
{
    MSH red(80.0, 1.08, 0.5);
    MSH blue(80.0, 1.08, -1.1);

    RGB r = red.toRGB();
    RGB b = blue.toRGB();

    Diverging cmap(0.0, 1.0);
    cmap.setLow(b._r, b._g, b._b);
    cmap.setHigh(r._r, r._g, r._b);
    cmap.setMidpoint(0.5);

    std::cout << "\\pgfplotsset{" << std::endl;
    std::cout << "   colormap={coolwarm}{" << std::endl;
    for (int i = 0; i <= 32; ++i)
    {
        RGB rgb = cmap.colormap(i / 32.0);
        std::cout << "    rgb=(" << rgb._r << "," << rgb._g << "," << rgb._b
            << ")" << std::endl;
    }
    std::cout << "   }" << std::endl;
    std::cout << "}" << std::endl;

}

void
print_pgfplotsset_redblue()
{
    Diverging cmap(0.0, 1.0);
    cmap.setLow(1.0, 0.0, 0.0);
    cmap.setHigh(0.0, 0.0, 1.0);
    cmap.setMidpoint(0.5);

    std::cout << "\\pgfplotsset{" << std::endl;
    std::cout << "   colormap={redblue}{" << std::endl;
    for (std::size_t i = 0; i <= 32; ++i)
    {
        RGB rgb = cmap.colormap(i / 32.0);
        std::cout << "    rgb=(" << rgb._r << "," << rgb._g << "," << rgb._b
            << ")" << std::endl;
    }
    std::cout << "   }" << std::endl;
    std::cout << "}" << std::endl;
}

void
print_pgfplotsset_greenred()
{
    Diverging cmap(0.0, 1.0);
    cmap.setLow(0.0, 1.0, 0.0);
    cmap.setHigh(1.0, 0.0, 0.0);
    cmap.setMidpoint(0.5);

    std::cout << "\\pgfplotsset{" << std::endl;
    std::cout << "   colormap={greenred}{" << std::endl;
    for (std::size_t i = 0; i <= 32; ++i)
    {
        RGB rgb = cmap.colormap(i / 32.0);
        std::cout << "    rgb=(" << rgb._r << "," << rgb._g << "," << rgb._b
            << ")" << std::endl;
    }
    std::cout << "   }" << std::endl;
    std::cout << "}" << std::endl;

}

int
main(int argc, char** argv)
{
    /* Examples (prints the colormap as a Latex pgfplotset to use with TikZ) */
    print_pgfplotsset_yellowblue();
    print_pgfplotsset_coolwarm();
    print_pgfplotsset_redblue();
    print_pgfplotsset_greenred();

    return 0;
}
