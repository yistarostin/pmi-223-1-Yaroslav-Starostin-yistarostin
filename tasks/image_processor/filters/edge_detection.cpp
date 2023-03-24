#include "edge_detection.h"

EdgeDetectionFilter::EdgeDetectionFilter(double threshhold) : threshhold_(threshhold) {
}

Pixel EdgeDetectionFilter::ApplyToPixel(const Kernel &kernel) const {
    Color r = std::min(
        COLOR_MAX,
        std::max(0, (-1) * static_cast<int>(kernel[ABOVE].red) + (-1) * static_cast<int>(kernel[LEFT].red) +
                        (-1) * static_cast<int>(kernel[RIGHT].red) + (-1) * static_cast<int>(kernel[UNDER].red) +
                        (4) * static_cast<int>(kernel[CENTER].red)));
    if (r * 1.0 / static_cast<double>(Pixel::PixelMax) > threshhold_) {
        return {Pixel::PixelMax, Pixel::PixelMax, Pixel::PixelMax};
    }
    return {0, 0, 0};
}