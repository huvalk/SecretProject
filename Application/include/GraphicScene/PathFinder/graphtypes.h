#pragma once

#include <cstdint>
#include <utility>

namespace GraphTypes
{
using Node = uint64_t;

using Edge = std::pair< Node, double >;
}
