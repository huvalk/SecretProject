#pragma once

#include <GraphicScene/graphictypes.h>
#include <GraphicScene/Items/graphicline.h>
#include <GraphicScene/Items/graphicpoint.h>
#include <GraphicScene/PathFinder/logicline.h>
#include <GraphicScene/PathFinder/graphtypes.h>
#include <GraphicScene/graphictypes.h>
#include <map>

class GraphManager
{
    class CmpLine
    {
    public:
        bool operator() ( const QLineF & left, const QLineF & right ) const
        {
            return left.p1().x() > right.p1().x() && left.p1().y() > right.p1().y() && left.p2().x() > right.p2().x() && left.p2().y() > right.p2().y();
        }
    };
    class CmpPoint
    {
    public:
        bool operator() ( const QPointF & left, const QPointF & right ) const
        {
            return left.x() > right.x() && left.y() > right.y();
        }
    };

public:
    GraphManager() = default;
    GraphManager(const GraphicTypes::building<GraphicLine> &walls, const GraphicTypes::building<GraphicPoint> &ladders);
    void repopulateGraph(const GraphicTypes::building<GraphicLine> &walls, const GraphicTypes::building<GraphicPoint> &ladders);
    void findPath(const GraphicPoint &from, const GraphicPoint &to);
    std::vector<GraphicLine> paths;

private:
    void findFloorPivotes(const int &floor);
    void repopulateFloor(const int &floor);
//    void findPathsFromPointOnFloor(const GraphTypes::logicFloor<QLineF> &walls, const int &floor);

     GraphicTypes::building<QLineF> _walls;
    GraphicTypes::building<QPointF> _pivots;
    std::map<QPointF, GraphTypes::Node, CmpPoint> _pivotToNode;
    std::map<GraphTypes::Node, QPointF> _nodeToPivot;
};
