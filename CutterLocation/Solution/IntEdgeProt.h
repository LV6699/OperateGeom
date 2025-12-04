#ifndef INTEDGEPROT_H
#define INTEDGEPROT_H

#include "BaseCalc.h"
#include "OperTriaCl.h"
#include "IntVertexProt.h"

#pragma optimize("", off)
#pragma GCC optimize("o0")

namespace grm {
class IntEdgeProt {
  public:
    IntEdgeProt() {}

    static void LineParame(const oft::Point &sP, const oft::Point &eP,
                           double p[3]) {
        if(std::abs(eP.Y() - sP.Y()) <= PreErr5_10) {
            p[0] = 0;
        } else {
            p[0] = eP.Y() - sP.Y();
        }
        if(std::abs(sP.X() - eP.X()) <= PreErr5_10) {
            p[1] = 0;
        } else {
            p[1] = sP.X() - eP.X();
        }
        p[2] = (eP.X() * sP.Y()) - (sP.X() * eP.Y());
    }
    static double PointToLineDist(const oft::Point &lSp, const oft::Point &lEp,
                                  const oft::Point &P) {
        double pa[3];
        LineParame(lSp, lEp, pa);
        if(std::abs(pa[0]) < PreErr5_12) {
            double dis1 = std::abs(lSp.Y() - P.Y());
            double dis2 = std::abs(lEp.Y() - P.Y());
            return (dis1 + dis2) / 2;
        } else if(std::abs(pa[1]) < PreErr5_12) {
            double dis1 = std::abs(lSp.X() - P.X());
            double dis2 = std::abs(lEp.X() - P.X());
            return (dis1 + dis2) / 2;
        }
        double A = pa[0], B = pa[1], C = pa[2];
        return std::abs(A * P.X() + B * P.Y() + C) / std::sqrt(A * A + B * B);
    }
    static oft::Point ProjPoint(const oft::Point &sp, const oft::Point &ep,
                                const oft::Point &p) {
        auto v = ep - sp;
        double squa = v.Dot(v);
        if(squa < PreErr_8) {
            return sp;
        }
        auto toSp = p - sp;
        double t = toSp.Dot(v) / squa;
        auto r = sp + v * t;
        return r;
    }
    static double PlaneEndToolCase(const DefTool &T, const oft::Point &sp,
                                   const oft::Point &ep, const oft::Point &p) {
        double RR = T.R() * T.R();
        auto proj = BaseCalc::GetProjPoint(p, sp, ep);

        const auto &pr = proj.P();
        double ym2 = p.DistSquare2D(pr);
        if(ym2 > RR) {
            return Min_Val;
        }
        double z = Min_Val;
        double xm = sp.Distance2D(pr);
        if(proj.Val() < 0) {
            xm = -xm;
        }

        double xf = -std::sqrt(RR - ym2);
        double len = sp.Distance2D(ep);
        double rat = (xm - xf) / len;
        if(rat < 0 || rat > 1) {
            return Min_Val;
        }
        double H = ep.Z() - sp.Z();
        double h = rat * H;
        z = sp.Z() + h; /**
         oft::Point p_(30,-38,24.1421);
         if(std::abs(zp - p_.Z()) < PreErr_4 &&
                 p.IsSamePoint2D(p_,PreErr_4)){
             std::cout<<1<<std::endl;
         }*/
        return z;
    }
    static double BallNoseToolCase(const DefTool &T, const oft::Point &p0,
                                   const oft::Point &p1, const oft::Point &p) {
        double RR = T._R * T._R;
        auto proj = BaseCalc::GetProjPoint(p, p0, p1);

        const auto &pr = proj.P();
        double ym2 = p.DistSquare2D(pr);
        if(ym2 > RR) {
            return Min_Val;
        }
        double z = Min_Val;
        double H = p1.Z() - p0.Z();
        double xm = p0.Distance2D(pr);
        if(proj.Val() < 0) {
            xm = -xm;
        }
        double len = p0.Distance2D(p1);
        double l = p0.Distance3D(p1);
        /// double bt = T.R() * H / l;
        /// double xk = -bt / T.R() * std::sqrt(RR - ym2);
        double xk = -(H / l) * std::sqrt(RR - ym2);
        double rat = (xm - xk) / len;
        if(rat < 0 || rat > 1) {
            return Min_Val;
        }
        double h = rat * H;
        /// if(std::abs(xk) < PreErr_10){
        /// z = p0.Z() - T._R + h;return z;
        ///}
        z = p0.Z() - T._R + h + std::abs(xk) * len / H;
        return z;
    }
    static double EdgeProtectCase(const DefTool &T, const oft::Point &p0,
                                  const oft::Point &p1, const oft::Point &p) {
        if(std::abs(p0.X() - p1.X()) < PreErr_8 &&
           std::abs(p0.Y() - p1.Y()) < PreErr_8) { /// 处理竖直边
            return Min_Val;
        }
        auto sp = p0, ep = p1;
        if(sp.Z() > ep.Z()) {
            std::swap(sp, ep);
        }
        double z = Min_Val;
        if(p1.Z() - p0.Z() < PreErr5_6){
            auto op = p;
            auto proj = BaseCalc::GetProjPoint(p, p0, p1);
            if(proj._val < 0){
                op = p0;
            }else if(proj._val > 1){
                op = p1;
            }else{
                op = proj._p;
            }
            double z = IntVertProt::VertexProtectZ(T,op,p);
            return z;
        }
        switch(T._type) {
        case ToolType::PlaneEnd: {
            z = PlaneEndToolCase(T, sp, ep, p);
            break;
        }
        case ToolType::BallNoseEnd: {
            z = BallNoseToolCase(T, sp, ep, p);
            break;
        }
        case ToolType::RoundNoseEnd: {
            break;
        }
        default:
            break;
        }
        return z;
    }

    static double EdgeProtectZ(const DefTool &T, const oft::Point &p0,
                               const oft::Point &p1, const oft::Point &p) {
        auto proj = BaseCalc::GetProjPoint(p, p0, p1);
        if(proj._val < 0 || proj._val > 1) {
            auto op = p;

            return Min_Val;
        }
#if 1
#endif

        double d2 = 0;
        if(proj._val <= 0) {
            d2 = p.DistSquare2D(p0);
        } else if(proj._val >= 1) {
            d2 = p.DistSquare2D(p1);
        } else {
            d2 = p.DistSquare2D(proj._p);
        }
        if(d2 > T._R * T._R) {
            return Min_Val;
        }
        double z = p0.Z() - T._R + std::sqrt(T._R * T._R - d2);
        return z;
    }

    static double TriangleEdgeCase(const DefTool &T, const Triangle &t,
                                   const oft::Point &p) {
        double z = Min_Val, z0 = Min_Val, z1 = Min_Val, z2 = Min_Val;
        if(t.IsInE0Range(p, T.R() + PreErr_10)) {
            z0 = EdgeProtectCase(T, t.P0(), t.P1(), p);
        }
        if(t.IsInE1Range(p, T.R() + PreErr_10)) {
            z1 = EdgeProtectCase(T, t.P1(), t.P2(), p);
        }
        if(t.IsInE2Range(p, T.R() + PreErr_10)) {
            z2 = EdgeProtectCase(T, t.P2(), t.P0(), p);
        }
        z = LimVal::ThreeMax(z0, z1, z2);
        return z;
    }
};
} // namespace grm

#endif // INTEDGEPROT_H
