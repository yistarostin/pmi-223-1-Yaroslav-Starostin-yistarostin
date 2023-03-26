#pragma once
#include "filter.h"

class GaussianBlurFilter : public Filter {
public:
    GaussianBlurFilter();

    void operator()(Image &image) const override;

    ~GaussianBlurFilter() noexcept override;
};
