
#pragma once
#include "filter.h"

static const constexpr double RED = 0.299;
static const constexpr double GREEN = 0.587;
static const constexpr double BLUE = 0.114;

class GrayScaleFilter : public Filter {

public:
    GrayScaleFilter();
    void operator()(Image &image) const override;
    ~GrayScaleFilter() noexcept override;
};
