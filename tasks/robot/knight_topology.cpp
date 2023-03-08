#include "knight_topology.h"

#include <algorithm>

KnightTopology::KnightTopology(const Field &f) : Topology(f) {
}

std::vector<Point> KnightTopology::GetNeighbours(const Point &point) const {
    std::vector<Point> neighbours;
    auto width = field_.GetWidth(), height = field_.GetHeight();
    const std::vector<Point> possible_neighbours = {{-2, -1}, {-2, 1}, {-1, 2}, {1, 2},
                                                    {2, 1},   {2, -1}, {1, -2}, {-1, -2}};
    for (const auto &[dx, dy] : possible_neighbours) {
        if (point.x - dx >= 0 && point.x - dx < height && point.y - dy >= 0 && point.y - dy < width) {
            neighbours.push_back({.x = point.x - dx, .y = point.y - dy});
        }
    }
    return FilterNeighbours(neighbours);
}
