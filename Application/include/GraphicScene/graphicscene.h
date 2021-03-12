#pragma once

#include <GraphicScene/graphiccontainer.h>
#include <GraphicScene/graphicconstructor.h>
#include <GraphicScene/graphicpainter.h>
#include <GraphicScene/Items/graphicpoint.h>
#include <GraphicScene/Items/graphicline.h>
#include <GraphicScene/Items/graphicimage.h>
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
    Q_PROPERTY(QPointF offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(int scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(int floor READ floor WRITE setFloor NOTIFY floorChanged)

public:
    explicit GraphicScene(QQuickItem *parent = nullptr);
    static void registerMe(const std::string& moduleName);
    void paint(QPainter* painter) override;

    QString name() const;
    QColor backgroundColor() const;
    int scale() const;
    QPointF offset() const;
    int floor() const;
    Q_INVOKABLE QString generateJSONScene();
    Q_INVOKABLE bool parseJSONScene(QString json);

public slots:
    void setScale(const int scale);
    void setOffset(const QPointF offset);
    void setFloor(const int floor);
    void setBackground(const QString path);
    void setEditingMod(const int mod);
    void setCursorMod(const int mod);
    void setBackgroundVisible(const bool is);
    void setBackgroundFloor(const int floor);
    void setBackgroundFloorVisible(const bool is);

signals:
    void scaleChanged(const int scale);
    void offsetChanged(const QPointF offset);
    void floorChanged(const int floor);
    void mapChanged(QVector<int> floors);

private:
    enum EditingMod {
        CreateWalls = 0,
        MoveBackground,
        CreateCamera,
        CreateMark,
        Nothing
    };
    enum CursorMod {
        NoCursor = 0,
        MagniteToGrid,
        MagniteToWalls
    };
    QRectF                         _canvasWindow;
    QRectF                         _changeArea;
    QPointF                        _offset;
    QPoint                          _dragPoint;
    std::shared_ptr<GraphicPoint> _cursorPoint;
    std::shared_ptr<GraphicImage>_image;
    GraphicContainer         _container;
    GraphicConstructor      _constructor;
    double                          _canvasWidth;
    double                          _canvasHeight;
    int                                _floor;
    int                                _backgroundFloor;
    int                                _editingMod;
    int                                _cursorMod;
    uint8_t                         _gridSize;
    uint8_t                         _scale;
    bool                              _lineBegins;
    bool                              _polyBegins;
    bool                             _isDragging;
    bool                             _isBackgroundDragging;
    bool                              _backgroundVisible;
    bool                              _backgroundFloorVisible;
    bool                              _ctrlPressed;

    void reset();
    QPointF getMousePosition(const QPointF& point);
    QPoint getMousePosition(const QPoint& point);
    void zoomIn(const QPoint& pos);
    void zoomOut(const QPoint& pos);
    void dragBegins(const QPoint& pos);
    void dragMove(const QPoint& pos);
    void dragEnds(const QPoint& pos);
    void dragBackgroundBegins(const QPoint& pos);
    void dragBackgroundMove(const QPoint& pos);
    void dragBackgroundEnds(const QPoint& pos);
    bool cursorShadow(const QPointF& pos);
    bool lineAttachment(const QPointF& pos);
    void drawGrid(QPainter* painter);
    void extendChangeArea(const QRectF& newRect);

    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent * event) override;
};
