#include "GraphicScene/graphicitem.h"
#include "cmath"

GraphicItem::GraphicItem(QRect&& boundingRect, bool visable)
    : _boundingRect(boundingRect.normalized()),
      _visable(visable)
{
}

GraphicItem::GraphicItem(QRect& boundingRect, bool visable)
    : _boundingRect(boundingRect.normalized()),
      _visable(visable)
{
}

GraphicItem::GraphicItem(QPoint topLeft, QPoint bottomRight, bool visable)
    : _boundingRect(topLeft, bottomRight),
      _visable(visable)
{
}

GraphicItem::GraphicItem(int x1, int y1, int w, int h, bool visable)
    : _boundingRect(x1, y1, w, h),
      _visable(visable)
{
}

QRect GraphicItem::boundingRect()
{
    return _boundingRect;
}

bool GraphicItem::pointInArea(const QPointF &pos)
{
    return _boundingRect.contains(pos.toPoint());
}

bool GraphicItem::pointInArea(float x, float y)
{
    return _boundingRect.contains(static_cast<int>(std::round(x)),
                                  static_cast<int>(std::round(y)));
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
