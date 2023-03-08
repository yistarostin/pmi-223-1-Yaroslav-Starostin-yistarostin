#include "knight_topology.h"

#include <algorithm>

KnightTopology::KnightTopology(const std::vector<std::vector<bool>> &barriers) : Topology(barriers) {
}

std::vector<Point> KnightTopology::GetNeighbours(const Point &point) const {
    std::vector<Point> neighbours;
    const std::vector<Point> possible_neighbours = {{-2, -1}, {-2, 1}, {-1, 2}, {1, 2},
                                                    {2, 1},   {2, -1}, {1, -2}, {-1, -2}};
    for (const auto &p : possible_neighbours) {
        auto move = point - p;
        if (field_.IsPointValid(move)) {
            neighbours.push_back(move);
        }
    }

    return FilterNeighbours(neighbours);
}
