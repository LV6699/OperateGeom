#ifndef CURVEBOXINT_H
#define CURVEBOXINT_H
#include"../GeomCalculate/BaseGeom2D.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class CurveBoxInt
{
public:
    CurveBoxInt(){}
    static void TwoPtLimitValue(const ofts::Point& P1,const ofts::Point& P2,
                                double& max_x,double& min_x,
                                double& max_y,double& min_y){
        if(P1.X() >= P2.X()){max_x = P1.X();min_x = P2.X();}
        else{max_x = P2.X();min_x = P1.X();}
        if(P1.Y() >= P2.Y()){max_y = P1.Y();min_y = P2.Y();}
        else{max_y = P2.Y();min_y = P1.Y();}
    }
    ///lSp、lEp 两点是矩形对角线端点.
    static bool IsPointInsideRect(const ofts::Point& sp,
                                  const ofts::Point& ep,
                                  const ofts::Point& P, double err)
    {
        double max_x,min_x,max_y,min_y,x = P.X(),y = P.Y();
        TwoPtLimitValue(sp,ep,max_x,min_x,max_y,min_y);
        return (min_x <= x + err) && (x - err <= max_x) &&
                (min_y <= y + err) && (y - err <= max_y);
    }
    static bool IsPtInsideValueRect(const ofts::Point& P,
                                    double max_x,double min_x,
                                    double max_y, double min_y, double err)
    {
        return (min_x <= P.X() + err) && (P.X() - err <= max_x) &&
                (min_y <= P.Y() + err) && (P.Y() - err <= max_y);
    }
    ///输入的两点是矩形对角线端点.
    static bool IsRectExistInt(const ofts::Point& sp1,const ofts::Point& ep1,
                               const ofts::Point& sp2,const ofts::Point& ep2,double err)
    {
        double max1_x,min1_x,max1_y,min1_y,max2_x,min2_x,max2_y,min2_y;
        TwoPtLimitValue(sp1,ep1,max1_x,min1_x,max1_y,min1_y);
        TwoPtLimitValue(sp2,ep2,max2_x,min2_x,max2_y,min2_y);
        if(min1_x - err > max2_x || min1_y - err > max2_y ||
                min2_x - err > max1_x || min2_y - err > max1_y){
            return false;
        }
        return true;
    }
    static bool IsIntResInsideBox(const ofts::Point& sp,const ofts::Point& ep,
                                  const ofts::GeomIntRes& intRes, double err)
    {
        if(!intRes.IsInt()){return false;}
        bool isInside = IsPointInsideRect(sp,ep,intRes.P1(),err);
        if(intRes.Num() == 1){
            return isInside;
        }
        if(isInside){
            return true;
        }
        return IsPointInsideRect(sp,ep,intRes.P2(),err);
    }
    static bool IsPointInCircle(const ofts::Point& c,double r,
                                const ofts::Point& p,double e){
        return p.IsSamePoint2D(c,r+e);
    }
    static bool IsLineCirceBoxInt(const ofts::Point& sp,const ofts::Point& ep,
                                  const ofts::DefArc& arc,double e)
    {
        auto op = BaseGeom2D::PointOnLineProj(arc.GetCp(),sp,ep);
        return op.IsEqual(arc.GetCp(),arc.GetOptR() + e);
    }
    static bool IsCirceCirceBoxInt(const ofts::Point& c1,double r1,
                                   const ofts::Point& c2,double r2,double e)
    {
        return c1.IsSamePoint2D(c2,r1 + r2 + e);
    }
};
#endif // CURVEBOXINT_H
