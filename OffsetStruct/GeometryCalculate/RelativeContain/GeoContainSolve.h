#ifndef GEOCONTAINSOLVE_H
#define GEOCONTAINSOLVE_H
#include"../../CommonFile/Header.h"
class GeoContainSolve
{
public:
    GeoContainSolve(){}
    void IsPointOnLoopBorder(const DefLoop& loop,
                             const Point& P,
                             double pre,
                             bool &isOnBorder);
    void IsPointInsideCircle(const Point& cp,double R,
                             const Point& P,double pre,
                             bool& inInside);
    void IsCircleContainCircle(const Point& outCp,double outRadius,
                               const Point& cp,double R,double pre,
                               bool& isContain);
    void IsCircleContainSeg(const Point& cp, double R,
                            const Point& lSp,
                            const Point& lEp, double pre,
                            bool& isContain);
    void IsCircleContainArc(const Point& cp, double R,
                            const DefArc& arc, double pre,
                            bool& isContain);
    void IsCircleContainElem(const Point& cp, double R,
                            const DefElem& ele, double pre,
                            bool& isContain);
    void IsCircleContainLoop(const Point& cp,double R,
                             const DefLoop& loop,
                             double pre,
                             bool& isContain);
    void IsLoopContainCircle(const DefLoop& loop,
                             const Point& cp,double R,
                             double pre,
                             bool& isContain);
    void IsLoopContainElemEndpoint(const DefLoop& loop,
                                   const DefElem &ele,
                                   double pre,
                                   bool isMatchBord,
                                   bool& matchResult);
    void IsLoopContainIntersectElem(const DefLoop& loop,
                                    const DefElem& ele,
                                    double incErrVal,
                                    double coinErrVal,
                                    double intPre,
                                    bool isMatchBord,
                                    bool& matchResult);
    void IsLoopContainCurve(const DefLoop& loop,
                              const DefElem& ele,
                              double intPre,                              double incPre,
                              bool& isInclude);
    void LoopContainElementSolve(const DefLoop& loop,
                                 const DefElem& ele,
                                 double errVal,
                                 bool &isCoincide,
                                 bool& isInclude);
};
#endif // GEOCONTAINSOLVE_H