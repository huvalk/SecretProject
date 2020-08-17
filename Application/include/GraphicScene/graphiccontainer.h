#pragma once

#include <GraphicScene/graphicpoint.h>
#include <GraphicScene/graphicline.h>
#include <GraphicScene/graphictypes.h>
#include <GraphicScene/graphicparser.h>
#include <map>
#include <memory>
#include <set>
#include <QPointF>

class GraphicContainer
{
public:
    GraphicContainer();
    void paintLines(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter);
    void paintPoints(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter);

    std::pair<bool, QPointF>  lineAttachment(const int floor, const double startX, const double startY, const QPointF &pos);
    std::shared_ptr<GraphicPoint> findPoint(const int floor, const QPointF& pos);
    std::shared_ptr<GraphicLine> findLine(const int floor, const QPointF& pos);
    std::pair<bool, QRectF> addPoint(const int floor, const QPointF &pos);
    std::pair<bool, QRectF>  addLine(const int floor, const QPointF &pos);
    std::pair<bool, QRectF>  deleteItem(const int floor, const QPointF &pos);
    GraphicTypes::floor<GraphicPoint>::iterator findPoints(const int floor);
    GraphicTypes::floor<GraphicPoint>::iterator beginPoints();
    GraphicTypes::floor<GraphicPoint>::iterator endPoints();
    GraphicTypes::floor<GraphicLine>::iterator findLines(const int floor);
    GraphicTypes::floor<GraphicLine>::iterator beginLines();
    GraphicTypes::floor<GraphicLine>::iterator endLines();
    QString generateJSONScene();
    bool parseJSONScene(QString json);

private:
    GraphicParser                                      _parser;
    GraphicTypes::floor<GraphicPoint>  _points;
    GraphicTypes::floor<GraphicLine>   _lines;
    QString                                                _jsonScene;
    std::shared_ptr<GraphicPoint>           _tempPoint;
    uint8_t                                                 _pointSize;
};
