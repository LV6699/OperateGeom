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
    static double RoundNoseToolCase(const DefTool& T, const ProjRes& pro,
                                    const ofts::Point& p0, const ofts::Point& p1,
                                    const ofts::Point& p)
    {
        double R = T._R;
        double r = T._cr;
        double diff = T._R_r;
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

    static double TaperEndLocation(const DefTool& T,const ofts::Point& op,
                                   double x1,double z1)
    {
        double cur_z = Min_Val;
        double R = T._R;
        double r = T._br;
        double h = T._h;
        double RR = T._RR;
        double rr = T._rr;
        double px = op.X();
        double py = op.Y();
        double x1_sq = x1 * x1;
        double py_sq = py * py;
        
        auto get_dt = [&](double t){
            return std::sqrt(std::pow(px-x1*t,2) + py*py);
        };
        auto get_zt = [&](double t) -> double {
            double d = get_dt(t);
            return z1*t + h*(R - d)/(R - r);
        };
        auto constraint = [&](double t) {
            double d = get_dt(t);
            return (d >= r-PreErr5_12) && (d <= R+PreErr5_12);
        };
        std::vector<double>cal_ts;
        //计算端点情况
        if (constraint(0.0)) {
            cal_ts.emplace_back(0.0);
        }
        if (constraint(1.0)) {
            cal_ts.emplace_back(1.0);
        }
        //计算约束边界点,即d(t)=R和d(t)=r时的t值
        auto boundary = [&](double dd){
            if(dd < py_sq){return;}
            double sqrt_d = std::sqrt(dd - py_sq);
            double t1 = (px - sqrt_d)/x1;
            double t2 = (px + sqrt_d)/x1;
            if (0 <= t1 && t1 <= 1){
                cal_ts.emplace_back(t1);
            }
            if (0 <= t2 && t2 <= 1){
                cal_ts.emplace_back(t2);
            }
        };
        boundary(rr);
        boundary(RR);
        //计算可能的内部驻点
        double k = h / (R - r);
        double A = k * x1;
        //情况1:A = z1(即h*x1 = (R-r)*z1),且py = 0
        //对于常数区间,整个区间都是最大值点,选择区间的中点作为代表
        if (std::abs(A - z1) < PreErr_12 && std::abs(py) < PreErr_12) {
            double t_min = pnum::Max(0.0,px/x1);  //常数区间的起点
            double t_max = 1.0;   //常数区间的终点,整个区间都是最大值点
            if (t_min < t_max) {  //如果常数区间非空
                double t_mid = (t_min + t_max) / 2;  //取区间中点作为代表
                double d = get_dt(t_mid);
                if (d >= r && d <= R) {
                    cal_ts.push_back(t_mid);
                }
            }
        }
        //情况2:A > z1,即h*x1 > (R-r)*z1
        //此时存在一个内部驻点,需要检查是否在有效区间内,且
        //t0 = (px + sqrt(z1^2*py^2/(A^2-z1^2))) / x1
        if (A > z1) {  //确保分母为正                         
            double deno = A*A - z1*z1;
            if(deno > 0) {
                double nume = z1*z1 * py_sq;
                double sqrt_term = std::sqrt(nume/deno);
                double t0 = (px + sqrt_term) / x1;
                if (t0 > 0 && t0 < 1) {
                    double d0 = get_dt(t0);
                    if (d0 >= r && d0 <= R) {
                        cal_ts.push_back(t0);
                    }
                }
            }
        }
        if(cal_ts.empty()){
            return cur_z;
        }
        std::vector<double>zs;
        for(auto& t : cal_ts){
            zs.emplace_back(get_zt(t));
        }
        std::sort(zs.begin(),zs.end(),[](const double& a,
            const double& b){return a < b;});
        cur_z = zs.back();
        return cur_z;
    }

    static double TaperEndToolCase(const DefTool& T, const ProjRes& pro,
                                   const ofts::Point& p0, const ofts::Point& p1,
                                   const ofts::Point& p)
    {
        double dist = BaseCalc::PtToSegDistSqua2D(p,p0,p1);
        if(dist > T._RR){
            return Min_Val;
        }
        double cur_z = Min_Val;
        double px = p0.Distance2D(pro._p);
        double py = p.Distance2D(pro._p);
        double x1 = p0.Distance2D(p1);
        double z1 = p1.Z() - p0.Z();
        if(pro._val < 0){px = -px;}
        ofts::Point op(px,py);

        double up_z = Min_Val,side_z = Min_Val,low_z = Min_Val;\
        {
            auto up0 = p0,up1 = p1;auto uT = T;uT.SetR(T._br);
            up0.SetZ(p0.Z()+T._h);up1.SetZ(p1.Z()+T._h);
            if(dist <= T._br*T._br){
                up_z = PlaneEndToolCase(uT,pro,up0,up1,p);
                up_z -= up0.Z();
            }
        }
        low_z = PlaneEndToolCase(T,pro,p0,p1,p);
        low_z -= p0.Z();
        //up_z = TaperEndToolUp(T,op,x1,z1);
        side_z = TaperEndLocation(T,op,x1,z1);
        double cal_z = pnum::ThreeMax(up_z,side_z,low_z);
        //if(std::abs(py - T._br) <= PreErr5_12){cal_z = up_z;}
        //else if(std::abs(py - T._R) <= PreErr5_12){cal_z = low_z;}
        //else{cal_z = side_z;}
        cur_z = p0.Z() - T._h + cal_z;
        //22,22->25 px=6.6086238112816140,py=1,i:1761(8.3406746) 1804(8.3406746)  
        //22,22->23 px=6.6086238112816140,py=2,i:1767(6.6086238) 1804(6.6086238)
        //36,-36->17 1591 1677 
        //37,-36->17 1591 1677  19,15,19.1234 19,14,20.1234
        if(p.IsSamePoint2D(ofts::Point(19,14,20.1234),PreErr5_6) && cur_z > 20){
            double dist = BaseCalc::PtToSegDistSqua2D(p,p0,p1);
            int tem = 1;
        }
        return cur_z;

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
