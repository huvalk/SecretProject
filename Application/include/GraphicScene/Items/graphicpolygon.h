#pragma once

#include "GraphicScene/Items/graphicitem.h"

class GraphicPolygon: public GraphicItem
{
public:
    GraphicPolygon (const QPolygonF &poly);

    QPointF pos() override;
    bool redrawRequest(const QRectF& changeArea) override;
    bool wasClicked(const QPointF& pos, const uint8_t scale = 1) override;
    bool wasClicked(const double x, const double y, const uint8_t scale = 1) override;
    GraphicTypes::GraphicItems type() override;
    void paint(QPainter* painter, const QPointF& offset, const uint8_t scale, const bool extColor = false) override;
    void moveTo(const QPointF& offset) override;
    void moveTo(const double x, const double y) override;
    bool setColor(const QColor &&changeArea);
    QPolygonF getPolygonPoints();

    void write(QJsonObject &json) const override;
    void read(const QJsonObject &json) override;

    ~GraphicPolygon() override;

private:
    QColor  _fillStyle;
    QPolygonF _poly;
    std::vector<QPointF> _points;
};

