#ifndef ELEBOXINTERSECT_H
#define ELEBOXINTERSECT_H
#include"../../CommonFile/Header.h"
class ElemBoxIntersect
{
public:
    ElemBoxIntersect();
    void TwoPointLimitValue(const Point& lSp,const Point& lEp,
                            double& max_x,double& min_x,
                            double& max_y,double& min_y);
    ///lSp、lEp两点是矩形对角线端点 .
    bool IsPointInsideRectangle(const Point& lSp,
                                const Point& lEp,
                                const Point& P, double err);
    bool IsPointInsideValueRectangle(const Point& P, double max_x, double min_x,
                                     double max_y, double min_y, double err);
    void IsPointInsideValueRectangle();
    ///输入的两点是矩形对角线端点 .
    bool IsRectangleExistOverlap(const Point& lSp1,
                                    const Point& lEp1,
                                    const Point& lSp2,
                                    const Point& lEp2, double err);
    bool IsIntResInsideBox(const Point& lSp,const Point& lEp,
                           const GeomIntRes& intRes, double err);
};
#endif // ELEBOXINTERSECT_H