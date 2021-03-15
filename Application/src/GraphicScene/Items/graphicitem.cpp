#include "GraphicScene/Items/graphicitem.h"
#include "cmath"

GraphicItem::GraphicItem(QRectF&& boundingRect, bool visable)
    : _boundingRect(boundingRect.normalized()),
      _id(0),
      _visable(visable)
{
    _boundingRect.adjust(-1, -1, 1, 1);
}

GraphicItem::GraphicItem(const QRectF& boundingRect, bool visable)
    : _boundingRect(boundingRect.normalized()),
      _id(0),
      _visable(visable)
{
    _boundingRect.adjust(-1, -1, 1, 1);
}

GraphicItem::GraphicItem(const QPointF& topLeft, const QPointF& bottomRight, bool visable)
    : _boundingRect(QRectF(topLeft, bottomRight).normalized()),
      _id(0),
      _visable(visable)
{
    _boundingRect.adjust(-1, -1, 1, 1);
}

GraphicItem::GraphicItem(double x1, double y1, double w, double h, bool visable)
    : _boundingRect(QRectF(x1, y1, w, h).normalized()),
      _id(0),
      _visable(visable)
{
    _boundingRect.adjust(-1, -1, 1, 1);
}

QRectF GraphicItem::boundingRect()
{
    return _boundingRect;
}

bool GraphicItem::pointInArea(const QPointF &pos)
{
    return _boundingRect.contains(pos.toPoint());
}

bool GraphicItem::pointInArea(double x, double y)
{
    return _boundingRect.contains(x,y);
}

void GraphicItem::setId(const uint16_t &id)
{
    _id = id;
}

uint16_t GraphicItem::getId()
{
    return _id;
}

void GraphicItem::setVisable(bool visable)
{
    _visable = visable;
}

bool GraphicItem::isVisable()
{
    return _visable;
}

GraphicItem::~GraphicItem()
{
}
