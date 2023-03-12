#pragma once
#include "filter.h"

class CropFilter : public Filter {
  std::size_t width_;
  std::size_t height_;

public:
  CropFilter(std::size_t width, std::size_t height)
      : width_(width), height_(height) {}
  void operator()(Image &image) const override {
    image.colors_.erase(image.colors_.begin() +
                            static_cast<std::ptrdiff_t>(height_),
                        image.colors_.end());
    for (auto &line : image.colors_) {
      line.erase(line.begin() + static_cast<std::ptrdiff_t>(width_),
                 line.end());
    }
  }
};
