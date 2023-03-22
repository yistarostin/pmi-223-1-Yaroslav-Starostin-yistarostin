#pragma once
#include <stdint.h>

#include <limits>
using Color = uint8_t;

struct Pixel {
    static const constexpr Color PixelMax = std::numeric_limits<Color>::max();
    Color red;
    Color green;
    Color blue;
};
