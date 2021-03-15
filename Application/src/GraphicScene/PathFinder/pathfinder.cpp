#include "GraphicScene/PathFinder/pathfinder.h"
#include "GraphicScene/PathFinder/customqueue.h"
#include <limits>


double Path( const ListGraph &graph, const GraphTypes::Node &begin, const GraphTypes::Node &end )
{
    const double INF = std::numeric_limits< double >::max();
    // вектор с расстояниями
    std::vector< double > dist( graph.VerticesCount(), INF) ;
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
            if ( dist[ next_node ] > (INF - 1) ) // если нода еще не посещена
            {
                dist[ next_node ] = dist[ current ] + next.second;
                q.push( std::make_pair( next_node, next.second ) );
            }
            else if( dist[ next_node ] > dist[ current ] + next.second ) // если найден путь короче
            {
                dist[ next_node ] = dist[ current ] + next.second;
                // изменить приоритет соединения
                q.decrease( next_node, dist[ next_node ] );
            }
        }
    }
    return dist[ end ];
}
