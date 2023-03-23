#pragma once

#include <cmath>

#include "filter.h"

static const constexpr int32_t MASK = 5;

class SharpeningFilter : public MatrixFilter {
public:
    SharpeningFilter() {
    }

    Pixel ApplyToPixel(const Kernel &kernel) const override {  // TODO: possible overflow in r/g/b
        Color r = static_cast<Color>(std::min(
            COLOR_MAX,
            std::max(0, (-1) * static_cast<int>(kernel[ABOVE].red) + (-1) * static_cast<int>(kernel[LEFT].red) +
                            (-1) * static_cast<int>(kernel[RIGHT].red) + (-1) * static_cast<int>(kernel[UNDER].red) +
                            (MASK) * static_cast<int>(kernel[CENTER].red))));
        Color g =
            static_cast<Color>(std::min(COLOR_MAX, std::max(0, (-1) * static_cast<int>(kernel[ABOVE].green) +
                                                                   (-1) * static_cast<int>(kernel[LEFT].green) +
                                                                   (-1) * static_cast<int>(kernel[RIGHT].green) +
                                                                   (-1) * static_cast<int>(kernel[UNDER].green) +
                                                                   (MASK) * static_cast<int>(kernel[CENTER].green))));
        Color b = static_cast<Color>(std::min(
            COLOR_MAX,
            std::max(0, (-1) * static_cast<int>(kernel[ABOVE].blue) + (-1) * static_cast<int>(kernel[LEFT].blue) +
                            (-1) * static_cast<int>(kernel[RIGHT].blue) + (-1) * static_cast<int>(kernel[UNDER].blue) +
                            (MASK) * static_cast<int>(kernel[CENTER].blue))));
        return Pixel{.red = r, .green = g, .blue = b};
    }
};
