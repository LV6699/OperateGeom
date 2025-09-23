#ifndef FITTINGARCSOLVE_H
#define FITTINGARCSOLVE_H
#include"../../../CommonFile/Header.h"
using namespace GeomFitting;
class FittingArcSolve
{
public:
    FittingArcSolve(){}
    void ResetSegParameter(const DefLoop &loop, const FittArc& fitArc,
                           double &A, double &B, double &C);
    void InitialFirstIndex(int i, const Point &lSp,
                           const Point &lEp, double& A,
                           double& B, double& C,
                           FittArc &fitArc);
    void ResetFittingIndex(FittArc &fitArc,
                           vector<FittArc> &vFitArc);
    void SetFittingArcData(int i, const Point &lSp,
                           const Point &lEp, const Point &lSp_,
                           const Point &lEp_, const Point &cp,
                           double angle, FittArc &fitArc);
    bool SegNotIntersect(const FittArc &fitArc,const Point &lSp,
                         const Point &lEp, const Point &lSp_,
                         const Point &lEp_, double valErr, Point &P);
    bool IsValidDirection(const FittArc &fitArc,
                          const Point &P1,
                          const Point &P2,
                          const Point &P3);
    bool IsValidSegment(const FittArc &fitArc,
                        Point& cp,
                        const Point &lSp,
                        const Point &lEp,
                        double disLim);
    bool IsValidCenter(const Point& cp, double disLim,
                       const FittArc &fitArc);
    bool IsValidCurvature(double angle, double angleLim,
                          FittArc &fitArc);
};
#endif // FITTINGARCSOLVE_H