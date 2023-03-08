#include "checkers_topology.h"

#include <queue>
#include <unordered_set>

CheckersTopology::CheckersTopology(const Field &field) : Topology(field) {
}

std::vector<Point> CheckersTopology::GetNeighbours(const Point &point) const {
    std::vector<Point> neighbours;
    std::unordered_set<Point> been;
    std::vector<Point> simple_jumps = {{-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
    std::queue<Point> dfs;
    for (const auto &p : simple_jumps) {
        auto move = point - p;
        if (field_.IsPointValid(move) && !field_.IsBlocked(move)) {
            neighbours.push_back(move);
        }
    }
    std::unordered_set<Point> can_get;
    dfs.push(point);
    while (!dfs.empty()) {
        auto now = dfs.front();
        dfs.pop();
        can_get.insert(now);
        for (const auto &p : simple_jumps) {
            auto move = now - p;
            if (field_.IsPointValid(move) && field_.IsBlocked(move)) {
                auto jump = now - p * 2;
                if (field_.IsPointValid(jump) && !field_.IsBlocked(jump)) {
                    if (can_get.count(jump) == 0) {
                        dfs.push(jump);
                    }
                }
            }
        }
    }
    can_get.erase(point);
    neighbours.insert(neighbours.begin(), can_get.begin(), can_get.end());
    // return FilterNeighbours(neighbours); Do we need that ?..
    return neighbours;
}
