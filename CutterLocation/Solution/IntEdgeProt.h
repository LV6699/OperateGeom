#ifndef INTEDGEPROT_H
#define INTEDGEPROT_H

#include <Eigen/Dense>
#include <algorithm>
#include <cmath>
#include <limits>
#include <unsupported/Eigen/Polynomials>

#include "BaseCalc.h"
#include "IntVertexProt.h"
#include "OperTriaCl.h"

#pragma optimize("", off)
#pragma GCC optimize("o0")

namespace grm {
class IntEdgeProt {
public:
    IntEdgeProt() {}

    static void LineParame(const ofts::Point& sP, const ofts::Point& eP,
                           double p[3]) {
        if (std::abs(eP.Y() - sP.Y()) <= PreErr_12) {
            p[0] = 0;
        } else {
            p[0] = eP.Y() - sP.Y();
        }
        if (std::abs(sP.X() - eP.X()) <= PreErr_12) {
            p[1] = 0;
        } else {
            p[1] = sP.X() - eP.X();
        }
        p[2] = (eP.X() * sP.Y()) - (sP.X() * eP.Y());
    }
    static double PointToLineDist(const ofts::Point& lSp, const ofts::Point& lEp,
                                  const ofts::Point& P) {
        double pa[3];
        LineParame(lSp, lEp, pa);
        if (std::abs(pa[0]) < PreErr5_12) {
            double dis1 = std::abs(lSp.Y() - P.Y());
            double dis2 = std::abs(lEp.Y() - P.Y());
            return (dis1 + dis2) / 2;
        } else if (std::abs(pa[1]) < PreErr5_12) {
            double dis1 = std::abs(lSp.X() - P.X());
            double dis2 = std::abs(lEp.X() - P.X());
            return (dis1 + dis2) / 2;
        }
        double A = pa[0], B = pa[1], C = pa[2];
        return std::abs(A * P.X() + B * P.Y() + C) / std::sqrt(A * A + B * B);
    }
    static ofts::Point ProjPoint(const ofts::Point& sp, const ofts::Point& ep,
                                 const ofts::Point& p) {
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
    double PlaneEndSweepInt(const DefTool& T, const ProjRes& pro,
                            const ofts::Point& p0, const ofts::Point& p1,
                            const ofts::Point& p,size_t id);
    double BallEndSweepInt(const DefTool& T, const ProjRes& pro,
                           const ofts::Point& p0, const ofts::Point& p1,
                           const ofts::Point& p,size_t id);
    double RoundNoseToolCase(const DefTool& T, const ProjRes& pro,
                             const ofts::Point& p0, const ofts::Point& p1,
                             const ofts::Point& p,size_t id);
    double TaperEndSweepInt(const DefTool& T, const ofts::Point& op,
                            double x1, double z1,size_t id);
    double TaperEndToolCase(const DefTool& T, const ProjRes& pro,
                            const ofts::Point& p0, const ofts::Point& p1,
                            const ofts::Point& p, double d2,size_t id);
    double TaperBallSweepInt(const DefTool& T, const ofts::Point& op,
                             double x1, double z1,size_t id);
    double TaperBallToolCase(const DefTool& T, const ProjRes& pro,
                             const ofts::Point& p0, const ofts::Point& p1,
                             const ofts::Point& p, double d2,size_t id);
    double EdgeProtectCase(const DefTool& T, const ProjRes& pro,
                           const ofts::Point& p0, const ofts::Point& p1,
                           const ofts::Point& p, double cur_z,size_t id);
    

    
};



}  // namespace grm

#endif  // INTEDGEPROT_H
