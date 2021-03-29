#include "GraphicScene/PathFinder/pathfinder.h"
#include "GraphicScene/PathFinder/customqueue.h"
#include <limits>
#include <unordered_map>


std::vector<GraphTypes::Node> Path( const ListGraph &graph, const GraphTypes::Node &begin, const GraphTypes::Node &end )
{
    // TODO A*
    // вектор с расстояниями
    std::unordered_map< GraphTypes::Node, double > dist( graph.VerticesCount()) ;
    std::unordered_map< GraphTypes::Node, GraphTypes::Node > path( graph.VerticesCount()) ;
    CustomQueue q;
    // начало поиска
    q.push( std::make_pair( begin, 0 ) );
    path[ begin ] = begin;
    dist[ begin ] = 0;

    // пока обход не выполнен
    while( ! q.empty() )
    {
        GraphTypes::Node current = q.top().first;
        if (current == end)
        {
            break;
        }

        q.pop();
        for( auto & next : graph.GetNextVertices( current ) )
        {
            double new_cost = dist[ current ] + next.second;
            GraphTypes::Node next_node = next.first;

            if (!dist.count(next_node) || new_cost < dist[next_node]) {
              dist[next_node] = new_cost;
              q.push(std::make_pair(next_node, new_cost));
              path[next_node] = current;
            }
        }
    }

    auto resultPath = std::vector< GraphTypes::Node >{};
    GraphTypes::Node start = end;
    for (; start != begin;)
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
