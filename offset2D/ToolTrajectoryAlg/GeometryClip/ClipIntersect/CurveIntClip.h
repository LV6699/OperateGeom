#ifndef CURVEINTCLIP_H
#define CURVEINTCLIP_H
#include "../../GeometryCalculate/GeomCalculate/CurveLog2D.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class CurveIntClip
{
public:
    CurveIntClip(){}

    static void PtClipCurve(const ofts::Point& p,const ofts::DefElem& c,
                            ofts::DefElem& c1,ofts::DefElem& c2)
    {
        auto t1 = c,t2 = c;
        t1.ChangeEndpt(p,true);t2.ChangeEndpt(p,false);
        c1 = t1;c2 = t2;
    }
    //c1 和c2 与c 起点距离由近到远排列
    static bool PointClipCurve(const ofts::Point& p,const ofts::DefElem& c,
                               double e,ofts::DefElem& c1,ofts::DefElem& c2)
    {
        if(c.IsEndpoint(p,e)){return false;}
        auto t1 = c,t2 = c;
        t1.ChangeEndpt(p,true);t2.ChangeEndpt(p,false);
        c1 = t1;c2 = t2;
        return true;
    }
    //c1 与c2 的相交点拆分c1.
    static void CurveClipCurve(const ofts::DefElem& c1,
                               const ofts::DefElem& c2,
                               const numst::IntParam& param,
                               std::vector<ofts::DefElem>& cs)
    {
        ofts::GeomIntRes iRes;
        CurveInt2D::GetCurveIntersect(c1,c2,param,iRes);
        if(!iRes.IsInt()){return;}
        ofts::DefElem cc1,cc2;
        cs.reserve(iRes.Num() + 1);

        if(iRes.Num() == 1 && PointClipCurve(iRes.P1(),c1,param._endptPre,cc1,cc2)){
            cs.emplace_back(cc1);cs.emplace_back(cc2);
            return;
        }
        auto ip1 = iRes.P1();
        auto ip2 = iRes.P2();
        bool isep1 = c1.IsEndpoint(ip1,param._endptPre);
        bool isep2 = c1.IsEndpoint(ip2,param._endptPre);

        if(isep1 && !isep2){
            PtClipCurve(ip2,c1,cc1,cc2);
            cs.emplace_back(cc1);cs.emplace_back(cc2);
            return;
        }else if(isep2 && !isep1){
            PtClipCurve(ip1,c1,cc1,cc2);
            cs.emplace_back(cc1);cs.emplace_back(cc2);
            return;
        }else if(isep1 && isep2){return;}

        double d1 = CurveLog2D::ToCurvesEndDist(c1,ip1,true,true);
        double d2 = CurveLog2D::ToCurvesEndDist(c1,ip2,true,true);
        if(d1 > d2){std::swap(ip1,ip2);}
        PtClipCurve(ip1,c1,cc1,cc2);
        ofts::DefElem cc3,cc4;
        PtClipCurve(ip2,cc2,cc3,cc4);
        cs.emplace_back(cc1);
        cs.emplace_back(cc3);
        cs.emplace_back(cc4);
    }
    ///c1 与c2 的相交点拆分c1.
    static std::vector<ofts::Point> CurveClipCurvePoint(const ofts::DefElem& c1,
                                                        const ofts::DefElem& c2,
                                                        const numst::IntParam& param)
    {
#if 0
        ofts::Point p1(-104,4.000000000000001);
        ofts::Point p2(-98,8);
        if(c1.GetSp().IsSamePoint2D(p1,PreErr5_4) &&
                c2.GetSp().IsSamePoint2D(p2,PreErr5_4)){
            std::cout<<1<<std::endl;
        }
#endif
        ofts::GeomIntRes iRes;
        std::vector<ofts::Point>ps;
        CurveInt2D::GetCurveIntersect(c1,c2,param,iRes);
        if(!iRes.IsInt()){return ps;}
        if(!c1.IsEndpoint(iRes.P1(),param._endptPre)){
            ps.emplace_back(iRes.P1());
        }
        if(iRes.Num() == 2 && !c1.IsEndpoint(iRes.P2(),param._endptPre)){
            ps.emplace_back(iRes.P2());
        }
        return ps;
    }
    ///loop 与curve 的相交点拆分curve.
    static void LoopClipCurve(const ofts::DefElem& curve,
                              const ofts::DefLoop& loop,
                              const numst::IntParam& param,
                              std::vector<ofts::DefElem>& curves)
    {
        std::vector<ofts::Point>ps;ps.reserve(5);
        for (size_t i = 0; i < loop.Size(); ++i) {
            auto ps1 = CurveClipCurvePoint(curve,loop[i],param);
            for(auto& p : ps1){ps.emplace_back(p);}
        }
        if(ps.empty()){return;}
        BaseGeom2D::DeduplicatePts(ps,param._endptPre);
        CurveLog2D::PointsClipCurve(curve,ps,curves);
#if 0
        DisplayGeom().DisplayLoop(loop,_colors.back(),1,true,true);
        DisplayGeom().DisplayElement(curve,_colors.back(),1);
        _mainwind->myOccView->fitAll();
        for(auto& p : ps){
            DisplayGeom().DisplayPoint(p,_colors[4]);
        }
        for (size_t i = 0; i < curves.size(); ++i) {
            DisplayGeom().DisplayElement(curves[i],_colors[i],2);
        }
#endif
    }
    ///loops 与curve 的相交点拆分curve.
    static void LoopsClipCurve(const ofts::DefElem& curve,
                               const std::vector<ofts::DefLoop>& loops,
                               const numst::IntParam& param,
                               std::vector<ofts::DefElem>& curves)
    {
        std::vector<ofts::Point>ps;ps.reserve(5);
        for (size_t l = 0; l < loops.size(); ++l) {
            const auto& loop = loops[l];
            for (size_t i = 0; i < loop.Size(); ++i) {
                auto ps1 = CurveClipCurvePoint(curve,loop[i],param);
                for(auto& p : ps1){ps.emplace_back(p);}
            }
        }
        if(ps.empty()){return;}
        BaseGeom2D::DeduplicatePts(ps,param._endptPre);
        CurveLog2D::PointsClipCurve(curve,ps,curves);
    }
    ///loop1 与loop2 的相交点拆分loop1.
    static void LoopClipLoop(const ofts::DefLoop& loop1,
                             const ofts::DefLoop& loop2,
                             const numst::IntParam& param,
                             ofts::DefLoop& rloop)
    {
        std::vector<ofts::DefElem> curves;
        for (size_t i = 0; i < loop1.Size(); ++i) {/**
            if(i == 235){
                std::cout<<i<<endl;
            }*/
            LoopClipCurve(loop1[i],loop2,param,curves);
            if(curves.empty()){
                rloop.AddElem(loop1[i]);
                continue;
            }
            for(auto& c : curves){
                rloop.AddElem(c);
            }
            curves.clear();
        }
    }
    ///loop 与loops 的相交点拆分loop.
    static void LoopsClipLoop(const ofts::DefLoop& loop,
                              const std::vector<ofts::DefLoop>& loops,
                              const numst::IntParam& param,
                              ofts::DefLoop& rloop)
    {
        std::vector<ofts::DefElem> curves;
        for (size_t i = 0; i < loop.Size(); ++i) {
            LoopsClipCurve(loop[i],loops,param,curves);
            if(curves.empty()){
                rloop.AddElem(loop[i]);
                continue;
            }
            for(auto& c : curves){
                rloop.AddElem(c);
            }
            curves.clear();
        }
    }
    ///输出rloops 中索引对应环与loops 中一一对应.
    static void LoopsMutualClip(const std::vector<ofts::DefLoop>& loops,
                                const numst::IntParam& param,
                                std::vector<ofts::DefLoop>& rloops)
    {
        if(loops.empty()){return;}
        else if(loops.size() < 2){rloops = loops;return;}
        rloops.reserve(loops.size());
        std::vector<ofts::DefLoop>ls;
        for (size_t i = 0; i < loops.size(); ++i) {
            for (size_t j = 0; j < loops.size(); ++j) {
                if(i != j){ls.emplace_back(loops[j]);}
            }
            ofts::DefLoop rloop;
            LoopsClipLoop(loops[i],ls,param,rloop);
            rloops.push_back(rloop);
            ls.clear();
        }
#if 0
        for (size_t i = 0; i < rloops.size(); ++i) {
            DisplayGeom().DisplayLoop(rloops[i],_colors[i],1);
        }
#endif
    }
};
#endif // CURVEINTCLIP_H
