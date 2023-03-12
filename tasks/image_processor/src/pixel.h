#pragma once
#include <limits>
using Color = unsigned char;

struct Pixel {
    static const constexpr Color PixelMax = std::numeric_limits<Color>::max();
    Color red;
    Color green;
    Color blue;
};
