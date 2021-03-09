#include <cmath>
#include "GraphicScene/Items/graphicimage.h"
#include <QDebug>

GraphicImage::GraphicImage(const double x, const double y, const QString path)
    : GraphicItem(x, y, 0, 0),
      _image(path),
      _topLeft(x, y)
{
    _boundingRect.setSize(_image.size());
    _source = _boundingRect;
    _target = _boundingRect;
}

GraphicImage::GraphicImage(const QPointF &pos, const QString path)
    : GraphicItem(pos, QPointF()),
      _image(path),
      _topLeft(pos)
{
    _boundingRect.setSize(_image.size());
    _source = _boundingRect;
    _target = _boundingRect;
}

QPointF GraphicImage::pos()
{
    return _topLeft;
}

bool GraphicImage::redrawRequest(const QRectF &changeArea)
{
    auto tempRect = _boundingRect.intersected(changeArea);
    if (tempRect.isNull())
    {
        return false;
    }
    _target = tempRect;
//    _source = tempRect;
    _target.moveTo(_target.topLeft() - _topLeft);

    return true;
}

bool GraphicImage::wasClicked(const QPointF &pos, const uint8_t scale)
{
    return wasClicked(pos.x(), pos.y(), scale);
}

bool GraphicImage::wasClicked(const double x, const double y, const uint8_t scale)
{
    double nx = x / scale;
    double ny = y / scale;

    return _boundingRect.contains(nx, ny);
}

GraphicTypes::GraphicItems GraphicImage::type()
{
    return GraphicTypes::GraphicItems::Image;
}

void GraphicImage::paint(QPainter *painter, const QPointF &offset, const uint8_t scale, const bool extColor)
{
    (void)extColor;
    painter->drawImage(QRectF(_boundingRect.topLeft().x() * scale - offset.x(), _boundingRect.topLeft().y() * scale - offset.y(), _boundingRect.width() * scale, _boundingRect.height() * scale),
                       _image, _source);
}

void GraphicImage::moveTo(const QPointF &offset)
{
    _topLeft = offset;
    _boundingRect.moveTo(_topLeft);
}

void GraphicImage::moveTo(const double x, const double y)
{
    moveTo(QPointF(x, y));
}

void GraphicImage::setImage(const QString &path)
{
    _image = QImage(path);
    _boundingRect.setSize(_image.size());
}

GraphicImage::~GraphicImage()
{
}
