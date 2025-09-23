#include "ElemBoxIntersect.h"
#if DebugModel
#pragma optimize("", off)
#endif
ElemBoxIntersect::ElemBoxIntersect(){}
void ElemBoxIntersect::TwoPointLimitValue(const Point& P1,const Point& P2,
                                          double& max_x,double& min_x,
                                          double& max_y,double& min_y)
{
    if(P1.X() >= P2.X()){
        max_x = P1.X();
        min_x = P2.X();
    }
    else{
        max_x = P2.X();
        min_x = P1.X();
    }
    if(P1.Y() >= P2.Y()){
        max_y = P1.Y();
        min_y = P2.Y();
    }
    else{
        max_y = P2.Y();
        min_y = P1.Y();
    }
}
///lSp、lEp两点是矩形对角线端点 .
bool ElemBoxIntersect::IsPointInsideRectangle(const Point& lSp,
                                              const Point& lEp,
                                              const Point& P,
                                              double err)
{
    double max_x,min_x,max_y,min_y,x = P.X(),y = P.Y();
    TwoPointLimitValue(lSp,lEp,max_x,min_x,max_y,min_y);
    return (min_x <= x + err) && (x - err <= max_x) &&
            (min_y <= y + err) && (y - err <= max_y);
}
bool ElemBoxIntersect::IsPointInsideValueRectangle(const Point& P,
                                                   double max_x,double min_x,
                                                   double max_y,double min_y,
                                                   double err)
{
    return (min_x <= P.X() + err) && (P.X() - err <= max_x) &&
            (min_y <= P.Y() + err) && (P.Y() - err <= max_y);
}
///输入的两点是矩形对角线端点 .
bool ElemBoxIntersect::IsRectangleExistOverlap(const Point& lSp1,
                                               const Point& lEp1,
                                               const Point& lSp2,
                                               const Point& lEp2,
                                               double err)
{
    double max1_x,min1_x,max1_y,min1_y,max2_x,min2_x,max2_y,min2_y;
    TwoPointLimitValue(lSp1,lEp1,max1_x,min1_x,max1_y,min1_y);
    TwoPointLimitValue(lSp2,lEp2,max2_x,min2_x,max2_y,min2_y);
    if(min1_x - err > max2_x || min1_y - err > max2_y ||            min2_x - err > max1_x || min2_y - err > max1_y){
        return false;
    }
    return true;
}
bool ElemBoxIntersect::IsIntResInsideBox(const Point& lSp,
                                         const Point& lEp,
                                         const GeomIntRes& intRes,
                                         double err)
{
    if(!intRes._isExistInt)
        return false;
    bool isInside = IsPointInsideRectangle(lSp,lEp,intRes._intPnt1,err);
    if(intRes._intNum == 1){
        return isInside;
    }
    if(isInside){
        return true;
    }
    return IsPointInsideRectangle(lSp,lEp,intRes._intPnt2,err);
}