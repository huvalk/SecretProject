#include <cmath>
#include "GraphicScene/graphiccontainer.h"
#include <QDebug>

GraphicContainer::GraphicContainer()
    : _pointSize(5)
{

}

void GraphicContainer::paintLines(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool bg)
{
    auto currentFloorLines = _lines.find(floor);
    if (currentFloorLines != _lines.end())
    {
        for (auto item: currentFloorLines->second)
        {
            if (item->redrawRequest(area))
            {
                item->paint(painter, offset, scale, bg);
            }
        }
    }
}

void GraphicContainer::paintPoints(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool extColor)
{
    auto currentFloorPoints = _points.find(floor);
    if (currentFloorPoints != _points.end())
    {
        for (auto item: currentFloorPoints->second)
        {
            if (item->redrawRequest(area))
            {
                item->paint(painter, offset, scale, extColor);
            }
        }
    }
}

void GraphicContainer::paintPolygons(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool extColor)
{
    auto currentFloorPolys = _polygons.find(floor);
    if (currentFloorPolys != _polygons.end())
    {
        for (auto item: currentFloorPolys->second)
        {
            if (item->redrawRequest(area))
            {
                item->paint(painter, offset, scale, extColor);
            }
        }
    }
}

void GraphicContainer::paintTemp(const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool extColor)
{
    for (auto item: _temp)
    {
        if (item->redrawRequest(area))
        {
            item->paint(painter, offset, scale, extColor);
        }
    }
}

std::pair<bool, QPointF> GraphicContainer::lineAttachment(const int floor, const double startX, const double startY, const QPointF &pos)
{
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

std::pair<bool, QRectF> GraphicContainer::addPolygon(const int floor, const QPolygonF &poly)
{
    _polygons[floor].insert(std::make_shared<GraphicPolygon>(poly));

    return std::make_pair(true, poly.boundingRect());
}

// Начинается создание полигона
std::pair<bool, QRectF> GraphicContainer::addTempPoint(const QPointF &pos)
{
    qDebug() << "tempPos created";
    _tempPoint = std::make_shared<GraphicPoint>(pos, _pointSize, 2);
    _tempPos = std::make_shared<QPointF>(_tempPoint->pos());
    _temp.insert(_tempPoint);
    _tempPoly.clear();

    return std::make_pair(true, _tempPoint->boundingRect());
}

// Дополнительная переменная для завершения создания
// Здесьже формируется полигон
std::tuple<bool, QPolygonF, QRectF>  GraphicContainer::addTempLine(const QPointF &pos)
{
    if (_tempPos == nullptr)
    {
        qWarning() << "tempPos is null";
        return std::make_tuple(false, QPolygonF(), QRectF());
    }
    auto newTempLine = std::make_shared<GraphicLine>(*_tempPos, pos);
    _temp.emplace(newTempLine);
    _tempPos = std::make_shared<QPointF>(pos);
    _tempPoly.append(pos);

    if (_tempPoint->wasClicked(pos))
    {
        auto result = std::make_tuple(true, _tempPoly, _tempPoly.boundingRect());

        _tempPoint = nullptr;
        _tempPos = nullptr;
        _tempPoly.clear();
        _temp.clear();

        return result;
    }

    return std::make_tuple(true, QPolygonF(), newTempLine->boundingRect());
}

bool GraphicContainer::clearPoints(const int floor)
{
    auto currentFloor = _points.find(floor);
    if (currentFloor == _points.end())
    {
        return false;
    }

    currentFloor->second.clear();
    return true;
}

bool GraphicContainer::clearTemp()
{
    if (_temp.size() == 0)
    {
        return false;
    }

    _temp.clear();
    return true;
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

GraphicTypes::building<GraphicPoint>::iterator GraphicContainer::findPoints(const int floor)
{
    return _points.find(floor);
}

GraphicTypes::building<GraphicPoint>::iterator GraphicContainer::beginPoints()
{
    return _points.begin();
}

GraphicTypes::building<GraphicPoint>::iterator GraphicContainer::endPoints()
{
    return _points.end();
}

GraphicTypes::building<GraphicLine>::iterator GraphicContainer::findLines(const int floor)
{
    return _lines.find(floor);
}

GraphicTypes::building<GraphicLine>::iterator GraphicContainer::beginLines()
{
    return _lines.begin();
}

GraphicTypes::building<GraphicLine>::iterator GraphicContainer::endLines()
{
    return _lines.end();
}

QString GraphicContainer::generateJSONScene()
{
    return _parser.generateJSONScene(_lines, _polygons);
}

std::pair<bool, QVector<int> > GraphicContainer::parseJSONScene(QString json)
{
    bool result = true;
    std::tie(result, _lines, _polygons) = _parser.parseJSONScene(json);

    QVector<int> floors{};
    if (result)
    {
        for(auto it = _lines.begin(); it != _lines.end(); ++it)
        {
            floors.push_back(it->first);
        }

        if (floors.size() == 0)
        {
            floors.push_back(1);
        }
    }

    return std::make_pair(result, std::move(floors));
}
