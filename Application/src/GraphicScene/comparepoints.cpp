#include "GraphicScene/comparepoints.h"

bool CmpPoint::operator() ( const QPointF & left, const QPointF & right ) const
{
    return left.x() < right.x() || ((std::abs(right.x() - left.x()) < std::numeric_limits<double>::epsilon()) && left.y() < right.y());
}

bool operator< ( const QPointF & left, const QPointF & right )
{
    return left.x() < right.x() || ((std::abs(right.x() - left.x()) < std::numeric_limits<double>::epsilon()) && left.y() < right.y());
}
