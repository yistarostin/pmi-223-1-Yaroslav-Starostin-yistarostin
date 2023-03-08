
#include "overflow_topology.h"

#include <algorithm>

OverflowTopology::OverflowTopology(const std::vector<std::vector<bool>> &barriers) : Topology(barriers) {
}

std::vector<Point> OverflowTopology::GetNeighbours(const Point &point) const {
    Distance width = field_.GetWidth();
    Distance height = field_.GetHeight();
    std::vector<Point> neighbours;
    const std::vector<Point> possible_neighbours = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (const auto &[dx, dy] : possible_neighbours) {
        auto move = Point{.x = ((point.x + width - dx) % width), .y = ((point.y + height - dy) % height)};
        if (!field_.IsBlocked(move)) {
            neighbours.push_back(move);
        }
    }

    return neighbours;
}
