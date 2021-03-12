#pragma once

#include <QPainter>
#include <memory>
#include <GraphicScene/Items/graphicimage.h>
#include <GraphicScene/Items/graphicpoint.h>
#include <GraphicScene/graphiccontainer.h>
#include <GraphicScene/graphicconstructor.h>

class GraphicPainter
{
public:
    GraphicPainter();
    static void paint(QPainter* painter,
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
               const QRectF &changeArea);
    static void drawGrid(QPainter* painter,
                  const uint8_t &scale,
                  const uint8_t &gridSize,
                  const QPointF &offset,
                  const int &floor,
                  const int &backgroundFloor,
                  const QRectF &changeArea);
};
