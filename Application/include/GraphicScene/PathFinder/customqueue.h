#pragma once
#include <queue>
#include "GraphicScene/PathFinder/graphtypes.h"

// компаратор для кастомной очереди
class Cmp
{
public:
    bool operator() ( const GraphTypes::Edge & left, const GraphTypes::Edge & right );
};

// кастомная очередь с приоритетом
class CustomQueue : public std::priority_queue< GraphTypes::Edge, std::vector< GraphTypes::Edge>, Cmp>
{
public:
    // изменить приоритет существующей связи
    void decrease(const GraphTypes::Node &val, const double &prior);
};
