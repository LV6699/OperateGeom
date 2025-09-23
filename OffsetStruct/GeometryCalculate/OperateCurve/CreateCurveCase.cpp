#include "CreateCurveCase.h"
#include <stdexcept>
#if 0
#include"../../OperateDebug/DisplayGeom.h"
#endif
void CreateCurveCase::PalnRectangle(const DefPlane& s,const Point& p,
                                    double l,vector<Point>& ps)
{
    Point normal = s.Normal().Normalize(); /// 建立局部坐标系
    ///  创建平面内的第一个基向量 - 任意选择一个不平行于法向量的向量
    Point basis1;
    if (std::abs(normal.X()) > 0.5) {  /// 如果法向量主要指向 x方向
        basis1 = Point(0, 1, 0);
    } else {
        basis1 = Point(1, 0, 0);
    }
    /// 投影到平面内并归一化
    basis1 = (basis1 - normal * normal.Dot(basis1)).Normalize();
    /// 创建第二个基向量（与第一个正交）
    Point basis2 = normal.Cross(basis1).Normalize();
    ps.push_back(p);  /// 顶点1: p (给定的点)
    ps.push_back(p + basis1 * l);  /// 顶点2: p + basis1 * l
    ps.push_back(p + basis1 * l + basis2 * l);  /// 顶点3: p + basis1 * l + 
basis2 * l
    ps.push_back(p + basis2 * l);  /// 顶点4: p + basis2 * l
}
void CreateCurveCase::ThreePointPlan(const Point& p0, const Point& p1,
                                     const Point& p2, DefPlane& plan)
{
#if 0
    ///_mainwind->myOccView->myRemoveall();
    DisplayGeom().DisplaySegment(p0,p1,_colors[3],1);///purple
    DisplayGeom().DisplaySegment(p1,p2,_colors[4],1);///yellow
    DisplayGeom().DisplaySegment(p2,p0,_colors[5],1);///orange
    _mainwind->myOccView->myView->FitAll(0.1);
#endif
    Point v1 = p1 - p0;
    Point v2 = p2 - p0;
    Point n = v1.Cross(v2);  /// 计算法向量
    double D = -n.Dot(p0);  /// 计算D值
    plan = DefPlane(n.X(),n.Y(),n.Z(),D);
#if 0
    vector<Point>ps;
    PalnRectangle(plan,p0,1,ps);
    DisplayGeom().DisplaySegment(ps[0],ps[1],_colors[0],1.5);
    DisplayGeom().DisplaySegment(ps[1],ps[2],_colors[1],1.5);
    DisplayGeom().DisplaySegment(ps[2],ps[3],_colors[1],1);
    DisplayGeom().DisplaySegment(ps[3],ps[0],_colors[1],1);
    _mainwind->myOccView->myView->FitAll(0.1);
    int tem = 1;
#endif
}
void CreateCurveCase::ThreePointNormalPlan(const Point& p0, const Point& p1,
                                           const Point& p2, DefPlane& plan)
{
#if 0
    ///_mainwind->myOccView->myRemoveall();
    DisplayGeom().DisplaySegment(p0,p1,_colors[3],1);///purple
    DisplayGeom().DisplaySegment(p1,p2,_colors[4],1);///yellow
    DisplayGeom().DisplaySegment(p2,p0,_colors[5],1);///orange
    _mainwind->myOccView->myView->FitAll(0.1);#endif
    Point v1 = p1 - p0,v2 = p2 - p0;  /// 该平面s经过点p0.
    Point n1 = v1.Cross(v2);  /// 计算原始平面的法向量
    Point n2 = n1.Cross(v1);  /// 选择v1作为方向向量计算垂直平面的法向量
    n2 = n2.Normalize();
    double D = -n2.Dot(p0);
    plan = DefPlane(n2.X(),n2.Y(),n2.Z(),D);
#if 0
    vector<Point>ps;
    PalnRectangle(plan,p0,1,ps);
    DisplayGeom().DisplaySegment(ps[0],ps[1],_colors[0],1.5);
    DisplayGeom().DisplaySegment(ps[1],ps[2],_colors[1],1.5);
    DisplayGeom().DisplaySegment(ps[2],ps[3],_colors[1],1);
    DisplayGeom().DisplaySegment(ps[3],ps[0],_colors[1],1);
    _mainwind->myOccView->myView->FitAll(0.1);
    int tem = 1;
#endif
}
void CreateCurveCase::PlanNormalPlan(const DefPlane& plan,
                                     const Point& sp,const Point& ep,
                                     DefPlane& nplan)
{
#if 0
    vector<Point>ps;
    PalnRectangle(plan,sp,1,ps);
    DisplayGeom().DisplaySegment(ps[0],ps[1],_colors[0],1.5);
    DisplayGeom().DisplaySegment(ps[1],ps[2],_colors[1],1.5);
    DisplayGeom().DisplaySegment(ps[2],ps[3],_colors[1],1);
    DisplayGeom().DisplaySegment(ps[3],ps[0],_colors[1],1);
    _mainwind->myOccView->myView->FitAll(0.1);
    int tem = 1;
#endif
    Point n = plan.Normal(); /// 获取原平面法向量
    Point dir = ep - sp; /// 计算线段方向向量
    if (dir.IsZero(PreErr_10)) {/// 检查线段长度是否为零
        throw std::invalid_argument(" 创建法平面时,线段起点和终点重合");
    }
    Point n_plan = n.Cross(dir);  /// 计算新平面的法向量（垂直于原平面且包含线段）
    if (n_plan.IsZero(PreErr_10)){  /// 检查计算结果是否有效
        throw std::invalid_argument(" 创建法平面时,线段与原平面法向量平行 ");
    }
    double D = -n_plan.Dot(sp);  /// 计算新平面的D值（使用起点sp）
    nplan = DefPlane(n_plan.X(), n_plan.Y(), n_plan.Z(),D);
#if 0
    vector<Point>ps;
    PalnRectangle(nplan,p0,1,ps);
    DisplayGeom().DisplaySegment(ps[0],ps[1],_colors[0],1.5);
    DisplayGeom().DisplaySegment(ps[1],ps[2],_colors[1],1.5);
    DisplayGeom().DisplaySegment(ps[2],ps[3],_colors[1],1);
    DisplayGeom().DisplaySegment(ps[3],ps[0],_colors[1],1);
    _mainwind->myOccView->myView->FitAll(0.1);
    int tem = 1;
#endif
}