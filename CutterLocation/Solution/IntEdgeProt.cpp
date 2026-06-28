#include "IntEdgeProt.h"
namespace grm {

double IntEdgeProt::PlaneEndSweepInt(const DefTool& T, const ProjRes& pro,
                                     const ofts::Point& p0, const ofts::Point& p1,
                                     const ofts::Point& p,size_t id)
{
    double RR = T._RR;
    const auto& pr = pro._p;
    double ym2 = p.DistSquare2D(pr);
    double z = Min_Value;
    double xm = p0.Distance2D(pr);
    if (pro.Val() < 0) {
        xm = -xm;
    }
    double xf = -std::sqrt(RR - ym2);
    double len = p0.Distance2D(p1);
    double rat = (xm - xf) / len;
    if (rat < 0 || rat > 1) {
        return Min_Value;
    }
    double H = p1.Z() - p0.Z();
    z = rat * H;
    return z;

}
double IntEdgeProt::PlaneEndEdgeProtZ(const DefTool& T, const ProjRes& pro,
                                      const ofts::Point& p0, const ofts::Point& p1,
                                      const ofts::Point& p,size_t id)
{
    double z = PlaneEndSweepInt(T,pro,p0,p1,p,id);
    return z + p0.Z();
}
double IntEdgeProt::BallEndSweepInt(const DefTool& T, const ProjRes& pro,
                                    const ofts::Point& p0, const ofts::Point& p1,
                                    const ofts::Point& p,size_t id) {
    double RR = T.RR();
    const auto& pr = pro.P();
    double ym2 = p.DistSquare2D(pr);
    // if (ym2 > RR) {
    // return Min_Value;
    //}
    double z = Min_Value;
    double H = p1.Z() - p0.Z();
    double xm = p0.Distance2D(pr);
    if (pro.Val() < 0) {
        xm = -xm;
    }
    double len = p0.Distance2D(p1);
    double l = p0.Distance3D(p1);
    // double bt = T.R() * H / l;
    // double xk = -bt / T.R() * std::sqrt(RR - ym2);
    double xk = -(H / l) * std::sqrt(RR - ym2);
    double rat = (xm - xk) / len;
    if (rat < 0 || rat > 1) {
        return Min_Value;
    }
    double h = rat * H;
    // if(std::abs(xk) < PreErr_10){
    // z = p0.Z() - T._R + h;return z;
    //}
    z = /*p0.Z() - T._R +*/ h + std::abs(xk) * len / H;
    return z;
}
double IntEdgeProt::BallEndEdgeProtZ(const DefTool& T, const ProjRes& pro,
                                     const ofts::Point& p0, const ofts::Point& p1,
                                     const ofts::Point& p,size_t id)
{
    double z = BallEndSweepInt(T,pro,p0,p1,p,id);
    return z + p0.Z() - T._R;
}
double IntEdgeProt::RoundNoseToolCase(const DefTool& T, const ProjRes& pro,
                                      const ofts::Point& p0, const ofts::Point& p1,
                                      const ofts::Point& p,size_t id) {
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
    double B = 2 * (KK + 1) * (diff)*r;
    double C = (KK + 1) * pnums::Square(diff) - KK * rr - yy;
    double D = -2 * KK * (diff)*r;
    double E = -KK * pnums::Square(diff);
    Eigen::VectorXd params(5);
    params[4] = A;params[3] = B;params[2] = C;
    params[1] = D;params[0] = E;
    Eigen::PolynomialSolver<double, Eigen::Dynamic> solver;
    solver.compute(params);
    Eigen::PolynomialSolver<double, Eigen::Dynamic>::RootsType roots =
            solver.roots();
    double minAlg = 0, maxAlg = Num_PI / 2;
    double val = std::sqrt(pnums::Square(x - L) + yy);
    if (pnums::RightInc2(diff, val, R)) {
        minAlg = std::acos(1 - (val / r));
    }
    if (y > diff && x >= 0) {
        maxAlg = std::acos(1 - (R - y) / r);
    } else {
        if (x < 0) {
            // double dist2 = p.DistSquare2D(p0);
            double dist2 = x * x + yy;
            if (dist2 > diff * diff) {
                maxAlg = std::acos(1 - (R - std::sqrt(dist2)) / r);
            }
        }
    }
    double res_z = Min_Value;
    double minVal = std::cos(maxAlg);
    double maxVal = std::cos(minAlg);
    minVal -= PreErr_15;
    maxVal += PreErr_15;
    std::vector<std::pair<double, double>> roots1;

    for (int i = 0; i < roots.size(); ++i) {
        double imag = roots[i].imag();
        double real = roots[i].real();
        roots1.push_back(std::make_pair(imag, real)); /**
        double curr_r = A * std::pow(real, 4) + B * std::pow(real, 3) +
            C * real * real + D * real + E;*/
        //仅计算虚部为0的实数根
        if (imag == 0 && minVal <= real && real <= maxVal) {
            double cosa = real;
            double sin2 = 1 - cosa * cosa;
            if (sin2 < 0) {
                continue;
            }
            double sin = std::sqrt(sin2);
            double xf2 = pnums::Square(R + r * (cosa - 1)) - yy;
            if (xf2 < -PreErr_15) {
                continue;
            }
            if (xf2 < 0) {
                xf2 = 0;
            }
            double lp = x + std::sqrt(xf2);
            if (lp > L) {
                continue;
            }
            double cz = p0.Z() + r * sin + K * lp - r;
            if (cz > res_z) {
                res_z = cz;
            }
        }
    }
    return res_z;
}

double IntEdgeProt::TaperEndSweepInt(const DefTool& T,const ofts::Point& op,
                                     double x1, double z1,size_t id) {
    double cur_z = Min_Value;
    double R = T._R;
    double r = T._br;
    double h = T._h;
    double RR = T._RR;
    double rr = T._br2;
    double px = op.X();
    double py = op.Y();
    double x1_sq = x1 * x1;
    double py_sq = py * py;

    auto get_dt = [&](double t) {
        return std::sqrt(std::pow(px - x1 * t, 2) + py * py);
    };
    auto get_zt = [&](double t) -> double {
        double d = get_dt(t);
        return z1 * t + h * (R - d) / (R - r);
    };
    auto constraint = [&](double t) {
        double d = get_dt(t);
        return (d >= r - PreErr5_12) && (d <= R + PreErr5_12);
    };
    std::vector<double> cal_ts;
    // 计算端点情况
    if (constraint(0.0)) {
        cal_ts.emplace_back(0.0);
    }
    if (constraint(1.0)) {
        cal_ts.emplace_back(1.0);
    }
    // 计算约束边界点,即d(t)=R和d(t)=r时的t值
    auto boundary = [&](double dd) {
        if (dd < py_sq) {
            return;
        }
        double sqrt_d = std::sqrt(dd - py_sq);
        double t1 = (px - sqrt_d) / x1;
        double t2 = (px + sqrt_d) / x1;
        if (0 <= t1 && t1 <= 1) {
            cal_ts.emplace_back(t1);
        }
        if (0 <= t2 && t2 <= 1) {
            cal_ts.emplace_back(t2);
        }
    };
    boundary(rr);
    boundary(RR);
    // 计算可能的内部驻点
    double k = h / (R - r);
    double A = k * x1;
    // 情况1:A = z1(即h*x1 = (R-r)*z1),且py = 0,此时可能的驻点t=px/x1
    // 对于常数区间,整个区间都是最大值点,选择区间的中点作为代表
    if (std::abs(A - z1) < PreErr_12 && std::abs(py) < PreErr_12) {
        double t_min = pnums::Max(0.0, px / x1);  // 常数区间的起点,满足px-x1t<0
        double t_max = 1.0;  // 常数区间的终点,整个区间都是最大值点
        if (t_min < t_max) {                   // 如果常数区间非空
            double t_mid = (t_min + t_max) / 2;  // 取区间中点作为代表
            double d = get_dt(t_mid);
            if (d >= r && d <= R) {
                cal_ts.push_back(t_mid);
            }
        }
    }
    // 情况2:A > z1,即h*x1 > (R-r)*z1
    // 此时存在一个内部驻点,需要检查是否在有效区间内,且
    // t0 = (px + sqrt(z1^2*py^2/(A^2-z1^2))) / x1
    if (A > z1 && std::abs(py) > 0) {  // 确保分母为正
        double deno = A * A - z1 * z1;
        if (deno > 0) {
            double nume = z1 * z1 * py_sq;
            double sqrt_term = std::sqrt(nume / deno);
            double t0 = (px + sqrt_term) / x1;
            if (t0 > 0 && t0 < 1) {
                double d0 = get_dt(t0);
                if (d0 >= r && d0 <= R) {
                    cal_ts.push_back(t0);
                }
            }
        }
    }
    if (cal_ts.empty()) {
        return cur_z;
    }
    std::vector<double> zs;
    for (auto& t : cal_ts) {
        zs.emplace_back(get_zt(t));
    }
    std::sort(zs.begin(), zs.end(),
              [](const double& a, const double& b) { return a < b; });
    cur_z = zs.back();
    return cur_z;
}
double IntEdgeProt::TaperEndToolCase(const DefTool& T, const ProjRes& pro,
                                     const ofts::Point& p0, const ofts::Point& p1,
                                     const ofts::Point& p, double d2,size_t id) {
#if 0
    ofts::Point op0(24.565000,-35.000000,25.606602);
    ofts::Point op1(18.707135,-35.000000,25.606602);
    if(p.IsSameCoord2D(ofts::Point(20,-36,25.3137),PreErr5_4) &&
            (p0.IsSameCoord2D(op0,PreErr5_4) || p0.IsSameCoord2D(op1,PreErr5_4)) &&
            (p1.IsSameCoord2D(op0,PreErr5_4) || p1.IsSameCoord2D(op1,PreErr5_4))){
        int tem = 1;
    }
#endif
    if (d2 > T._RR) {
        return Min_Value;
    }
    double cur_z = Min_Value;
    double low_z = Min_Value,side_z = Min_Value,up_z = Min_Value;
    low_z = PlaneEndSweepInt(T, pro, p0, p1, p,id);
    low_z -= T._h;
    if (d2 <= T._br * T._br) {
        up_z = PlaneEndSweepInt(*T._subTool, pro, p0, p1, p,id);
    }
    double px = p0.Distance2D(pro._p);
    double py = p.Distance2D(pro._p);
    double x1 = p0.Distance2D(p1);
    double z1 = p1.Z() - p0.Z();
    if (pro._val < 0) {
        px = -px;
    }
    ofts::Point op(px, py);
    side_z = TaperEndSweepInt(T, op, x1, z1,id);
    side_z -= T._h;
    
    double cal_z = pnums::ThreeMax(low_z,side_z,up_z);
    cur_z = p0.Z() + cal_z;
#if 0
    // 18,-36,25.5541 id:1632
    if (p.IsSamePoint2D(ofts::Point(18, -36, 25.5541), PreErr5_6) &&
            cur_z > 25) {
        double dist = BaseCalc::PtToSegDistSqua2D(p, p0, p1);
        int tem = 1;
    }
#endif
    return cur_z;
}
double IntEdgeProt::TaperBallSweepInt(const DefTool& T, const ofts::Point& op,
                                      double x1, double z1,size_t id)
{
    double cur_z = Min_Value;
    double R = T._R;
    double RR = T._RR;
    double px = op.X();
    double py = op.Y();
    double x1_sq = x1 * x1;
    double z1_sq = z1 * z1;
    double py_sq = py * py;
    //计算d(t)
    auto dist = [&](double t){
        return std::sqrt(std::pow(px - x1 * t, 2) + py_sq);
    };
    //计算z(t)
    auto z_t = [&](double t){
        double d = dist(t);
        return z1 * t + std::sqrt(RR - d * d);
    };
    //检查t是否在定义域内
    auto is_in_domain = [&](double t){
        if (t < 0 || t > 1) {return false;}
        double d = dist(t);
        return d <= R + PreErr_10;  //考虑浮点误差
    };
    auto update_z = [&](double t){
        if(!is_in_domain(t)){
            return;
        }
        double z = z_t(t);
        if(cur_z < z){
            cur_z = z;
        }
    };
    //添加t=0的端点
    update_z(0);
    //添加t=1的端点
    update_z(1);
    double M = std::sqrt(RR - py_sq);
    //计算定义域边界,即计算由d(t)=R决定的边界点
    double t_min = (px - M) / x1;
    double t_max = (px + M) / x1;
    update_z(t_min);
    update_z(t_max);
    //计算极值点ts(驻点)
    double v1 = std::sqrt(x1_sq + z1_sq);
    double v2 = z1 * M / v1;
    double ts = (px + v2) / x1;
    //检查驻点是否满足t>px/x1的条件
    if (ts > px / x1 + PreErr_10) {
        update_z(ts);
    }
    return cur_z;
}
double IntEdgeProt::TaperBallToolCase(const DefTool& T, const ProjRes& pro,
                                      const ofts::Point& p0, const ofts::Point& p1,
                                      const ofts::Point& p, double d2,size_t id) {
    double cur_z = Min_Value;
    if (d2 > T._RR) {
        return cur_z;
    }
#if 0
    ofts::Point cp1(-3,19,22.5755); //side(1764,1768)
    ofts::Point cp2(-4,19,22.4035); //side(1764,1768)
    ofts::Point cp3(-5,19,22.1944);
    ofts::Point cp4(-56,-50,22.1944);
    if(p.IsSameCoord2D(cp4,PreErr5_4) && id == 4065){
        auto d1 = p.Distance2D(p0);
        auto d2 = p.Distance2D(p1);
        std::cout<<"";
    }
#endif
    double low_z = Min_Value,side_z = Min_Value,up_z = Min_Value;
    low_z = PlaneEndSweepInt(T, pro, p0, p1, p,id);
    
    double px = p0.Distance2D(pro._p);
    double py = p.Distance2D(pro._p);
    double x1 = p0.Distance2D(p1);
    double z1 = p1.Z() - p0.Z();
    if (pro._val < 0) {px = -px;}
    
    ofts::Point op(px, py);
    side_z = TaperEndSweepInt(T, op, x1, z1,id);
    
    if(d2 < T._br2){
        //up_z = BallEndSweepInt(*T._subTool,pro,p0,p1,p,id);
        up_z = TaperBallSweepInt(*T._subTool,op,x1,z1,id);
        up_z += (T._h - T._cr);
        /*if(std::abs(up_z1 - up_z2)>1e-5){
            std::cout<<"";
        }*/
    }
    double cal_z = pnums::ThreeMax(up_z, side_z, low_z);
    cur_z = p0.Z() + cal_z - T._h;
#if 0
    if(p.IsSameCoord2D(cp2,PreErr5_4) && pnums::IsNear(cur_z,22.4035,1e-4) ){
        std::cout<<"";
    }
#endif
    return cur_z;
}
double IntEdgeProt::EdgeProtectCase(const DefTool& T, const ProjRes& pro,
                                    const ofts::Point& p0, const ofts::Point& p1,
                                    const ofts::Point& p, double cur_z,size_t id) {
    double z = Min_Value;
    switch (T._type) {
    case ToolType::PlaneEnd: {
        z = PlaneEndEdgeProtZ(T, pro, p0, p1, p,id);
        break;
    }
    case ToolType::BallNoseEnd: {
        z = BallEndEdgeProtZ(T, pro, p0, p1, p,id);
        break;
    }
    case ToolType::RoundNoseEnd: {
        double d2 = p.DistSquare2D(p1);
        double z1 = IntVertProt::RoundNoseVertProtZ(T, p1, p, d2);
        if (z1 <= cur_z) {
            break;
        }
        z = RoundNoseToolCase(T, pro, p0, p1, p,id);
        break;
    }
    case ToolType::TaperEnd: {
        double d2 = BaseCalc::PtToSegDistSqua2D(p, p0, p1);
        z = TaperEndToolCase(T, pro, p0, p1, p, d2,id);
        break;
    }
    case ToolType::TaperBall: {
        double d2 = BaseCalc::PtToSegDistSqua2D(p, p0, p1);
        z = TaperBallToolCase(T, pro, p0, p1, p, d2,id);
        break;
    }
    default:
        break;
    }
    // if(std::isinf(z)){
    //     int tem = 1;
    // }
    return z;
}




}
