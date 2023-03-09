#include "planar_topology.h"

#include <algorithm>

PlanarTopology::PlanarTopology(const std::vector<std::vector<bool>> &barriers) : Topology(barriers) {
}

std::vector<Point> PlanarTopology::GetNeighbours(const Point &point) const {
    std::vector<Point> neighbours;
    const std::vector<Point> possible_neighbours = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (const auto &p : possible_neighbours) {
        auto move = point - p;
        if (field_.IsPointValid(move) && !field_.IsBlocked(move)) {
            neighbours.push_back(move);
        }
    }
    return neighbours;
}
