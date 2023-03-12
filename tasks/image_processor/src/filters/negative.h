
#pragma once
#include "filter.h"

class NegativeFilter : public Filter {
public:
  NegativeFilter() {}
  void operator()(Image &image) const override {
    for (auto &line : image.colors_) {
      for (auto &pixel : line) {
        auto [r, g, b] = pixel;
        pixel = {static_cast<char>(Pixel::PixelMax - r),
                 static_cast<char>(Pixel::PixelMax - g),
                 static_cast<char>(Pixel::PixelMax - b)};
      }
    }
  }
};
