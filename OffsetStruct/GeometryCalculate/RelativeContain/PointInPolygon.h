Fj6M-BDW4YWzfeOq7n0m9lss0nTN
Page 1#ifndef PNTINPOLY_H
#define PNTINPOLY_H
#include"../../CommonFile/Header.h"
class PointInPolygon
{
public:
    PointInPolygon(){}
    void IsPointLocationLoopBoundary(const DefLoop& loop
                                     const Point& P
                                     bool& isOnBorder);
    void IsPointInsideLoop(const DefLoop& loop
                           const Point& P
                           double bordPre
                           bool& isOnBorder
                           bool& isInPoly);
};
#endif // PNTINPOLY_H