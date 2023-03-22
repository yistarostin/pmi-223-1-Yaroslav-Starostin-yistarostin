#pragma once

#include "../image.h"

class Filter {
public:
  Filter() {}
  virtual void operator()(Image &image) const = 0;
  virtual ~Filter() {}
};

class MatrixFilter : public Filter {
public:
  MatrixFilter() {}
  virtual Pixel ApplyToPixel(const Kernel &kernel) const = 0;

  void operator()(Image &image) const override {
    for (std::size_t y = 0; auto &row : image.colors_) {
      for (std::size_t x = 0; auto &pixel : row) {
        pixel = ApplyToPixel(image.GetPixelWithNeighbours(y, x));
        ++x;
      }
      ++y;
    }
  }
  ~MatrixFilter() override {}

public:
  MatrixFilter(const MatrixFilter &) = default;
  MatrixFilter(MatrixFilter &&) = default;
  MatrixFilter &operator=(const MatrixFilter &) = default;
  MatrixFilter &operator=(MatrixFilter &&) = default;
};
