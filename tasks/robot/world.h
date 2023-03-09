#pragma once

#include <exception>
#include <unordered_map>

#include "point.h"
#include "topology.h"

class World {
public:
    class IllegalMoveException : public std::exception {};

    World(const Topology& topology, Point start, Point end);

    std::unordered_map<Point, Topology::Distance> Lookup() const;

    const Point& GetStart() const;
    const Point& GetEnd() const;
    const Point& GetCurrentPosition() const;

    void Move(const Point& to);

private:
    const Topology& topology_;
    Point start_, current_, end_;
};
