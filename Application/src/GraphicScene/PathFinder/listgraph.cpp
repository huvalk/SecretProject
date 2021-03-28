#include "GraphicScene/PathFinder/listgraph.h"

ListGraph::ListGraph( )
{
}

void ListGraph::AddEdge(const GraphTypes::Node &from, const GraphTypes::Node &to, const double &time )
{
    if (time < 0)
    {
        return;
    }

    vert_in_[ to ].push_back( std::make_pair( from, time ) );
    vert_out_[ from ].push_back( std::make_pair( to, time ) );
}

size_t ListGraph::VerticesCount() const
{
    return vert_in_.size();
}

std::vector< GraphTypes::Edge > ListGraph::GetNextVertices( const GraphTypes::Node &vertex ) const
{
    auto next = vert_out_.find(vertex);

    if (next != vert_out_.end())
    {
        return next->second;
    } else
    {
        return std::vector< GraphTypes::Edge >( 0 );
    }
}

std::vector< GraphTypes::Edge > ListGraph::GetPrevVertices( const GraphTypes::Node &vertex ) const
{
    auto prev = vert_in_.find(vertex);

    if (prev != vert_in_.end())
    {
        return prev->second;
    } else
    {
        return std::vector< GraphTypes::Edge >( 0 );
    }
}
