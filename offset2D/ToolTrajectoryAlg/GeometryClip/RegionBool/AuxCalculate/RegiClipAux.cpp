#include "RegiClipAux.h"
#include "../AuxCalculate/HandleSelInt.h"
#include "../../../GeometryCalculate/GeomCalculate/BaseGeom2D.h"
#include "../../../EquidCurve/EquidCurve/EquidCurve/EquidCurve2D.h"
#include "../../../GeometryCalculate/GeomQuality/CurveQual2D.h"
#include "../../../GeometryCalculate/GeomIntersect/CurveInt2D.h"
#include "../../../OperCurve2D/OperCurve2D/OperateCurve.h"
#include "../../../GeometryCalculate/RelativeContain/PointInLoop.h"
#include "../../../GeometryCalculate/GeomCalculate/LoopDirection.h"
#include "../../../GeometryCalculate/RelativeContain/LimitCoord.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
bool RegiClipAux::IsValidWindNum(const clst::FillRule& rule,int wn)
{
    switch (rule){
    case clst::FillRule::EvenOdd : return wn % 2 != 0;
    case clst::FillRule::NonZero : return wn != 0;
    case clst::FillRule::Positive : return wn > 0;
    case clst::FillRule::Negative : return wn < 0;
    default:break;
    }
    return false;
}
void RegiClipAux::OperateInput(const clst::FillRule& rule,
                               const std::vector<ofts::DefLoop>& sloops,
                               const std::vector<ofts::DefLoop>& cloops,
                               std::vector<ofts::DefLoop>& scl_ls,
                               std::vector<ofts::DefLoop>& ccl_ls)
{
    HandleSelInt hst;RegiClipAux rcaux;
    auto GetWindNum = [&](const std::vector<ofts::DefLoop>& loops,
            const ofts::Point& p){
        int wn = 0;
        for(size_t i = 0;i < loops.size();++i){
            int wn1 = rcaux.PointWindNum(loops[i],p,PreErr5_10);
            wn += wn1;
        }
        return wn;
    };
    auto ToLoops = [&](const std::vector<ofts::DefLoop>& loops,
            std::vector<ofts::DefLoop>& rloops){
        for(size_t i = 0;i < loops.size();++i){
            const auto& l = loops[i];
            auto cl_ls = hst.HandleSelIntLoop(l);
            for (size_t j = 0; j < cl_ls.size(); j++){
                auto& cl_l = cl_ls[j];
                bool iscw = LoopDirection().IsClockwiseLoop(cl_l);
                cl_l.SetCW(iscw);
                auto op = LimitCoord2D::LoopLimitPoint(cl_l,true,true);
                int wn = GetWindNum(loops,op);
                wn = iscw ? wn -- : wn ++;
                cl_l.SetWindNum(wn);
            }
            for(auto& d : cl_ls){
                rloops.emplace_back(d);
            }
        }
    };
    ToLoops(sloops,scl_ls);
    ToLoops(cloops,ccl_ls);
    auto SetValid = [&](std::vector<ofts::DefLoop>& loops){
        for (size_t i = 0; i < loops.size(); i++){
            auto& loop = loops[i];
            bool isVal = IsValidWindNum(rule,loop.WindNum());
            loop.SetValid(isVal);
        }
    };
    SetValid(scl_ls);
    SetValid(ccl_ls);
    scl_ls.erase(std::remove_if(scl_ls.begin(),scl_ls.end(),[](const ofts::DefLoop& l){
        return !l.IsValid();}),scl_ls.end());
    ccl_ls.erase(std::remove_if(scl_ls.begin(),scl_ls.end(),[](const ofts::DefLoop& l){
        return !l.IsValid();}),scl_ls.end());
}
ofts::Point RegiClipAux::LoopInternalPt(const ofts::DefLoop& loop)
{
    if(loop.Size() < 2){
        if(loop.IsCircleLoop()){return loop[0].GetCircle().GetCp();}
        return ofts::Point();
    }
    ofts::Point op;
    double d = PreErr_3;
    auto c0 = loop[0];
#if 1
    const auto& c1 = loop[1];
    auto p0 = CurveLog2D::NearCurveEndpt(c0,false,false,d,false);
    auto p1 = CurveLog2D::NearCurveEndpt(c1,true,false,d,false);
    op = BaseGeom2D::ArcMidpoint(ofts::DefArc(p0,p1,c0.GetEp(),!loop.IsCW()));
#else
    bool leftward = loop.IsCW();
    if(!loop.IsCW()){c0.Reverse();}
    auto ec = EquidCurve2D::EquidistantCurve(c0,d,true);
    op = BaseGeom2D::CurveMidpoint(ec);
#endif
    return op;
}
int RegiClipAux::PointWindNum(const ofts::DefLoop& loop,const ofts::Point& P,double bPre)
{
    numst::IntParam ipa;ofts::GeomIntRes iRes;
    ipa.SetEndptRele(true,PreErr5_10);ipa.SetIntErr(PreErr_10);
    ipa.SetOnCurveErr(PreErr_10);ipa.SetTanErr(PreErr_10);
    ipa.SetParalErr(PreErr_10);ipa.SetParalAxisErr(PreErr_10);
    int windNum = 0;
    double step = PreErr_8;
    ofts::DefSeg seg(P,ofts::Point(20000,P.Y()));

    auto GetWindNum = [&](size_t i){
        if(iRes.Num() > 1){return;}
        const auto& p = iRes.P1();
        const auto& c = loop[i];
        ofts::Point op1,op2;
        if(p.IsEqual(c.GetSp(),ipa.EndptPre())){
            op1 = OperateLoop().LoopLenthPoint(loop,loop.PreId(i),false,step);
            op2 = OperateLoop().LoopLenthPoint(loop,i,true,step);
        }else if(p.IsEqual(c.GetEp(),ipa.EndptPre())){
            op1 = OperateLoop().LoopLenthPoint(loop,i,false,step);
            op2 = OperateLoop().LoopLenthPoint(loop,loop.NexId(i),true,step);
        }
        if(BaseGeom2D::TwoPtLineOneSide(seg.GetSp(),seg.GetEp(),
                                        op1,op2,PreErr_12)){
            return;
        }
        bool iscw = BaseGeom2D::IsTriaRotDireCW(P,op1,op2);
        windNum = iscw ? windNum ++ : windNum --;
    };
    for (size_t i = 0; i < loop.Size(); ++i){/**
        if(i == 7){
            int tem = 0;
        }*/
        ofts::GeomIntRes iRes;
        const auto& curve = loop[i];
        switch (curve._type) {
        case Ofte::LINETYPE:{
            const auto& sp = curve._segment.GetSp();
            const auto& ep = curve._segment.GetEp();
            double x = sp.X() > ep.X() ? sp.X() : ep.X();
            if(x + bPre < P.X()){continue;}
            double y1 = sp.Y() < ep.Y() ? sp.Y() : ep.Y();
            double y2 = sp.Y() > ep.Y() ? sp.Y() : ep.Y();
            if(y1 - bPre > P.Y() || y2 + bPre < P.Y()){
                continue;
            }
            if(CurveQual2D::IsHorizontalLine(sp,ep,PreErr_8)){continue;}
            CurveInt2D::GetSegmentIntersect(seg,curve.GetSeg(),ipa,iRes);
            if(!iRes.IsInt()){continue;}
            break;
        }
        case Ofte::ARCTYPE:{
            const auto& arc = curve.GetArc();
            CurveInt2D::GetSegmentArcInt(seg,arc,ipa,iRes);
            if(!iRes.IsInt()){continue;}
            break;
        }
        case Ofte::CIRCLETYPE:{return 0;}
        default: break;
        }
    }
    return windNum;
}
