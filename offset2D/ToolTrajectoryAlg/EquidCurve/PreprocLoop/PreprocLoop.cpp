#include "PreprocLoop.h"
#include "../../GeometryCalculate/GeomFitting2D/CreateFitting.h"
#include "../../GeometryCalculate/GeomCalculate/CurveLog2D.h"
#include "../../OperCurve2D/OperCurve2D/OperateCurve.h"
#if oft_nopti_code
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
// 处理一般流程:
// 需要先初始化圆弧半径
// 处理环中存在完全或部分重合的线段.
// 处理相邻元素连接点、线段水平或垂直特性、重置圆弧结束点等.
void PreprocLoop::SetLineHorVerQual(ofts::Point& sp, ofts::Point& ep,bool isChEp, double pre)
{
    if(CurveQual2D::IsVerticalLine(sp,ep,pre)){
        if(isChEp){ep.SetX(sp.X());}
        else{sp.SetX(ep.X());}
    }
    if(CurveQual2D::IsHorizontalLine(sp,ep,pre)){
        if(isChEp){ep.SetY(sp.Y());}
        else{sp.SetY(ep.Y());}
    }
}
void PreprocLoop::ResetArcCenter(ofts::DefArc& arc)
{
    arc.OptInitR(PreErr5_3);
    double R = arc.R();
    const auto& sp = arc.GetSp();
    const auto& ep = arc.GetEp();
    const auto& cp_ = arc.GetCp();
    double A1,B1,C1,A2,B2,C2;
    BaseGeom2D::SegMidnormalParam(sp,ep,A1,B1,C1);
    if(sp.DistSquare2D(cp_) > ep.DistSquare2D(cp_)){
        BaseGeom2D::GetLineParam(sp,cp_,PreErr_10,PreErr_10,A2,B2,C2);
    }else{
        BaseGeom2D::GetLineParam(ep,cp_,PreErr_10,PreErr_10,A2,B2,C2);
    }
    numst::IntParam ipa;ofts::GeomIntRes iRes;
    ipa.SetIntErr(PreErr_12);ipa.SetEndptRele(true,PreErr_12);
    ipa.SetOnCurveErr(PreErr_12); ipa.SetTanErr(PreErr_12);
    CurveInt2D::GetLineParamInt(A1,B1,C1,A2,B2,C2,ipa,iRes);
    if(iRes.IsInt()){
        arc.SetCp(iRes.P1());arc.InitR();
        return;
    }
    ipa.SetIntErr(PreErr_14);
    ipa.SetTanErr(PreErr_14);
    ipa.SetCalcTan(true);
    CurveInt2D::GetCircleIntersect(sp,R,ep,R,ipa,iRes);
    if(!iRes.IsInt()){
        double d = sp.Distance(ep);
        d /= 2;
        CurveInt2D::GetCircleIntersect(sp,d,ep,d,ipa,iRes);
    }
    if(!iRes.IsInt()){
        arc.InitR();return;
    }
    if(iRes.Num() == 1){
        arc.SetCp(iRes.P1());
        arc.InitR();return;
    }
    double d1= iRes.P1().DistSquare2D(cp_);
    double d2 = iRes.P2().DistSquare2D(cp_);
    if(d1 < d2){
        arc.SetCp(iRes.P1());
    }else{
        arc.SetCp(iRes.P2());
    }
    arc.InitR();
}
void PreprocLoop::ResetArcEndpoint(bool isLast,ofts::DefArc& arc)
{
    if(isLast){
        ResetArcCenter(arc);return;
    }
    const auto& sp = arc.GetSp();
    const auto& ep = arc.GetEp();
    const auto& cp = arc.GetCp();
    double R1 = sp.Distance2D(cp),R2 = ep.Distance2D(cp);
    if(std::abs(R1 - R2) < PreErr_16){
        arc.SetR((R1 + R2) / 2);return;
    }
    double endAgl = std::atan2((ep.Y() - cp.Y()),(ep.X() - cp.X()));
    double ix = R1 * std::cos(endAgl);
    double iy = R1 * std::sin(endAgl);
    ofts::Point aep(cp.X() + ix,cp.Y() + iy);
    arc.SetEp(aep);
    R2 = aep.Distance2D(cp);
    arc.SetR((R1 + R2) / 2);
    //auto d = R1 - R2;
}
void PreprocLoop::OptInputPreprocess(const ofts::OffsetParam& param,
                                     ofts::DefLoop& loop,
                                     std::vector<ofts::DefLoop>&loops)
{
#if 1
    if(!param._prepOpti.IsAbsPrep()) {
        if(!param._prepOpti.IsAblePrep()) {return;}
        if(!param._prepOpti.IsAbsPrep() && param._offsetUnit.IsFixedOffset() &&
                (param._prepOpti.OftValue() <= PreprocValLim)) {
            return;
        }
    }
#endif
    InputPreprocess(param,loop,loops);
}
void PreprocLoop::OptPreprocessLoop(const ofts::OffsetParam& param,
                                    ofts::DefLoop& loop)
{
#if 1
    if(!param._prepOpti.IsAbsPrep()) {
        if(!param._prepOpti.IsAblePrep()) {return;}
        if(!param._prepOpti.IsAbsPrep() && param._offsetUnit.IsFixedOffset() &&
                (param._prepOpti.OftValue() <= PreprocValLim)) {
            return;
        }
    }
#endif
    PreprocessLoop(param,loop);
}
void PreprocLoop::InputPreprocess(const ofts::OffsetParam& param,
                                  ofts::DefLoop& loop,
                                  std::vector<ofts::DefLoop>& loops)
{
    PreprocessLoop(param,loop);
    for(auto& d : loops){PreprocessLoop(param,d);}
}
void PreprocLoop::PreprocessLoop(const ofts::OffsetParam& param,ofts::DefLoop& loop)
{
    if(loop.IsEmpty()){return;}
    loop.OptInitArcR(PreErr5_3);
    if(param._prepOpti.IsDelInval()){
        OperateCurve().DelInvalidGeomCurves(loop.m_vElem,PreErr5_6,
                                            PreErr5_6,false,PreErr5_6);
    }
    if(param._prepOpti.IsOperSupe()){
        CreateFitting().FittingLoopArcSeg(param._fitParam,loop);
        //DisplayGeom().DisplayLoop(loop,_colors.back(),1,true,true);
    }
    if(param._prepOpti.IsEndtoend()){
        if(loop._label._isPreReverse){
            loop.Reverse();
            OperateLoop().LoopCurveEndToEnd(loop);
            loop.Reverse();
        }else{
            OperateLoop().LoopCurveEndToEnd(loop);
        }
    }
    //DisplayGeom().DisplayLoop(loop,_colors[1],1.5,true);
    if(param._prepOpti.IsClose() && !loop.IsOpen()){
        OperateLoop().SetLoopEndToEnd(loop,PreErr_12);
    }
    //DisplayGeom().DisplayLoop(loop,_colors[2],3,true);
}
void PreprocLoop::DeleteInvalidCurve(double segPre,double arcPre,
                                     std::vector<ofts::DefElem>& cs)
{
    std::vector<size_t>ids;
    for (size_t i = 0; i < cs.size(); ++i) {
        bool isVal = false;
        if(cs[i].Type() == ARCTYPE){
            cs[i]._arc.OptInitR(PreErr5_3);
        }
        if(!CurveQual2D::IsValidGeomCurve(cs[i],segPre,arcPre,true,arcPre)){
            ids.emplace_back(i);
        }
    }
    cs.erase(std::remove_if(cs.begin(),cs.end(),[](const ofts::DefElem& d){
        return !d.IsValid();}),cs.end());
}
void PreprocLoop::DeleteInvalidCurve(double segPre,double arcPre,
                                     ofts::DefLoop& loop)
{
    DeleteInvalidCurve(segPre,arcPre,loop.m_vElem);
}
