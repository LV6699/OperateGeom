#ifndef GEOMTANGENCY_H
#define GEOMTANGENCY_H
#include"../../CommonFile/Header.h"
class GeomTangency
{
public:
    GeomTangency(){}
    void IsLineCircleTangency(const Point& lSp,
                              const Point& lEp,
                              const Point& cp,
                              double R, double pre,
                              bool& isTan);
    bool IsLineCircleTangency(const Point& lSp,
                              const Point& lEp,
                              const Point& cp,
                              double R, double pre);
    void IsCircleCircleTangency(const Point& cp1,
                                double R1,
                                const Point& cp2,
                                double R2,
                                double pre,
                                bool& isTan);
    bool IsEleEleTangency(const DefElem& ele1,
                          const DefElem& ele2,
                          bool isJudLine,
                          double pre);
    void IsSegmentExistCoincide(const Point &l1Sp,
                                const Point &l1Ep,
                                const Point &l2Sp,
                                const Point &l2Ep,
                                double pre, bool &isCoin);
};
#endif // GEOMTANGENCY_H