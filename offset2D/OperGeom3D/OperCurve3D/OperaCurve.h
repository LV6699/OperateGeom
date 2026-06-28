#ifndef OPERACURVE_H
#define OPERACURVE_H
#include"../../ToolTrajectoryAlg/CommonFile/DataStructure.h"
class OperaCurve
{
public:
    OperaCurve(){}
    static void CircleToTwoArcs(const ofts::DefCircle& cir,
                                const ofts::Point& normal,
                                ofts::DefArc& arc1,ofts::DefArc& arc2)
    {
        const auto& cp = cir.GetCp();
        const auto& radius = cir.R();
        auto StartDir = [](const ofts::Point& normal)->ofts::Point {
            double nx = std::abs(normal.X());
            double ny = std::abs(normal.Y());
            double nz = std::abs(normal.Z());
            ofts::Point u;
            if (nx <= ny && nx <= nz) {
                u = normal.Cross(ofts::Point(1, 0, 0));
            } else if (ny <= nx && ny <= nz) {
                u = normal.Cross(ofts::Point(0, 1, 0));
            } else {
                u = normal.Cross(ofts::Point(0, 0, 1));
            }
            return u.Normalized();
        };
        ofts::Point sDir = StartDir(normal);
        ofts::Point u = sDir.Normalized();
        ofts::Point p0 = cp + u * radius;
        ofts::Point p1 = cp - u * radius;
        arc1 = ofts::DefArc(p0,p1,cp,true);
        arc2 = ofts::DefArc(p1,p0,cp,true);
    }
    static double ArcCenterAngle(const ofts::DefArc& arc)
    {
        const auto& nor = arc.Normal();
        auto vs = arc.GetSp() - arc.GetCp();
        auto ve = arc.GetEp() - arc.GetCp();
        if(arc.GetOptR3D(PreErr5_3) < PreErr5_12){
            return 0;
        }
        double dot = vs.Dot(ve);
        auto cross = vs.Cross(ve);
        double signed_y = cross.Dot(nor);  //计算带符号的y 分量(vs×ve)·n
        double ccw_rad = std::atan2(signed_y,dot);  //计算带符号的逆时针弧度
        if(ccw_rad < 0){ccw_rad += 2 * M_PI;}  //转换为[0,2pi]范围内的逆时针弧度
        double agl = ccw_rad;
        if(arc.IsCW()){agl = 2 * M_PI - ccw_rad;}
        agl *= _AlgRad;
        if (agl >= 360.0){agl = 0.0;}
        if(agl < 0){agl += 360.0;}
        return agl;
    }
    static bool IsPointOnArc(const ofts::DefArc& arc,const ofts::Point& p,
                             double endErr = PreErr5_10,double err = PreErr5_10,
                             bool isMatchR = true)
    {
        if(p.IsSamePoint3D(arc.GetSp(),endErr) ||
                p.IsSamePoint3D(arc.GetEp(),endErr)){
            return true;
        }
        if(isMatchR){
            double r = arc.GetOptR3D(PreErr5_3);
            double d = p.DistSquare3D(arc.GetCp());
            if(std::abs(d - r*r) > err){return false;}
        }
        double agl = ArcCenterAngle(arc);
        auto arc1 = arc;arc1.SetEp(p);
        double agl1 = OperaCurve::ArcCenterAngle(arc1);
        bool is = pnum::AglIsBetweenAgls(0,agl,arc.IsCW(),agl1,err);
        return is;
    }




};
#endif // OPERACURVE_H
