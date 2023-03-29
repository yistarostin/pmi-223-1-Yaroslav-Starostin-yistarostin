#include "filter.h"

Filter::~Filter() noexcept {
}

void MatrixFilter::operator()(Image &image) const {
    Image copy = image;
    for (std::size_t y = 0; auto &row : image.colors_) {
        for (std::size_t x = 0; auto &pixel : row) {
            pixel = ApplyToPixel(copy.GetPixelWithNeighbours(y, x));
            ++x;
        }
        ++y;
    }
}

MatrixFilter::~MatrixFilter() noexcept {
}