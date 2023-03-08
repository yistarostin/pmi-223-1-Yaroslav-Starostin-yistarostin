#pragma once
#include <functional>
#include <cstddef>
using Coordinate = std::ptrdiff_t;
struct Point {
    Coordinate x = 0;
    Coordinate y = 0;

    /*Point(Coordinate x, Coordinate y) : x(x), y(y) {
    }
    Point(std::initializer_list<Coordinate> coordinates): x(*coordinates.begin()), y(*(coordinates.begin() + 1)){

    }*/
    Point operator-(const Point& other) const{
        return {x - other.x, y - other.y};
    }
    Point operator*(Coordinate lambda) const{
        return Point({.x = x * lambda, .y = y * lambda});
    }
    bool operator==(const Point& other) const = default;
    bool operator!=(const Point& other) const = default;
};

namespace std{
template<>
    struct hash<Point>{
    std::size_t operator()(const Point& point) const{
        return point.x * 1337 + point.y;
    }
};
}