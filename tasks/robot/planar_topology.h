#pragma once

#include "topology.h"

class PlanarTopology : public Topology {
public:
    std::vector<Point> GetNeighbours(const Point& point) const override;
    PlanarTopology(const Field& f);
};
