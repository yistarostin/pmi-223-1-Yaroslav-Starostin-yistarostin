#pragma once
#include "filter.h"

class GaussianBlurFilter : public Filter {
public:
  void operator()(Image &image) const override;
  GaussianBlurFilter() {}
};
