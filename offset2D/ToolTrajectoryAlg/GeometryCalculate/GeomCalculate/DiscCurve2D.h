#ifndef DISCCURVE2D_H
#define DISCCURVE2D_H
#include "BaseGeom2D.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class DiscCurve2D
{
public:
    DiscCurve2D(){}
    static std::vector<ofts::Point> SegToPoints(const numst::DiscParam& param,
                                                const ofts::DefSeg& s)
    {
        const auto& sp = s.GetSp();
        const auto& ep = s.GetEp();
        std::vector<ofts::Point>ps;
        auto dir = ep - sp;
        double length = dir.Length2D();
        auto unit_dir = dir * (1.0 / length);

        const auto& d = param.SegStep();
        int num = static_cast<int>(std::floor(length / d));
        ps.reserve(num + 2);

        ps.emplace_back(sp);
        for(int i = 1; i <= num; ++i) {
            double distance = i * d;
            ps.push_back(sp + unit_dir * distance);
        }
        double remaining = length - num * d;
        if(remaining > PreErr5_12) {
            ps.push_back(ep);
        }
        return ps;
    }
    static std::vector<ofts::Point> ArcToPoints(const numst::DiscParam& param,
                                                const ofts::DefArc& arc)
    {
        const auto& step = param.CirStep();
        std::vector<ofts::Point>ps;
        double cAgl = BaseGeom2D::ArcCenterAngle(arc);
        ps.reserve(static_cast<int>(std::floor(cAgl / step) + 2));

        ps.emplace_back(arc._arcSp);
        if(cAgl <= step || cAgl >= 360) {
            if(!arc._arcSp.IsSamePoint2D(arc._arcEp,PreErr5_12)) {
                ps.emplace_back(arc._arcEp);
            }
            return ps;
        }
        if(cAgl <= step * 2) {
            auto mp = BaseGeom2D::ArcMidpoint(arc);
            ps.emplace_back(mp);
            ps.emplace_back(arc._arcEp);
            return ps;
        }
        if(step < PreErr_4){return ps;}
        ofts::Point pt;
        double curAgl = step;
        while(curAgl < cAgl) {
            BaseGeom2D::PointRotate2D(arc._arcSp,arc._arcCp,
                                      curAgl,arc.IsCW(),pt);
            ps.emplace_back(pt);
            curAgl += step;
        }
        if(!ps.back().IsSamePoint2D(arc._arcEp,PreErr5_12)) {
            ps.emplace_back(arc.GetEp());
        }
        return ps;
    }
    static std::vector<ofts::Point> CircleToPoints(const numst::DiscParam& param,
                                                   const ofts::DefCircle& cir)
    {
        std::vector<ofts::Point>ps;
        double R = cir.R();
        const auto& cp = cir.GetCp();
        if((R < PreErr_6) || (!cp.IsLegal())){return ps;}

        ofts::Point sp(cp.X() + R, cp.Y()), ep(cp.X() - R, cp.Y());
        ofts::DefArc arc1(sp, ep, cp, R, cir.IsCW());
        ofts::DefArc arc2(ep, sp, cp, R, cir.IsCW());

        ps = ArcToPoints(param,arc1);
        auto ps1 = ArcToPoints(param,arc2);
        if(ps1.size() > 1) {
            ps1.erase(ps1.begin());
            ps1.pop_back();
        }
        for(auto& p: ps1) {
            ps.emplace_back(p);
        }
        return ps;
    }
    static std::vector<ofts::Point> CurveToPoints(const numst::DiscParam& param,
                                                  const ofts::DefElem& curve)
    {
        switch(curve.Type()) {
        case Ofte::LINETYPE: {
            return SegToPoints(param,curve.GetSeg());
        }
        case Ofte::ARCTYPE: {
            return ArcToPoints(param,curve.GetArc());
        }
        case Ofte::CIRCLETYPE: {
            return CircleToPoints(param,curve.GetCircle());
        }
        default:
            break;
        }
        return std::vector<ofts::Point>();
    }
    static std::vector<ofts::Point> CurveOptToPoints(const numst::DiscParam& param,
                                                     const ofts::DefElem& curve)
    {
        std::vector<ofts::Point>ps;
        switch(curve.Type()) {
        case Ofte::LINETYPE: {
            if(param.IsDiscSeg()){
                return SegToPoints(param,curve.GetSeg());
            }
            ps.emplace_back(curve.GetSeg().GetSp());break;
        }
        case Ofte::ARCTYPE: {
            if(param.IsDiscArc()){
                return ArcToPoints(param,curve.GetArc());
            }
            ps.emplace_back(curve.GetArc().GetSp());break;
        }
        case Ofte::CIRCLETYPE: {
            if(param.IsDiscCir()){
                return CircleToPoints(param,curve.GetCircle());
            }
            break;
        }
        default:
            break;
        }
        return ps;
    }
    static std::vector<ofts::Point> LooopToPoints(const numst::DiscParam& param,
                                                  const ofts::DefLoop& loop)
    {
        std::vector<ofts::Point>ps;
        ps.reserve(loop.Size()*200);
        for(size_t i = 0;i < loop.Size();i++){
            auto ps1 = CurveToPoints(param,loop[i]);
            if(!ps.empty() && !ps1.empty() &&
                    ps.back().IsEqual(ps1.front(),PreErr_14)){
                ps.pop_back();
            }
            for(auto& p : ps1){ps.emplace_back(p);}
        }
        if(!ps.empty() && ps.back().IsEqual(ps.front(),PreErr_14)){
            ps.pop_back();
        }
        return ps;
    }
    static std::vector<ofts::Point> LooopOptToPoints(const numst::DiscParam& param,
                                                     const ofts::DefLoop& loop)
    {
        std::vector<ofts::Point>ps;
        if(loop.IsEmpty()){return ps;}
        ps.reserve(loop.Size()*200);
        for(size_t i = 0;i < loop.Size();i++){
            auto ps1 = CurveOptToPoints(param,loop[i]);
            if(!ps.empty() && !ps1.empty() &&
                    ps.back().IsEqual(ps1.front(),PreErr_14)){
                ps.pop_back();
            }
            for(auto& p : ps1){ps.emplace_back(p);}
        }
        if(!ps.back().IsEqual(loop.Endpoint(true),PreErr_14) && !loop.IsCircleLoop()){
            ps.emplace_back(loop.Endpoint(true));
        }
        if(ps.back().IsEqual(ps.front(),PreErr_14)){ps.pop_back();}
        return ps;
    }
};
#endif // DISCCURVE2D_H
