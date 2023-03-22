#pragma once
#include "filter.h"

class GaussianBlurFilter : public Filter {
public:
  GaussianBlurFilter() {}
  void operator()(Image &image) const override{
      ; //TODO implement gau
  }
  ~GaussianBlurFilter() override{};
};
