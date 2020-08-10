﻿#pragma once

#include <GraphicScene/graphicpoint.h>
#include <GraphicScene/graphicline.h>
#include <map>
#include <memory>
#include <set>
#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QSGGeometryNode>

class GraphicScene : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QPointF offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(int scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(int floor READ floor WRITE setFloor NOTIFY floorChanged)


public:
    explicit GraphicScene(QQuickItem *parent = 0);
    static void registerMe(const std::string& moduleName);
    void paint(QPainter* painter) override;

        // Методы, доступные из QML для ...
//        Q_INVOKABLE void clear();   // ... очистки времени, ...

    QString name() const;
    QColor backgroundColor() const;
    int scale() const;
    QPointF offset() const;
    int floor() const;

public slots:
    void setName(const QString name);
    void setBackgroundColor(const QColor backgroundColor);
    void setScale(const int scale);
    void setOffset(const QPointF offset);
    void setFloor(const int floor);

signals:
    void nameChanged(const QString name);
    void backgroundColorChanged(const QColor backgroundColor);
    void scaleChanged(const int scale);
    void offsetChanged(const QPointF offset);
    void floorChanged(const int floor);

private:
    //TODO Заменить на один мап с очередью на отрисовку
    std::map<int, std::set<std::shared_ptr<GraphicPoint>>> _points;
    std::map<int, std::set<std::shared_ptr<GraphicLine>>> _lines;
    QString                         _name;                 // Название объекта, по большей части до кучи добавлено
    QColor                          _backgroundColor;
    QRectF                          _canvasWindow;
    QRectF                          _changeArea;
    QPointF                         _offset;
    QPoint                          _dragPoint;
    double                          _canvasWidth;
    double                          _canvasHeight;
    std::shared_ptr<GraphicPoint>   _tempPoint;
    std::unique_ptr<GraphicPoint>   _cursorPoint;
    int                             _floor;
    uint8_t                         _pointSize;
    uint8_t                         _gridSize;
    uint8_t                         _scale;
    bool                            _lineBegins;
    bool                            _isDragging;
    bool                            _ctrlPressed;

    void reset();
    QPointF getMousePosition(const QPointF& point);
    QPoint getMousePosition(const QPoint& point);
    void zoomIn(const QPoint& pos);
    void zoomOut(const QPoint& pos);
    void dragBegins(const QPoint& pos);
    void dragMove(const QPoint& pos);
    void dragEnds(const QPoint& pos);
    bool cursorShadow(const QPointF& pos);
    bool lineAttachment(const QPointF& pos);
    std::shared_ptr<GraphicPoint> findPoint(const QPointF& pos);
    std::shared_ptr<GraphicLine> findLine(const QPointF& pos);
    bool addPoint(const QPointF &pos);
    bool addLine(const QPointF &pos);
    bool deleteItem(const QPointF &pos);
    void extendChangeArea(const QRectF& newRect);
    void drawGrid(QPainter* painter);

    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent * event) override;
};