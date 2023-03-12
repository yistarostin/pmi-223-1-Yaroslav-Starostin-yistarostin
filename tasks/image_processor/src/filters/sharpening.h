#pragma once
#include "filter.h"
#include <cmath>
class SharpeningFilter : public MatrixFilter {
public:
  SharpeningFilter() {}
  Pixel ApplyToPixel(const Kernel &kernel) const override {
    auto r =
        std::min(Pixel::PixelMax,
                 std::max(0, (-1) * kernel[1].red + (-1) * kernel[3].red +
                                 (-1) * kernel[5].red + (-1) * kernel[8].red +
                                 (5) + kernel[4].red));
    auto g = std::min(
        Pixel::PixelMax,
        std::max(0, (-1) * kernel[1].green + (-1) * kernel[3].green +
                        (-1) * kernel[5].green + (-1) * kernel[8].green + (5) +
                        kernel[4].green));
    auto b =
        std::min(Pixel::PixelMax,
                 std::max(0, (-1) * kernel[1].blue + (-1) * kernel[3].blue +
                                 (-1) * kernel[5].blue + (-1) * kernel[8].blue +
                                 (5) + kernel[4].blue));
    return {r, g, b};
  }
};
