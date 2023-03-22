#pragma once

#include <cmath>

#include "filter.h"

static const constexpr int32_t MASK = 5;
static const constexpr int32_t COLOR_MAX = 255;

class SharpeningFilter : public MatrixFilter {
public:
    SharpeningFilter() {
    }

    Pixel ApplyToPixel(const Kernel &kernel) const override {  // TODO: possible overflow in r/g/b
        Color r = std::min(COLOR_MAX,
                           std::max(0, (-1) * kernel[ABOVE].red + (-1) * kernel[LEFT].red + (-1) * kernel[RIGHT].red +
                                           (-1) * kernel[UNDER].red + (MASK) + kernel[CENTER].red));
        Color g = std::min(
            COLOR_MAX, std::max(0, (-1) * kernel[ABOVE].green + (-1) * kernel[LEFT].green + (-1) * kernel[RIGHT].green +
                                       (-1) * kernel[UNDER].green + (MASK) + kernel[CENTER].green));
        Color b = std::min(
            COLOR_MAX, std::max(0, (-1) * kernel[ABOVE].blue + (-1) * kernel[LEFT].blue + (-1) * kernel[RIGHT].blue +
                                       (-1) * kernel[UNDER].blue + (MASK) + kernel[CENTER].blue));
        return Pixel{.red = r, .green = g, .blue = b};
    }
};
