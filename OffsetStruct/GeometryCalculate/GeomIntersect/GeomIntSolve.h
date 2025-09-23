#ifndef GEOMINTSOLVE_H
#define GEOMINTSOLVE_H
#include"../../CommonFile/Header.h"
class GeomIntSolve
{
public:
    GeomIntSolve(){}
    void JudgeArcRadius(DefArc& arc);
    void SetHorVerLineIntPnt(const Point& lSp,
                             const Point& lEp,
                             Point& P);
    void ExicuteEndpointOnTuple(bool isSpOn,
                                bool isEpOn,
                                const Point &sPnt,
                                const Point &ePnt,
                                bool& isEndpntOn,
                                Point& onEndpnt,
                                GeomIntRes &intRes);
    void HandleEndpointOnTuple(bool isEndpntOn,
                               const Point &onEndpnt,
                               const GeomIntRes& intRes_,
                               GeomIntRes& intRes);
    void IntersectPointNotOnArc(const DefArc &arc,
                                Point &intPnt);
};
#endif // GEOMINTSOLVE_H