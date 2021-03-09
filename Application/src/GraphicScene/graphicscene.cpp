#include "GraphicScene/graphicscene.h"
#include "GraphicScene/graphicpainter.h"
#include <cmath>
#include <QtMath>
#include <QSGFlatColorMaterial>
#include <QSGSimpleTextureNode>
#include <QImage>
#include <QQuickWindow>

GraphicScene::GraphicScene(QQuickItem *parent):
    QQuickPaintedItem(parent),
    _canvasWindow(),
    _changeArea(_canvasWindow),
    _offset(0, 0),
    _dragPoint(0, 0),
    _image(std::make_shared<GraphicImage>(0, 0, "")),
    _canvasWidth(),
    _canvasHeight(),
    _floor(1),
    _backgroundFloor(1),
    _editingMod(EditingMod::CreateWalls),
    _cursorMod(CursorMod::MagniteToGrid),
    _gridSize(8),
    _scale(2),
    _lineBegins(false),
    _polyBegins(false),
    _isDragging(false),
    _isBackgroundDragging(false),
    _backgroundVisible(false),
    _backgroundFloorVisible(false),
    _ctrlPressed(false)
{
    _cursorPoint = std::make_shared<GraphicPoint>(0, 0, 5, 2, QColor("#A60000"), QColor("#FF9E00"));

    forceActiveFocus();
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemHasContents);
}

void GraphicScene::registerMe(const std::string& moduleName)
{
    qmlRegisterType<GraphicScene>(moduleName.c_str(), 1, 0, "GraphicScene");
}

void GraphicScene::paint(QPainter* painter)
{
    GraphicPainter::paint(painter,
                   _container,
                   _cursorPoint,
                   _image,
                    (_editingMod != EditingMod::Nothing),
                   _backgroundVisible,
                   _backgroundFloorVisible,
                   _isDragging,
                   _scale,
                   _gridSize,
                   _offset,
                   _floor,
                   _backgroundFloor,
                   _canvasWindow,
                   _changeArea);
}

QColor GraphicScene::backgroundColor() const
{

}

int GraphicScene::scale() const
{

}

QPointF GraphicScene::offset() const
{

}

int GraphicScene::floor() const
{
    return _floor;
}

QString GraphicScene::generateJSONScene()
{
    return _container.generateJSONScene();
}

bool GraphicScene::parseJSONScene(QString json)
{
    auto result = _container.parseJSONScene(json);
    if (result.first)
    {
        emit mapChanged(result.second);
        update();
    }

    return result.first;
}

void GraphicScene::setBackground(const QString path)
{
    QUrl url(path);

    _image->setImage(url.path());
    update();
}

void GraphicScene::setEditingMod(const int mod)
{
    _editingMod = mod;
    _container.clearPoints(_floor);
    qDebug() << _editingMod;
}

void GraphicScene::setCursorMod(const int mod)
{
    _cursorMod = mod;
}

void GraphicScene::setBackgroundVisible(const bool is)
{
    _backgroundVisible = is;
    update();
}

void GraphicScene::setBackgroundFloor(const int floor)
{
    _backgroundFloor = floor;
    update();
}

void GraphicScene::setBackgroundFloorVisible(const bool is)
{
    _backgroundFloorVisible = is;
    update();
}

void GraphicScene::setScale(const int scale)
{

}

void GraphicScene::setOffset(const QPointF offset)
{

}

void GraphicScene::setFloor(const int floor)
{
    if (_floor == floor)
    {
        return;
    }

    _floor = floor;
    update();
    emit floorChanged(floor);
}

void GraphicScene::reset()
{
    _dragPoint.setX(0);
    _dragPoint.setY(0);
    _isDragging = false;
    _isBackgroundDragging = false;
    // TODO все ли сбрасывается?
}

QPointF GraphicScene::getMousePosition(const QPointF &point)
{
    return point + _offset;
}

void GraphicScene::zoomIn(const QPoint& pos)
{
    _scale *= 2;
    _offset = 2 * _offset + pos;
    _canvasWindow.setRect(_offset.x() / _scale,
                          _offset.y() / _scale,
                          static_cast<int>(_canvasWidth / _scale),
                          static_cast<int>(_canvasHeight / _scale));
}

void GraphicScene::zoomOut(const QPoint &pos)
{
    _scale /= 2;
    _offset = _offset / 2 - pos / 2;
    _canvasWindow.setRect(_offset.x() / _scale,
                          _offset.y() / _scale,
                          static_cast<int>(_canvasWidth / _scale),
                          static_cast<int>(_canvasHeight / _scale));
}

void GraphicScene::dragBegins(const QPoint &pos)
{
    _isDragging = true;
    _dragPoint = pos;
    _cursorPoint->setVisable(false);
}

void GraphicScene::dragMove(const QPoint &pos)
{
    _offset += _dragPoint - pos;
    _canvasWindow.moveTo(_offset / _scale);
    _dragPoint = pos;
}

void GraphicScene::dragEnds(const QPoint &pos)
{
    (void)pos;

    _isDragging = false;
    _cursorPoint->setVisable(true);
}

void GraphicScene::dragBackgroundBegins(const QPoint &pos)
{
    _isBackgroundDragging = true;
    _dragPoint = pos;
    _cursorPoint->setVisable(false);
}

void GraphicScene::dragBackgroundMove(const QPoint &pos)
{
    QPointF div = _dragPoint - pos;
    _offset += div;
    _canvasWindow.moveTo(_offset / _scale);
    _image->moveTo(_image->pos() + (div / _scale));
    _dragPoint = pos;
}

void GraphicScene::dragBackgroundEnds(const QPoint &pos)
{
    (void)pos;

    _isBackgroundDragging = false;
    _cursorPoint->setVisable(true);
}

bool GraphicScene::cursorShadow(const QPointF& pos)
{
    QPointF virtPos = getMousePosition(pos);
    double startX = std::round(virtPos.x() / (_gridSize * _scale)) *
                            (_gridSize);
    double startY = std::round(virtPos.y() / (_gridSize * _scale)) *
                            (_gridSize);

    if (_cursorPoint->wasClicked(startX, startY, _scale))
    {
        return false;
    }

    extendChangeArea(_cursorPoint->boundingRect());
    _cursorPoint->moveTo(startX, startY);

    return true;
}

bool GraphicScene::lineAttachment(const QPointF &pos)
{
    QPointF virtPos = getMousePosition(pos);
    double startX = std::round(virtPos.x() / (_gridSize * _scale)) *
            (_gridSize);
    double startY = std::round(virtPos.y() / (_gridSize * _scale)) *
            (_gridSize);

    if (_cursorPoint->wasClicked(startX, startY, _scale))
    {
        return false;
    }

    bool result = false;
    std::tie(result, virtPos) = _container.lineAttachment(_floor, startX, startY, virtPos / _scale);

    if (result == true)
    {
        extendChangeArea(_cursorPoint->boundingRect());
        _cursorPoint->moveTo(virtPos);
    }

    return result;
}


void GraphicScene::extendChangeArea(const QRectF &newRect)
{
    if (_changeArea.isEmpty())
    {
        _changeArea = newRect;
    } else
    {
        _changeArea = _changeArea.united(newRect);
    }
}

void GraphicScene::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    (void)oldGeometry;
    _changeArea = newGeometry;
    _canvasWidth = newGeometry.width();
    _canvasHeight = newGeometry.height();
    _canvasWindow.setWidth(_canvasWidth / _scale);
    _canvasWindow.setHeight(_canvasHeight / _scale);
    update();
}

void GraphicScene::mousePressEvent(QMouseEvent *event)
{
    bool result = false;
    switch (_editingMod)
    {
    case EditingMod::CreateWalls:
        switch (event->button())
        {
        case Qt::LeftButton:
            std::tie(result, std::ignore) = _container.addPoint(_floor, _cursorPoint->pos());
            _lineBegins = true;
            break;
        case Qt::RightButton:
            std::tie(result, std::ignore)  = _container.deleteItem(_floor, _cursorPoint->pos());
            break;
        default:
            break;
        }
        break;

    case EditingMod::CreateCamera:
        switch (event->button())
        {
        case Qt::LeftButton:
            if (_polyBegins == false)
            {
                qDebug() << "start creating";
                std::tie(result, std::ignore) = _container.addTempPoint(_cursorPoint->pos());
                _polyBegins = true;
            }
            break;
        case Qt::RightButton:
            if (_polyBegins == true)
            {
                _polyBegins = false;
                _container.clearTemp();
            } else {
                std::tie(result, std::ignore)  = _container.deleteItem(_floor, _cursorPoint->pos());
            }
            break;
        default:
            break;
        }
        break;

    case EditingMod::MoveBackground:
        switch (event->button())
        {
        case Qt::LeftButton:
            dragBackgroundBegins(event->pos());
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    if (event->button() == Qt::MidButton)
    {
        dragBegins(event->pos());
    }

    if (result)
    {
        update();
    }
}

void GraphicScene::mouseReleaseEvent(QMouseEvent *event)
{
    bool result = false;
    switch (_editingMod)
    {
    case EditingMod::CreateWalls:
        if (event->button() == Qt::LeftButton && _lineBegins)
        {
            std::tie(result, std::ignore)  = _container.addLine(_floor, _cursorPoint->pos());
            _lineBegins = !result;
        }
        break;

    case EditingMod::CreateCamera:
        if (event->button() == Qt::LeftButton && _polyBegins)
        {
            QPolygonF newPoly;
            // TODO обернуть создание компонентов в отдельный класс
            std::tie(result, newPoly, std::ignore)  = _container.addTempLine(_cursorPoint->pos());

            if (newPoly.size() > 0)
            {
                _polyBegins = false;
                if (newPoly.size() > 1)
                {
                    _container.addPolygon(_floor, newPoly);
                }
            }
        }
        break;

    case EditingMod::MoveBackground:
        switch (event->button())
        {
        case Qt::LeftButton:
            dragBackgroundEnds(event->pos());
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    if (event->button() == Qt::MidButton)
    {
        dragEnds(event->pos());
    }

    if (result)
    {
        update();
    }
}

void GraphicScene::mouseMoveEvent(QMouseEvent *event)
{
    bool result = false;
    if (_isDragging)
    {
        dragMove(event->pos());
        result = true;
    } else if (_isBackgroundDragging)
    {
        dragBackgroundMove(event->pos());
        result = true;
    } else if (_cursorMod == CursorMod::MagniteToWalls)
    {
        result = lineAttachment(event->pos());
    } else
    {
        result = cursorShadow(event->pos());
    }

    if (result)
    {
        update();
    }
}

void GraphicScene::wheelEvent(QWheelEvent *event)
{
    auto delta = event->angleDelta().y();

    if (delta > 0) {
      if (_scale < 16) {
        zoomIn(QPoint(event->x(), event->y()));
      }
    } else if (delta < 0) {
      if (_scale > 1) {
        zoomOut(QPoint(event->x(), event->y()));
      }
    }

    update();
}

void GraphicScene::hoverMoveEvent(QHoverEvent *event)
{
    bool result = false;
    switch (_cursorMod)
    {
    case CursorMod::MagniteToGrid:
            result = cursorShadow(event->posF());
            break;
    case CursorMod::MagniteToWalls:
        result = lineAttachment(event->posF());
        break;
    default:
        break;
    }

    if (result)
    {
        update();
    }
}

void GraphicScene::hoverLeaveEvent(QHoverEvent *event)
{
    reset();
}

void GraphicScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Control:
        _ctrlPressed = true;
        break;
    case Qt::Key_Shift:
        break;
    default:
        break;
    }
}

void GraphicScene::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Control:
        _ctrlPressed = false;
        break;
    default:
        break;
    }
}

void GraphicScene::focusInEvent(QFocusEvent *event)
{
    (void)event;
}
