#pragma once

#include "GraphicScene/graphicitem.h"

class GraphicLine: public GraphicItem
{
public:
    GraphicLine(const QPointF& firstPoint, const QPointF& secondPoint);

    QPointF pos() override;
    bool redrawRequest(const QRectF& changeArea) override;
    bool wasClicked(const QPointF& pos, const uint8_t scale = 1) override;
    bool wasClicked(const double x, const double y, const uint8_t scale = 1) override;
    GraphicTypes::GraphicItems type() override;
    void paint(QPainter* painter, const QPointF& offset, const uint8_t scale) override;
    void moveTo(const QPointF& offset) override;
    void moveTo(const double x, const double y) override;
    double getYbyX(double x) const;
    double getXbyY(double y) const;
    const QPointF& getFirstPoint() const;
    const QPointF& getSecondPoint() const;

    ~GraphicLine() override;

private:
    QColor  _fillStyle;
    QPointF _firstPoint;
    QPointF _secondPoint;
    int8_t  _width;
};

