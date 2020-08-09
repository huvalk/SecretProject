#pragma once

#include "GraphicScene/graphicitem.h"
#include "QColor"

class GraphicPoint: public GraphicItem
{
public:
    GraphicPoint(int x, int y, int radius, int8_t border, QColor strokeStyle = "#000000", QColor fillStyle = "#38DF64");
    GraphicPoint(const QPoint& pos, int radius, int8_t border, QColor strokeStyle = "#000000", QColor fillStyle = "#38DF64");

    QPoint pos() override;
    bool redrawRequest(const QRect& changeArea) override;
    bool wasClicked(const QPointF& pos, const uint8_t scale = 1) override;
    bool wasClicked(const float x, const float y, const uint8_t scale = 1) override;
    GraphicType type() override;
    void paint(QPainter* painter, const QPoint& offset, const uint8_t scale) override;
    void moveTo(const QPoint& offset) override;
    void moveTo(const int x, const int y) override;

    ~GraphicPoint() override;

private:
    QColor  _strokeStyle;
    QColor  _fillStyle;
    QPoint  _topLeft;
    QPoint  _center;
    int     _radius;
    int     _diametr;
    int8_t  _border;
};
