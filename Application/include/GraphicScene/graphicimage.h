#pragma once

#include "GraphicScene/graphicitem.h"

class GraphicImage: public GraphicItem
{
public:
    GraphicImage(const double x, const double y, const QString path);
    GraphicImage(const QPointF& pos, const QString path);

    QPointF pos() override;
    bool redrawRequest(const QRectF& changeArea) override;
    bool wasClicked(const QPointF& pos, const uint8_t scale = 1) override;
    bool wasClicked(const double x, const double y, const uint8_t scale = 1) override;
    GraphicTypes::GraphicItems type() override;
    void paint(QPainter* painter, const QPointF& offset, const uint8_t scale, const bool extColor = false) override;
    void moveTo(const QPointF& offset) override;
    void moveTo(const double x, const double y) override;
    void setImage(const QString& path);

    ~GraphicImage() override;

private:
    QImage    _image;
    QRectF     _source;
    QRectF    _target;
    QPointF  _topLeft;
};
