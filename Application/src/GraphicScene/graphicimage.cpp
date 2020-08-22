#include <cmath>
#include "GraphicScene/graphicimage.h"
#include <QDebug>

GraphicImage::GraphicImage(const double x, const double y, const QString path)
    : GraphicItem(x, y, 0, 0),
      _image(path),
      _topLeft(x, y)
{
    _boundingRect.setSize(_image.size());
}

GraphicImage::GraphicImage(const QPointF &pos, const QString path)
    : GraphicItem(pos, QPointF()),
      _image(path),
      _topLeft(pos)
{
    _boundingRect.setSize(_image.size());
}

QPointF GraphicImage::pos()
{
    return _topLeft;
}

bool GraphicImage::redrawRequest(const QRectF &changeArea)
{
    // TODO добавить расчет sorce и target для метода painter->drawImage
    return _boundingRect.intersects(changeArea);
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
    //TODO Предварительно прощитывать настоящее положение
    painter->drawImage(QRectF(_topLeft.x() * scale - offset.x(), _topLeft.y() * scale -offset.y(), _image.width() * scale, _image.height() * scale), _image);
}

void GraphicImage::moveTo(const QPointF &offset)
{
    _topLeft = offset;
    _boundingRect.setTopLeft(_topLeft);
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
