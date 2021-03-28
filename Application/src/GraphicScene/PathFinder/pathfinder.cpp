#include "GraphicScene/PathFinder/pathfinder.h"
#include "GraphicScene/PathFinder/customqueue.h"
#include <limits>
#include <unordered_map>


std::vector<GraphTypes::Node> Path( const ListGraph &graph, const GraphTypes::Node &begin, const GraphTypes::Node &end )
{
    // вектор с расстояниями
    std::unordered_map< GraphTypes::Node, double > dist( graph.VerticesCount()) ;
    std::unordered_map< GraphTypes::Node, GraphTypes::Node > path( graph.VerticesCount()) ;
    CustomQueue q;
    // начало поиска
    q.push( std::make_pair( begin, 0 ) );
    dist[ begin ] = 0;

    // пока обход не выполнен
    while( ! q.empty() )
    {
        GraphTypes::Node current = q.top().first;
        q.pop();
        for( auto & next : graph.GetNextVertices( current ) )
        {
            GraphTypes::Node next_node = next.first;
            auto nextNodeDist = dist.find(next_node);
            auto currentNodeDist = dist.find(current);
            if (nextNodeDist == dist.end()) // если нода еще не посещена
            {
                dist[ next_node ] = currentNodeDist->second + next.second;
                q.push( std::make_pair( next_node, next.second ) );
            }
            else if( nextNodeDist->second > currentNodeDist->second + next.second ) // если найден путь короче
            {
                path[next_node] = current;
                dist[ next_node ] = dist[ current ] + next.second;
                // изменить приоритет соединения
                q.decrease( next_node, dist[ next_node ] );
            }
        }
    }

    auto resultPath = std::vector< GraphTypes::Node >{};
    GraphTypes::Node start = begin;
    for (; start != end;)
    {
        resultPath.push_back(start);
        auto it = path.find(start);
        if (it == path.end())
        {
            return std::vector<GraphTypes::Node>{};
        }
        start = it->second;
    }
    resultPath.push_back(start);
    return resultPath;
}
