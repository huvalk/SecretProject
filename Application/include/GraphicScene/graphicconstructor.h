#pragma once

#include <QPainter>
#include <memory>
#include <GraphicScene/Items/graphicimage.h>
#include <GraphicScene/Items/graphicpoint.h>
#include <GraphicScene/graphiccontainer.h>

class GraphicConstructor
{
public:
    GraphicConstructor();
    void paintTemp(const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool extColor = false) const;
    std::pair<bool, QRectF> addTempPoint(const QPointF &pos);
    std::tuple<bool, QPolygonF, QRectF> constructPoly(const QPointF &pos);
    std::tuple<bool, QLineF, QRectF>  constructLine(const QPointF &pos);
    bool clearTemp();

    QPolygonF                                          _tempPoly;
    GraphicTypes::floor<GraphicItem>   _temp;
    std::shared_ptr<QPointF>                   _tempPos;
    std::shared_ptr<GraphicPoint>           _tempPoint;
    uint8_t                                                 _pointSize;
};
