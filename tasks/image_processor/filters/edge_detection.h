#pragma once
#include <cmath>

#include "filter.h"

class EdgeDetectionFilter : public MatrixFilter {
public:
    explicit EdgeDetectionFilter(double threshhold);

    Pixel ApplyToPixel(const Kernel &kernel) const override;

private:
    double threshhold_;
};
