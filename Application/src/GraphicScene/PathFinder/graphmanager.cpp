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
    auto result = std::vector<GraphicLine>{};
    auto floorHeight = 10;
    for (auto floor: walls)
    {
        auto z = floorHeight * floor.first;
        auto floorPivots = GraphicTypes::floor<QPointF>{};
        auto floorWalls = GraphicTypes::floor<QLineF>{};

        for (auto item: floor.second)
        {
            auto line = item->getLine();
//            _walls[floor.first].insert(std::make_shared<QLineF>(line));
            floorWalls.insert(std::make_shared<QLineF>(line));
            floorPivots.insert(std::make_shared<QPointF>(line.p1()));
            floorPivots.insert(std::make_shared<QPointF>(line.p2()));
        }
        _walls[floor.first] = floorWalls;
        // TODO разделять поворотные точки
        _pivots[floor.first] = floorPivots;

        findFloorPivotes(floor.first);
        repopulateFloor(floor.first);
    }
}

void GraphManager::findFloorPivotes(const int &floor)
{
    auto currentFloorWalls = _walls.find(floor);
    auto newFloorPivots = GraphicTypes::floor<QPointF>{};
    if (currentFloorWalls == _walls.end())
    {
        return;
    }
    auto currentFloorPivots = _pivots.find(floor);
    if (currentFloorPivots == _pivots.end())
    {
        return;
    }

    for (const auto &line: currentFloorWalls->second)
    {
        auto p11  = line->p1();
        auto p12 = line->p2();
        auto pointsArray1 = std::vector<QPointF>{};
        auto pointsArray2 = std::vector<QPointF>{};
        for (const auto &line: currentFloorWalls->second)
        {
            auto p21  = line->p1();
            auto p22 = line->p2();
            if (p11 == p21)
            {
                pointsArray1.push_back(p21);
            }
            if (p12 == p21)
            {
                pointsArray2.push_back(p21);
            }
            if (p11 == p22)
            {
                pointsArray1.push_back(p22);
            }
            if (p12 == p22)
            {
                pointsArray2.push_back(p22);
            }
        }

        findPivotesFromPoint(pointsArray1, p11);
        findPivotesFromPoint(pointsArray2, p12);
    }

    _pivots[floor] = newFloorPivots;
}

void GraphManager::findPivotesFromPoint(std::vector<QPointF> &vector, const QPointF &from)
{
    if (vector.size() == 0)
    {
        return;
    }

    std::sort( vector.begin(), vector.end(), AngleComparator(from, vector[0]) );
    vector.push_back(vector[0]);


}

void GraphManager::repopulateFloor(const int &floor)
{
    auto currentFloorWalls = _walls.find(floor);
    if (currentFloorWalls == _walls.end())
    {
        return;
    }
    auto currentFloorPivots = _pivots.find(floor);
    if (currentFloorPivots == _pivots.end())
    {
        return;
    }

    for (auto from = currentFloorPivots->second.begin(); from != currentFloorPivots->second.end(); from++)
    {
        auto tempFrom = from;
        for (auto to = ++tempFrom; to != _pivots[floor].end(); to++)
        {
            auto newPath = QLineF(from->get()->x(), from->get()->y(), to->get()->x(), to->get()->y());

            bool success = true;
            for (const auto &line: _walls[floor])
            {
                auto intersectes = line->intersect(newPath, nullptr);
                if (intersectes == QLineF::BoundedIntersection)
                {
                    success = false;
                    break;
                }
            }

            if (success)
            {
                paths.push_back(GraphicLine(newPath.p1(), newPath.p2()));
            }
        }
    }
}
