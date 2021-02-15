#pragma once
#include <map>
#include <memory>
#include <set>

namespace GraphicTypes
{
enum class GraphicItems {
    Point,
    Line,
    Image,
    Polygon
};

template <class Item>
using floor = std::set<std::shared_ptr<Item>>;

template <class Item>
using building = std::map<int, floor<Item>>;
}
