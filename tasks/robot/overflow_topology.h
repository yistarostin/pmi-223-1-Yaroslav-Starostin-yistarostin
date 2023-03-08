#pragma once

#include "topology.h"

class OverflowTopology : public Topology {public:
    explicit OverflowTopology(const Field& f);
    std::vector<Point> GetNeighbours(const Point& point) const override;
};
