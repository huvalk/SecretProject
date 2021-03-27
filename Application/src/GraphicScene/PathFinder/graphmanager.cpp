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

        repopulateFloor(floor.first);
    }
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
            auto tempPath = QLineF(from->get()->x(), from->get()->y(), to->get()->x(), to->get()->y());
            auto pathCenter = tempPath.center();
            auto c = tempPath.length();
            auto cos = (tempPath.p1().x() - tempPath.p2().x()) / c;
            auto sin = (tempPath.p1().y() - tempPath.p2().y()) / c;
            c = c / 2 - 5;
            tempPath.setP1(QPointF(
                               (pathCenter.x() - cos * c), (pathCenter.y() - sin * c)
                               ));
            tempPath.setP2(QPointF(
                               (pathCenter.x() + cos * c), (pathCenter.y() + sin * c)
                               ));

            bool success = true;
            for (const auto &line: _walls[floor])
            {
                auto intersectes = line->intersect(tempPath, nullptr);
                if (intersectes == QLineF::BoundedIntersection)
                {
                    success = false;
                    break;
                }
            }

            if (success)
            {
                paths.push_back(GraphicLine(tempPath.p1(), tempPath.p2()));
            }
        }
    }
}
