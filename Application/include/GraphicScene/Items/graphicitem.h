#pragma once

#include <GraphicScene/graphictypes.h>
#include <QRectF>
#include <QPainter>
#include <QJsonObject>

class GraphicItem
{
public:
    GraphicItem(QRectF&& boundingRect, bool visable = true);
    GraphicItem(const QRectF& boundingRect, bool visable = true);
    GraphicItem(const QPointF& topLeft, const QPointF& bottomRight, bool visable = true);
    GraphicItem(double x1, double y1, double w, double h, bool visable = true);

    virtual QPointF pos() = 0;
    virtual bool redrawRequest(const QRectF& changeArea) = 0;
    virtual bool wasClicked(const QPointF& pos, const uint8_t scale) = 0;
    virtual bool wasClicked(const double x, const double y, const uint8_t scale) = 0;
    virtual GraphicTypes::GraphicItems type() = 0;
    virtual void paint(QPainter* painter, const QPointF& offset, const uint8_t scale, const bool extColor) = 0;
    virtual void moveTo(const QPointF& offset) = 0;
    virtual void moveTo(const double x, const double y) = 0;
    virtual QRectF boundingRect();
    virtual bool pointInArea(const QPointF& pos);
    virtual bool pointInArea(double x, double y);
    void setId(const uint16_t &id = 0);
    uint16_t getId();
    void setVisable(bool visable = true);
    bool isVisable();

    virtual void write(QJsonObject &json) const = 0;
    virtual void read(const QJsonObject &json) = 0;

    virtual ~GraphicItem() = 0;

protected:
    QRectF  _boundingRect;
    uint16_t _id;
    bool    _visable;
};

