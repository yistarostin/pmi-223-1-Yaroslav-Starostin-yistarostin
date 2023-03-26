
#pragma once
#include "filter.h"

class NegativeFilter : public Filter {
public:
    NegativeFilter();

    void operator()(Image &image) const override;

    ~NegativeFilter() noexcept override;
};
