#include "world.h"

World::World(const Topology& topology, Point start, Point end)
    : topology_(topology), start_(start), current_(start_), end_(end) {
}

const Point& World::GetCurrentPosition() const {
    return current_;
}

const Point& World::GetStart() const {
    return start_;
}

const Point& World::GetEnd() const {
    return end_;
}

std::unordered_map<Point, Topology::Distance> World::Lookup() const {
    std::unordered_map<Point, Topology::Distance> ways;
    auto neighbours = topology_.GetNeighbours(current_);
    for (auto& point : neighbours) {
        ways.insert({point, topology_.MeasureDistance(point, end_)});
    }
    return ways;
}

void World::Move(const Point& to) {
    auto neighbours = topology_.GetNeighbours(current_);
    if (std::find(neighbours.begin(), neighbours.end(), to) == neighbours.end()) {
        throw World::IllegalMoveException();
    }
    current_ = to;
}
