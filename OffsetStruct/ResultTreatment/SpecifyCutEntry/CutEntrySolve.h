#ifndef CUTENTRYSOLVE_H
#define CUTENTRYSOLVE_H
#include"../../CommonFile/Header.h"
class CutEntrySolve
{
public:
    CutEntrySolve(){}
    void PointToClosetSplitLoop(const Point& P,DefLoop& loop);
    void PointSplitReorederLoop(const Point& P,int index,DefLoop& loop);
    void ReorederSplitElement(DefLoop& loop);
    void PointToLoopClosestPoint(const Point& P, const DefLoop& loop,
                                 int& index, Point& cloPt);
    void SegLoopSpecifyIntersect(const DefLoop& loop,
                                 const Point& Sp,
                                 const Point& Ep,
                                 int& index,Point& intPnt);
    void ReorederIntermediateLoop(const Point &Sp,
                                  const Point &Ep,
                                  vector<DefLoop>&vLoop);
};
#endif // CUTENTRYSOLVE_H