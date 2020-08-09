#pragma once

#include <GraphicScene/graphictypes.h>
#include <QRectF>
#include <QPainter>

class GraphicItem
{
public:
    GraphicItem(QRect&& boundingRect, bool visable = true);
    GraphicItem(QRect& boundingRect, bool visable = true);
    GraphicItem(QPoint topLeft, QPoint bottomRight, bool visable = true);
    GraphicItem(int x1, int y1, int w, int h, bool visable = true);

    virtual QPoint pos() = 0;
    virtual bool redrawRequest(const QRect& changeArea) = 0;
    virtual bool wasClicked(const QPointF& pos, const uint8_t scale) = 0;
    virtual bool wasClicked(const float x, const float y, const uint8_t scale) = 0;
    virtual GraphicType type() = 0;
    virtual void paint(QPainter* painter, const QPoint& offset, const uint8_t scale) = 0;
    virtual void moveTo(const QPoint& offset) = 0;
    virtual void moveTo(const int x, const int y) = 0;
    virtual QRect boundingRect();
    virtual bool pointInArea(const QPointF& pos);
    virtual bool pointInArea(float x, float y);
    void setVisable(bool visable = true);
    bool isVisable();

    virtual ~GraphicItem() = 0;

protected:
    QRect  _boundingRect;
    bool    _visable;
};

