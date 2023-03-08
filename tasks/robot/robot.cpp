#include "robot.h"

#include <unordered_map>
using Distance = Topology::Distance;

namespace robot {
Path FindPath(World& world) {
    Path path;
    auto start = world.GetStart();
    auto target = world.GetEnd();
    auto now = start;
    while (now != target) {
        path.push_back(now);
        auto can_go = world.Lookup();
        auto best_move = std::min_element(can_go.begin(), can_go.end(),
                                          [](const std::pair<Point, Distance>& p1,
                                             const std::pair<Point, Distance>& p2) { return p1.second < p2.second; });
        if (best_move->second == Topology::UNREACHABLE) {
            return {};
        }
        world.Move(best_move->first);
        now = world.GetCurrentPosition();
    }
    if (now == target) {
        path.push_back(now);
    }
    return path;
}
}  // namespace robot
