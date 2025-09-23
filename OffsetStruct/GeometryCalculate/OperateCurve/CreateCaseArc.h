#ifndef CREATECASEARC_H
#define CREATECASEARC_H
#include"../../CommonFile/Header.h"
class CreateCaseArc
{
public:
    CreateCaseArc(){}
    void PointRotateBasePoint(const DefPlane& plan,
                              const Point& p, const Point& fix,
                              bool isCW, double angle, Point& res);
    bool IsPointOnLineLeft(const Point& sp,
                           const Point& ep,const Point& p);
    double ArcCenterAngle3D(const DefPlane& plan,const DefArc& arc);
    void ArcToPoint3D(const DefPlane& plan,
                      const DefArc& arc,bool isAgl,
                      double step,vector<Point>& points);
    void PlanSegTangentArc(const DefPlane &plan, const Point &sp,
                           const Point &ep, bool isLeft, 
                           double R, double agl,
                           DefArc &arc);
    void PlanArcTangentArc(const DefArc& arc,
                           const DefPlane& plan,
                           double R, double agl,double h,
                           std::vector<Point> &points);
    void CurvePlanTangentArc(const DefElem& elem, const DefPlane& plan,
                             double R, double agl, double h,
                             std::vector<Point>& points);
};
#endif // CREATECASEARC_H