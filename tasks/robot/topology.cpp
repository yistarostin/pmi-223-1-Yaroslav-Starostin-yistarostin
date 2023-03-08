#include "topology.h"
#include <queue>
#include <unordered_map>
auto Topology::MeasureDistance(const Point& from, const Point& to) const -> Distance {
    std::queue<Point> dfs;
    std::unordered_map<Point, Distance> distances;
    dfs.push(from);
    distances[from] = 0;
    while(!dfs.empty()){
        auto curr = dfs.front();
        if(curr == to){
            break;
        }
        auto distances_to_curr = distances[curr];
        dfs.pop();
        auto neightbours{GetNeighbours(curr)};
        for(const auto& next : neightbours){
            if(distances.count(next) == 0){
                distances[next] = distances_to_curr + 1;
                dfs.push(next);
            }
        }
    }
    auto distance = distances.find(to);
    if(distance == distances.end()){
        return UNREACHABLE;
    }
    return distance->second;
}
std::vector<Point> Topology::FilterNeighbours(const std::vector<Point>& possible_neighbours) const {
    std::vector<Point> available_neighbours;
    for(const auto& p : possible_neighbours){
        auto blocked = field_.IsBlocked(p);
        if(!blocked){
            available_neighbours.push_back(p);
        }
    }
    return available_neighbours;
}
