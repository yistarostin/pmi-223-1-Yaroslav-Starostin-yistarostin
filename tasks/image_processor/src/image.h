#pragma once
#include <array>
#include <vector>

#include "pixel.h"

const constexpr std::size_t NEIGHBOURS = 9;
using RowImage = std::vector<std::vector<Pixel>>;
using Kernel = std::array<Pixel, NEIGHBOURS>;
using Bytes = std::vector<std::byte>;

class Image {
public:
    explicit Image(const Bytes& bytes);
    Bytes ToBytes() const;
    RowImage colors_;

    Kernel GetPixelWithNeighbours(std::size_t y, std::size_t x) {  // TODO: fix bound
        return {colors_[y - 1][x - 1], colors_[y - 1][x], colors_[y - 1][x + 1],
                colors_[y][x - 1],     colors_[y][x],     colors_[y][x + 1],
                colors_[y + 1][x - 1], colors_[y + 1][x], colors_[y + 1][x + 1]};
    }
};
