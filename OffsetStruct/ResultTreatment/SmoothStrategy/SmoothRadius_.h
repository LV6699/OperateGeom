#ifndef SMOOTHRADIUS_H
#define SMOOTHRADIUS_H
#include"../../CommonFile/Header.h"
class SmoothRadius
{
public:
    SmoothRadius(){}
    void ResetSmoonthRadiusBaseSeg(const Point& lSp,
                                   const Point& lEp,
                                   double times,
                                   double& radius);
    void ResetSmoonthRadiusBaseArc(const DefArc& arc,
                                   double times,
                                   double& radius);
    void IsSigSigSmoothExtrOnOriEle(const Point& l1Sp,
                                    const Point& l1Ep,
                                    const Point& l2Sp,
                                    const Point& l2Ep,
                                    const Point& arcSp,
                                    const Point& arcEp,
                                    bool& isOnOriEle);
    void IsSigArcSmoothExtrOnOriEle(const Point& lSp,
                                    const Point& lEp,
                                    const DefArc& arc,
                                    const Point& arcSp,
                                    const Point& arcEp,
                                    bool isLineFont,
                                    bool& isOnOriEle);
    void IsArcArcSmoothExtrOnOriEle(const DefArc& arc1,
                                    const DefArc& arc2,
                                    const Point& arcSp,
                                    const Point& arcEp,
                                    bool& isOnOriEle);
};
#endif // SMOOTHRADIUS_H