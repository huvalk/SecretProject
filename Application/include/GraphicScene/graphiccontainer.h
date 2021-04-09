#pragma once

#include <GraphicScene/Items/graphicpoint.h>
#include <GraphicScene/Items/graphicline.h>
#include <GraphicScene/graphictypes.h>
#include <GraphicScene/Items/graphicpolygon.h>
#include <map>
#include <array>
#include <memory>
#include <set>
#include <QPointF>
#include "GraphicScene/comparepoints.h"

class GraphicContainer
{
public:
    GraphicContainer();
    void paintPath(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool extColor = false) const;
    void paintPoints(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool extColor = false) const;
    void paintLines(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool bg = false) const;
    void paintPolygons(const int floor, const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool extColor = false) const;

    std::pair<bool, QPointF>  lineAttachment(const int floor, const double startX, const double startY, const QPointF &pos);
    std::shared_ptr<GraphicPoint> findPoint(const int floor, const QPointF& pos);
    std::shared_ptr<GraphicLine> findLine(const int floor, const QPointF& pos);
    QRectF addFrom(const int floor, const QPointF &pos);
    QRectF addTo(const int floor, const QPointF &pos);
    QRectF addPoint(const int floor, const QPointF &pos);
    QRectF  addLine(const int floor, const QLineF &line);
    QRectF addPolygon(const int floor, const QPolygonF &poly);
    std::pair<bool, QRectF>  deleteItem(const int floor, const QPointF &pos);
    bool clearPoints(const int floor);
    GraphicTypes::building<GraphicPoint>::iterator findPoints(const int floor);
    GraphicTypes::building<GraphicPoint>::iterator beginPoints();
    GraphicTypes::building<GraphicPoint>::iterator endPoints();
    GraphicTypes::building<GraphicLine>::iterator findLines(const int floor);
    GraphicTypes::building<GraphicLine>::iterator beginLines();
    GraphicTypes::building<GraphicLine>::iterator endLines();
    QString generateJSONScene();
    std::pair<bool, QVector<int>> parseJSONScene(QString json);
    void findPath();
    void updateCameras(std::unordered_map<u_int64_t, u_int64_t> &cameras);

    QString write();
    void read(QString json);

private:
    GraphicTypes::building<GraphicPoint>  _points;
    GraphicTypes::building<GraphicLine>   _lines;
    GraphicTypes::building<GraphicLine>   _path;
    GraphicTypes::building<GraphicPolygon>   _polygons;
    std::array<std::pair<int, std::shared_ptr<QPointF>>, 2> _fromToPoints;
    QString                                                _jsonScene;
    uint8_t                                                 _pointSize;
};
