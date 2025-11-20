#ifndef INTEDGEPROT_H
#define INTEDGEPROT_H

#include "OperTriaCl.h"
#include "BaseCalc.h"

#pragma optimize("", off)
#pragma GCC optimize ("o0")

namespace grm{
class IntEdgeProt
{
public:
    IntEdgeProt(){}

    static void LineParame(const oft::Point& sP, const oft::Point& eP,
                           double p[3])
    {
        if(std::abs(eP.Y() - sP.Y()) <= PreErr5_10){p[0] = 0;}
        else{p[0] = eP.Y() - sP.Y();}
        if(std::abs(sP.X() - eP.X()) <= PreErr5_10){p[1] = 0;}
        else{p[1] = sP.X() - eP.X();}
        p[2] = (eP.X() * sP.Y()) - (sP.X() * eP.Y());
    }
    static double PointToLineDist(const oft::Point &lSp,
                                  const oft::Point &lEp,
                                  const oft::Point &P)
    {
        double pa[3];
        LineParame(lSp,lEp,pa);
        if(std::abs(pa[0]) < PreErr5_12){
            double dis1 = std::abs(lSp.Y() - P.Y());
            double dis2 = std::abs(lEp.Y() - P.Y());
            return (dis1 + dis2) / 2;
        }else if(std::abs(pa[1]) < PreErr5_12){
            double dis1 = std::abs(lSp.X() - P.X());
            double dis2 = std::abs(lEp.X() - P.X());
            return (dis1 + dis2) / 2;
        }
        double A = pa[0],B = pa[1],C = pa[2];
        return std::abs(A * P.X() + B * P.Y() + C) / std::sqrt(A * A + B * B);
    }
    static oft::Point ProjPoint(const oft::Point& sp,const oft::Point& ep,
                                const oft::Point& p){
        auto v = ep - sp;
        double squa = v.Dot(v);
        if (squa < PreErr_8) {
            return sp;
        }
        auto toSp = p - sp;
        double t = toSp.Dot(v) / squa;
        auto r = sp + v * t;
        return r;
    }
    static double PlaneEndToolCase(const DefTool& T,const oft::Point& sp,
                                   const oft::Point& ep,const oft::Point& p)
    {
        double zp = Min_Val;
        double RR = T.R()*T.R();
        auto proj = BaseCalc::GetProjPoint(p,sp,ep);
#if 0
        double xm = proj.P().Distance2D(sp);
        if(xm < 0){xm = -xm;}
        if(xm > T.R()){
            return Min_Val;
        }
        double ym2 = p.DistSquare2D(proj.P());
        if(RR < ym2){
            return Min_Val;
        }
        double xf = -std::sqrt(RR - ym2);
        double len = sp.Distance2D(ep);
        double rat = (xm-xf) / len;
        if(rat < 0 || rat > 1){
            return Min_Val;
        }
#else
        const auto& pr = proj.P();
        double xm = sp.Distance2D(pr);
        double ym2 = p.DistSquare2D(pr);
        if(ym2 > RR){
            return Min_Val;
        }
        if(proj.Val() < 0){
            xm = -xm;
        }
        double xf = -std::sqrt(RR - ym2);
        double len = sp.Distance2D(ep);
        double rat = (xm-xf) / len;
        if(rat < 0 || rat > 1){
            return Min_Val;
        }
#endif
        double H = ep.Z() - sp.Z();
        double h = rat * H;
        zp = sp.Z() + h;/**
        oft::Point p_(30,-38,24.1421);
        if(std::abs(zp - p_.Z()) < PreErr_4 &&
                p.IsSamePoint2D(p_,PreErr_4)){
            std::cout<<1<<std::endl;
        }*/
        return zp;
    }
    static double BallEndToolCase(const DefTool& T,const oft::Point& sp,
                                  const oft::Point& ep,const oft::Point& p)
    {
        double H = ep.Z() - sp.Z();
        double l = sp.Distance3D(ep);
        double len = sp.Distance2D(ep);
        return 0;
    }
    static double  EdgeProtectCase(const DefTool& T,const oft::Point& p0,
                                   const oft::Point& p1,const oft::Point& p)
    {

        if(std::abs(p0.X() - p1.X()) < PreErr_8 &&
                std::abs(p0.Y() - p1.Y()) < PreErr_8){  ///处理竖直边
            return Min_Val;
        }
        auto sp = p0,ep = p1;
        if(sp.Z() > ep.Z()){std::swap(sp,ep);}
        double z = Min_Val;

        switch (T._type) {
        case ToolType::PlaneEnd:{
            z = PlaneEndToolCase(T,sp,ep,p);
            break;
        }
        case ToolType::BallNoseEnd:{
            break;
        }
        case ToolType::RoundNoseEnd:{

            break;
        }
        default: break;
        }



        return z;

    }
    static double  TriangleEdgeCase(const DefTool& T,const Triangle& t,
                                    const oft::Point& p)
    {
        double z0 = Min_Val,z1 = Min_Val,z2 = Min_Val;
        if(t.IsInE0Range(p,T.R()+PreErr_10)){
            z0 = EdgeProtectCase(T,t.P0(),t.P1(),p);
        }
        if(t.IsInE1Range(p,T.R()+PreErr_10)){
            z1 = EdgeProtectCase(T,t.P1(),t.P2(),p);
        }
        if(t.IsInE2Range(p,T.R()+PreErr_10)){
            z2 = EdgeProtectCase(T,t.P2(),t.P0(),p);
        }
        return LimVal::ThreeMax(z0,z1,z2);

    }




};
}

#endif // INTEDGEPROT_H
