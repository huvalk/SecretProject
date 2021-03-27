#include <GraphicScene/PathFinder/graphmanager.h>

GraphManager::GraphManager(const GraphicTypes::building<GraphicLine> &walls, const GraphicTypes::building<GraphicPoint> &ladders)
{
    repopulateGraph(walls, ladders);
}

void GraphManager::findPath(const GraphicPoint &from, const GraphicPoint &to)
{

}

void GraphManager::repopulateGraph(const GraphicTypes::building<GraphicLine> &walls, const GraphicTypes::building<GraphicPoint> &ladders)
{

}

void GraphManager::proceedFloor(const GraphicTypes::floor<GraphicLine> &walls, const int &floor)
{

}
