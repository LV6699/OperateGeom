#ifndef BASEGEOM2D_H
#define BASEGEOM2D_H
#include"../../CommonFile/DataStructure.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class BaseGeom2D
{
public:
    BaseGeom2D(){}
    static ofts::Point GetPtBaseDist(const ofts::Point& sp,
                                     const ofts::Point& p1,
                                     const ofts::Point& p2,
                                     bool isClose)
    {
        auto d1 = sp.DistSquare2D(p1);
        auto d2 = sp.DistSquare2D(p2);
        if(isClose){return d1 < d2 ? p1 : p2;}
        return d1 > d2 ? p1 : p2;
    }
    static void GetLineParam(const ofts::Point& sp,const ofts::Point& ep,
                             double paralX,double paralY,
                             double param[3])
    {
        if(!sp.IsLegal() || !ep.IsLegal()){return;}
        if(std::abs(ep.Y() - sp.Y()) <= paralY){param[0] = 0;}
        else{param[0] = ep.Y() - sp.Y();}
        if(std::abs(sp.X() - ep.X()) <= paralX){param[1] = 0;}
        else{param[1] = sp.X() - ep.X();}
        param[2] = (ep.X() * sp.Y()) - (sp.X() * ep.Y());
    }
    static void GetLineParam(const ofts::Point& sp,const ofts::Point& ep,
                             double paralX,double paralY,double& A,double& B,double& C)
    {
        double ra[3];
        GetLineParam(sp,ep,paralX,paralY,ra);
        A = ra[0];B = ra[1];C = ra[2];
    }
    static double PointAngle(const ofts::Point& sp,const ofts::Point& ep)
    {
        double x = ep.X() - sp.X(),y = ep.Y() - sp.Y();
        double a = std::atan2(y,x) * _AlgRad;
        if (a < 0) {a += 360;}
        return a;
    }
    static double SegmentAngle(const ofts::Point& sp,const ofts::Point& ep)
    {
        double x = ep.X() - sp.X(),y = ep.Y() - sp.Y();
        if(std::abs(x) < PreErr_10){
            if(std::abs(y) < PreErr_10){return Min_Value;}
            if (y > 0) {return 90.0;}
            else {return 270.0;}
        }
        if (std::abs(y) < PreErr_10) {
            if (x > 0) {return 0.0;}
            else {return 180.0;}
        }
        double a = std::atan2(y,x) * _AlgRad;
        if (a < 0) {a += 360;}
        return a;
    }
    static double SegmentSlope(const ofts::Point& sp,const ofts::Point& ep)
    {
        double x = ep.X() - sp.X(),y = ep.Y() - sp.Y();
        if (std::abs(x) < PreErr_10) {return Max_Value;}
        return y / x;
    }
    static double PointToLineDist(const ofts::Point &sp,
                                  const ofts::Point &ep,
                                  const ofts::Point &P)
    {
        double pa[3];
        GetLineParam(sp,ep,PreErr5_16,PreErr5_16,pa);
        if(std::abs(pa[0]) < PreErr5_16){
            double d1 = std::abs(sp.Y() - P.Y()),d2 = std::abs(ep.Y() - P.Y());
            return (d1 + d2) / 2;
        }
        if(std::abs(pa[1]) < PreErr5_16){
            double d1 = std::abs(sp.X() - P.X()),d2 = std::abs(ep.X() - P.X());
            return (d1 + d2) / 2;
        }
        return std::abs(pa[0] * P.X() + pa[1] * P.Y() + pa[2]) /
                std::sqrt(pa[0] * pa[0] + pa[1] * pa[1]);
    }
    static double PtToLineDistSqur(const ofts::Point &sp,
                                   const ofts::Point &ep,
                                   const ofts::Point &p)
    {
        double dx = ep.X() - sp.X();
        double dy = ep.Y() - sp.Y();
        double demo = dx * dx + dy * dy;
        if(demo < PreErr5_16){return Max_Value;}
        double cross = dx * (sp.Y() - p.Y()) - dy * (sp.X() - p.X());
        return (cross * cross) / demo;
    }
    static void PointOnLineProj(const ofts::Point &p,const ofts::Point &sp,
                                const ofts::Point &ep,ofts::Point& op)
    {
#if 0
        double param[3];
        GetLineParam(sp,ep,0,0,param);
        double x0 = p.X(),y0 = p.Y();
        double A = param[0],B = param[1],C = param[2];
        double t = A*A + B*B;
        op.SetX((B*B*x0 - A*B*y0 - A*C) / t);
        op.SetY((-A*B*x0 + A*A*y0 - B*C) / t);
#else
        double abx = ep.X() - sp.X();
        double aby = ep.Y() - sp.Y();
        double apx = p.X() - sp.X();
        double apy = p.Y() - sp.Y();
        double dot_ap_ab = apx * abx + apy * aby;
        double dot_ab_ab = abx * abx + aby * aby;
        double t = dot_ap_ab / dot_ab_ab;
        op.SetX(sp.X() + t * abx);
        op.SetY(sp.Y() + t * aby);
#endif
        op.SetZ(sp.Z());
    }
    static ofts::Point PointOnLineProj(const ofts::Point& p,
                                       const ofts::Point& sp,
                                       const ofts::Point& ep)
    {
        ofts::Point op;PointOnLineProj(p,sp,ep,op);return op;
    }
    static void ArcEndpointAngle(const ofts::Point& sp,
                                 const ofts::Point& ep,
                                 const ofts::Point& cp,
                                 double& startAgl,
                                 double& endAgl)
    {
        startAgl = std::atan2((sp.Y() - cp.Y()),(sp.X() - cp.X()));
        endAgl = std::atan2((ep.Y() - cp.Y()),(ep.X() - cp.X()));
        startAgl = startAgl *_AlgRad;
        endAgl = endAgl * _AlgRad;
        if (startAgl < 0){ startAgl = 360 + startAgl;}
        if (endAgl < 0){endAgl = 360 + endAgl;}
    }
    static void ArcEndpointAngle(const ofts::DefArc& arc,
                                 double& startAgl,
                                 double& endAgl)
    {
        ArcEndpointAngle(arc._arcSp,arc._arcEp,
                         arc._arcCp,startAgl,endAgl);
    }
    static double ArcCenterAngle(const ofts::DefArc& arc)
    {
        double startAgl,endAgl,agl;
        ArcEndpointAngle(arc._arcSp,arc._arcEp,
                         arc._arcCp,startAgl,endAgl);
        agl = pnum::TwoAglIncludedAgl(startAgl,endAgl,arc.IsCW());
        return agl;
    }
    static void DiscreteArcToPoint(const ofts::DefArc& arc,
                                   double stepVal,
                                   std::vector<ofts::Point>& points)
    {
        double centerAgl = ArcCenterAngle(arc);
        points.emplace_back(arc._arcSp);
        if(centerAgl <= stepVal || centerAgl >= 360){
            if(!arc._arcSp.IsSamePoint2D(arc._arcEp,PreErr5_12)){
                points.emplace_back(arc._arcEp);
            }
            return;
        }
        if(centerAgl <= stepVal * 2){
            auto mp = ArcMidpoint(arc);
            points.emplace_back(mp);
            points.emplace_back(arc._arcEp);
            return;
        }
        if(stepVal < PreErr_4){return;}
        ofts::Point pt;
        double curAgl = stepVal;
        bool isCW = arc.IsCW();
        while (curAgl < centerAgl) {
            PointRotate2D(arc._arcSp,arc._arcCp,
                          curAgl,isCW,pt);
            points.emplace_back(pt);
            curAgl += stepVal;
        }
        if(!points.back().IsSamePoint2D(arc._arcEp,PreErr5_12)){
            points.emplace_back(arc.GetEp());
        }
    }
    static void PointRotate2D(const ofts::Point& p,const ofts::Point& fix,
                              double A, bool isCW, ofts::Point& pt)
    {
        A = A * _RadAlg;
        const double& x = p.X(),y = p.Y(),cx = fix.X(),cy = fix.Y();
        if(isCW){
            pt.SetX((x - cx)*std::cos(-A) - (y - cy)*std::sin(-A) + cx);
            pt.SetY((x - cx)*std::sin(-A) + (y - cy)*std::cos(-A) + cy);
        }else{
            pt.SetX((x - cx)*std::cos(A) - (y - cy)*std::sin(A) + cx);
            pt.SetY((x - cx)*std::sin(A) + (y - cy)*std::cos(A) + cy);
        }
        pt.SetZ(p.Z());
    }
    static ofts::Point PointRotate2D(const ofts::Point& p,const ofts::Point& cp,
                                     double A, bool isCW)
    {
        ofts::Point op;PointRotate2D(p,cp,A,isCW,op);
        return op;
    }
    static void DeduplicatePts(std::vector<ofts::Point>& pts,double err){
        std::vector<size_t>ids;
        for (size_t i = 0; i < pts.size(); ++i) {
            for (size_t j = i+1; j < pts.size(); ++j) {
                if(pts[i].IsSamePoint2D(pts[j],err)){
                    ids.emplace_back(i);break;
                }
            }
        }
        if(ids.empty()){return;}
        std::vector<ofts::Point>ps;
        ps.reserve(pts.size() - ids.size());
        for (size_t i = 0; i < pts.size(); ++i) {
            bool is = false;
            for(auto& d : ids){
                if(i == d){is = true;break;}
            }
            if(!is){ps.emplace_back(pts[i]);}
        }
        pts = ps;
    }
    static ofts::Point ArcMidpoint(const ofts::DefArc& arc)
    {
        ofts::Point mp;
        double sAng,eAng,agl;
        ArcEndpointAngle(arc,sAng,eAng);
        agl = pnum::TwoAglIncludedAgl(sAng,eAng,arc.IsCW());
        double R = arc.R();
        if((R < PreErr5_3)){R = arc.GetCalR();}
        double chAng = agl / 2;
        double mAgl = pnum::ArcShiftAngle(sAng,eAng,chAng,arc.IsCW());
        double x = R * std::cos(mAgl * _RadAlg);
        double y = R * std::sin(mAgl * _RadAlg);
        mp.SetX(arc._arcCp.X() + x);
        mp.SetY(arc._arcCp.Y() + y);
        mp.SetZ(arc.GetCp().Z());
        return mp;
    }
    static ofts::Point CurveMidpoint(const ofts::DefElem& c){
        switch (c._type) {
        case Ofte::LINETYPE:
            return c._segment.GetSp().Midpoint(c._segment.GetEp());
            break;
        case Ofte::ARCTYPE:
            return ArcMidpoint(c.GetArc());
            break;
        case Ofte::CIRCLETYPE:
            return c.GetCircle().GetCp();
            break;
        default:break;}
        return ofts::Point();
    }
    /// 根据弓高误差计算圆离散成线段的对应圆角度
    static double AngleBaseBowH(double R, double h)
    {
        if (h >= R - PreErr5_6){return 360;}
        return std::acos(1 - (h / R)) * _AlgRad;
    }
    static double ArcBowHeight(const ofts::DefArc& arc)
    {
        auto mp = arc.GetSp().Midpoint(arc.GetEp());
        double dis = arc.GetCp().Distance(mp,arc.Is3D());
        double agl = ArcCenterAngle(arc);
        double R = arc.GetOptR();
        double h = agl < 180 ? R - dis : R + dis;
        return h;
    }
    static void SegMidnormalParam(const ofts::Point& sp,const ofts::Point& ep,
                                  double& A,double& B,double& C)
    {
        double x = (sp.X() + ep.X());
        double y = (sp.Y() + ep.Y());
        A = ep.X() - sp.X();
        B = ep.Y() - sp.Y();
        C = -(A * x + B * y) / 2.0;
    }
    static bool IsTriaRotDireCW(const ofts::Point& p0,const ofts::Point& p1,
                                const ofts::Point& p2)
    {
        double cross = (p1.X() - p0.X()) * (p2.Y() - p1.Y()) -
                (p1.Y() - p0.Y()) * (p2.X() - p1.X());
        return cross < 0;
    }
    static double ArcPointTangAgl(const ofts::DefArc& arc,const ofts::Point& p){
        auto op = PointRotate2D(p,arc.GetCp(),90,arc.IsCW());
        double agl = SegmentAngle(arc.GetCp(),op);
        return agl;
    }
    static bool IsThreePointLinear(const ofts::Point& p0,const ofts::Point& p1,
                                   const ofts::Point& p2,double err)
    {
        double cross = (p1.X() - p0.X()) * (p2.Y() - p0.Y()) -
                (p1.Y() - p0.Y()) * (p2.X() - p0.X());
        return std::abs(cross) < err;
    }
    static bool IsPtOnSegPosDire(const ofts::Point& p0,const ofts::Point& p1,
                                 const ofts::Point& p,double err)
    {
        auto op = PointOnLineProj(p,p0,p1);
        auto v = p1 - p0;
        auto w = op - p0;
        double v_sq = v.LengthSquare2D();
        if (v_sq < err * err) {
            return w.LengthSquare2D() < err * err;
        }
        double dot = v.Dot(w);
        return dot >= -err;  //>=0
    }
    static double ArcLenth(const ofts::DefArc& arc){
        double agl = ArcCenterAngle(arc);
        double R = arc.GetOptR(PreErr5_3);
        double C = 2 * Num_PI * R;
        double len = (agl / 360.0) * C;
        return len;
    }
    //判断两点是否在线段同一侧
    static bool TwoPtLineOneSide(const ofts::Point& sp,const ofts::Point& ep,
                                 const ofts::Point& p0,const ofts::Point& p1,double err)
    {
        //叉积:(ep - sp) × (p0 - sp)和(ep - sp) × (p1 - sp)
        double d0 = (ep.X() - sp.X()) * (p0.Y() - sp.Y()) - (ep.Y() - sp.Y()) * (p0.X() - sp.X());
        double d1 = (ep.X() - sp.X()) * (p1.Y() - sp.Y()) - (ep.Y() - sp.Y()) * (p1.X() - sp.X());
        if (std::fabs(d0) < err) {return true;}  //在直线上
        if (std::fabs(d1) < err) {return true;}  //在直线上
        return (d0 * d1) > 0;
    }
    static ofts::Point MovePoint(const ofts::Point& p,const ofts::Point& dir,double d)
    {
        auto dir_unit = dir.Normalized();
        auto v = dir_unit * d;
        auto res = p + v;
        return res;
    }
    static bool IsEqualDire(const ofts::Point& v1,const ofts::Point& v2,double e)
    {
        ofts::Point p(0,0,0);
        auto p1 = MovePoint(p,v1,1);
        auto p2 = MovePoint(p,v2,1);
        return p1.IsEqual(p2,e);
    }
    bool static PtLocatedSegLeft(const ofts::Point& sp,const ofts::Point& ep,
                                 const ofts::Point& p,double e = 0)
    {
        double c = (ep.X()-sp.X())*(p.Y()-sp.Y()) - (ep.Y()-sp.Y())*(p.X()-sp.X());
        return c > e;  //等于e 则共线
    }
};
#endif // BASEGEOM2D_H
