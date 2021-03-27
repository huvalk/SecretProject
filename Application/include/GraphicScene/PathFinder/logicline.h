#pragma once
#include <GraphicScene/PathFinder/logicpoint.h>

struct LogicLine
{
    explicit LogicLine(const LogicPoint &_p1, const LogicPoint &_p2);
    explicit LogicLine(LogicPoint &&_p1, LogicPoint &&_p2);
    explicit LogicLine(const LogicLine&) = default;
    explicit LogicLine(LogicLine&&) = default;

    LogicPoint p1;
    LogicPoint p2;
    double _length;
};
