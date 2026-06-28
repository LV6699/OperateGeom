#include <stdexcept>
#include "CreateCaseArc.h"
#if 0
#include"../../../../offset2D/ToolTrajectoryAlg/OperateDebug/DisplayGeom.h"
#endif
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
void CreateCaseArc::PointRotateBasePoint(const DefPlane& plan,
                                         const Point& p, const Point& fix,
                                         bool isCW,double angle, Point& res)
{
    /// 转换为弧度并确定方向
    double radians = angle * _RadAlg;
    if (isCW) {
        radians = -radians; /// 顺时针为负角度
    }
    ///建立平面局部坐标系
    Point normal(plan.A(), plan.B(), plan.C());
    normal = normal.Normalized();
    ///平移坐标系使fix 点为原点
    Point translated = p - fix;
    ///构建平面局部坐标系,选择一个不平行于法向量的向量作为第一个轴
    Point axis1;
    if (std::abs(normal.X()) > std::abs(normal.Y())) {
        axis1 = Point(-normal.Z(), 0, normal.X());
    } else {
        axis1 = Point(0, normal.Z(), -normal.Y());
    }
    axis1 = axis1.Normalized();
    ///第二个轴为法向量与axis1 的叉积
    Point axis2 = normal.Cross(axis1).Normalized();
    ///将点投影到局部坐标系
    double u = translated.Dot(axis1);
    double v = translated.Dot(axis2);
    ///在2D 平面内旋转
    double cos_angle = std::cos(radians);
    double sin_angle = std::sin(radians);
    double u_rotated = u * cos_angle - v * sin_angle;
    double v_rotated = u * sin_angle + v * cos_angle;
    ///转换回3D 坐标
    ///Point rotated = u_rotated * axis1 + v_rotated * axis2;
    Point rotated = axis1*u_rotated + axis2*v_rotated;
    ///平移回原坐标系
    res = rotated + fix;
}
bool CreateCaseArc::IsPointOnLineLeft(const Point& sp,
                                      const Point& ep,const Point& p)
{
    /// 计算二维向量（忽略Z 坐标）
    Point dir = ep - sp;
    Point vec = p - sp;
    /// 计算二维叉积
    double cross = dir.X() * vec.Y() - dir.Y() * vec.X();
    return cross > 0;
}
double CreateCaseArc::ArcCenterAngle3D(const DefPlane& plan,const DefArc& arc)
{
    const Point& sp = arc._arcSp;
    const Point& ep = arc._arcEp;
    const Point& cp = arc._arcCp;
    ///建立局部坐标系
    double A = plan.A(),B = plan.B(),C = plan.C();
    Point n(A, B, C);
    n = n.Normalized();
    Point u = sp - cp;
    u = u.Normalized();
    ///计算起点和终点角度
    Point ep_vec = ep - cp;
    ep_vec = ep_vec.Normalized();
    double end_angle = std::atan2(u.Cross(ep_vec).Dot(n),u.Dot(ep_vec));
    ///计算圆心角
    double angle = end_angle;
    if (arc.IsCW()) {
        if (angle > 0) {
            angle -= 2 * M_PI;
        }
        angle = -angle;
    } else {
        if (angle < 0) {
            angle += 2 * M_PI;
        }
    }
    angle *= _AlgRad;
    return angle;
}
void CreateCaseArc::ArcToPoint3D(const DefPlane& plan,
                                 const DefArc& arc,bool isAgl,
                                 double step,vector<Point>& points)
{
#if 0
    DisplayGeom().DisplayPoint(arc._arcSp,_colors[0]);
    DisplayGeom().DisplayPoint(arc._arcCp,_colors[1]);
    DisplayGeom().DisplayPoint(arc._arcEp,_colors[2]);
    _mainwind->myOccView->fitAll();
#endif
    double R = arc._arcR;
    Point sp = arc._arcSp,ep = arc._arcEp,cp = arc._arcCp;
    double value = step;
    if(!isAgl){
        value = MathCalculate().AngleBaseBowHeight(R,step);
    }
    points.emplace_back(sp);
    double centerAgl = ArcCenterAngle3D(plan,arc);
    if(value + PreErr5_3 > centerAgl){
        points.emplace_back(ep);
        return;
    }
    bool isCW = arc.IsCW();
    if(value * 2 + PreErr5_3 > centerAgl){
        Point mp;
        PointRotateBasePoint(plan,sp,cp,isCW,centerAgl / 2,mp);
        points.emplace_back(mp);
        points.emplace_back(ep);
        return;
    }
    Point res;
    double curAgl = value;
    while (curAgl < centerAgl) {
        PointRotateBasePoint(plan,sp,cp,isCW,curAgl,res);
        points.emplace_back(res);
        curAgl += value;
#if 0
        DisplayGeom().DisplayPoint(res,_colors[2]);
#endif
    }
    if(!points.back().IsSamePoint3D(ep,PreErr5_4)){
        points.emplace_back(ep);
    }
}
void CreateCaseArc::PlanSegTangentArc(const DefPlane &plan, const Point &sp,
                                      const Point &ep, bool isLeft, double R,
                                      double agl, DefArc &arc)
{
    ////建立局部坐标系
    /// isLeft 确定水平平面时圆心是否在线段左侧
    double A = plan.A(),B = plan.B(),C = plan.C();
    Point n(A, B, C); /// 平面法向量
    n = n.Normalized();   /// 单位化
    /// 线段方向向量（切线方向是 ep -> sp）
    Point tangent = sp - ep;
    tangent = tangent.Normalized();
    /// 计算垂直于切线和法线的方向,即圆心方向
    Point m = n.Cross(tangent).Normalized();
    /// 圆心候选位置
    Point c = sp + m * R;
    if(std::abs(C) > PreErr_8 && std::abs(A) < PreErr_8 &&
            std::abs(B) < PreErr_8){
        bool isLeft_ = false;
        isLeft_ = IsPointOnLineLeft(sp,ep,c);
        if(isLeft != isLeft_){
            m = m * -1;
            c = sp + m * R;
        }
    }else{
        if (c.Z() <= sp.Z()) {
            m = m * -1;
            c = sp + m * R;
        }
    }
    /// 确定旋转方向（使用右手法则）
    Point radial = c - sp;
    Point cross_prod = tangent.Cross(radial);
    bool isCW = n.Dot(cross_prod) < 0;
    /// 计算终点
    Point axis = n.Normalized();
    Point v = sp - c;
    double angle_rad = agl * _RadAlg;
    if (isCW) {
        angle_rad = -angle_rad;
    }
    double cos_theta = std::cos(angle_rad);
    double sin_theta = std::sin(angle_rad);
    Point rotated = v * cos_theta + axis.Cross(v) *
            sin_theta + axis * axis.Dot(v) * (1 - cos_theta);
    Point arcEp = c + rotated;
    arc = DefArc(sp,arcEp,c,R,isCW);
}
void CreateCaseArc::PlanArcTangentArc(const DefArc& arc,
                                      const DefPlane& plan,
                                      double R, double agl,double h,
                                      std::vector<Point> &points)
{
    ///在以下条件下解是唯一的:
    ///(1)半径R ≠ 原有圆弧半径,(2)平面法向量与切线方向不平行,(3)严格满足非包含关系约束
    double A = plan.A(),B = plan.B(),C = plan.C();
    const Point& sp = arc._arcSp,cp = arc._arcCp;
    ///计算原圆弧切线方向
    Point normal = {A, B, C};
    normal = normal.Normalized();
    Point radial = (sp - cp).Normalized();
    Point tangent = arc.IsCW() ? normal.Cross(radial) : radial.Cross(normal);
    tangent = tangent.Normalized();
    ///取反方向作为新切线
    Point newTangent = tangent * -1;
    ///计算候选圆心
    Point binormal = normal.Cross(newTangent).Normalized();///副法线
    Point cp1 = sp + binormal * R;
    Point cp2 = sp - binormal * R;
    ///选择不包含原圆心的解
    Point newcp;
    if((cp1 - cp).Length() > (cp2 - cp).Length()) {
        newcp = cp1;
    }else {
        newcp = cp2;
    }
    ///建立局部坐标系
    Point localX = (sp - newcp).Normalized();
    Point localY = normal.Cross(localX).Normalized();
    ///生成离散点
    double step = MathCalculate().AngleBaseBowHeight(R,h);
    int steps = static_cast<int>(std::fabs(agl)/step);
    for (int i = 0; i <= steps; ++i) {
        double theta = i * step;
        Point p = newcp + localX*(R*std::cos(theta)) +
                localY*(R*std::sin(theta));
        points.push_back(p);
    }
}
void CreateCaseArc::CurvePlanTangentArc(const DefElem& elem,
                                        const DefPlane& plan,
                                        double R, double agl,
                                        double h,
                                        std::vector<Point> &points)
{
    ///在elem 开始点处创建与elem 相切的圆弧arc,该圆弧与elem 在同一平面上.
    ///且当elem 是圆弧时,arc 旋向与elem 相反.
    /*if(elem.Type() == LINETYPE){
        Point sp = elem._segment._segSp,
                ep = elem._segment._segEp;
        SegmentPlanTangentArc(sp,ep,plan,R,agl,h,points);
        return;
    }else if(elem.Type() == ARCTYPE){
        ArcPlanTangentArc(elem._arc,plan,R,agl,h,points);
    }*/
}
