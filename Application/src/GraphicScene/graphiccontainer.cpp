#include <cmath>
#include "GraphicScene/graphiccontainer.h"
#include "GraphicScene/PathFinder/graphmanager.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextCodec>

GraphicContainer::GraphicContainer()
    : _pointSize(5)
{

}

void GraphicContainer::paintPath(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool bg) const
{
    // TODO отрисовывать точки и путь
    auto currentFloorLines = _path.find(floor);
    if (currentFloorLines != _path.end())
    {
        for (auto item: currentFloorLines->second)
        {
            if (item->redrawRequest(area))
            {
                item->paint(painter, offset, scale, bg);
            }
        }
    }

    if (_fromToPoints[0].second != nullptr && _fromToPoints[0].first == floor)
    {
        painter->drawEllipse((*_fromToPoints[0].second * scale - offset), _pointSize, _pointSize);
    }
    if (_fromToPoints[1].second != nullptr && _fromToPoints[1].first == floor)
    {
        painter->drawEllipse((*_fromToPoints[1].second * scale - offset), _pointSize, _pointSize);
    }
}

void GraphicContainer::paintLines(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool bg) const
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

void GraphicContainer::paintPoints(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool extColor) const
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

void GraphicContainer::paintPolygons(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool extColor) const
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

QRectF GraphicContainer::addFrom(const int floor, const QPointF &pos)
{
    _fromToPoints[0] = std::make_pair(floor, std::make_shared<QPointF>(pos));
}

QRectF GraphicContainer::addTo(const int floor, const QPointF &pos)
{
    _fromToPoints[1] = std::make_pair(floor, std::make_shared<QPointF>(pos));
}

QRectF GraphicContainer::addPoint(const int floor, const QPointF &pos)
{
    auto tempPoint = findPoint(floor, pos);
    if (tempPoint == nullptr)
    {
        tempPoint = std::make_shared<GraphicPoint>(pos, _pointSize, 2);
        _points[floor].insert(tempPoint);
    }
    return tempPoint->boundingRect();
}

QRectF GraphicContainer::addLine(const int floor, const QLineF &line)
{
    auto newLine = std::make_shared<GraphicLine>(line.p1(), line.p2());
    _lines[floor].insert(newLine);

    return newLine->boundingRect();
}

QRectF GraphicContainer::addPolygon(const int floor, const QPolygonF &poly)
{
    _polygons[floor].insert(std::make_shared<GraphicPolygon>(poly));

    return poly.boundingRect();
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

std::pair<bool, QRectF> GraphicContainer::deleteItem(const int floor, const QPointF &pos)
{
    // TODO удалять полигоны
    auto point = findPoint(floor, pos);

    if (point != nullptr)
    {
        _points[floor].erase(point);

        return std::make_pair(true, point->boundingRect());
    }

    auto line = findLine(floor, pos);

    if (line != nullptr)
    {
//        addPoint(floor, line->getFirstPoint());
//        addPoint(floor, line->getSecondPoint());
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
    return write();
}

QString GraphicContainer::write()
{
    QJsonObject jsonLines;
    for (const auto &floor: _lines)
    {
        QJsonArray jsonLineArray;
        for (const auto &line: floor.second)
        {
            QJsonObject jsonLine;
            line->write(jsonLine);

            jsonLineArray.append(jsonLine);
        }

        jsonLines[QString::number(floor.first)] = jsonLineArray;
    }

    QJsonObject jsonPolys;
    for (const auto &floor: _polygons)
    {
        QJsonArray jsonPolyArray;
        for (const auto &poly: floor.second)
        {
            QJsonObject jsonPoly;
            poly->write(jsonPoly);

            jsonPolyArray.append(jsonPoly);
        }

        jsonPolys[QString::number(floor.first)] = jsonPolyArray;
    }

    QJsonObject jsonPoints;
    for (const auto &floor: _points)
    {
        QJsonArray jsonPointsArray;
        for (const auto &point: floor.second)
        {
            QJsonObject jsonPoint;
            point->write(jsonPoint);

            jsonPointsArray.append(jsonPoint);
        }

        jsonPoints[QString::number(floor.first)] = jsonPointsArray;
    }

    QJsonObject jsonMap;
    jsonMap["lines"] = jsonLines;
    jsonMap["polys"] = jsonPolys;
    jsonMap["points"] = jsonPoints;

    QJsonDocument jsonDoc(jsonMap);
    return QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
}

std::pair<bool, QVector<int> > GraphicContainer::parseJSONScene(QString json)
{
    read(json);
    QVector<int> result;
    result.reserve(static_cast<int>(_lines.size()));

    for (const auto &i: _lines)
    {
        result.append(i.first);
    }
    return std::make_pair(true, std::move(result));
}

void GraphicContainer::read(QString json)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jsonObject = jsonDoc.object();

    if (json.contains("lines"))
    {
        QJsonObject jsonLines = jsonObject["lines"].toObject();
        for (const auto &floor: jsonLines.keys())
        {
            QJsonArray jsonLineArray = jsonLines[floor].toArray();
            std::set<std::shared_ptr<GraphicLine>> lines;
             for (int lineIndex = 0; lineIndex < jsonLineArray.size(); ++lineIndex)
            {
                QJsonObject jsonLine = jsonLineArray[lineIndex].toObject();
                auto line = std::make_shared<GraphicLine>(QPointF(), QPointF());
                line->read(jsonLine);

                lines.insert(std::move(line));
            }

            _lines[floor.toInt()] = lines;
        }
    } else {
        qWarning() << "Missing lines";
    }

    if (json.contains("polys"))
    {
        QJsonObject jsonPolys = jsonObject["polys"].toObject();
        for (const auto &floor: jsonPolys.keys())
        {
            QJsonArray jsonPolysArray = jsonPolys[floor].toArray();
            std::set<std::shared_ptr<GraphicPolygon>> polys;
             for (int polyIndex = 0; polyIndex < jsonPolysArray.size(); ++polyIndex)
            {
                QJsonObject jsonPoly = jsonPolysArray[polyIndex].toObject();
                auto poly = std::make_shared<GraphicPolygon>(QPolygonF());
                poly->read(jsonPoly);

                polys.insert(std::move(poly));
            }

            _polygons[floor.toInt()] = polys;
        }
    } else {
        qWarning() << "Missing polys";
    }

    if (json.contains("points"))
    {
        QJsonObject jsonPoints = jsonObject["points"].toObject();
        for (const auto &floor: jsonPoints.keys())
        {
            QJsonArray jsonPointsArray = jsonPoints[floor].toArray();
            std::set<std::shared_ptr<GraphicPoint>> points;
             for (int polyIndex = 0; polyIndex < jsonPointsArray.size(); ++polyIndex)
            {
                QJsonObject jsonPoint = jsonPointsArray[polyIndex].toObject();
                auto point = std::make_shared<GraphicPoint>(QPointF(), _pointSize, 2);
                point->read(jsonPoint);

                points.insert(std::move(point));
            }

            _points[floor.toInt()] = points;
        }
    } else {
        qWarning() << "Missing points";
    }
}

void GraphicContainer::findPath()
{
    auto grap = std::make_shared<GraphManager>(_lines, _points);
    if (_fromToPoints[0].second == nullptr || _fromToPoints[1].second == nullptr)
    {
        return;
    }

    _path = grap->findPath(
            std::make_pair(_fromToPoints[0].first, *_fromToPoints[0].second),
            std::make_pair(_fromToPoints[1].first, *_fromToPoints[1].second)
            );
}

void GraphicContainer::updateCameras(std::unordered_map<u_int64_t, u_int64_t> &cameras)
{
    for (auto const &floor: _polygons)
    {
        for (auto const &poly: floor.second)
        {
            auto curPeople = cameras[static_cast<u_int64_t>(poly->getId())];
            if (curPeople > 1)
            {
                poly->setColor("orange");
            } else {
                poly->setColor("blue");
            }
        }
    }
}
