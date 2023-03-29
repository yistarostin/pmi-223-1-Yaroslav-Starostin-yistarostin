#include "grayscale.h"

GrayScaleFilter::GrayScaleFilter() {
}

void GrayScaleFilter::operator()(Image &image) const {
    for (auto &line : image.colors_) {
        for (auto &pixel : line) {
            auto &[r, g, b] = pixel;
            auto new_color = static_cast<Color>(RED * r + GREEN * g + BLUE * b);
            pixel = {new_color, new_color, new_color};
        }
    }
}

GrayScaleFilter::~GrayScaleFilter() noexcept {
}