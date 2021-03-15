#include <GraphicScene/PathFinder/customqueue.h>

bool Cmp::operator() ( const GraphTypes::Edge & left, const GraphTypes::Edge & right )
{
    // сравнение связей по расстоянию
    return left.second > right.second;
}

void CustomQueue::decrease(const GraphTypes::Node &val, const double &prior )
{
    auto cur_val = c.begin();
    // поиск связи
    while( cur_val < c.end() && cur_val->first != val )
        cur_val++;

    if( cur_val != c.end() )
    {
        // замена приоритета
        cur_val->second = prior;
        std::make_heap( c.begin(), c.end(), comp );
    }
}
