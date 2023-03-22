#pragma once
#include "filter.h"
#include <cmath>
class EdgeDetectionFilter : public MatrixFilter {
public:
  explicit EdgeDetectionFilter(std::size_t threshhold)
      : threshhold_(threshhold) {}
  Pixel ApplyToPixel(const Kernel &kernel) const override {
    Color r =
        std::min(255,
                 std::max(0, (-1) * kernel[1].red + (-1) * kernel[3].red +
                                 (-1) * kernel[5].red + (-1) * kernel[8].red +
                                 (4) + kernel[4].red));
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
    if (r > threshhold_) { // r = g = b, cuase the image is grayscaled
      return {Pixel::PixelMax, Pixel::PixelMax, Pixel::PixelMax};
    }
    return {0, 0, 0};
  }

private:
  std::size_t threshhold_;
};
