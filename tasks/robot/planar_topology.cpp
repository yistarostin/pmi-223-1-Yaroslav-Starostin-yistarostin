#include "planar_topology.h"

#include <algorithm>

PlanarTopology::PlanarTopology(const Field &field) : Topology(field) {
}

std::vector<Point> PlanarTopology::GetNeighbours(const Point &point) const {
    auto width = field_.GetWidth(), height = field_.GetHeight();

    std::vector<Point> neighbours;
    const std::vector<Point> possible_neighbours = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (const auto &[dx, dy] : possible_neighbours) {
        if (point.x + dx >= 0 && point.x + dx < width && point.y + dy >= 0 && point.y + dy < height) {
            neighbours.push_back({.x = point.x + dx, .y = point.y + dy});
        }
    }

    // neighbours.erase(std::remove_if(neighbours.begin(), neighbours.end(), [&](const Point &p) { return
    // field_.IsBlocked(p); }));
    return FilterNeighbours(neighbours);
}
