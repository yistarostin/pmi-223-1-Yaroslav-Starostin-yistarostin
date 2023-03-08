#pragma once

#include "topology.h"

class CheckersTopology : public Topology {public:

    explicit CheckersTopology(const Field& f);
    std::vector<Point> GetNeighbours(const Point& point) const override;
};
