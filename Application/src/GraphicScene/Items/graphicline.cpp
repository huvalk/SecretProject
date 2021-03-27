#include <cmath>
#include "GraphicScene/Items/graphicline.h"
#include <QDebug>

GraphicLine::GraphicLine(const QPointF& firstPoint, const QPointF& secondPoint)
    : GraphicItem(firstPoint, secondPoint, true),
      _fillStyle("#FF4040"),
//      _firstPoint(firstPoint), _secondPoint(secondPoint),
      _line(firstPoint, secondPoint),
      _width(3)
{
    //TODO Проверить создание _boundingRect
}

QPointF GraphicLine::pos()
{
    qWarning() << "Использование GraphicLine::pos() не рекомендуется";
    return _line.p1();
}

bool GraphicLine::redrawRequest(const QRectF &changeArea)
{
    qDebug() << _boundingRect;
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
        double r1 = std::hypot((nx - _line.p1().x()),
                               (ny - _line.p1().y()));
        double r2 = std::hypot((nx - _line.p2().x()),
                               (ny - _line.p2().y()));
        double r12 = std::hypot((_line.p1().x() - _line.p2().x()),
                                (_line.p1().y() - _line.p2().y()));

        if ((r1 <= accuracy) || (r2 <= accuracy))
        {
            return true;
        } else if ((r1 < std::hypot(r2, r12)) && (r2 < std::hypot(r1, r12)))
        {
            double a = _line.p2().y() - _line.p1().y();
            double b = _line.p1().x() - _line.p2().x();
            double c = - (_line.p1().x() * a + _line.p1().y() * b);
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
    painter->drawLine(static_cast<int>(_line.p1().x() * scale - offset.x()),
                      static_cast<int>(_line.p1().y() * scale - offset.y()),
                      static_cast<int>(_line.p2().x() * scale - offset.x()),
                      static_cast<int>(_line.p2().y() * scale - offset.y()));
}

void GraphicLine::moveTo(const QPointF &offset)
{
    // перемещение всегда относительно первой точки
    _line.setP2(offset + _line.p2() - _line.p1());
    _line.setP1(offset);
    _boundingRect.moveTo(offset.x(), offset.y());
}

void GraphicLine::moveTo(const double x, const double y)
{
    // перемещение всегда относительно первой точки
    _line.setP2( QPointF(
                     (x + _line.p2().x() - _line.p1().x()),
                     (y + _line.p2().y() - _line.p1().y())
                     )
                 ) ;
    _line.setP1(QPointF(x, y)) ;
    _boundingRect.moveTo(x, y);
}

double GraphicLine::getYbyX(double x) const
{
    return ((x - _line.p1().x()) *
            (_line.p1().y() - _line.p2().y()) /
            (_line.p1().x() - _line.p2().x()) +
            _line.p1().y());
}

double GraphicLine::getXbyY(double y) const
{
    return ((y - _line.p1().y()) *
            (_line.p1().x() - _line.p2().x()) /
            (_line.p1().y() - _line.p2().y()) +
            _line.p1().x());
}

const QPointF  GraphicLine::getFirstPoint() const
{
    return _line.p1();
}

const QPointF  GraphicLine::getSecondPoint() const
{
    return _line.p2();
}

void GraphicLine::write(QJsonObject &json) const
{
    json["x1"] = _line.p1().x();
    json["y1"] = _line.p1().y();
    json["x2"] = _line.p2().x();
    json["y2"] = _line.p2().y();
}

void GraphicLine::read(const QJsonObject &json)
{
    if (json.contains("x1") && json.contains("y1"))
    {
        _line.setP1(QPointF(
                        json["x1"].toDouble(0),
                        json["y1"].toDouble(0)
                        ));
    } else {
        qWarning() << "Missing first point in line";
    }

    if (json.contains("x2") && json.contains("y2"))
    {
        _line.setP2(QPointF(
                        json["x2"].toDouble(0),
                        json["y2"].toDouble(0)
                        ));
    } else {
        qWarning() << "Missing second point in line";
    }

    _boundingRect = QRectF(_line.p1(), _line.p2()).normalized();
}

GraphicLine::~GraphicLine()
{
}
