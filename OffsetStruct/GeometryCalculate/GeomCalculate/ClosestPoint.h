#ifndef CLOSESTPOINT_H
#define CLOSESTPOINT_H
#include"../../CommonFile/Header.h"
class ClosestPoint
{
public:
    ClosestPoint(){}
    void PointToSegmentClosestPoint(const Point &P,
                                    const Point &lSp,
                                    const Point &lEp,
                                    Point& cloPt);
    void PointToArcClosestPoint(const Point &P,
                                const DefArc &arc,
                                Point& cloPt);
    void PointToCircleClosestPoint(const Point &P,
                                   const Point &cp,
                                   double R,
                                   Point& cloPt);
    void PointToElementClosestPoint(const Point &P,
                                    const DefElem &element,
                                    Point& cloPt);
    void SegToSegClosestPoint(const Point &l1Sp,
                              const Point &l1Ep,
                              const Point &l2Sp,
                              const Point &l2Ep,
                              Point& cloPt1,
                              Point &cloPt2);
    void SegmentToArcClosestPoint(const Point &lSp,
                                  const Point &lEp,
                                  const DefArc &arc,
                                  Point& cloPt1,
                                  Point &cloPt2);
    void ArcToArcClosestPoint(const DefArc &arc1,
                              const DefArc &arc2,
                              Point &cloPt1,
                              Point &cloPt2);
};
#endif // CLOSESTPOINT_H