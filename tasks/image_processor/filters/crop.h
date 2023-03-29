#pragma once
#include "filter.h"

class CropFilter : public Filter {
    std::size_t width_;
    std::size_t height_;

public:
    CropFilter(std::size_t width, std::size_t height);
    void operator()(Image &image) const override;
};
