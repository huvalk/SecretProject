#include "GraphicScene/PathFinder/pathfinder.h"
#include "GraphicScene/PathFinder/customqueue.h"
#include <limits>


std::vector<GraphTypes::Node> Path( const ListGraph &graph, const GraphTypes::Node &begin, const GraphTypes::Node &end )
{
    const double INF = std::numeric_limits< double >::max();
    // вектор с расстояниями
    std::vector< double > dist( graph.VerticesCount(), INF) ;
    std::vector< GraphTypes::Node > path( graph.VerticesCount(), -1) ;
    CustomQueue q;
    // начало поиска
    q.push( std::make_pair( begin, 0 ) );
    dist[ static_cast<size_t>(begin) ] = 0;

    // пока обход не выполнен
    while( ! q.empty() )
    {
        GraphTypes::Node current = q.top().first;
        q.pop();
        for( auto & next : graph.GetNextVertices( current ) )
        {
            GraphTypes::Node next_node = next.first;
            if ( dist[ static_cast<size_t>(next_node) ] > (INF - 1) ) // если нода еще не посещена
            {
                dist[ static_cast<size_t>(next_node) ] = dist[ static_cast<size_t>(current) ] + next.second;
                q.push( std::make_pair( next_node, next.second ) );
            }
            else if( dist[ static_cast<size_t>(next_node) ] > dist[ static_cast<size_t>(current) ] + next.second ) // если найден путь короче
            {
                path[static_cast<size_t>(next_node)] = current;
                dist[ static_cast<size_t>(next_node) ] = dist[ static_cast<size_t>(current) ] + next.second;
                // изменить приоритет соединения
                q.decrease( next_node, dist[ static_cast<size_t>(next_node) ] );
            }
        }
    }

    auto resultPath = std::vector< GraphTypes::Node >{};
    GraphTypes::Node start = begin;
    for (; start != end;)
    {
        if (start == -1)
        {
            return std::vector<GraphTypes::Node>{};
        }
        resultPath.push_back(start);
        start = path[static_cast<size_t>(start)];
    }
    resultPath.push_back(start);
    return resultPath;
}
