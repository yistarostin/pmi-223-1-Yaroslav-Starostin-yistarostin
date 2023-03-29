#include "negative.h"

NegativeFilter::NegativeFilter() {
}

void NegativeFilter::operator()(Image &image) const {
    for (auto &line : image.colors_) {
        for (auto &pixel : line) {
            auto [r, g, b] = pixel;
            pixel = {static_cast<Color>(Pixel::PixelMax - r), static_cast<Color>(Pixel::PixelMax - g),
                     static_cast<Color>(Pixel::PixelMax - b)};
        }
    }
}

NegativeFilter::~NegativeFilter() noexcept {
}
