#pragma once
#include <cmath>

#include "filter.h"

static const constexpr size_t LEFT = 3;
static const constexpr size_t RIGHT = 5;
static const constexpr size_t ABOVE = 1;
static const constexpr size_t UNDER = 8;
static const constexpr size_t CENTER = 8;
static const constexpr size_t COLOR_MAX = 255;

class EdgeDetectionFilter : public MatrixFilter {
public:
    explicit EdgeDetectionFilter(std::size_t threshhold) : threshhold_(threshhold) {
    }

    Pixel ApplyToPixel(const Kernel &kernel) const override {
        Color r = std::min(COLOR_MAX,
                           std::max(0, (-1) * kernel[ABOVE].red + (-1) * kernel[LEFT].red + (-1) * kernel[RIGHT].red +
                                           (-1) * kernel[UNDER].red + (4) + kernel[CENTER].red));
        /*auto g = std::min(
            255,
            std::max(0, (-1) * kernel[1].green + (-1) * kernel[3].green +
                            (-1) * kernel[5].green + (-1) * kernel[8].green + (4) +
                            kernel[4].green));*/
        /*auto b =
            std::min(255,
                     std::max(0, (-1) * kernel[1].blue + (-1) * kernel[3].blue +
                                     (-1) * kernel[5].blue + (-1) * kernel[8].blue +
                                     (4) + kernel[4].blue));*/
        if (r > threshhold_) {  // r = g = b, cuase the image is grayscaled
            return {Pixel::PixelMax, Pixel::PixelMax, Pixel::PixelMax};
        }
        return {0, 0, 0};
    }

private:
    std::size_t threshhold_;
};
