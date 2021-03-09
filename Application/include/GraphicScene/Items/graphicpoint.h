#pragma once

#include "GraphicScene/Items/graphicitem.h"
#include "QColor"

class GraphicPoint: public GraphicItem
{
public:
    GraphicPoint(double x, double y, int radius, int8_t border, QColor strokeStyle = "#000000", QColor fillStyle = "#38DF64");
    GraphicPoint(const QPointF& pos, int radius, int8_t border, QColor strokeStyle = "#000000", QColor fillStyle = "#38DF64");

    QPointF pos() override;
    bool redrawRequest(const QRectF& changeArea) override;
    bool wasClicked(const QPointF& pos, const uint8_t scale = 1) override;
    bool wasClicked(const double x, const double y, const uint8_t scale = 1) override;
    GraphicTypes::GraphicItems type() override;
    void paint(QPainter* painter, const QPointF& offset, const uint8_t scale, const bool extColor = false) override;
    void moveTo(const QPointF& offset) override;
    void moveTo(const double x, const double y) override;

    ~GraphicPoint() override;

private:
    QColor  _strokeStyle;
    QColor  _fillStyle;
    QPointF  _topLeft;
    QPointF  _center;
    int     _radius;
    int     _diametr;
    int8_t  _border;
};
