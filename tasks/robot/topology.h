#pragma once

#include <vector>

#include "field.h"
#include "point.h"

class Topology {
public:
    explicit Topology(const std::vector<std::vector<bool>> barriers) : field_(barriers) {
    }

    explicit Topology(const Field& field): field_(field){
        field_ = field;
    }
    using Distance = std::ptrdiff_t;
    static const constexpr Distance UNREACHABLE = -1;

    virtual std::vector<Point> GetNeighbours(const Point& point) const = 0;
    Distance MeasureDistance(const Point& from, const Point& to) const;

protected:
    Field field_;
    std::vector<Point> FilterNeighbours(const std::vector<Point>& possible_neighbours) const;
};
