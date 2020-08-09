#pragma once

#include "GraphicScene/graphicitem.h"

class GraphicLine: public GraphicItem
{
public:
    GraphicLine(QPoint firstPoint, QPoint secondPoint);

    QPoint pos() override;
    bool redrawRequest(const QRect& changeArea) override;
    bool wasClicked(const QPointF& pos, const uint8_t scale = 1) override;
    bool wasClicked(const float x, const float y, const uint8_t scale = 1) override;
    GraphicType type() override;
    void paint(QPainter* painter, const QPoint& offset, const uint8_t scale) override;
    void moveTo(const QPoint& offset) override;
    void moveTo(const int x, const int y) override;
    float getYbyX(float x);
    float getXbyY(float y);
    const QPoint& getFirstPoint();
    const QPoint& getSecondPoint();

    ~GraphicLine() override;

private:
    QColor  _fillStyle;
    QPoint _firstPoint;
    QPoint _secondPoint;
    int8_t  _width;
};

