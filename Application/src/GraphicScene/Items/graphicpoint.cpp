#include <cmath>
#include "GraphicScene/Items/graphicpoint.h"
#include <QDebug>

GraphicPoint::GraphicPoint(double x, double y, int radius, int8_t border, QColor strokeStyle, QColor fillStyle)
    : GraphicItem((x - border - radius),
                  (y - border - radius),
                  (2 * (border + radius)),
                  (2 * (border + radius))),
      _strokeStyle(strokeStyle), _fillStyle(fillStyle),
      _topLeft( _boundingRect.topLeft() ), _center(x, y),
      _radius(radius), _diametr(2 * radius),
      _border(border)
{
}

GraphicPoint::GraphicPoint(const QPointF &pos, int radius, int8_t border, QColor strokeStyle, QColor fillStyle)
    : GraphicItem((pos.x() - border - radius),
                  (pos.y() - border - radius),
                  (2 * (border + radius)),
                  (2 * (border + radius))),
      _strokeStyle(strokeStyle), _fillStyle(fillStyle),
      _topLeft( _boundingRect.topLeft() ), _center(pos),
      _radius(radius), _diametr(2 * radius),
      _border(border)
{
}

QPointF GraphicPoint::pos()
{
    return _center;
}

bool GraphicPoint::redrawRequest(const QRectF &changeArea)
{
    return _boundingRect.intersects(changeArea);
}

bool GraphicPoint::wasClicked(const QPointF &pos, const uint8_t scale)
{
    return wasClicked(pos.x(), pos.y(), scale);
}

bool GraphicPoint::wasClicked(const double x, const double y, const uint8_t scale)
{
    //TODO неправильное преобразование
    double nx = x / scale;
    double ny = y / scale;

    if (_boundingRect.contains(nx, ny))
    {
        return (std::hypot((_center.x() - nx),
                           (_center.y() - ny)) <=
                (_radius + _border));
    }

    return false;
}

GraphicTypes::GraphicItems GraphicPoint::type()
{
    return GraphicTypes::GraphicItems::Point;
}

void GraphicPoint::paint(QPainter *painter, const QPointF &offset, const uint8_t scale, const bool extColor)
{
    //TODO Предварительно прощитывать настоящее положение
    if (!extColor)
    {
        painter->setPen(QPen(QBrush(_strokeStyle), 1, Qt::SolidLine, Qt::RoundCap));
        painter->setBrush(QBrush(_fillStyle));
    }
    painter->drawEllipse((_center * scale - offset), _radius, _radius);
}

void GraphicPoint::moveTo(const QPointF &offset)
{
    moveTo(offset.x(), offset.y());
}

void GraphicPoint::moveTo(const double x, const double y)
{
    _center.setX(x);
    _center.setY(y);
    _boundingRect.moveTo(x - _border - _radius, y - _border - _radius);
}

void GraphicPoint::write(QJsonObject &json) const
{
    json["id"] = _id;
    json["x"] = _center.x();
    json["y"] = _center.y();
}

void GraphicPoint::read(const QJsonObject &json)
{
    if (json.contains("x") && json.contains("y"))
    {
        _center.setX(json["x"].toDouble(0));
        _center.setY(json["y"].toDouble(0));
    } else {
        qWarning() << "Missing pos in point";
    }

    if (json.contains("id") && json.contains("id"))
    {
        _id = static_cast<uint16_t>(json["id"].toInt(0));
    } else {
        qWarning() << "Missing id in point";
    }

    _boundingRect = QRectF((_center.x() - _border - _radius),
                  (_center.y() - _border - _radius),
                  (2 * (_border + _radius)),
                  (2 * (_border + _radius)));
}

GraphicPoint::~GraphicPoint()
{
}
