#pragma once

#include <GraphicScene/graphicpoint.h>
#include <GraphicScene/graphicline.h>
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
    std::map<int, std::set<std::shared_ptr<GraphicPoint>>>::iterator findPoints(const int floor);
    std::map<int, std::set<std::shared_ptr<GraphicPoint>>>::iterator beginPoints();
    std::map<int, std::set<std::shared_ptr<GraphicPoint>>>::iterator endPoints();
    std::map<int, std::set<std::shared_ptr<GraphicLine>>>::iterator findLines(const int floor);
    std::map<int, std::set<std::shared_ptr<GraphicLine>>>::iterator beginLines();
    std::map<int, std::set<std::shared_ptr<GraphicLine>>>::iterator endLines();
    Q_INVOKABLE QString generateJSONScene();
private:
    std::map<int, std::set<std::shared_ptr<GraphicPoint>>>  _points;
    std::map<int, std::set<std::shared_ptr<GraphicLine>>>   _lines;
    QString                                                                               _jsonScene;
    std::shared_ptr<GraphicPoint>                                          _tempPoint;
    uint8_t                         _pointSize;
};
