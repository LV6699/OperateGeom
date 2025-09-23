#ifndef SMOOTHPROCAL_H
#define SMOOTHPROCAL_H
#include"../../CommonFile/Header.h"
class CAMALGORITHM_EXPORT SmoothProCal
{
public:
    SmoothProCal(){}
    void CalSigAnglBisectorStrechLenth(const Point& l1Sp,
                                       const Point& l1Ep,
                                       const Point& l2Sp,
                                       const Point& l2Ep,
                                       const double& strechLimit,
                                       DefArc& arcData);
    void GetArcArcConnectArcCenterPnt(const DefArc& arcData1,
                                      const DefArc& arcData2,
                                      const double& radius,
                                      bool& isExiCenterPnt,
                                      Point &centerPnt);
};
#endif // SMOOTHPROCAL_H