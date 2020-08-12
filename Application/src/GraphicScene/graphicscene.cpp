#include "GraphicScene/graphicscene.h"
#include <cmath>
#include <QtMath>
#include <QSGFlatColorMaterial>
#include <QSGSimpleTextureNode>
#include <QImage>
#include <QQuickWindow>

GraphicScene::GraphicScene(QQuickItem *parent):
    QQuickPaintedItem(parent),
    _backgroundColor(Qt::black),
    _canvasWindow(),
    _changeArea(_canvasWindow),
    _offset(0, 0),
    _dragPoint(0, 0),
    _canvasWidth(),
    _canvasHeight(),
    _floor(1),
    _pointSize(5),
    _gridSize(16),
    _scale(2),
    _lineBegins(false),
    _isDragging(false),
    _ctrlPressed(false)
{
    _points.emplace(std::make_pair(_floor, std::set<std::shared_ptr<GraphicPoint>>{}));
    _lines.emplace(std::make_pair(_floor, std::set<std::shared_ptr<GraphicLine>>{}));
    _cursorPoint = std::make_unique<GraphicPoint>(0, 0, _pointSize, 2, QColor("#A60000"), QColor("#FF9E00"));

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
    if (_scale > 1)
    {
        drawGrid(painter);
    }

    //TODO избавиться от changeArea пока
    painter->setRenderHint(QPainter::Antialiasing, true);
    auto currentFloorLines = _lines.find(_floor);
    if (currentFloorLines != _lines.end())
    {
        for (auto item: currentFloorLines->second)
        {
            if (item->redrawRequest(_canvasWindow))
            {
                item->paint(painter, _offset, _scale);
            }
        }
    }

    if (!_isDragging)
    {
        painter->setRenderHint(QPainter::Antialiasing, false);
        auto currentFloorPoints = _points.find(_floor);
        if (currentFloorPoints != _points.end())
        {
            for (auto item: currentFloorPoints->second)
            {
                if (item->redrawRequest(_canvasWindow))
                {
                    item->paint(painter, _offset, _scale);
                }
            }
        }

        _cursorPoint->paint(painter, _offset, _scale);
    }
}

QString GraphicScene::name() const
{

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

}

void GraphicScene::setName(const QString name)
{

}

void GraphicScene::setBackgroundColor(const QColor backgroundColor)
{

}

void GraphicScene::setScale(const int scale)
{

}

void GraphicScene::setOffset(const QPointF offset)
{

}

void GraphicScene::setFloor(const int floor)
{

}

void GraphicScene::reset()
{

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

    auto currentFloor = _lines.find(_floor);
    if (currentFloor == _lines.end())
    {
        return false;
    }

    double newX = startX;
    double newY = startY;
    double minDistance = std::numeric_limits<double>::max();
    virtPos = virtPos / _scale;

    for (auto item: currentFloor->second)
    {
        if (item->pointInArea(startX, startY))
        {
          double pointCrossX = item->getXbyY(startY);
          double pointCrossY = item->getYbyX(startX);
          double distanceCrossX = std::hypot((pointCrossX - virtPos.x()),
                                            (startY - virtPos.y()));
          double distanceCrossY = std::hypot((startX - virtPos.x()),
                                             (pointCrossY - virtPos.y()));

          if (distanceCrossX <= distanceCrossY) {
            if (distanceCrossX < minDistance) {
              newX = pointCrossX;
              newY = startY;
              minDistance = distanceCrossX;
            }
          } else if (distanceCrossY < minDistance) {
            newX = startX;
            newY = pointCrossY;
            minDistance = distanceCrossY;
          }
        }
    }

    extendChangeArea(_cursorPoint->boundingRect());
    _cursorPoint->moveTo(newX, newY);

    return true;
}

std::shared_ptr<GraphicPoint> GraphicScene::findPoint(const QPointF &pos)
{
    //TODO передавать нужный этаж
    auto currentFloor = _points.find(_floor);
    if (currentFloor == _points.end())
    {
        return nullptr;
    }

    for (auto item : currentFloor->second)
    {
        if (item->wasClicked(pos))
        {
            return item;
        }
    }
    return nullptr;
}

std::shared_ptr<GraphicLine> GraphicScene::findLine(const QPointF &pos)
{
    //TODO передавать нужный этаж
    auto currentFloor = _lines.find(_floor);
    if (currentFloor == _lines.end())
    {
        return nullptr;
    }

    for (auto item : currentFloor->second)
    {
        if (item->wasClicked(pos))
        {
            return item;
        }
    }
    return nullptr;
}

bool GraphicScene::addPoint(const QPointF &pos)
{
    _tempPoint = findPoint(pos);
    if (_tempPoint == nullptr)
    {
        _tempPoint = std::make_shared<GraphicPoint>(pos, _pointSize, 2);
        _points[_floor].insert(_tempPoint);
    }
    _lineBegins = true;
    return true;
}

bool GraphicScene::addLine(const QPointF &pos)
{
    if (_lineBegins)
    {
        auto currentFloor = _points.find(_floor);
        if (currentFloor == _points.end())
        {
            return false;
        }

        if (_tempPoint->wasClicked(pos))
        {
            return false;
        }

        _lines[_floor].emplace(std::make_shared<GraphicLine>(_tempPoint->pos(), pos));
        currentFloor->second.erase(_tempPoint);
        currentFloor->second.erase(findPoint(pos));
        _tempPoint = nullptr;
        _lineBegins = false;

        return true;
    }

    return false;
}

bool GraphicScene::deleteItem(const QPointF &pos)
{
    auto point = findPoint(pos);

    if (point != nullptr)
    {
        extendChangeArea(point->boundingRect());
        _points[_floor].erase(point);

        return true;
    }

    auto line = findLine(pos);

    if (line != nullptr)
    {
        extendChangeArea(line->boundingRect());
        addPoint(line->getFirstPoint());
        addPoint(line->getSecondPoint());
        _lines[_floor].erase(line);

        return true;
    }

    return false;
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

void GraphicScene::drawGrid(QPainter *painter)
{
    double x1 = _changeArea.x();
    double y1 = _changeArea.y();
    double x2 = x1 + _changeArea.width();
    double y2 = y1 + _changeArea.height();
    double startX = std::round((x1 + _offset.x()) / (_gridSize * _scale)) *
                                  (_gridSize * _scale) - _offset.x();
    double startY = std::round((y1 + _offset.y()) / (_gridSize * _scale)) *
                                  (_gridSize * _scale) - _offset.y();

    painter->setPen("#AAAAAA");
    for (double i = startY; i <= y2; i += _gridSize * _scale)
    {
        painter->drawLine(static_cast<int>(x1),
                          static_cast<int>(i),
                          static_cast<int>(x2),
                          static_cast<int>(i));
    }
    for (double i = startX; i <= x2; i += _gridSize * _scale)
    {
        painter->drawLine(static_cast<int>(i),
                          static_cast<int>(y1),
                          static_cast<int>(i),
                          static_cast<int>(y2));
    }
}

void GraphicScene::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    (void)oldGeometry;
    _canvasWindow = newGeometry;
    _canvasWidth = _canvasWindow.width();
    _canvasHeight = _canvasWindow.height();
    _changeArea = _canvasWindow;
    update();
}

void GraphicScene::mousePressEvent(QMouseEvent *event)
{
    bool result = false;
    switch (event->button())
    {
    case Qt::LeftButton:
        result = addPoint(_cursorPoint->pos());
        break;
    case Qt::RightButton:
        result = deleteItem(_cursorPoint->pos());
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
    if (event->button() == Qt::LeftButton)
    {
        result = addLine(_cursorPoint->pos());
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
    } else if (_ctrlPressed)
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
    if (_ctrlPressed)
    {
        result = lineAttachment(event->posF());
    } else
    {
        result = cursorShadow(event->posF());
    }

    if (result)
    {
        update();
    }
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


