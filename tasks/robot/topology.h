#pragma once

#include <vector>

#include "field.h"
#include "point.h"

class Topology {
public:
    using Distance = std::ptrdiff_t;
    static const constexpr Distance UNREACHABLE = -1;

    explicit Topology(const std::vector<std::vector<bool>> barriers) : field_(barriers) {
    }

    virtual std::vector<Point> GetNeighbours(const Point& point) const = 0;
    Distance MeasureDistance(const Point& from, const Point& to) const;

protected:
    Field field_;
};
