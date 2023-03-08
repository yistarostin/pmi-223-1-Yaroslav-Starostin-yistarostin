#include "robot.h"
#include <unordered_map>
// #include <queue>
using Distance = Topology::Distance;
namespace robot{
    Path FindPath(World &world){
        Path path;
        //std::unordered_map<Point, Point> from;
        //std::unordered_map<Point, Distance> distance;
        auto start = world.GetStart();
        auto target = world.GetEnd();
        /*std::queue<Point> dfs;
        while(!dfs.empty()){
            Point now = dfs.front();
            dfs.pop();
            if(now == target){
                break;
            }
            auto distance_to_now = distance[now];
            world.Move(now);
            for(auto& [next, dist] : world.Lookup()){
                //TODO: push neighbours
            }
        }*/
        auto now = start;
        while(now != target){
            path.push_back(now);
            auto can_go = world.Lookup();
            auto best_move = std::min_element(can_go.begin(), can_go.end(), [](const std::pair<Point, Distance>& p1, const std::pair<Point, Distance>& p2){return p1.second < p2.second;});
            if(best_move->second == Topology::UNREACHABLE){
                return {};
            }
            world.Move(best_move->first);
            now = world.GetCurrentPosition();
        }
        if(now == target){
            path.push_back(now);
        }
        return path;
    }
}
