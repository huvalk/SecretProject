#include <GraphicScene/graphicconstructor.h>
#include <QDebug>

GraphicConstructor::GraphicConstructor() :
    _pointSize(5)
{

}

void GraphicConstructor::paintTemp(const uint8_t scale, const QPointF& offset, const  QRectF& area, QPainter* painter, const bool extColor) const
{
    for (auto item: _temp)
    {
        if (item->redrawRequest(area))
        {
            item->paint(painter, offset, scale, extColor);
        }
    }
}

bool GraphicConstructor::clearTemp()
{
    if (_temp.size() == 0)
    {
        return false;
    }

    _temp.clear();
    return true;
}

// Начинается создание полигона
std::pair<bool, QRectF> GraphicConstructor::addTempPoint(const QPointF &pos)
{
    qDebug() << "tempPos created";
    _tempPoint = std::make_shared<GraphicPoint>(pos, _pointSize, 2);
    _tempPos = std::make_shared<QPointF>(_tempPoint->pos());
    _temp.insert(_tempPoint);
    _tempPoly.clear();

    return std::make_pair(true, _tempPoint->boundingRect());
}

std::tuple<bool, QLineF, QRectF> GraphicConstructor::constructLine(const QPointF &pos)
{
    if (_tempPoint->wasClicked(pos))
    {
        return std::make_tuple(false, QLineF(), QRectF());
    }
    auto line = QLineF(_tempPoint->pos(), pos);
    auto result = std::make_tuple(true, line, _tempPoint->boundingRect());
    _tempPoint = nullptr;

    return result;
}

// Дополнительная переменная для завершения создания
// Здесьже формируется полигон
std::tuple<bool, QPolygonF, QRectF>  GraphicConstructor::constructPoly(const QPointF &pos)
{
    if (_tempPos == nullptr)
    {
        qWarning() << "tempPos is null";
        return std::make_tuple(false, QPolygonF(), QRectF());
    }
    auto newTempLine = std::make_shared<GraphicLine>(*_tempPos, pos);
    _temp.emplace(newTempLine);
    _tempPos = std::make_shared<QPointF>(pos);
    _tempPoly.append(pos);

    if (_tempPoint->wasClicked(pos))
    {
        auto result = std::make_tuple(true, _tempPoly, _tempPoly.boundingRect());

        _tempPoint = nullptr;
        _tempPos = nullptr;
        _tempPoly.clear();
        _temp.clear();

        return result;
    }

    return std::make_tuple(true, QPolygonF(), newTempLine->boundingRect());
}
