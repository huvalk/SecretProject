#pragma once

#include <GraphicScene/Items/graphicpoint.h>
#include <GraphicScene/Items/graphicline.h>
#include <GraphicScene/graphictypes.h>
#include <GraphicScene/graphicparser.h>
#include <GraphicScene/Items/graphicpolygon.h>
#include <map>
#include <memory>
#include <set>
#include <QPointF>

class GraphicContainer
{
public:
    GraphicContainer();
    void paintPoints(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool extColor = false) const;
    void paintLines(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool bg = false) const;
    void paintPolygons(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool extColor = false) const;
    void paintTemp(const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool extColor = false) const;

    std::pair<bool, QPointF>  lineAttachment(const int floor, const double startX, const double startY, const QPointF &pos);
    std::shared_ptr<GraphicPoint> findPoint(const int floor, const QPointF& pos);
    std::shared_ptr<GraphicLine> findLine(const int floor, const QPointF& pos);
    std::pair<bool, QRectF> addPoint(const int floor, const QPointF &pos);
    std::pair<bool, QRectF>  addLine(const int floor, const QPointF &pos);
    std::pair<bool, QRectF> addPolygon(const int floor, const QPolygonF &poly);
    std::pair<bool, QRectF> addTempPoint(const QPointF &pos);
    std::tuple<bool, QPolygonF, QRectF> addTempLine(const QPointF &pos);
    std::pair<bool, QRectF>  deleteItem(const int floor, const QPointF &pos);
    bool clearPoints(const int floor);
    bool clearTemp();
    GraphicTypes::building<GraphicPoint>::iterator findPoints(const int floor);
    GraphicTypes::building<GraphicPoint>::iterator beginPoints();
    GraphicTypes::building<GraphicPoint>::iterator endPoints();
    GraphicTypes::building<GraphicLine>::iterator findLines(const int floor);
    GraphicTypes::building<GraphicLine>::iterator beginLines();
    GraphicTypes::building<GraphicLine>::iterator endLines();
    QString generateJSONScene();
    std::pair<bool, QVector<int>> parseJSONScene(QString json);

private:
    GraphicParser                                      _parser;
    GraphicTypes::building<GraphicPoint>  _points;
    GraphicTypes::building<GraphicLine>   _lines;
    GraphicTypes::building<GraphicPolygon>   _polygons;
    GraphicTypes::floor<GraphicItem>   _temp;
    QString                                                _jsonScene;
    std::shared_ptr<GraphicPoint>           _tempPoint;
    QPolygonF                                          _tempPoly;
    std::shared_ptr<QPointF>                   _tempPos;
    uint8_t                                                 _pointSize;
};
