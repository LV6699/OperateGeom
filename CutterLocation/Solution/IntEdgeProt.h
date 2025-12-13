#ifndef INTEDGEPROT_H
#define INTEDGEPROT_H

#include <Eigen/Dense>
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
    if (std::abs(eP.Y() - sP.Y()) <= PreErr5_10) {
      p[0] = 0;
    } else {
      p[0] = eP.Y() - sP.Y();
    }
    if (std::abs(sP.X() - eP.X()) <= PreErr5_10) {
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
  static double PlaneEndToolCase(const DefTool& T, const ProjRes& pro,
                                 const ofts::Point& sp, const ofts::Point& ep,
                                 const ofts::Point& p) {
    double RR = T.RR();
    const auto& pr = pro.P();
    double ym2 = p.DistSquare2D(pr);
    //if (ym2 > RR) {
    //return Min_Val;
    //}
    double z = Min_Val;
    double xm = sp.Distance2D(pr);
    if (pro.Val() < 0) {
      xm = -xm;
    }

    double xf = -std::sqrt(RR - ym2);
    double len = sp.Distance2D(ep);
    double rat = (xm - xf) / len;
    if (rat < 0 || rat > 1) {
      return Min_Val;
    }
    double H = ep.Z() - sp.Z();
    double h = rat * H;
    z = sp.Z() + h; /**
     ofts::Point p_(30,-38,24.1421);
     if(std::abs(zp - p_.Z()) < PreErr_4 &&
             p.IsSamePoint2D(p_,PreErr_4)){
         std::cout<<1<<std::endl;
     }*/
    //if(z > 15){
        //int tem = 1;
    //}
    return z;
  }
  static double BallNoseToolCase(const DefTool& T, const ProjRes& pro,
                                 const ofts::Point& p0, const ofts::Point& p1,
                                 const ofts::Point& p) {
    double RR = T.RR();
    const auto& pr = pro.P();
    double ym2 = p.DistSquare2D(pr);
    // if (ym2 > RR) {
    // return Min_Val;
    //}
    double z = Min_Val;
    double H = p1.Z() - p0.Z();
    double xm = p0.Distance2D(pr);
    if (pro.Val() < 0) {
      xm = -xm;
    }
    double len = p0.Distance2D(p1);
    double l = p0.Distance3D(p1);
    /// double bt = T.R() * H / l;
    /// double xk = -bt / T.R() * std::sqrt(RR - ym2);
    double xk = -(H / l) * std::sqrt(RR - ym2);
    double rat = (xm - xk) / len;
    if (rat < 0 || rat > 1) {
      return Min_Val;
    }
    double h = rat * H;
    /// if(std::abs(xk) < PreErr_10){
    /// z = p0.Z() - T._R + h;return z;
    ///}
    z = p0.Z() - T._R + h + std::abs(xk) * len / H;
    return z;
  }
  static double RoundNoseToolCase(const DefTool& T, const ProjRes& pro,
                                  const ofts::Point& p0, const ofts::Point& p1,
                                  const ofts::Point& p) {
    double R = T._R;
    double r = T._cr;
    double L = p0.Distance2D(p1);
    double K = (p1.Z() - p0.Z()) / L;
    double KK = K * K;
    double rr = r * r;
    double x = pro._p.Distance2D(p0);
    if (pro._val < 0) {
      x = -x;
    }
    double yy = pro._p.DistSquare2D(p);
    double y = std::sqrt(yy);
    double A = (KK + 1) * rr;
    double B = 2 * (KK + 1) * (R - r) * r;
    double C = (KK + 1) * pnum::Square(R - r) - KK * rr - yy;
    double D = -2 * KK * (R - r) * r;
    double E = -KK * pnum::Square(R - r);
    Eigen::VectorXd coefficients(5);
    coefficients[4] = A;
    coefficients[3] = B;
    coefficients[2] = C;
    coefficients[1] = D;
    coefficients[0] = E;
    // 使用求解多项式
    Eigen::PolynomialSolver<double, Eigen::Dynamic> solver;
    solver.compute(coefficients);
    // 获取所有根
    Eigen::PolynomialSolver<double, Eigen::Dynamic>::RootsType roots =
        solver.roots();
    double left = 0, right = M_PI / 2;
    if (pnum::RightInc2(R - r, std::sqrt(pnum::Square(x - L) + yy), R)) {
      left = std::acos(1 - (R - std::sqrt(pnum::Square(x - L) + yy)) / r);
    }
    if (y > (R - r) && x >= 0) {
      right = std::acos(1 - (R - y) / r);
    } else {
      double tt = p.DistSquare2D(p0);
      if (tt > pnum::Square(R - r) && x < 0) {
        right = std::acos(1 - (R - std::sqrt(tt)) / r);
      }
    }
    double mint = std::cos(right);
    double maxt = std::cos(left);
    double out = Min_Val;
    for (int i = 0; i < roots.size(); ++i) {
      double imag = roots[i].imag();
      double real = roots[i].real();
      double curr_r = A * std::pow(real, 4) + B * std::pow(real, 3) +
                      C * real * real + D * real + E;
      // 虚部为 0,只输出实数根
      if (imag == 0 && mint <= real && real <= maxt) {
        double cosa = real;
        double sin2a = 1 - cosa * cosa;
        if (sin2a < 0) {
          continue;
        }
        double sina = std::sqrt(sin2a);
        double xf2 = pnum::Square(R + r * (cosa - 1)) - yy;
        if (xf2 < 0) {
          continue;
        }
        double lp = x + std::sqrt(xf2);
        if (lp > L) {
          continue;
        }
        double cz = p0.Z() - r + r * sina + K * lp;
        if (cz > out) {
          out = cz; /**
           // 计算法线
           double base_z = p0.Z() - r + K * lp;
           auto tp0 = p0 - ofts::Point(0, 0, r);
           auto tp1 = p1 - ofts::Point(0, 0, r);
           auto base = tp0 + (base_z - tp0.Z()) / (tp1.Z() - tp0.Z()) * (tp1 -
           tp0);  // 基础点 auto dir = (p - base).Normalized2D(); auto dr = dir
           * (R - r); auto base_1 = base + ofts::Point(dr.X(),dr.Y(), 0); auto
           curr = ofts::Point(p.X(), p.Y(),cz); out_nol = (curr -
           base_1).normalization();*/
        }
      }
    }
    return out;
  }
  static double EdgeProtectCase(const DefTool& T, const ProjRes& pro,
                                const ofts::Point& p0, const ofts::Point& p1,
                                const ofts::Point& p, double cur_z) {
    /*if (std::abs(p0.X() - p1.X()) < PreErr_8 &&
        std::abs(p0.Y() - p1.Y()) < PreErr_8) {  /// 处理竖直边
      return Min_Val;
    }*/
    double z = Min_Val;
    switch (T._type) {
      case ToolType::PlaneEnd: {
        z = PlaneEndToolCase(T, pro, p0, p1, p);
        break;
      }
      case ToolType::BallNoseEnd: {
        z = BallNoseToolCase(T, pro, p0, p1, p);
        break;
      }
      case ToolType::RoundNoseEnd: {
        //double d2 = p.DistSquare2D(p1);
        double z1 = IntVertProt::VertexProtectZ(T, p1, p);
        if(z1 <= cur_z){break;}
        z = RoundNoseToolCase(T, pro, p0, p1, p);
        break;
      }
      default:
        break;
    }
    return z;
  }

  static double EdgeProtectZ(const DefTool& T, const ofts::Point& p0,
                             const ofts::Point& p1, const ofts::Point& p) {
    auto proj = BaseCalc::GetProjPoint(p, p0, p1);
    if (proj._val < 0 || proj._val > 1) {
      auto op = p;

      return Min_Val;
    }
#if 1
#endif

    double d2 = 0;
    if (proj._val <= 0) {
      d2 = p.DistSquare2D(p0);
    } else if (proj._val >= 1) {
      d2 = p.DistSquare2D(p1);
    } else {
      d2 = p.DistSquare2D(proj._p);
    }
    if (d2 > T._R * T._R) {
      return Min_Val;
    }
    double z = p0.Z() - T._R + std::sqrt(T._R * T._R - d2);
    return z;
  }

  static double TriangleEdgeCase(const DefTool& T, const Triangle& t,
                                 const ofts::Point& p) {
    double z = Min_Val, z0 = Min_Val, z1 = Min_Val, z2 = Min_Val;
    /*if (t.IsInE0Range(p, T.R() + PreErr_10)) {
      auto pro = BaseCalc::GetProjPoint(p, t.P0(), t.P1());
      z0 = EdgeProtectCase(T, pro, t.P0(), t.P1(), p,p.Z());
    }
    if (t.IsInE1Range(p, T.R() + PreErr_10)) {
      auto pro = BaseCalc::GetProjPoint(p, t.P1(), t.P2());
      z1 = EdgeProtectCase(T, pro, t.P1(), t.P2(), p,p.Z());
    }
    if (t.IsInE2Range(p, T.R() + PreErr_10)) {
      auto pro = BaseCalc::GetProjPoint(p, t.P2(), t.P0());
      z2 = EdgeProtectCase(T, pro, t.P2(), t.P0(), p,p.Z());
    }
    z = LimVal::ThreeMax(z0, z1, z2);*/
    return z;
  }
};
}  // namespace grm

#endif  // INTEDGEPROT_H
