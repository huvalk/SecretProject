#pragma once

#include <GraphicScene/graphictypes.h>
#include <GraphicScene/Items/graphicline.h>
#include <GraphicScene/Items/graphicpoint.h>
#include <GraphicScene/PathFinder/logicline.h>
#include <GraphicScene/PathFinder/graphtypes.h>
#include <GraphicScene/PathFinder/listgraph.h>
#include <GraphicScene/graphictypes.h>
#include <map>

class GraphManager
{
    class AngleComparator
    {
        QPointF origin_;
        QPointF dreference_;

        static double IsSamePoint( QPointF a, QPointF b ) { return a.x() * b.y() - a.y() * b.x(); }
    public:
        AngleComparator( const QPointF origin, const QPointF reference ) : origin_( origin ), dreference_( reference - origin ) {}
        bool operator()( const QPointF a, const QPointF b ) const
        {
            const QPointF da = a - origin_, db = b - origin_;
            const double detb = IsSamePoint( dreference_, db );

            if ( detb == 0 && db.x() * dreference_.x() + db.y() * dreference_.y() >= 0 )
                return false;

            const double deta = IsSamePoint( dreference_, da );

            if ( deta == 0 && da.x() * dreference_.x() + da.y() * dreference_.y() >= 0 )
                return true;

            if ( deta * detb >= 0 )
            {
                return IsSamePoint( da, db ) > 0;
            }

            return deta > 0;
        }
    };
    class CmpPoint
    {
    public:
        bool operator() ( const QPointF & left, const QPointF & right ) const
        {
            return left.x() < right.x() || ((abs(right.x() - left.x()) < std::numeric_limits<double>::epsilon()) && left.y() < right.y());
        }
    };

public:
    GraphManager();
    GraphManager(const GraphicTypes::building<GraphicLine> &walls, const GraphicTypes::building<GraphicPoint> &ladders);
    void repopulateGraph(const GraphicTypes::building<GraphicLine> &walls, const GraphicTypes::building<GraphicPoint> &ladders);
    void findPath(const GraphicPoint &from, const GraphicPoint &to);
    void findPath();
    std::vector<GraphicLine> paths;


private:
    void findLinePivotes(const QLineF &line, const int &floor);
    void findPivotesFromPoint(std::vector<QPointF> &vector, const QPointF &from);
    void repopulateFloor(const int &floor);
//    void findPathsFromPointOnFloor(const GraphTypes::logicFloor<QLineF> &walls, const int &floor);

     GraphicTypes::building<QLineF> _walls;
    GraphicTypes::building<QPointF> _pivots;
    std::map<QPointF, GraphTypes::Node, CmpPoint> _pivotToNode;
    std::map<GraphTypes::Node, QPointF> _nodeToPivot;
    ListGraph _graph;
};
