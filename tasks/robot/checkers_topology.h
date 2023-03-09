#pragma once

#include "topology.h"

class CheckersTopology : public Topology {
public:
    explicit CheckersTopology(const std::vector<std::vector<bool>>& barriers);
    std::vector<Point> GetNeighbours(const Point& point) const override;
};
