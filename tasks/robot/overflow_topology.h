#pragma once

#include "topology.h"

class OverflowTopology : public Topology {
public:
    explicit OverflowTopology(const std::vector<std::vector<bool>>& barriers);
    std::vector<Point> GetNeighbours(const Point& point) const override;
};
