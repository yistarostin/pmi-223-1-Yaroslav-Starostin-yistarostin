#include "crop.h"

CropFilter::CropFilter(std::size_t width, std::size_t height) : width_(width), height_(height) {
}

void CropFilter::operator()(Image &image) const {
    auto new_height = std::min(height_, image.Height());
    auto new_width = std::min(width_, image.Width());
    image.colors_.erase(image.colors_.begin() + static_cast<std::ptrdiff_t>(new_height), image.colors_.end());
    for (auto &line : image.colors_) {
        line.erase(line.begin() + static_cast<std::ptrdiff_t>(new_width), line.end());
    }
}