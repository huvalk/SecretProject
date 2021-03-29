#include <GraphicScene/PathFinder/graphmanager.h>
#include <GraphicScene/PathFinder/pathfinder.h>
#include <GraphicScene/Items/graphicpoint.h>
#include <math.h>
#include <QDebug>

GraphManager::GraphManager(const GraphicTypes::building<GraphicLine> &walls, const GraphicTypes::building<GraphicPoint> &ladders)
{
    repopulateGraph(walls, ladders);
}

void GraphManager::findPath(GraphicPoint &from, GraphicPoint &to, const int &fromFloor, const int &toFloor)
{
    auto firstId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
    auto secondId = firstId + 1;
    // Первый этаж
    for (const auto & toPivote: _pivots[fromFloor])
    {
        auto newPath = QLineF(from.pos(), *toPivote);

        bool success = true;
        for (const auto &line: _walls[fromFloor])
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
            _graph.AddEdge(firstId, _pivotToNode[*toPivote], newPath.length());
        }
    }
    // Второй этаж
    for (const auto & toPivote: _pivots[toFloor])
    {
        auto newPath = QLineF(to.pos(), *toPivote);

        bool success = true;
        for (const auto &line: _walls[toFloor])
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
            _graph.AddEdge(secondId, _pivotToNode[*toPivote], newPath.length());
        }
    }
    // Между точками напрямую
    auto newPath = QLineF(to.pos(), from.pos());

    bool success = true;
    for (const auto &line: _walls[toFloor])
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
        _graph.AddEdge(secondId, firstId, newPath.length());
    }

    _pivots[fromFloor].insert(std::make_shared<QPointF>(from.pos()));
    // TODO брать позицию точки начала и конца, достраивать линии на этажах
    // TODO временное хранилище для достроенныхлиний
    _nodeToPivot[firstId] = from.pos();
    _pivotToNode[from.pos()] = firstId;

    _pivots[toFloor].insert(std::make_shared<QPointF>(to.pos()));
    _nodeToPivot[secondId] = to.pos();
    _pivotToNode[to.pos()] = secondId;

    const auto result = Path(_graph, 0, static_cast<GraphTypes::Node>(_graph.VerticesCount()) - 1);
    size_t i = 0;
    for (; i + 1 <  result.size(); ++i)
    {
        paths.push_back(GraphicLine(_nodeToPivot[result[i]], _nodeToPivot[result[i+1]]));
    }
}

void GraphManager::repopulateGraph(const GraphicTypes::building<GraphicLine> &walls, const GraphicTypes::building<GraphicPoint> &ladders)
{
    auto result = std::vector<GraphicLine>{};
    auto floorHeight = 10;
    for (auto floor: walls)
    {
        auto z = floorHeight * floor.first;
        auto floorWalls = GraphicTypes::floor<QLineF>{};

        for (auto item: floor.second)
        {
            auto line = item->getLine();

            floorWalls.insert(std::make_shared<QLineF>(line));
            findLinePivotes(line, floor.first);
        }
        _walls[floor.first] = floorWalls;

        repopulateFloor(floor.first);
    }
}

void GraphManager::findLinePivotes(const QLineF &line, const int &floor)
{
    // TODO сортировать по углу и находить биссектрису угла
    auto newFloorPivots = _pivots.find(floor);
    if (newFloorPivots == _pivots.end())
    {
        _pivots[floor] = GraphicTypes::floor<QPointF>{};
        newFloorPivots = _pivots.find(floor);
    }

    auto padding = 5;
    auto p1  = line.p1();
    auto p2 = line.p2();
    auto pathCenter = line.center();
    auto c = line.length();
    auto cos = (p2.x() - p1.x()) / c;
    auto sin = (p2.y() - p1.y()) / c;

    auto centerPivot1 = QPointF(
                (pathCenter.x() - sin * 15), (pathCenter.y() - cos * padding * 3)
                );
    auto centerPivot2 = QPointF(
                (pathCenter.x() + sin * 15), (pathCenter.y() + cos * padding * 3)
                );
//    newFloorPivots.insert(std::make_shared<QPointF>(centerPivot1));
//    newFloorPivots.insert(std::make_shared<QPointF>(centerPivot2));

    auto c1 = std::hypot(centerPivot1.x() - p1.x(), centerPivot1.y() - p1.y());
    auto c2 = std::hypot(centerPivot2.x() - p1.x(), centerPivot2.y() - p1.y());
    auto cos1 = (centerPivot1.x() - p1.x()) / c1;
    auto sin1 = (centerPivot1.y() - p1.y()) / c1;
    auto cos2 = (centerPivot2.x() - p1.x()) / c2;
    auto sin2 = (centerPivot2.y() - p1.y()) / c2;

    auto dx1 = cos1 * padding;
    auto dy1 = sin1 * padding;
    auto dx2 = cos2 * padding;
    auto dy2 = sin2 * padding;

    auto newPivote = QPointF(p1.x() + dx1, p1.y() + dy1);
    newFloorPivots->second.insert(std::make_shared<QPointF>(newPivote));
    auto newId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
    _nodeToPivot[newId] = newPivote;
    _pivotToNode[newPivote] = newId;


    newPivote = QPointF(p1.x() + dx2, p1.y() + dy2);
    newFloorPivots->second.insert(std::make_shared<QPointF>(newPivote));
    newId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
    _nodeToPivot[newId] = newPivote;
    _pivotToNode[newPivote] = newId;

    newPivote = QPointF(p2.x() - dx1, p2.y() - dy1);
    newFloorPivots->second.insert(std::make_shared<QPointF>(newPivote));
    newId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
    _nodeToPivot[newId] = newPivote;
    _pivotToNode[newPivote] = newId;

    newPivote = QPointF(p2.x() - dx2, p2.y() - dy2);
    newFloorPivots->second.insert(std::make_shared<QPointF>(newPivote));
    newId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
    _nodeToPivot[newId] = newPivote;
    _pivotToNode[newPivote] = newId;
}

void GraphManager::findLadderPivotes(const GraphicTypes::building<GraphicPoint> &ladders)
{
    std::unordered_map<int, std::map<int, QPointF>> floorIdLadders;

    for (const auto &floor: ladders)
    {
        for (const auto &ladder: floor.second)
        {
            floorIdLadders[ladder->getId()][floor.first] = ladder->pos();
        }
    }

    auto floorHeight = 10;
    for (const auto &id: floorIdLadders)
    {
        auto ladderFrom = id.second.begin();
        if (ladderFrom == id.second.end())
        {
            continue;
        }
        auto ladderTo = ladderFrom;
        ladderTo++;
        auto z1 = ladderFrom->first * floorHeight;
        for (; ladderTo != id.second.end(); ladderTo++)
        {
            _pivots[ladderFrom->first].insert(std::make_shared<QPointF>(ladderFrom->second));
            auto firstId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
            _nodeToPivot[firstId] = ladderFrom->second;
            _pivotToNode[ladderFrom->second] = firstId;

            _pivots[ladderTo->first].insert(std::make_shared<QPointF>(ladderTo->second));
            auto secondId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
            _nodeToPivot[secondId] = ladderTo->second;
            _pivotToNode[ladderTo->second] = secondId;

            auto z2 = ladderTo->first * floorHeight;
            auto length = std::hypot(ladderFrom->second.x() - ladderTo->second.x(),
                                     ladderFrom->second.y() - ladderTo->second.y(),
                                     z2 - z1
                                     );

            _graph.AddEdge(firstId, secondId, length);
        }
    }
}

void GraphManager::findPivotesFromPoint(std::vector<QPointF> &vector, const QPointF &from)
{

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
        auto curId = _pivotToNode[*from->get()];
        auto startTo = from;
        for (auto to = ++startTo; to != _pivots[floor].end(); to++)
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
                auto a = *to;
                auto r = _pivotToNode.find(*to->get());
                if (r == _pivotToNode.end())
                {
                    qDebug() << "err";
                }

                _graph.AddEdge(curId, _pivotToNode[*to->get()], newPath.length());
            }
        }
    }
}
