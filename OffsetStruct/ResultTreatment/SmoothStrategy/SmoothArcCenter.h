#ifndef SMOOTHARCCENTER_H
#define SMOOTHARCCENTER_H
#include"../../CommonFile/Header.h"
class SmoothArcCenter
{
public:
    SmoothArcCenter(){}
    void SegmentSegmentSmoothArcCenter(const Point& l1Sp,
                                       const Point& l1Ep,
                                       const Point& l2Sp,
                                       const Point& l2Ep,
                                       double& radius,
                                       bool& isExiCenter,
                                       Point &center);
    void SegmentArcSmoothArcCenter(const Point& lSp,
                                   const Point& lEp,
                                   const DefArc& arc,
                                   bool isLineFont,
                                   double &radius,
                                   bool& isExiCenter,
                                   Point &center);
    void ArcArcSmoothArcCenter(const DefArc& arc1,
                               const DefArc& arc2,
                               double& radius,
                               bool& isExiCenter,
                               Point &center);
};
#endif // SMOOTHARCCENTER_H