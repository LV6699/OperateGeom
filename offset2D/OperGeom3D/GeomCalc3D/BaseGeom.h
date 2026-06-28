#ifndef BASEGEOM_H
#define BASEGEOM_H
#include"../../ToolTrajectoryAlg/CommonFile/DataStructure.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class BaseGeom
{
public:
    BaseGeom(){}
    static bool IsSameDire(const ofts::Point& v1,
                           const ofts::Point& v2,double e){
        double l_sq1 = v1.LengthSquare();
        double l_sq2 = v2.LengthSquare();
        double ee = e * e;
        if (l_sq1 < ee || l_sq2 < ee) {return false;}
        double mag1 = std::sqrt(l_sq1);
        double mag2 = std::sqrt(l_sq2);
        double product = v1.Dot(v2);
        double cos_t = product / (mag1 * mag2);///计算夹角余弦
        if (std::abs(cos_t - 1.0) < e) {return true;}///主要判断:余弦值是否接近1
        ///对于边界情况,进行额外的稳定性检查,如果余弦值很接近1 但在容差边界,使用更严格的检查
        ///方法A:检查归一化向量的差向量的模长
        auto u1 = v1 * (1.0 / mag1); /// 避免调用Normalize()的开销
        auto u2 = v2 * (1.0 / mag2);
        auto diff = u1 - u2;
        double diff_mag_sq = diff.LengthSquare();
        if (diff_mag_sq < 2.0 * ee) {return true;} /// 2.0 是安全系数
        double rel_err_x = std::abs(u1.X() - u2.X());///方法B：检查各分量的相对误差
        double rel_err_y = std::abs(u1.Y() - u2.Y());
        double rel_err_z = std::abs(u1.Z() - u2.Z());
        if (rel_err_x < e && rel_err_y < e && rel_err_z < e) {return true;}
        return false;
    }
    static ofts::Point ProjPointOnSeg(const ofts::Point& p,
                                      const ofts::Point& sp,
                                      const ofts::Point& ep)
    {
        auto AB = ep - sp;
        auto AP = p - sp;
        double t = AP.Dot(AB)/AB.Dot(AB);
        auto res = sp + AB * t;
        return res;
    }
    static ofts::Point MovePoint(const ofts::Point& p,
                                 const ofts::Point& dir,double d)
    {
        auto dir_unit = dir.Normalized();
        auto v = dir_unit * d;
        auto res = p + v;
        return res;
    }
    static ofts::Point MovePoint(const ofts::Point& p,
                                 const ofts::Point& sp,
                                 const ofts::Point& ep,double d)
    {
        double dx = ep.X() - sp.X();
        double dy = ep.Y() - sp.Y();
        double dz = ep.Z() - sp.Z();
        double dir_len = std::sqrt(dx * dx + dy * dy + dz * dz);
        if(dir_len < PreErr_15){return p;}
        double disp_x = (dx / dir_len) * d;///计算位移分量
        double disp_y = (dy / dir_len) * d;
        double disp_z = (dz / dir_len) * d;
        return ofts::Point(p.X() + disp_x,p.Y() + disp_y,p.Z() + disp_z);
    }
    static ofts::Point RotatePoint(const ofts::RotParam& param,
                                   const ofts::Point& p)
    {
        const auto& loc = param.Axis().Location();
        const auto& dir = param.Axis().Direction();
        auto u = dir.Normalized();
        auto v = p - loc;
        double ct,st;
        auto agl = param.Angle() * _RadAlg;
        if(param.IsCW()) {
            ct = std::cos(agl);st = std::sin(agl);
        }else{
            ct = std::cos(-agl);st = std::sin(-agl);
        }
        double u_dot_v = u.Dot(v);
        auto u_cross_v = u.Cross(v);
        auto v_rot = v * ct + u_cross_v * st + u * (u_dot_v * (1 - ct));
#if 0
        DisplayGeom().DisplaySegment(loc,dir,_colors[1],2);
        DisplayGeom().DisplayPoint(op,_colors[1]);
        auto rpt = BaseGeom::RotatePoint(rp,op);
        DisplayGeom().DisplayPoint(rpt,_colors[3]);
        auto jp = BaseGeom::ProjPointOnSeg(op,loc,dir);
        DisplayGeom().DisplayPoint(jp,_colors[1]);
        DisplayGeom().DisplaySegment(jp,op,_colors[0],2);
        DisplayGeom().DisplaySegment(jp,rpt,_colors[0],2);
        ///DisplayGeom().DisplaySegment(jp,rpt,_colors[0],2);
        DefArc arc(op,rpt,jp,!iscw);
        DisplayGeom().DisplayArc(arc,_colors[2],2,dir);
        _mainwind->myOccView->fitAll();
#endif
        return loc + v_rot;
    }
    static bool IsPointOnLine(const ofts::Point& p0,const ofts::Point& p1,
                              const ofts::Point& p,double e){
        auto v1 = p1 - p0;
        auto v2 = p - p0;
        if(v1.LengthSquare() < e*e){
            return p.IsSamePoint3D(p0,e) || p.IsSamePoint3D(p1,e);
        }
        auto cross = v1.Cross(v2);
        double magSq = cross.LengthSquare();
        return magSq <= e*e;
    }
};
#endif // BASEGEOM_H
