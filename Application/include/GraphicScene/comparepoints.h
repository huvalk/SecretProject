#pragma once

#include <QPointF>

class CmpPoint
{
public:
    bool operator() ( const QPointF & left, const QPointF & right ) const;
};

bool operator< ( const QPointF & left, const QPointF & right );
