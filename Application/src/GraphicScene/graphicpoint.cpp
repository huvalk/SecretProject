#include <cmath>
#include "GraphicScene/graphicpoint.h"
#include <QDebug>

GraphicPoint::GraphicPoint(int x, int y, int radius, int8_t border, QColor strokeStyle, QColor fillStyle)
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

GraphicPoint::GraphicPoint(const QPoint &pos, int radius, int8_t border, QColor strokeStyle, QColor fillStyle)
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

QPoint GraphicPoint::pos()
{
    return _center;
}

bool GraphicPoint::redrawRequest(const QRect &changeArea)
{
    return _boundingRect.intersects(changeArea);
}

bool GraphicPoint::wasClicked(const QPointF &pos, const uint8_t scale)
{
    return wasClicked(static_cast<float>(pos.x()), static_cast<float>(pos.y()), scale);
}

bool GraphicPoint::wasClicked(const float x, const float y, const uint8_t scale)
{
    //TODO неправильное преобразование
    float nx = x / scale;
    float ny = y / scale;

    if (_boundingRect.contains(static_cast<int>(nx), static_cast<int>(ny)))
    {
        return (std::hypot((_center.x() - nx),
                           (_center.y() - ny)) <=
                (_radius + _border));
    }

    return false;
}

GraphicType GraphicPoint::type()
{
    return GraphicType::Point;
}

void GraphicPoint::paint(QPainter *painter, const QPoint &offset, const uint8_t scale)
{
    //TODO Предварительно прощитывать настоящее положение
    painter->setPen(_strokeStyle);
    painter->setBrush(_fillStyle);
    painter->drawEllipse((_center * scale - offset), _radius, _radius);
}

void GraphicPoint::moveTo(const QPoint &offset)
{
    _topLeft.setX(offset.x() - _border - _radius);
    _topLeft.setY(offset.y() - _border - _radius);
    _center = offset;
    _boundingRect.moveTo(_topLeft);
}

void GraphicPoint::moveTo(const int x, const int y)
{
    _topLeft.setX(x - _border - _radius);
    _topLeft.setY(y - _border - _radius);
    _center.setX(x);
    _center.setY(y);
    _boundingRect.moveTo(_topLeft);
}

GraphicPoint::~GraphicPoint()
{
}
