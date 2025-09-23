#ifndef CreateSmoothArc_H
#define CreateSmoothArc_H
#include"../../CommonFile/Header.h"
class CAMALGORITHM_EXPORT CreateSmoothArc
{
public:
    CreateSmoothArc(){}
    void CreateSegmentSegmentSmoothArc(const Point& l1Sp,
                                       const Point& l1Ep,
                                       const Point& l2Sp,
                                       const Point& l2Ep,
                                       double radius,
                                       double strechLimit,
                                       bool &isExiSmooth,
                                       DefArc& smoothArc);
    void CreateSegmentArcSmoothArc(const Point& lSp,
                                   const Point& lEp,
                                   const DefArc& arc,
                                   double radius,
                                   double strechLimit,
                                   bool isLineFont,
                                   bool &isExiSmooth,
                                   DefArc &smoothArc);
    void CreateArcArcSmoothArc(const DefArc& arc1,
                               const DefArc& arc2,
                               double radius,
                               double strechLimit,
                               bool& isExiSmooth,
                               DefArc& smoothArc);
};
#endif // CreateSmoothArc_H