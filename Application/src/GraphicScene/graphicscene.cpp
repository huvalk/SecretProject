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
    _floor(1),
    _canvasWidth(),
    _canvasHeight(),
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

    _cursorPoint->paint(painter, _offset, _scale);
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

    update();
}

void GraphicScene::zoomOut(const QPoint &pos)
{
    _scale /= 2;
    _offset = _offset / 2 - pos / 2;
    _canvasWindow.setRect(_offset.x() / _scale,
                          _offset.y() / _scale,
                          static_cast<int>(_canvasWidth / _scale),
                          static_cast<int>(_canvasHeight / _scale));

    update();
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

    update();
}

void GraphicScene::dragEnds(const QPoint &pos)
{
    (void)pos;

    _isDragging = false;
    _cursorPoint->setVisable(true);
}

void GraphicScene::cursorShadow(const QPointF& pos)
{
    QPointF virtPos = getMousePosition(pos);
    float startX = std::round(static_cast<float>(virtPos.x()) / (_gridSize * _scale)) *
                            (_gridSize);
    float startY = std::round(static_cast<float>(virtPos.y()) / (_gridSize * _scale)) *
                            (_gridSize);

    if (_cursorPoint->wasClicked(startX, startY, _scale))
    {
        return;
    }

//    extendChangeArea(_cursorPoint->boundingRect());
    _cursorPoint->moveTo(static_cast<int>(std::round(startX)), static_cast<int>(std::round(startY)));

    update();
}

void GraphicScene::lineAttachment(const QPointF &pos)
{
    QPointF virtPos = getMousePosition(pos);
    float startX = std::round(static_cast<float>(virtPos.x()) / (_gridSize * _scale)) *
            (_gridSize);
    float startY = std::round(static_cast<float>(virtPos.y()) / (_gridSize * _scale)) *
            (_gridSize);

    if (_cursorPoint->wasClicked(startX, startY, _scale))
    {
        return;
    }

    auto currentFloor = _lines.find(_floor);
    if (currentFloor == _lines.end())
    {
        return;
    }

    float newX = startX;
    float newY = startY;
    float minDistance = std::numeric_limits<float>::max();
    virtPos = virtPos / _scale;

    for (auto item: currentFloor->second)
    {
        if (item->pointInArea(startX, startY))
        {
          float pointCrossX = item->getXbyY(startY);
          float pointCrossY = item->getYbyX(startX);
          float distanceCrossX = std::hypotf((pointCrossX - static_cast<float>(virtPos.x())),
                                            (startY - static_cast<float>(virtPos.y())));
          float distanceCrossY = std::hypotf((startX - static_cast<float>(virtPos.x())),
                                             (pointCrossY - static_cast<float>(virtPos.y())));

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

    newX = std::round(newX);
    newY = std::round(newY);
    extendChangeArea(_cursorPoint->boundingRect());
    _cursorPoint->moveTo(static_cast<int>(newX), static_cast<int>(newY));
    update();
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

void GraphicScene::addPoint(const QPointF &pos)
{
    _tempPoint = findPoint(pos);
    if (_tempPoint == nullptr)
    {
        _tempPoint = std::make_shared<GraphicPoint>(pos.toPoint(), _pointSize, 2);
        _points[_floor].insert(_tempPoint);
    }
    _lineBegins = true;
}

void GraphicScene::addLine(const QPointF &pos)
{
    if (_lineBegins)
    {
        auto currentFloor = _points.find(_floor);
        if (currentFloor == _points.end())
        {
            return;
        }

        if (!_tempPoint->wasClicked(pos))
        {
            _lines[_floor].emplace(std::make_shared<GraphicLine>(_tempPoint->pos(), pos.toPoint()));
            currentFloor->second.erase(_tempPoint);
            currentFloor->second.erase(findPoint(pos));
            //TODO Утечка?
            _tempPoint = nullptr;
            _lineBegins = false;
        }
    }
}

void GraphicScene::deleteItem(const QPointF &pos)
{
    auto point = findPoint(pos);

    if (point != nullptr)
    {
        extendChangeArea(point->boundingRect());
        _points[_floor].erase(point);

        return;
    }

    auto line = findLine(pos);

    if (line != nullptr)
    {
        extendChangeArea(line->boundingRect());
        addPoint(line->getFirstPoint());
        addPoint(line->getSecondPoint());
        _lines[_floor].erase(line);
    }
}

void GraphicScene::extendChangeArea(const QRect &newRect)
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
    int x1 = _changeArea.x();
    int y1 = _changeArea.y();
    int x2 = x1 + _changeArea.width();
    int y2 = y1 + _changeArea.height();
    int startX = static_cast<int>(std::round((x1 + _offset.x()) / (_gridSize * _scale)) *
                                  (_gridSize * _scale) - _offset.x());
    int startY = static_cast<int>(std::round((y1 + _offset.y()) / (_gridSize * _scale)) *
                                  (_gridSize * _scale) - _offset.y());

    painter->setPen("#AAAAAA");
    for (int i = startY; i <= y2; i += _gridSize * _scale)
    {
        painter->drawLine(x1 - 2, i, x2 + 2, i);
    }
    for (int i = startX; i <= x2; i += _gridSize * _scale)
    {
        painter->drawLine(i, y1 - 2, i, y2 + 2);
    }
}

void GraphicScene::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    _canvasWindow = newGeometry.toRect();
    _canvasWidth = _canvasWindow.width();
    _canvasHeight = _canvasWindow.height();
    _changeArea = _canvasWindow;
    update();
}

void GraphicScene::mousePressEvent(QMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
        addPoint(_cursorPoint->pos());
        break;
    case Qt::RightButton:
        deleteItem(_cursorPoint->pos());
        break;
    default:
        break;
    }

    if (event->button() == Qt::MidButton)
    {
        dragBegins(event->pos());
    }
}

void GraphicScene::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        addLine(_cursorPoint->pos());
    }

    if (event->button() == Qt::MidButton)
    {
        dragEnds(event->pos());
    }
}

void GraphicScene::mouseMoveEvent(QMouseEvent *event)
{
    if (_isDragging)
    {
        dragMove(event->pos());
    } else if (_ctrlPressed)
    {
        lineAttachment(event->pos());
    } else
    {
        cursorShadow(event->pos());
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
}

void GraphicScene::hoverMoveEvent(QHoverEvent *event)
{
    if (_ctrlPressed)
    {
        lineAttachment(event->posF());
    } else
    {
        cursorShadow(event->posF());
    }
}

void GraphicScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Control:
        _ctrlPressed = true;
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
    qDebug() << "focus";
}


