#include <cmath>
#include "GraphicScene/Items/graphicline.h"
#include <QDebug>

GraphicLine::GraphicLine(const QPointF& firstPoint, const QPointF& secondPoint)
    : GraphicItem(firstPoint, secondPoint, true),
      _fillStyle("#FF4040"),
      _firstPoint(firstPoint), _secondPoint(secondPoint),
      _width(3)
{
    //TODO Проверить создание _boundingRect
}

QPointF GraphicLine::pos()
{
    qWarning() << "Использование GraphicLine::pos() не рекомендуется";
    return _firstPoint;
}

bool GraphicLine::redrawRequest(const QRectF &changeArea)
{
    return _boundingRect.intersects(changeArea);
}

bool GraphicLine::wasClicked(const QPointF &pos, const uint8_t scale)
{
    return wasClicked(pos.x(), pos.y(), scale);
}

bool GraphicLine::wasClicked(const double x, const double y, const uint8_t scale)
{
    double nx = x / scale;
    double ny = y / scale;

    if (_boundingRect.contains(static_cast<int>(nx), static_cast<int>(ny)))
    {
        double accuracy = std::ceil(_width / 2);
        double r1 = std::hypot((nx - _firstPoint.x()),
                               (ny - _firstPoint.y()));
        double r2 = std::hypot((nx - _secondPoint.x()),
                               (ny - _secondPoint.y()));
        double r12 = std::hypot((_firstPoint.x() - _secondPoint.x()),
                                (_firstPoint.y() - _secondPoint.y()));

        if ((r1 <= accuracy) || (r2 <= accuracy))
        {
            return true;
        } else if ((r1 < std::hypot(r2, r12)) && (r2 < std::hypot(r1, r12)))
        {
            double a = _secondPoint.y() - _firstPoint.y();
            double b = _firstPoint.x() - _secondPoint.x();
            double c = - (_firstPoint.x() * a + _firstPoint.y() * b);
            double t = std::hypot(a, b);

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

GraphicTypes::GraphicItems GraphicLine::type()
{
    return GraphicTypes::GraphicItems::Line;
}

void GraphicLine::paint(QPainter *painter, const QPointF &offset, const uint8_t scale, const bool extColor)
{
    if (!extColor)
    {
        painter->setPen(QPen(QBrush(_fillStyle), 5, Qt::SolidLine, Qt::RoundCap));
    }
    painter->drawLine(static_cast<int>(_firstPoint.x() * scale - offset.x()),
                      static_cast<int>(_firstPoint.y() * scale - offset.y()),
                      static_cast<int>(_secondPoint.x() * scale - offset.x()),
                      static_cast<int>(_secondPoint.y() * scale - offset.y()));
}

void GraphicLine::moveTo(const QPointF &offset)
{
    // перемещение всегда относительно первой точки
    _secondPoint = offset + _secondPoint - _firstPoint;
    _firstPoint = offset;
    _boundingRect.moveTo(offset.x(), offset.y());
}

void GraphicLine::moveTo(const double x, const double y)
{
    // перемещение всегда относительно первой точки
    _secondPoint.setX(x + _secondPoint.x() - _firstPoint.x());
    _secondPoint.setY(y + _secondPoint.y() - _firstPoint.y());
    _firstPoint.setX(x);
    _firstPoint.setY(y);
    _boundingRect.moveTo(x, y);
}

double GraphicLine::getYbyX(double x) const
{
    return ((x - _firstPoint.x()) *
            (_firstPoint.y() - _secondPoint.y()) /
            (_firstPoint.x() - _secondPoint.x()) +
            _firstPoint.y());
}

double GraphicLine::getXbyY(double y) const
{
    return ((y - _firstPoint.y()) *
            (_firstPoint.x() - _secondPoint.x()) /
            (_firstPoint.y() - _secondPoint.y()) +
            _firstPoint.x());
}

const QPointF&  GraphicLine::getFirstPoint() const
{
    return _firstPoint;
}

const QPointF&  GraphicLine::getSecondPoint() const
{
    return _secondPoint;
}

void GraphicLine::write(QJsonObject &json) const
{
    json["x1"] = _firstPoint.x();
    json["y1"] = _firstPoint.y();
    json["x2"] = _secondPoint.x();
    json["y2"] = _secondPoint.y();
}

void GraphicLine::read(const QJsonObject &json)
{
    if (json.contains("x1") && json.contains("y1"))
    {
        _firstPoint.setX(json["x1"].toDouble(0));
        _firstPoint.setY(json["y1"].toDouble(0));
    } else {
        qWarning() << "Missing first point in line";
    }

    if (json.contains("x2") && json.contains("y2"))
    {
        _secondPoint.setX(json["x2"].toDouble(0));
        _secondPoint.setY(json["y2"].toDouble(0));
    } else {
        qWarning() << "Missing second point in line";
    }
}

GraphicLine::~GraphicLine()
{
}
