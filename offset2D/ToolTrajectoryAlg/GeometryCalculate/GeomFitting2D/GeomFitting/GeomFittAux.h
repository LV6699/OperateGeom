#ifndef GEOMFITTAUX_H
#define GEOMFITTAUX_H
#include "../../../CommonFile/Header.h"
#include "../../../GeometryCalculate/GeomIntersect/CurveInt2D.h"
#include"../../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class GeomFittAux
{
public:
    GeomFittAux(){}
    bool IsMeetAdjoinAngle(const numst::FitParam& param,
                           const ofts::DefLoop& loop,size_t i)
    {
        const auto& p0 = loop.PreElem(i).GetSeg().GetSp();
        const auto& p1 = loop.PreElem(i).GetSeg().GetEp();
        const auto& p2 = loop[i].GetSeg().GetEp();
        double agl1 = BaseGeom2D::SegmentAngle(p0,p1);
        double agl2 = BaseGeom2D::SegmentAngle(p0,p2);
        bool isCW = BaseGeom2D::IsTriaRotDireCW(p0,p1,p2);
        double agl = pnum::TwoAglIncludedAgl(agl1,agl2,isCW);
        if(agl > param.AglVar()){
            if(std::abs(360.0 - agl) > param.AglVar()){
                return false;
            }
        }
        return true;
    }
    bool IsCurveMeet(const numst::FitParam& param,const ofts::DefElem& c){
        if(c._type != Ofte::LINETYPE){return false;}
        double l = c.GetSeg().LenSquare();
        if(l > param.MaxLenSqur()){return false;}
        return true;
    }
    void ResetFittData(gfit::FittArc& fit,std::vector<gfit::FittArc>& fitts)
    {
        if(fit.Ids().size() > 1){
            fitts.push_back(fit);
        }
        fit = gfit::FittArc();
    }
    double SegArcIncludeAgl(const Point& sp,const Point& ep,
                            const DefArc& arc,const Point& p){
        double sAgl = BaseGeom2D::SegmentAngle(sp,ep);
        double aAgl = BaseGeom2D::ArcPointTangAgl(arc,p);
        double agl = pnum::TwoAglIncludedAgl(aAgl,sAgl,arc.IsCW());
        return agl;
    }
    bool IsMeetParam(const ofts::DefLoop& loop,
                     const numst::FitParam& param,gfit::FittArc& fit)
    {
        if(fit.Ids().size() < 2){return false;}  //理论上不应该出现
        fit.SetPreArc(fit.FitArc());
        const auto& ids = fit.Ids();
        if(fit.Nodes().empty()){  //Ids().size() = 2,初始化节点
            fit.AddNode(loop[ids[0]].GetSeg().GetSp());
            fit.AddNode(loop[ids[1]].GetSeg().GetSp());
        }
        fit.AddNode(loop[ids.back()].GetSeg().GetEp());
        const auto& nps = fit.Nodes();
        size_t m = static_cast<size_t>(nps.size() / 2);
        const auto& sp = nps[0];
        const auto& mp = nps[m];
        const auto& ep = nps.back();

        ofts::Point op;
        ofts::GeomIntRes iRes;
        numst::IntParam ipara;
        double A1,B1,C1,A2,B2,C2;
        BaseGeom2D::SegMidnormalParam(sp,mp,A1,B1,C1);
        BaseGeom2D::SegMidnormalParam(mp,ep,A2,B2,C2);
        CurveInt2D::GetLineParamInt(A1,B1,C1,A2,B2,C2,ipara,iRes,true);
        //DisplayGeom().DisplaySegment(sp,mp,_colors[1],2,false,false,true);
        //DisplayGeom().DisplaySegment(mp,ep,_colors[2],2);
        if(iRes.IsInt()){
            op = iRes.P1();
            //DisplayGeom().DisplayPoint(iRes.P1(),_colors[2],2,true);
            //_mainwind->myOccView->fitAll();
        }else{
            if(!fit.HasInit()){  //未初始化情况
                return false;
            }
            auto mp0 = sp.Midpoint(mp),mp1 = mp.Midpoint(ep);
            op = mp0.Midpoint(mp1);
        }
        if(!fit.HasInit()){
            bool isCW = BaseGeom2D::IsTriaRotDireCW(op,sp,ep);
            fit.SetCW(isCW);
        }else{
            if(!op.IsEqual(fit.FitArc().GetCp(),param.CpVar())){
                return false;
            }
            const auto& seg = loop[ids.back()].GetSeg();
            const auto& sp1 = seg.GetSp();
            const auto& ep1 = seg.GetEp();
            bool isCW = BaseGeom2D::IsTriaRotDireCW(op,sp1,ep1);
            if(isCW != fit.IsCW()){
                return false;
            }
        }
        auto& arc = fit.FitArc();
        double d1 = sp.Distance2D(op),d2 = ep.Distance2D(op);
        double R = (d1 + d2) / 2;
        arc.SetSp(sp);arc.SetEp(ep);arc.SetCp(op);arc.SetR(R);
        if(R < param.MinR() || R > param.MaxR()){
            return false;
        }
        if(fit.HasInit() && std::abs(R - arc.R()) > param.RVar()){
            return false;
        }
        fit.AddCenter(op);fit.AddRs(R);
        //全局计算
        for (size_t i = 0; i < ids.size(); i++){
            const auto& id = ids[i];
            const auto& sp1 = loop[id].GetSeg().GetSp();
            const auto& ep1 = loop[id].GetSeg().GetEp();
            double agl = SegArcIncludeAgl(sp1,ep1,arc,sp1);
            if(agl > param.AglVar()){
                if(std::abs(360.0 - agl) > param.AglVar()){
                    return false;
                }
            }
        }
        for (size_t i = 0; i < nps.size() - 1; i++){
            double d = nps[i].Distance2D(op);
            if(std::abs(d - R) > param.Bh()){
                return false;
            }
            auto mp1 = nps[i].Midpoint(nps[i+1]);
            d = mp1.Distance2D(op);
            if(std::abs(d - R) > param.Bh()){
                return false;
            }
        }
        if(std::abs(op.Distance2D(nps.back()) - R) > param.Bh()){
            return false;
        }
        fit.SetHasInit(true);
        return true;
    }
};
#endif // GEOMFITTAUX_H
