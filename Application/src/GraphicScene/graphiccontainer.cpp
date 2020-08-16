#include <cmath>
#include "GraphicScene/graphiccontainer.h"

GraphicContainer::GraphicContainer()
    : _pointSize(5)
{

}

void GraphicContainer::paintLines(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter)
{
    auto currentFloorLines = _lines.find(floor);
    if (currentFloorLines != _lines.end())
    {
        for (auto item: currentFloorLines->second)
        {
            if (item->redrawRequest(area))
            {
                item->paint(painter, offset, scale);
            }
        }
    }
}

void GraphicContainer::paintPoints(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter)
{
    auto currentFloorPoints = _points.find(floor);
    if (currentFloorPoints != _points.end())
    {
        for (auto item: currentFloorPoints->second)
        {
            if (item->redrawRequest(area))
            {
                item->paint(painter, offset, scale);
            }
        }
    }
}

std::pair<bool, QPointF> GraphicContainer::lineAttachment(const int floor, const double startX, const double startY, const QPointF &pos)
{
    // TODO не срабатывает при наклоне в право
    auto currentFloor = _lines.find(floor);
    if (currentFloor == _lines.end())
    {
        return std::make_pair(false, QPointF());
    }

    double newX = startX;
    double newY = startY;
    double minDistance = std::numeric_limits<double>::max();

    for (auto item: currentFloor->second)
    {
        if (item->pointInArea(startX, startY))
        {
          double pointCrossX = item->getXbyY(startY);
          double pointCrossY = item->getYbyX(startX);
          double distanceCrossX = std::hypot((pointCrossX - pos.x()),
                                            (startY - pos.y()));
          double distanceCrossY = std::hypot((startX - pos.x()),
                                             (pointCrossY - pos.y()));

          if (distanceCrossX <= distanceCrossY) {
            if (distanceCrossX < minDistance) {
              newX = pointCrossX;
              newY = startY;
              minDistance = distanceCrossX;
            }
          } else if (distanceCrossY < minDistance) {
            newX = startX;
            newY = pointCrossY;
            minDistance = distanceCrossY;
          }
        }
    }

    return std::make_pair(true, QPointF(newX, newY));
}

std::shared_ptr<GraphicPoint> GraphicContainer::findPoint(const int floor, const QPointF &pos)
{
    //TODO передавать нужный этаж
    auto currentFloor = _points.find(floor);
    if (currentFloor == _points.end())
    {
        return nullptr;
    }

    for (auto item : currentFloor->second)
    {
        if (item->wasClicked(pos))
        {
            return item;
        }
    }
    return nullptr;
}

std::shared_ptr<GraphicLine> GraphicContainer::findLine(const int floor, const QPointF &pos)
{
    //TODO передавать нужный этаж
    auto currentFloor = _lines.find(floor);
    if (currentFloor == _lines.end())
    {
        return nullptr;
    }

    for (auto item : currentFloor->second)
    {
        if (item->wasClicked(pos))
        {
            return item;
        }
    }
    return nullptr;
}

std::pair<bool, QRectF> GraphicContainer::addPoint(const int floor, const QPointF &pos)
{
    _tempPoint = findPoint(floor, pos);
    if (_tempPoint == nullptr)
    {
        _tempPoint = std::make_shared<GraphicPoint>(pos, _pointSize, 2);
        _points[floor].insert(_tempPoint);
    }
    return std::make_pair(true, _tempPoint->boundingRect());
}

std::pair<bool, QRectF> GraphicContainer::addLine(const int floor, const QPointF &pos)
{
    auto currentFloor = _points.find(floor);
    if (_tempPoint->wasClicked(pos) || currentFloor == _points.end())
    {
        return std::make_pair(true, QRectF());
    }

    _lines[floor].emplace(std::make_shared<GraphicLine>(_tempPoint->pos(), pos));
    auto result = std::make_pair(true, _tempPoint->boundingRect());
    auto pointInPos = findPoint(floor, pos);
    if (pointInPos != nullptr)
    {
        result.second = result.second.united(pointInPos->boundingRect());
    }
    currentFloor->second.erase(_tempPoint);
    currentFloor->second.erase(pointInPos);
    _tempPoint = nullptr;

    return result;
}

std::pair<bool, QRectF> GraphicContainer::deleteItem(const int floor, const QPointF &pos)
{
    auto point = findPoint(floor, pos);

    if (point != nullptr)
    {
        _points[floor].erase(point);

        return std::make_pair(true, point->boundingRect());
    }

    auto line = findLine(floor, pos);

    if (line != nullptr)
    {
        addPoint(floor, line->getFirstPoint());
        addPoint(floor, line->getSecondPoint());
        _lines[floor].erase(line);

        return std::make_pair(true, line->boundingRect());
    }

    return std::make_pair(false, QRectF());
}

std::map<int, std::set<std::shared_ptr<GraphicPoint>>>::iterator GraphicContainer::findPoints(const int floor)
{
    return _points.find(floor);
}

std::map<int, std::set<std::shared_ptr<GraphicPoint>>>::iterator GraphicContainer::beginPoints()
{
    return _points.begin();
}

std::map<int, std::set<std::shared_ptr<GraphicPoint>>>::iterator GraphicContainer::endPoints()
{
    return _points.end();
}

std::map<int, std::set<std::shared_ptr<GraphicLine>>>::iterator GraphicContainer::findLines(const int floor)
{
    return _lines.find(floor);
}

std::map<int, std::set<std::shared_ptr<GraphicLine>>>::iterator GraphicContainer::beginLines()
{
    return _lines.begin();
}

std::map<int, std::set<std::shared_ptr<GraphicLine>>>::iterator GraphicContainer::endLines()
{
    return _lines.end();
}

QString GraphicContainer::generateJSONScene()
{
    _jsonScene = "{";
    for(auto i = _lines.begin(); i != _lines.end(); ++i)
    {
        _jsonScene.append(QString::number(i->first));
        _jsonScene.append(": { lines: [");

        for(auto& item: i->second)
        {
            _jsonScene.append("{");
            auto point = item->getFirstPoint();
            _jsonScene.append(QString::number(point.x()));
            _jsonScene.append(", ");
            _jsonScene.append(QString::number(point.y()));
            _jsonScene.append(", ");
            point = item->getSecondPoint();
            _jsonScene.append(QString::number(point.x()));
            _jsonScene.append(", ");
            _jsonScene.append(QString::number(point.y()));
            _jsonScene.append("}, ");
        }
        if (i->second.size() != 0)
        {
            _jsonScene.remove(_jsonScene.size() - 2, 2);
        }
        _jsonScene.append("]}, ");
    }
    if (_lines.size() != 0)
    {
        _jsonScene.remove(_jsonScene.size() - 2, 2);
    }
    _jsonScene.append("}");

    return _jsonScene;
}