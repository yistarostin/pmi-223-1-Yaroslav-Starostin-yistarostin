#pragma once

#include <array>
#include <vector>

#include "bmp_chunks.h"
#include "pixel.h"

const constexpr std::size_t NEIGHBOURS = 9;
using RowImage = std::vector<std::vector<Pixel>>;
using Kernel = std::array<Pixel, NEIGHBOURS>;
using Bytes = std::vector<std::byte>;

class Image {
public:
    RowImage colors_;
    BitMapHeader bitMapHeader_;
    InfoHeader infoHeader_;

    Kernel GetPixelWithNeighbours(std::size_t y, std::size_t x) {
        auto under = y == colors_.size() - 1 ? colors_[y][x] : colors_[y + 1][x];
        auto above = y == 0 ? colors_[y][x] : colors_[y - 1][x];
        auto right = x == colors_.back().size() - 1 ? colors_[y][x] : colors_[y][x + 1];
        auto left = x == 0 ? colors_[y][x] : colors_[y][x - 1];
        auto current = colors_[y][x];
        auto empty = Pixel{0, 0, 0};
        return {empty, above, empty, left, current, right, empty, under, empty};
    }

    std::size_t Width() const {
        return colors_.back().size();
    }

    std::size_t Height() const {
        return colors_.size();
    }
};
