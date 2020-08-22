#pragma once
#include <map>
#include <memory>
#include <set>

namespace GraphicTypes
{
enum class GraphicItems {
    Point,
    Line,
    Image
};

template <class Item>
using floor = std::map<int, std::set<std::shared_ptr<Item>>>;
}
