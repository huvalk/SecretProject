#include "GraphicScene/PathFinder/listgraph.h"
#include <QDebug>

ListGraph::ListGraph( )
{
}

void ListGraph::AddEdge(const GraphTypes::Node &from, const GraphTypes::Node &to, const double &time )
{
    if (time < 0)
    {
        return;
    }

    vert_out_[ to ].push_back( std::make_pair( from, time ) );
    vert_out_[ from ].push_back( std::make_pair( to, time ) );
}

size_t ListGraph::VerticesCount() const
{
    qWarning() << "Not valid";
    return vert_out_.size();
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
