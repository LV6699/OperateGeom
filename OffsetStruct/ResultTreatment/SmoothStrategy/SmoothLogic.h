#ifndef SMOOTHLOGIC_H
#define SMOOTHLOGIC_H
#include"../../CommonFile/Header.h"
class SmoothLogic
{
public:
    SmoothLogic(){}
    void SegmentSegmentSmoothArc(DefLoop& loop,
                                 const Point& l1Sp,
                                 const Point& l1Ep,
                                 const Point& l2Sp,
                                 const Point& l2Ep,
                                 double radius,
                                 double strechLimit,
                                 int& i,
                                 int nex);
    void SegmentArcSmoothArc(DefLoop& loop,
                             const Point& lSp,
                             const Point& lEp,
                             const DefArc &arc,
                             bool isLineFont,
                             double radius,
                             double strechLimit,
                             int &i, int nex);
    void ArcArcSmoothArc(DefLoop& loop,
                         const DefArc& arc1,
                         const DefArc& arc2,
                         double radius,
                         double strechLimit,
                         int& i,
                         int nex);
};
#endif // SMOOTHLOGIC_H