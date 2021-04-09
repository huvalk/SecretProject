#include <GraphicScene/PathFinder/graphmanager.h>
#include <GraphicScene/PathFinder/pathfinder.h>
#include <GraphicScene/Items/graphicpoint.h>
#include <math.h>
#include <QDebug>

GraphManager::GraphManager(const GraphicTypes::building<GraphicLine> &walls, const GraphicTypes::building<GraphicPoint> &ladders)
{
    repopulateGraph(walls, ladders);
}

GraphicTypes::building<GraphicLine> GraphManager::findPath(std::pair<int, QPointF> from, std::pair<int, QPointF> to)
{
    auto firstId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
    auto secondId = firstId + 1;
    // Первый этаж
    for (const auto & toPivote: _pivots[from.first])
    {
        auto newPath = QLineF(from.second, *toPivote);

        bool success = true;
        for (const auto &line: _walls[from.first])
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
            _graph.AddEdge(firstId, _pivotToNode[std::make_pair(from.first, *toPivote)], newPath.length());
        }
    }
    // Второй этаж
    for (const auto & toPivote: _pivots[to.first])
    {
        auto newPath = QLineF(to.second, *toPivote);

        bool success = true;
        for (const auto &line: _walls[to.first])
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
            _graph.AddEdge(secondId, _pivotToNode[std::make_pair(to.first, *toPivote)], newPath.length());
        }
    }
    // Между точками напрямую
    auto newPath = QLineF(to.second, from.second);

    bool success = true;
    if (from.first == to.first)
    {
        for (const auto &line: _walls[to.first])
        {
            auto intersectes = line->intersect(newPath, nullptr);
            if (intersectes == QLineF::BoundedIntersection)
            {
                success = false;
                break;
            }
        }
    } else {
        success = false;
    }

    if (success)
    {
        _graph.AddEdge(secondId, firstId, newPath.length());
    }

    _pivots[from.first].insert(std::make_shared<QPointF>(from.second));
    // TODO брать позицию точки начала и конца, достраивать линии на этажах
    // TODO временное хранилище для достроенныхлиний
    _nodeToPivot[firstId] = from;
    _pivotToNode[from] = firstId;

    _pivots[to.first].insert(std::make_shared<QPointF>(to.second));
    _nodeToPivot[secondId] = to;
    _pivotToNode[to] = secondId;

    GraphicTypes::building<GraphicLine> resultLines;
    const auto result = Path(_graph, firstId, secondId);
    size_t i = 0;
    for (; i + 1 <  result.size(); ++i)
    {
      auto fromPivote = _nodeToPivot[result[i]];
      auto toPivote = _nodeToPivot[result[i+1]];
      if (fromPivote.first == toPivote.first)
      {
          resultLines[fromPivote.first].insert(std::make_shared<GraphicLine>(GraphicLine(fromPivote.second, toPivote.second)));
      }
    }

    return resultLines;
}

void GraphManager::repopulateGraph(const GraphicTypes::building<GraphicLine> &walls, const GraphicTypes::building<GraphicPoint> &ladders)
{
    findLadderPivotes(ladders);

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

    auto padding = 10;
    int n = 8;
    auto p1  = line.p1();
    auto p2 = line.p2();
    for( int i=0; i<n; i++ )
    {
       double x = i*padding/n;
       double y = sqrt( padding*padding - x*x );
       auto newPivote = QPointF(p1.x() + x, p1.y() + y);
       newFloorPivots->second.insert(std::make_shared<QPointF>(newPivote));
       auto newId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
       _nodeToPivot[newId] = std::make_pair(floor, newPivote);
       _pivotToNode[std::make_pair(floor, newPivote)] = newId;

       newPivote = QPointF(p2.x() + x, p2.y() + y);
       newFloorPivots->second.insert(std::make_shared<QPointF>(newPivote));
       newId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
       _nodeToPivot[newId] = std::make_pair(floor, newPivote);
       _pivotToNode[std::make_pair(floor, newPivote)] = newId;

       newPivote = QPointF(p1.x() + x, p1.y() - y);
       newFloorPivots->second.insert(std::make_shared<QPointF>(newPivote));
       newId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
       _nodeToPivot[newId] = std::make_pair(floor, newPivote);
       _pivotToNode[std::make_pair(floor, newPivote)] = newId;

       newPivote = QPointF(p2.x() + x, p2.y() - y);
       newFloorPivots->second.insert(std::make_shared<QPointF>(newPivote));
       newId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
       _nodeToPivot[newId] = std::make_pair(floor, newPivote);
       _pivotToNode[std::make_pair(floor, newPivote)] = newId;
    }
}

void GraphManager::findLadderPivotes(const GraphicTypes::building<GraphicPoint> &ladders)
{
    // id - этаж - позиция
    std::unordered_map<int, std::map<int, QPointF>> floorIdLadders;

    for (const auto &floor: ladders)
    {
        for (const auto &ladder: floor.second)
        {
            floorIdLadders[ladder->getId()][floor.first] = ladder->pos();
        }
    }

    auto floorHeight = 10;
    for (const auto &id: floorIdLadders) // лестница по id
    {
        auto ladderFrom = id.second.begin();
        if (ladderFrom == id.second.end())
        {
            continue;
        }
        auto ladderTo = ladderFrom;
        ladderTo++;
        auto z1 = ladderFrom->first * floorHeight;
        for (; ladderTo != id.second.end(); ladderTo++) // пролет лестницы на этаже
        {
            _pivots[ladderFrom->first].insert(std::make_shared<QPointF>(ladderFrom->second));
            auto firstId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
            _nodeToPivot[firstId] = *ladderFrom;
            _pivotToNode[*ladderFrom] = firstId;

            _pivots[ladderTo->first].insert(std::make_shared<QPointF>(ladderTo->second));
            auto secondId = static_cast<GraphTypes::Node>(_nodeToPivot.size());
            _nodeToPivot[secondId] = *ladderTo;
            _pivotToNode[*ladderTo] = secondId;

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
        auto curId = _pivotToNode[std::make_pair(floor, *from->get())];
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
                auto r = _pivotToNode.find(std::make_pair(floor, *to->get()));
                if (r == _pivotToNode.end())
                {
                    qDebug() << "err";
                }

                _graph.AddEdge(curId, _pivotToNode[std::make_pair(floor, *to->get())], newPath.length());
            }
        }
    }
}

QString GraphManager::write()
{

}

void GraphManager::read(QString json)
{

}
