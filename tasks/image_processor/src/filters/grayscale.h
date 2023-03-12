
#pragma once
#include "filter.h"
const constexpr double RED = 0.299;
const constexpr double GREEN = 0.587;
const constexpr double BLUE = 0.114;

class GrayScaleFilter : public Filter {
public:
  GrayScaleFilter() {}
  void operator()(Image &image) const override {
    for (auto &line : image.colors_) {
      for (auto &pixel : line) {
        auto &[r, g, b] = pixel;
        auto new_color = static_cast<Color>(RED * r + GREEN * g + BLUE * b);
        pixel = {new_color, new_color, new_color};
      }
    }
  }
};
