#pragma once

#include "topology.h"

class PlanarTopology : public Topology {
public:
    explicit PlanarTopology(const std::vector<std::vector<bool>>& barriers_);
    std::vector<Point> GetNeighbours(const Point& point) const override;
};
