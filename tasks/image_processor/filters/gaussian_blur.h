#pragma once
#include "filter.h"

class GaussianBlurFilter : public Filter {
public:
    explicit GaussianBlurFilter(double sigma);

    void operator()(Image &image) const override;

    ~GaussianBlurFilter() noexcept override;
    double sigma_;
};
