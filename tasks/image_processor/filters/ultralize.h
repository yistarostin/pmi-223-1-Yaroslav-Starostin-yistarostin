#pragma once
#include <cmath>

#include "filter.h"

class UltralizeFilter : public Filter {
public:
    UltralizeFilter() {
    }

    void operator()(Image &image) const override {
        for (auto &line : image.colors_) {
            for (auto &pixel : line) {
                auto &[r, g, b] = pixel;
                auto maximum_color = std::max(std::max(r, g), b);
                if (maximum_color == r) {
                    pixel = {255, 0, 0};
                } else if (maximum_color == g) {
                    pixel = {0, 255, 0};
                } else {
                    pixel = {0, 0, 255};
                }
            }
        }
    }

    ~UltralizeFilter() noexcept {
    }
};
