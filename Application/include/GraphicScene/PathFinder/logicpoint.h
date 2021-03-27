#pragma once

struct LogicPoint
{
    explicit LogicPoint(double _x, double _y, double _z);
    explicit LogicPoint(const LogicPoint&) = default;
    explicit LogicPoint(LogicPoint&&) = default;

    double x;
    double y;
    double z;
};
