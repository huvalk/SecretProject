#include <GraphicScene/graphicpainter.h>
#include <GraphicScene/graphiccontainer.h>
#include <QRectF>
#include <cmath>
#include <QtMath>

GraphicPainter::GraphicPainter()
{

}

 void GraphicPainter::paint(QPainter* const painter,
                           const GraphicContainer &container,
                            const GraphicConstructor &constructor,
                           const  std::shared_ptr<GraphicPoint> cursorPoint,
                           const std::shared_ptr<GraphicImage> image,
                           const bool &minimizePicture,
                           const bool &backgroundVisible,
                           const bool &backgroundFloorVisible,
                           const bool &isDragging,
                           const uint8_t &scale,
                           const uint8_t &gridSize,
                           const QPointF &offset,
                           const int &floor,
                           const int &backgroundFloor,
                           const QRectF &canvasWindow,
                           const QRectF &changeArea)
{
    // TODO баг при загрузке до смены масштаба неправильное значение canvasWindow
    if (scale > 1 && minimizePicture)
    {
        drawGrid(painter,
                 scale,
                 gridSize,
                 offset,
                 floor,
                 backgroundFloor,
                 changeArea);
    }

    if (backgroundVisible && image->redrawRequest(canvasWindow))
    {
        image->paint(painter, offset, scale);
    }

    painter->setRenderHint(QPainter::Antialiasing, true);
    if (backgroundFloorVisible && backgroundFloor != floor && minimizePicture)
    {
        painter->setPen(QPen(QBrush("black"), 8, Qt::SolidLine, Qt::RoundCap));
        container.paintLines(backgroundFloor, scale, offset, canvasWindow, painter, true);
    }

    //TODO избавиться от changeArea пока

     painter->setPen(QPen(QBrush("red"), 6, Qt::SolidLine, Qt::RoundCap));
    container.paintLines(floor, scale, offset, canvasWindow, painter, true);
    painter->setBrush(QBrush("blue", Qt::SolidPattern));
    painter->setPen(QPen(QBrush("blue"), 6, Qt::SolidLine, Qt::RoundCap));
    container.paintPolygons(floor, scale, offset, canvasWindow, painter, true);
    painter->setPen(QPen(QBrush("pink"), 1, Qt::SolidLine, Qt::RoundCap));
    painter->setBrush(QBrush("pink", Qt::SolidPattern));
    constructor.paintTemp(scale, offset, canvasWindow, painter, true);

    if (!isDragging)
    {
        painter->setRenderHint(QPainter::Antialiasing, false);
        container.paintPoints(floor, scale, offset, canvasWindow, painter);

        cursorPoint->paint(painter, offset, scale);
    }
}

void GraphicPainter::drawGrid(QPainter *painter,
                              const uint8_t &scale,
                              const uint8_t &gridSize,
                              const QPointF &offset,
                              const int &floor,
                              const int &backgroundFloor,
                              const QRectF &changeArea)
{
    double x1 = changeArea.x();
    double y1 = changeArea.y();
    double x2 = x1 + changeArea.width();
    double y2 = y1 + changeArea.height();
    double startX = std::round((x1 + offset.x()) / (gridSize * scale)) *
                                  (gridSize * scale) - offset.x();
    double startY = std::round((y1 + offset.y()) / (gridSize * scale)) *
                                  (gridSize * scale) - offset.y();

    painter->setPen("#AAAAAA");
    for (double i = startY; i <= y2; i += gridSize * scale)
    {
        painter->drawLine(static_cast<int>(x1),
                          static_cast<int>(i),
                          static_cast<int>(x2),
                          static_cast<int>(i));
    }
    for (double i = startX; i <= x2; i += gridSize * scale)
    {
        painter->drawLine(static_cast<int>(i),
                          static_cast<int>(y1),
                          static_cast<int>(i),
                          static_cast<int>(y2));
    }
}
