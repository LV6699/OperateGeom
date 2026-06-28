#ifndef LOOPCONTAINAUX_H
#define LOOPCONTAINAUX_H
#include"PointInLoop.h"
class LoopContAux
{
public:
    LoopContAux(){}
    static numst::IntParam ContToIntParam(const numst::ContParam& cpa)
    {
        numst::IntParam ipa;
        ipa.SetIntErr(cpa._intPre);
        ipa.SetCalcTan(false);ipa.SetCalcParal(false);
        ipa.SetOnCurveErr(PreErr5_8);ipa.SetParalErr(PreErr5_8);
        ipa.SetTanErr(PreErr5_8);ipa.SetEndptRele(true,PreErr5_8);
        ipa.SetEndptRele(false,PreErr5_8);
        return ipa;
    }
    static bool IsCircleContPoint(const ofts::DefCircle& c,
                                  const ofts::Point& p,double e){
        return c.GetCp().IsSamePoint2D(p,c.R() + e);
    }
    static bool IsCircleContCircle(const ofts::DefCircle& out,
                                   const ofts::DefCircle& in,double e){
        double d = out.GetCp().DistSquare2D(in.GetCp());
        double rr = (out.R() + in.R()) * (out.R() + in.R());
        return d <= rr + e;
    }
    static bool IsCircleContSeg(const ofts::DefCircle& c,
                                const ofts::Point& sp,
                                const ofts::Point& ep,double e)
    {
        return IsCircleContPoint(c,sp,e) && IsCircleContPoint(c,ep,e);
    }
    static bool IsCircleContArc(const ofts::DefCircle& c,
                                const ofts::DefArc& arc,double e)
    {
        if(!IsCircleContPoint(c,arc.GetSp(),e) ||
                !IsCircleContPoint(c,arc.GetEp(),e)){
            return false;
        }
        numst::IntParam param;
        param._cal_CC_Tan = false;
        param._calEndpt = false;
        param._endptPre = e;
        if(IsCurveInt2D::IsArcCircleInt(arc,c.GetCp(),c.R(),param)){
            return false;
        }
        return true;
    }
    static bool IsCircleContCurve(const ofts::DefCircle& c,
                                  const ofts::DefElem& curve,double e)
    {
        switch (curve.Type()) {
        case Ofte::LINETYPE:
            return IsCircleContSeg(c,curve._segment.GetSp(),
                                   curve._segment.GetEp(),e);
        case Ofte::ARCTYPE:
            return IsCircleContArc(c,curve._arc,e);
        case Ofte::CIRCLETYPE:
            return IsCircleContCircle(c,curve._circle,e);
        default: break;
        }
        return false;
    }
    static bool IsCircleContLoop(const ofts::DefCircle& circle,
                                 const ofts::DefLoop& loop,double e)
    {
        for(const auto& c : loop){
            if(!IsCircleContCurve(circle,c,e)){
                return false;
            }
        }
        return true;
    }
    static bool IsLoopContCircle(const ofts::DefLoop& loop,
                                 const ofts::DefCircle& circle,double e)
    {
        double dist = 0;
        for(const auto& c : loop){
            dist = CurveDist2D::PointToCurveDist(circle.GetCp(),c);
            if(dist < circle.R() - e){
                return false;
            }
        }
        return true;
    }
    static bool IsLoopContCurveEndpt(const ofts::DefLoop& loop,
                                     const ofts::DefElem& curve,
                                     const numst::ContParam& cpa)
    {
        const auto& sp = curve.GetSp();
        const auto& ep = curve.GetEp();
        if(loop.IsIncPtCoord(sp,cpa._incPre)){
            bool isOnBord = false,isInPoly = false;
            PointInLoop().GetPointInsideLoop(loop,sp,cpa._onBordPre,
                                             isOnBord,isInPoly);
            if(isInPoly || (cpa._onBordPre && isOnBord)){
                return true;
            }
        }
        if(loop.IsIncPtCoord(ep,cpa._incPre)){
            bool isOnBord = false,isInPoly = false;
            PointInLoop().GetPointInsideLoop(loop,ep,cpa._onBordPre,
                                             isOnBord,isInPoly);
            if(isInPoly || (cpa._onBordPre && isOnBord)){
                return true;
            }
        }
        return false;
    }
    static bool IsCurveSpecifyInt(const ofts::DefElem& c1,
                                  const ofts::DefElem& c2,
                                  const numst::ContParam& cpa){
        auto ipa = ContToIntParam(cpa);
        bool is = IsCurveInt2D::IsCurveIntersect(c1,c2,ipa);
        return is;
    }
    static bool IsLoopCurveSpecifyInt(const ofts::DefLoop& loop,
                                      const ofts::DefElem& curve,
                                      const numst::ContParam& cpa){
        auto ipa = ContToIntParam(cpa);
        bool is = IsCurveInt2D().IsCurveLoopInt(curve,loop,ipa);
        return is;
    }
    static void LoopContCurveAux(const ofts::DefLoop& loop,
                                 const ofts::DefElem& curve,
                                 double bPre,bool& isCoincide,
                                 bool& isInclude){
        isCoincide = false;
        auto mp = BaseGeom2D::CurveMidpoint(curve);
        bool isOnBord = false;
        PointInLoop().GetPointInsideLoop(loop,mp,bPre,isOnBord,isInclude);
        if(isInclude || !isOnBord){
            return;
        }
        bool isOnBord1 = false;
        const auto& sp = curve.GetSp();
        PointInLoop().GetPointInsideLoop(loop,sp,bPre,isOnBord1,isInclude);
        if(isInclude || !isOnBord1){
            return;
        }
        bool isOnBord2 = false;
        const auto& ep = curve.GetEp();
        PointInLoop().GetPointInsideLoop(loop,ep,bPre,isOnBord2,isInclude);
        if(isInclude || !isOnBord2){
            return;
        }
        if(isOnBord && isOnBord1 && isOnBord2){
            auto curve1 = curve;
            curve1.ChangeEndpt(mp,true);
            auto mp1 = BaseGeom2D::CurveMidpoint(curve1);
            bool isOnBord3 = false;
            PointInLoop().GetPointInsideLoop(loop,mp1,bPre,isOnBord3,isInclude);
            if(isInclude || !isOnBord3){
                return;
            }
            isCoincide = true;
        }
    }
    //必须初始化loop.GetLimCoord()和curve.GetLimCoord()
    static bool IsLoopContCurve(ofts::DefLoop& loop,
                                ofts::DefElem& curve,
                                const numst::ContParam& cpa,
                                bool isCalInt = true)
    {
        if(!loop.GetLimCoord().HasInit()){
            LimitCoord2D::SetLoopLimCoord(loop);
        }
        if(!curve.GetLimCoord().HasInit()){
            LimitCoord2D::SetCurveLimitCoord(curve);
        }
        if(!LimitCoord2D::IsLimitCoordCont(loop.GetLimCoord(),curve.GetLimCoord(),cpa._incPre)){
            return false;
        }
        if(isCalInt && IsLoopCurveSpecifyInt(loop,curve,cpa)){
            return false;
        }
        bool isCoincide = false,isInclude = false;
        LoopContCurveAux(loop,curve,cpa._onBordPre,isCoincide,isInclude);
        if(!isInclude && isCoincide){
            isInclude = true;
        }
        return isInclude;
    }
    //isMatchBorder 表示是否匹配ele 端点在loop 边界上.
    static bool IsLoopContIntCurve(ofts::DefLoop& loop,
                                   ofts::DefElem& curve,
                                   const numst::ContParam& cpa)
    {
        if(!loop.GetLimCoord().HasInit()){
            LimitCoord2D::SetLoopLimCoord(loop);
        }
        if(!curve.GetLimCoord().HasInit()){
            LimitCoord2D::SetCurveLimitCoord(curve);
        }
        if(LimitCoord2D::IsLimitCoordOut(loop.GetLimCoord(),
                                         loop.GetLimCoord(),cpa._incPre)){
            return false;
        }
        if(IsLoopCurveSpecifyInt(loop,curve,cpa)){
            return true;
        }
        bool is = IsLoopContCurve(loop,curve,cpa,false);
        return is;
    }
};
#endif // LOOPCONTAINAUX_H
