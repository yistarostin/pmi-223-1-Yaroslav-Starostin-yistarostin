#pragma once

#include "topology.h"

class KnightTopology : public Topology {
public:
    explicit KnightTopology(const std::vector<std::vector<bool>>& barriers);
    std::vector<Point> GetNeighbours(const Point& point) const override;
};
