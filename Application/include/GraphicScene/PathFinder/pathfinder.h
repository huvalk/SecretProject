#pragma once

#include <GraphicScene/PathFinder/listgraph.h>

std::vector<GraphTypes::Node> Path( const ListGraph &graph, const GraphTypes::Node &begin, const GraphTypes::Node &end );
