#pragma once

#include <cstdint>
#include <utility>
#include <set>
#include <map>

namespace GraphTypes
{
using Node = uint64_t;

using Edge = std::pair< Node, double >;

template <class Item>
using logicFloor = std::set<Item>;

template <class Item>
using logicBuilding = std::map<int, logicFloor<Item>>;
}
