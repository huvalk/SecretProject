#include <cmath>
#include "GraphicScene/graphicline.h"
#include <QDebug>

GraphicLine::GraphicLine(QPoint firstPoint, QPoint secondPoint)
    : GraphicItem(QRect(firstPoint, secondPoint), true),
      _fillStyle("#FF4040"),
      _firstPoint(firstPoint), _secondPoint(secondPoint),
      _width(3)
{
    //TODO Проверить создание _boundingRect
}

QPoint GraphicLine::pos()
{
    qWarning() << "Использование GraphicLine::pos() не рекомендуется";
    return _firstPoint;
}

bool GraphicLine::redrawRequest(const QRect &changeArea)
{
    return _boundingRect.intersects(changeArea);
}

bool GraphicLine::wasClicked(const QPointF &pos, const uint8_t scale)
{
    return wasClicked(static_cast<float>(pos.x()), static_cast<float>(pos.y()), scale);
}

bool GraphicLine::wasClicked(const float x, const float y, const uint8_t scale)
{
    float nx = x / scale;
    float ny = y / scale;

    if (_boundingRect.contains(static_cast<int>(nx), static_cast<int>(ny)))
    {
        float accuracy = std::ceil(static_cast<float>(_width / 2));
        float r1 = std::hypotf((nx - _firstPoint.x()),
                               (ny - _firstPoint.y()));
        float r2 = std::hypotf((nx - _secondPoint.x()),
                               (ny - _secondPoint.y()));
        float r12 = std::hypotf((_firstPoint.x() - _secondPoint.x()),
                                (_firstPoint.y() - _secondPoint.y()));

        if ((r1 <= accuracy) || (r2 <= accuracy))
        {
            return true;
        } else if ((r1 < std::hypot(r2, r12)) && (r2 < std::hypot(r1, r12)))
        {
            float a = _secondPoint.y() - _firstPoint.y();
            float b = _firstPoint.x() - _secondPoint.x();
            float c = - (_firstPoint.x() * a + _firstPoint.y() * b);
            float t = std::hypot(a, b);

            if (c > 0)
            {
                a = -a;
                b = -b;
                c = -c;
            }

            if ((std::abs((a * nx +
                           b * ny + c) / t)) <= accuracy)
            {
              return true;
            }
        }
    }

    return false;
}

GraphicType GraphicLine::type()
{
    return GraphicType::Line;
}

void GraphicLine::paint(QPainter *painter, const QPoint &offset, const uint8_t scale)
{
    painter->setBrush(_fillStyle);
    painter->drawLine(_firstPoint.x() * scale - offset.x(),
                      _firstPoint.y() * scale - offset.y(),
                      _secondPoint.x() * scale - offset.x(),
                      _secondPoint.y() * scale - offset.y());
}

void GraphicLine::moveTo(const QPoint &offset)
{
    _secondPoint = offset + _secondPoint - _firstPoint;
    _firstPoint = offset;
    _boundingRect.moveTo(static_cast<int>(offset.x()),
                         static_cast<int>(offset.y()));
}

void GraphicLine::moveTo(const int x, const int y)
{
    _secondPoint.setX(x + _secondPoint.x() - _firstPoint.x());
    _secondPoint.setY(y + _secondPoint.y() - _firstPoint.y());
    _firstPoint.setX(x);
    _firstPoint.setY(y);
    _boundingRect.moveTo(x, y);
}

float GraphicLine::getYbyX(float x)
{
    return ((x - _firstPoint.x()) *
            (_firstPoint.y() - _secondPoint.y()) /
            (_firstPoint.x() - _secondPoint.x()) +
            _firstPoint.y());
}

float GraphicLine::getXbyY(float y)
{
    return ((y - _firstPoint.y()) *
            (_firstPoint.x() - _secondPoint.x()) /
            (_firstPoint.y() - _secondPoint.y()) +
            _firstPoint.x());
}

const QPoint&  GraphicLine::getFirstPoint()
{
    return _firstPoint;
}

const QPoint&  GraphicLine::getSecondPoint()
{
    return _secondPoint;
}

GraphicLine::~GraphicLine()
{
}
