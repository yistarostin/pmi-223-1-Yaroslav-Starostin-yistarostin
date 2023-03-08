#pragma once

#include "topology.h"

class KnightTopology : public Topology {
public:
    explicit KnightTopology(const Field& f);
    std::vector<Point> GetNeighbours(const Point& point) const override;
};
