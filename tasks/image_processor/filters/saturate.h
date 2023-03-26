#pragma once
#include <cmath>

#include "filter.h"

struct HsvColor {
    Color h;
    Color s;
    Color v;
};

Pixel HsvToRgb(HsvColor hsv);
HsvColor RgbToHsv(Pixel rgb);

class SaturateFilter : public Filter {
public:
    bool add;
    std::size_t saturation_delta;
    SaturateFilter(bool plus, std::size_t delta);

    void operator()(Image &image) const override;
    ~SaturateFilter() noexcept override;
};
