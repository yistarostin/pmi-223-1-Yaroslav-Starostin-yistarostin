#pragma once

#include "topology.h"

class PlanarTopology : public Topology {
public:
    explicit PlanarTopology(const Field& f);
    std::vector<Point> GetNeighbours(const Point& point) const override;
};
