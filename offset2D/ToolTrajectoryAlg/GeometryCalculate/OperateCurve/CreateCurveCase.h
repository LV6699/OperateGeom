#ifndef CREATECURVECASE_H
#define CREATECURVECASE_H
#include"../../CommonFile/Header.h"
class CreateCurveCase
{
public:
    CreateCurveCase(){}
    void PalnRectangle(const DefPlane &s,
                       const Point& p, double l, vector<Point>& ps);
    void ThreePointPlan(const Point& p0, const Point& p1,
                        const Point& p2, DefPlane& plan,
                        double e = PreErr_8);
    void ThreePointNormalPlan(const Point& p0, const Point& p1,
                              const Point& p2, DefPlane& plan,
                              double e = PreErr_8);
    void PlanNormalPlan(const DefPlane& plan,
                        const Point& sp, const Point& ep,
                        DefPlane& nplan,double e = PreErr_8);
};
#endif // CREATECURVECASE_H
