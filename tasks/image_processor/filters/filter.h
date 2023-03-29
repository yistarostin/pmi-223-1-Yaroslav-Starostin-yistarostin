#pragma once

#include "../image.h"

const int32_t COLOR_MAX = 255;
const std::size_t LEFT = 3;
const std::size_t RIGHT = 5;
const std::size_t ABOVE = 1;
const std::size_t UNDER = 7;
const std::size_t CENTER = 4;

class Filter {
public:
    virtual void operator()(Image &image) const = 0;

    virtual ~Filter() noexcept;
};

class MatrixFilter : public Filter {

public:
    virtual Pixel ApplyToPixel(const Kernel &kernel) const = 0;

    void operator()(Image &image) const override;

    ~MatrixFilter() noexcept override;

    MatrixFilter() = default;
    MatrixFilter(const MatrixFilter &) = default;
    MatrixFilter(MatrixFilter &&) = default;
    MatrixFilter &operator=(const MatrixFilter &) = default;
    MatrixFilter &operator=(MatrixFilter &&) = default;
};
