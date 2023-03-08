#pragma once
#include <vector>

#include "point.h"

class Field {
public:
    explicit Field(const std::vector<std::vector<bool>>& field)
        : barriers_(field), width_(barriers_.back().size()), height_(barriers_.size()) {
    }

    bool IsBlocked(const Point& p) const {
        return barriers_[p.y][p.x];
    }

    std::ptrdiff_t GetHeight() const {
        return height_;
    }

    std::ptrdiff_t GetWidth() const {
        return width_;
    }

    bool IsPointValid(const Point& point) const {
        return point.x >= 0 && point.x < width_ && point.y >= 0 && point.y < height_;
    }

private:
    std::vector<std::vector<bool>> barriers_;
    std::ptrdiff_t width_;
    std::ptrdiff_t height_;
};