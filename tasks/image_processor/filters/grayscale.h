
#pragma once
#include "filter.h"

class GrayScaleFilter : public Filter {
    const double RED = 0.299;
    const double GREEN = 0.587;
    const double BLUE = 0.114;

public:
    GrayScaleFilter();
    void operator()(Image &image) const override;
    ~GrayScaleFilter() noexcept override;
};
