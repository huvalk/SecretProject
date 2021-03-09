#include <cmath>
#include "GraphicScene/Items/graphicpolygon.h"
#include <QDebug>

GraphicPolygon::GraphicPolygon(const QPolygonF &poly)
    : GraphicItem(poly.boundingRect().topLeft(), poly.boundingRect().bottomRight(), true),
      _fillStyle("#1F75FE"),
      _poly(poly)
{

}

QPointF GraphicPolygon::pos()
{
    qWarning() << "Использование GraphicLine::pos() не рекомендуется";
    return _boundingRect.topLeft();
}

bool GraphicPolygon::redrawRequest(const QRectF &changeArea)
{
    return _boundingRect.intersects(changeArea);
}

bool GraphicPolygon::wasClicked(const QPointF &pos, const uint8_t scale)
{
    return wasClicked(pos.x(), pos.y(), scale);
}

bool GraphicPolygon::wasClicked(const double x, const double y, const uint8_t scale)
{
    double nx = x / scale;
    double ny = y / scale;

    return _poly.containsPoint(QPointF(nx, ny), Qt::OddEvenFill);
}

GraphicTypes::GraphicItems GraphicPolygon::type()
{
    return GraphicTypes::GraphicItems::Polygon;
}

void GraphicPolygon::paint(QPainter *painter, const QPointF &offset, const uint8_t scale, const bool extColor)
{
    if (!extColor)
    {
        painter->setPen(QPen(QBrush(_fillStyle, Qt::SolidPattern), 5, Qt::SolidLine, Qt::RoundCap));
        painter->setBrush(QBrush(_fillStyle, Qt::SolidPattern));
    }

    _points.clear();
    for (const auto &point: _poly)
    {
        _points.emplace(_points.end(), point.x() * scale - offset.x(), point.y() * scale - offset.y());
    }
    painter->drawPolygon(_points.data(), static_cast<int>(_points.size()));
}

void GraphicPolygon::moveTo(const QPointF &offset)
{
    // перемещение всегда относительно первой точки
    qWarning() << "Not implemented";
}

void GraphicPolygon::moveTo(const double x, const double y)
{
    // перемещение всегда относительно первой точки
    qWarning() << "Not implemented";
}

QPolygonF GraphicPolygon::getPolygonPoints()
{
    return _poly;
}

GraphicPolygon::~GraphicPolygon()
{
}
