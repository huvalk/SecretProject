#pragma once

#include <GraphicScene/graphictypes.h>
#include <GraphicScene/Items/graphicline.h>
#include <GraphicScene/Items/graphicpoint.h>
#include <GraphicScene/PathFinder/logicline.h>
#include <GraphicScene/PathFinder/graphtypes.h>
#include <map>

class GraphManager
{
public:
    GraphManager(const GraphicTypes::building<GraphicLine> &walls, const GraphicTypes::building<GraphicPoint> &ladders);
    void findPath(const GraphicPoint &from, const GraphicPoint &to);

private:
    void repopulateGraph(const GraphicTypes::building<GraphicLine> &walls, const GraphicTypes::building<GraphicPoint> &ladders);
    void proceedFloor(const GraphicTypes::floor<GraphicLine> &walls, const int &floor);

    std::vector<LogicLine> _walls;
    std::vector<LogicPoint> _pivots;
    std::map<LogicPoint, GraphTypes::Node> _pivotToNode;
    std::map<GraphTypes::Node, LogicPoint> _nodeToPivot;
};
