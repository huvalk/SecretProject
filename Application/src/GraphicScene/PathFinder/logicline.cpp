#include <GraphicScene/PathFinder/logicline.h>
#include <cmath>

LogicLine::LogicLine(const LogicPoint &_p1, const LogicPoint &_p2)
    : p1(_p1), p2(_p2), _length(std::hypot(_p2.x - _p1.x, _p2.y - _p1.y,  _p2.z - _p1. z))
{
}

LogicLine::LogicLine(LogicPoint &&_p1, LogicPoint &&_p2)
    : p1(_p1), p2(_p2), _length(std::hypot(_p2.x - _p1.x, _p2.y - _p1.y,  _p2.z - _p1. z))
{
}
