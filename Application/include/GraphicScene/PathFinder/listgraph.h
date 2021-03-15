#pragma once

#include <vector>
#include <map>
#include "GraphicScene/PathFinder/graphtypes.h"

struct ListGraph
{
    ListGraph( int count );
    ~ListGraph() = default;
    // Добавление ребра от from к to.
    void AddEdge(const GraphTypes::Node &from, const GraphTypes::Node &to, const double &time = 0);

    size_t VerticesCount() const;

    std::vector< GraphTypes::Edge > GetNextVertices(const GraphTypes::Node &vertex) const;
    std::vector< GraphTypes::Edge > GetPrevVertices(const GraphTypes::Node &vertex) const;

private:
    // хранится вес и номер ноды
    std::map< GraphTypes::Node, std::vector< GraphTypes::Edge > > vert_in_;
    std::map< GraphTypes::Node, std::vector< GraphTypes::Edge > > vert_out_;
};
