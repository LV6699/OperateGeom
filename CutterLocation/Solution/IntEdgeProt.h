#ifndef INTEDGEPROT_H
#define INTEDGEPROT_H

#include <Eigen/Dense>
#include <unsupported/Eigen/Polynomials>
#include <limits>
#include <cmath>
#include <algorithm>
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
                                 const ofts::Point& p)
  {
    double RR = T._RR;
    const auto& pr = pro._p;
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
                                 const ofts::Point& p) 
  {
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

  static double TaperEndToolUp(const DefTool& T, const ofts::Point& op, 
                               double x1, double z1)
  {
    double r2 = T._rr;
    double h = T._l;
    double px = op.X();
    double py2 = op.Y() * op.Y();
    
    double cur_z = Min_Val;
    if(py2 > r2){
        return cur_z;
    }  
    double L_r_sq = pnum::Max(0.0, r2 - py2);
    double L_r = std::sqrt(L_r_sq);
    double t_max = Min_Val;
    
    if (std::abs(x1) < PreErr5_10) {
        if (std::abs(px) <= L_r + PreErr5_10) {
            t_max = 1.0;
        }
    } else {
        double t1 = (px - L_r) / x1;
        double t2 = (px + L_r) / x1;
        if (t1 > t2) {
            std::swap(t1, t2);
        }

        t1 = pnum::Max(t1, 0.0);
        t2 = pnum::Min(t2, 1.0);
        if (t2 >= t1 - PreErr5_10) {
            t_max = t2;
        }
    }
    if (t_max < -PreErr5_10){
        return cur_z;
    } 
    cur_z = z1 * t_max + h;
    return cur_z;
  }

  static void GetSideIntervals(double px, double py, double x1, double r,double R,
                               vector<std::pair<double, double>>& intervals) {
    double py_sq = py * py;
    double L_r_sq = pnum::Max(0.0, r * r - py_sq);
    double L_R_sq = pnum::Max(0.0, R * R - py_sq);

    if (L_R_sq <= L_r_sq + PreErr5_10) {
      return;
    }
    double L_r = std::sqrt(L_r_sq);
    double L_R = std::sqrt(L_R_sq);
    if (std::abs(x1) < PreErr5_10) {
      // x1 = 0 特殊情况
      double dist = std::sqrt(px * px + py_sq);
      if (L_r < dist && dist < L_R) {
        intervals.push_back({0.0, 1.0});
      }
      return;
    }
    // 情况1: q ∈ (L_r, L_R)
    double t1_min = (px - L_R) / x1;
    double t1_max = (px - L_r) / x1;
    if (t1_min > t1_max) {
      std::swap(t1_min, t1_max);
    }
    // 情况2: q ∈ (-L_R, -L_r)
    double t2_min = (px + L_r) / x1;
    double t2_max = (px + L_R) / x1;
    if (t2_min > t2_max) {
      std::swap(t2_min, t2_max);
    }
    // 与 [0,1] 取交集
    auto clip = [](double a, double b) -> std::pair<double, double> {
      double left = pnum::Max(0.0, pnum::Min(a, b));
      double right = pnum::Min(1.0, pnum::Max(a, b));
      if (right > left + PreErr5_10) {
        return {left, right};
      }
      return {Max_Val, Max_Val};
    };
    auto iv1 = clip(t1_min, t1_max);
    auto iv2 = clip(t2_min, t2_max);

    if (iv1.first < Max_Val) {
      intervals.push_back(iv1);
    }
    if (iv2.first < Max_Val) {
      intervals.push_back(iv2);
    }
  }
  static double PosToLineDist(double t, double px, double py, double x1) {
    double q = px - x1 * t;
    return std::sqrt(q * q + py * py);
  };
  static bool IsInSide_t(double t, double px, double py, double x1, 
                         double r,double R) {
    double dist = PosToLineDist(t, px, py, x1);
    return (r + PreErr5_10 < dist && dist < R - PreErr5_10);
  };
  // 处理常数区间情况 (x1^2 = K^2 且 py=0)
  static void GetConstIntervalPts(double px, double py, double x1, double r,
                                  double R, double K, vector<double>& ps) {
    if (std::abs(py) > PreErr5_10) {
      return;  // 仅当 py=0 时可能有常数区间
    }
    if (x1 > PreErr5_10) {
      // q ∈ (-R, -r)
      double t_low = (px + r) / x1;
      double t_high = (px + R) / x1;
      if (t_low > t_high) {
        std::swap(t_low, t_high);
      }
      // 与 [0,1] 取交集
      t_low = pnum::Max(t_low, 0.0);
      t_high = pnum::Min(t_high, 1.0);

      if (t_high > t_low + PreErr5_10) {
        // 取中点作为代表点
        double t_mid = (t_low + t_high) / 2.0;
        if (IsInSide_t(t_mid, px, py, x1, r, R)) {
          ps.push_back(t_mid);
        }
      }
    } else if (x1 < -PreErr5_10) {
      // q ∈ (r, R)
      double t_low = (px - R) / x1;
      double t_high = (px - r) / x1;
      if (t_low > t_high) {
        std::swap(t_low, t_high);
      }
      t_low = pnum::Max(t_low, 0.0);
      t_high = pnum::Min(t_high, 1.0);
      if (t_high > t_low + PreErr5_10) {
        double t_mid = (t_low + t_high) / 2.0;
        if (IsInSide_t(t_mid, px, py, x1, r, R)) {
          ps.push_back(t_mid);
        }
      }
    }
  }
  static double TaperEndToolSide(const DefTool& T,const ofts::Point& op,
                                 double x1,double z1) 
  {
    double R = T._R;
    double r = T._cr;
    double h = T._l;
    double RR = T._RR;
    double rr = T._rr;
    double py = op.Y();
    double px = op.X();
    // 1. 计算 K
    double K = z1 * (R - r) / h;
    double K_sq = K * K;
    double x1_sq = x1 * x1;
    double py_sq = py * py;

    std::vector<double> candidates;
    // 1. 端点 t=0, t=1
    if(IsInSide_t(0,px,py,x1,r,R)){
        candidates.push_back(0);
    }
    if(IsInSide_t(1,px,py,x1,r,R)){
        candidates.push_back(1);
    }
    // 2. 获取 T_side 区间
    std::vector<std::pair<double,double>>intervals;
    GetSideIntervals(px,py,x1,r,R,intervals);
    // 3. 处理常数区间 (x1^2 = K^2 且 py=0)
    if (std::abs(x1_sq - K_sq) < PreErr5_10 && std::abs(py) < PreErr5_10) {
        vector<double>ps;
        GetConstIntervalPts(px, py, x1, r, R, K,ps);
        candidates.insert(candidates.end(), ps.begin(), ps.end());
    }
    // 4. 处理驻点 (x1^2 ≠ K^2 或 py ≠ 0)
    if (std::abs(x1_sq - K_sq) > PreErr5_10 || std::abs(py) > PreErr5_10) {
        if (x1_sq > K_sq + PreErr5_10) {
            double denom = std::sqrt(x1_sq - K_sq);
            double q1 = K * std::fabs(py) / denom;
            double q2 = -q1;
            
            vector<double> q_candidates;
            if (x1 * q1 < -PreErr5_10){
                q_candidates.push_back(q1);
            } 
            if (x1 * q2 < -PreErr5_10){
                q_candidates.push_back(q2);
            }
            for (double q : q_candidates) {
                double t_s = (px - q) / x1;
                if (t_s >= -PreErr5_10 && t_s <= 1.0 + PreErr5_10 && 
                    IsInSide_t(t_s, px, py, x1, r, R)) {
                    candidates.push_back(t_s);
                }
            }
        }
    }
    // 5. 对每个区间，添加中点和端点作为候选点
    for (const auto& interval : intervals) {
      double t_mid = (interval.first + interval.second) / 2.0;
      candidates.push_back(t_mid);
      // 端点 (如果不在常数区间内)
      bool is_const_interval = (std::abs(py) < PreErr5_10 && 
                                std::abs(x1_sq - K_sq) < PreErr5_10);
      if (!is_const_interval) {
        candidates.push_back(interval.first);
        candidates.push_back(interval.second);
      }
    }
    // 6. 去重
    std::sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end(),
                     [](double a, double b) {
                        return std::fabs(a - b) < PreErr5_10;}),
                        candidates.end());
    // 计算侧面 z 坐标
    auto z_side = [](double t, double px, double py, double x1, double z1,
                     double h, double R, double r) {
      double dist = PosToLineDist(t, px, py, x1);
      return z1 * t + h * (dist - R) / (r - R);
    };
    // 7. 计算最大值
    double cur_z = Min_Val;
    for (double t : candidates) {
        double z_val = z_side(t, px, py, x1, z1, h, R, r);
        if (z_val > cur_z) {
            cur_z = z_val;
        }
    }
    return cur_z; 
  }

  static double TaperEndToolCase(const DefTool& T, const ProjRes& pro,
                                 const ofts::Point& p0, const ofts::Point& p1,
                                 const ofts::Point& p) 
  {
    if(BaseCalc::PtToSegDistSqua2D(p,p0,p1) > T._RR){
        return Min_Val;
    }
    double cur_z = Min_Val;
    double rr = T._rr;
    double px = p0.Distance2D(pro._p);
    double py = p.Distance2D(pro._p);
    double x1 = p0.Distance2D(p1);
    double z1 = p1.Z() - p0.Z();
    ofts::Point op(px,py);
    
    double up_z = Min_Val,side_z = Min_Val,low_z = Min_Val;
    low_z = PlaneEndToolCase(T,pro,p0,p1,p);
    up_z = TaperEndToolUp(T,op,x1,z1);
    side_z = TaperEndToolSide(T,op,x1,z1);
    double cal_z = pnum::ThreeMax(up_z,side_z,low_z-p0.Z());
    cur_z = p0.Z() - T._l + cal_z;
    
    if(p.IsSamePoint2D(ofts::Point(37,-34,18.8009),PreErr5_6) && cur_z > 18){
        double dist = BaseCalc::PtToSegDistSqua2D(p,p0,p1);
        int tem = 1;
    }
    return cur_z;

  }

  static double RoundNoseToolCase(const DefTool& T, const ProjRes& pro,
                                  const ofts::Point& p0, const ofts::Point& p1,
                                  const ofts::Point& p) 
  {
    double R = T._R;
    double r = T._cr;
    double diff = T._diff;
    double L = p0.Distance2D(p1);
    double K = (p1.Z() - p0.Z()) / L;
    double KK = K * K;
    double rr = r * r;
    double x = pro._p.Distance2D(p0);
    if (pro._val < 0) {
      x = -x;
    }
    double yy = p.DistSquare2D(pro._p);
    double y = std::sqrt(yy);
    double A = (KK + 1) * rr;
    double B = 2 * (KK + 1) * (diff) * r;
    double C = (KK + 1) * pnum::Square(diff) - KK * rr - yy;
    double D = -2 * KK * (diff) * r;
    double E = -KK * pnum::Square(diff);
    Eigen::VectorXd params(5);
    params[4] = A;params[3] = B;params[2] = C;
    params[1] = D;params[0] = E;
    Eigen::PolynomialSolver<double, Eigen::Dynamic> solver;
    solver.compute(params);
    Eigen::PolynomialSolver<double, Eigen::Dynamic>::RootsType roots =
        solver.roots();
    double minAlg = 0, maxAlg = _PI_2_;
    double val = std::sqrt(pnum::Square(x - L) + yy);
    if (pnum::RightInc2(diff, val, R)) {
      minAlg = std::acos(1 - (val / r));
    }
    if (y > diff && x >= 0) {
      maxAlg = std::acos(1 - (R - y) / r);
    } else {
        if(x < 0){
          // double dist2 = p.DistSquare2D(p0);
          double dist2 = x * x + yy;
          if (dist2 > diff * diff) {
            maxAlg = std::acos(1 - (R - std::sqrt(dist2)) / r);
          }
        }
    }
    double res_z = Min_Val;
    double minVal = std::cos(maxAlg);
    double maxVal = std::cos(minAlg);
    minVal -= PreErr_15;
    maxVal += PreErr_15;
    std::vector<std::pair<double,double>>roots1;
    
    for (int i = 0; i < roots.size(); ++i) {
      double imag = roots[i].imag();
      double real = roots[i].real();
      roots1.push_back(std::make_pair(imag,real));/**
      double curr_r = A * std::pow(real, 4) + B * std::pow(real, 3) +
                      C * real * real + D * real + E;*/
      /// 仅计算虚部为0的实数根
      if (imag == 0 && minVal <= real && real <= maxVal) {
        double cosa = real;
        double sin2 = 1 - cosa * cosa;
        if (sin2 < 0) {
          continue;
        }
        double sin = std::sqrt(sin2);
        double xf2 = pnum::Square(R + r * (cosa - 1)) - yy;
        if(xf2 < -PreErr_15){
            continue;
        }
        if(xf2 < 0){
            xf2 = 0;
        }
        double lp = x + std::sqrt(xf2);
        if (lp > L) {
          continue;
        }
        double cz = p0.Z() - r + r * sin + K * lp;
        if (cz > res_z) {
          res_z = cz; 
        }
      }
    }
    return res_z;
  }

  static double EdgeProtectCase(const DefTool& T, const ProjRes& pro,
                                const ofts::Point& p0, const ofts::Point& p1,
                                const ofts::Point& p, double cur_z) {
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
        double d2 = p.DistSquare2D(p1);
        double z1 = IntVertProt::RoundNoseVertProtZ(T,p1,p,d2);
        if(z1 <= cur_z){break;}
        z = RoundNoseToolCase(T, pro, p0, p1, p);
        break;
      }
      case ToolType::TaperEnd: {
        z = TaperEndToolCase(T, pro, p0, p1, p);
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


};
}  // namespace grm

#endif  // INTEDGEPROT_H
