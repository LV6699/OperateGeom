#ifndef ISCURVEINT2D_H
#define ISCURVEINT2D_H
#include"CurveInt2D.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class IsCurveInt2D
{
public:
    IsCurveInt2D(){}
    static bool IsLineIntersect(const ofts::Point &sp1,
                                const ofts::Point &ep1,
                                const ofts::Point &sp2,
                                const ofts::Point &ep2,
                                const numst::IntParam& param)
    {
        return CurveInt2D::IsLineIntersect(sp1,ep1,sp2,ep2,param);
    }
    static bool IsLineCircleInt(const ofts::Point& sp,
                                const ofts::Point& ep,
                                const ofts::Point& cp,double R,
                                const numst::IntParam& param)
    {
        return CurveInt2D::IsLineCircleInt(sp,ep,cp,R,param);
    }
    static bool IsCircleIntersect(const ofts::Point &cp1,double R1,
                                  const ofts::Point &cp2,double R2,
                                  const numst::IntParam& param)
    {
        return CurveInt2D::IsCircleIntersect(cp1,R1,cp2,R2,param);
    }
    static bool IsLineArcInt(const ofts::Point& sp,const ofts::Point& ep,
                             const ofts::DefArc& arc,
                             const numst::IntParam& param)
    {
        if(!CurveInt2D::LineCircleIntCond(
                    sp,ep,arc.GetCp(),arc.R(),param)){return false;}
        const auto& endE = param.EndptPre();
        const auto& parE = param.LCIntPre();
        if(param.CalEndpt()){
            bool isSpOn = false,isEpOn = false;
            isSpOn =  PtOnCurve2D::IsPointOnLine(sp,ep,arc.GetSp(),endE);
            if(isSpOn){return true;}
            isEpOn =  PtOnCurve2D::IsPointOnLine(sp,ep,arc.GetEp(),endE);
            if(isEpOn){return true;}
        }
        ofts::GeomIntRes iRes;
        CurveInt2D::GetLineCircleInt(sp,ep,arc.GetCp(),arc.R(),param,iRes,true);
        if(!iRes.IsInt()){return false;}
        bool isP1On = PtOnCurve2D::IsPointOnArc(arc,iRes.P1(),endE,parE,false);
        if(isP1On){
            if(param.CalEndpt()){return true;}
            if(!arc.IsEndpoint(iRes.P1(),endE)){return true;}
        }
        if(iRes.Num() == 2){
            bool isP2On = PtOnCurve2D::IsPointOnArc(arc,iRes.P2(),endE,parE,false);
            if(isP2On){
                if(param.CalEndpt()){return true;}
                if(!arc.IsEndpoint(iRes.P2(),endE)){return true;}
            }
        }
        return false;
    }
    static bool IsSegmentIntersect(const ofts::Point& sp1,
                                   const ofts::Point& ep1,
                                   const ofts::Point& sp2,
                                   const ofts::Point& ep2,
                                   const numst::IntParam& param)
    {
        if(!CurveBoxInt().IsRectExistInt(sp1,ep1,sp2,ep2,param.LLIntPre() * ErrTimes_10)){
            return false;
        }
        if(!CurveInt2D::LineIntCondition(sp1,ep1,sp2,ep2,param)){return false;}
        const auto& endE = param.EndptPre();
        if(param.CalEndpt()){
            bool isSp1On = false,isEp1On = false;
            isSp1On = PtOnCurve2D::IsPointOnSegment(sp2,ep2,sp1,endE,param.LLIntPre(),true);
            if(isSp1On){return true;}
            isEp1On = PtOnCurve2D::IsPointOnSegment(sp2,ep2,ep1,endE,param.LLIntPre(),true);
            if(isEp1On){return true;}
            bool isSp2On = false,isEp2On = false;
            isSp2On = PtOnCurve2D::IsPointOnSegment(sp1,ep1,sp2,endE,param.LLIntPre(),true);
            if(isSp2On){return true;}
            isEp2On = PtOnCurve2D::IsPointOnSegment(sp1,ep1,ep2,endE,param.LLIntPre(),true);
            if(isEp2On){return true;}
        }
        ofts::GeomIntRes iRes;
        CurveInt2D::GetLineIntersect(sp1,ep1,sp2,ep2,param,iRes,false);
        if(!iRes.IsInt()){return false;}
        bool isOnL1 = false,isOnL2 = false;
        isOnL1 = PtOnCurve2D::IsPointOnSegment(sp1,ep1,iRes.P1(),endE,param.LLIntPre(),false);
        isOnL2 = PtOnCurve2D::IsPointOnSegment(sp2,ep2,iRes.P1(),endE,param.LLIntPre(),false);
        if(!isOnL1 || !isOnL2){return false;}
        if(IntSegEndptCase(param,sp1,ep1,iRes.P1()) &&
                IntSegEndptCase(param,sp2,ep2,iRes.P1())){
            return true;
        }
        return false;
    }
    static bool IsSegmentCircleInt(const ofts::Point& sp,const ofts::Point& ep,
                                   const ofts::Point& cp,double R,
                                   const numst::IntParam& param)
    {
        if(!CurveInt2D::LineCircleIntCond(sp,ep,cp,R,param)){return false;}
        const auto& endE = param.EndptPre();
        if(param.CalEndpt()){
            if(PtOnCurve2D::IsPtOnCircle(sp,cp,R,endE) ||
                    PtOnCurve2D::IsPtOnCircle(ep,cp,R,endE)) {
                return true;
            }
        }
        ofts::GeomIntRes iRes;
        CurveInt2D::GetLineCircleInt(sp,ep,cp,R,param,iRes,false);
        if(!iRes.IsInt()){return false;}
        if(!CurveBoxInt().IsIntResInsideBox(
                    sp,ep,iRes,param.LCIntPre()*ErrTimes_10)){
            return false;
        }
        const auto& ip1 = iRes.P1();
        const auto& onE = param.OnSegPre();
        if(PtOnCurve2D::IsPointOnSegment(sp,ep,ip1,endE,onE,false)){
            if(param.CalEndpt()){return true;}
            if(!ip1.IsEqual(sp,endE) && !ip1.IsEqual(ep,endE)){return true;}
        }
        if(iRes.Num() == 2 ){
            const auto& ip2 = iRes.P2();
            if(PtOnCurve2D::IsPointOnSegment(sp,ep,ip2,endE,onE,false)){
                if(param.CalEndpt()){return true;}
                if(!ip2.IsEqual(sp,endE) && !ip2.IsEqual(ep,endE)){return true;}
            }
        }
        return false;
    }
    static bool IsArcCircleInt(const ofts::DefArc& arc,
                               const ofts::Point& cp,double R,
                               const numst::IntParam& param)
    {
        if(!CurveInt2D::CircleIntCond(arc.GetCp(),arc.R(),cp,R,param)){return false;}
        const auto& endE = param.EndptPre();
        if(param.CalEndpt()){
            if(PtOnCurve2D::IsPtOnCircle(arc.GetSp(),cp,R,endE) ||
                    PtOnCurve2D::IsPtOnCircle(arc.GetEp(),cp,R,endE)){
                return true;
            }
        }
        ofts::GeomIntRes iRes;
        CurveInt2D::GetCircleIntersect(arc.GetCp(),arc.R(),cp,R,param,iRes,false);
        if(!iRes.IsInt()){return false;}
        const auto& ip1 = iRes.P1();
        const auto& onE = param.OnArcPre();
        if(PtOnCurve2D::IsPointOnArc(arc,ip1,endE,onE,false)){
            if(param.CalEndpt()){return true;}
            if(!arc.IsEndpoint(ip1,endE)){return true;}
        }
        if(iRes.Num() == 2){
            const auto& ip2 = iRes.P2();
            if(PtOnCurve2D::IsPointOnArc(arc,ip2,endE,onE,false)){
                if(param.CalEndpt()){return true;}
                if(!arc.IsEndpoint(ip2,endE)){return true;}
            }
        }
        return false;
    }
    static bool IsSegmentArcInt(const ofts::Point& sp,
                                const ofts::Point& ep,
                                const ofts::DefArc& arc,
                                const numst::IntParam& param)
    {
        if(!CurveBoxInt::IsLineCirceBoxInt(sp,ep,arc,param._endptPre * ErrTimes_10)){
            return false;
        }
        if(!CurveInt2D::LineCircleIntCond(sp,ep,arc.GetCp(),arc.R(),param)){
            return false;
        }
        const auto& endE = param.EndptPre();
        const auto& onSegE = param.OnSegPre();
        const auto& onArcE = param.OnArcPre();
        if(param.CalEndpt()){
            if(PtOnCurve2D::IsPointOnArc(arc,sp,endE,onArcE,true)){return true;}
            if(PtOnCurve2D::IsPointOnArc(arc,ep,endE,onArcE,true)){return true;}
            if(PtOnCurve2D::IsPointOnSegment(sp,ep,arc.GetSp(),endE,onSegE,true)){return true;}
            if(PtOnCurve2D::IsPointOnSegment(sp,ep,arc.GetEp(),endE,onSegE,true)){return true;}
        }
        ofts::GeomIntRes iRes;
        CurveInt2D::GetLineCircleInt(sp,ep,arc.GetCp(),arc.R(),param,iRes,true);
        if(!iRes.IsInt() || !CurveBoxInt::IsIntResInsideBox(
                    sp,ep,iRes,param.LCIntPre() * ErrTimes_10)){
            return false;
        }
        const auto& ip1 = iRes.P1();
        const auto& ip2 = iRes.P2();

        if(PtOnCurve2D::IsPointOnSegment(sp,ep,ip1,endE,onSegE,false) &&
                PtOnCurve2D::IsPointOnArc(arc,ip1,endE,onArcE,false)){
            if(param.CalEndpt()){return true;}
            if(!arc.IsEndpoint(ip1,endE) && !ip1.IsEqual(sp,endE) &&
                    !ip1.IsEqual(ep,endE)){
                return true;
            }
        }
        if(iRes.Num() == 2){
            if(!PtOnCurve2D::IsPointOnSegment(sp,ep,ip2,endE,onSegE,false)){return false;}
            if(!PtOnCurve2D::IsPointOnArc(arc,ip2,endE,onArcE,false)){return false;}
            if(param.CalEndpt()){return true;}
            if(!arc.IsEndpoint(ip2,endE) && !ip2.IsEqual(sp,endE) &&
                    !ip2.IsEqual(ep,endE)){
                return true;
            }
        }
        return false;
    }
    static bool IsArcIntersect(const ofts::DefArc& arc1,
                               const ofts::DefArc& arc2,
                               const numst::IntParam& param)
    {
        const auto& endE = param.EndptPre();
        if(!CurveBoxInt::IsCirceCirceBoxInt(arc1.GetCp(),arc1.R(),arc2.GetCp(),
                                            arc2.R(),endE * ErrTimes_10)){
            return false;
        }
        if(!CurveInt2D::CircleIntCond(arc1.GetCp(),arc1.R(),
                                      arc2.GetCp(),arc2.R(),param)){
            return false;
        }
        ofts::GeomIntRes iRes;
        if(param.CalEndpt()){
            if(arc1.IsEndpoint(arc2.GetSp(),endE) || arc1.IsEndpoint(arc2.GetEp(),endE) ||
                    arc2.IsEndpoint(arc1.GetSp(),endE) || arc2.IsEndpoint(arc1.GetEp(),endE)){
                return true;
            }
        }
        CurveInt2D::GetCircleIntersect(arc1.GetCp(),arc1.R(),arc2.GetCp(),arc2.R(),param,iRes);
        if(!iRes.IsInt()){return false;}
        const auto& ip1 = iRes.P1();
        const auto& onE = param.OnArcPre();
        if(PtOnCurve2D::IsPointOnArc(arc1,ip1,endE,onE,false) &&
                PtOnCurve2D::IsPointOnArc(arc2,ip1,endE,onE,false)){
            if(param.CalEndpt()){return true;}
            if(!arc1.IsEndpoint(ip1,endE) && !arc2.IsEndpoint(ip1,endE)){
                return true;
            }
        }
        if(iRes.Num() == 2){
            const auto& ip2 = iRes.P2();
            if(!PtOnCurve2D::IsPointOnArc(arc1,ip2,endE,onE,false)){return false;}
            if(!PtOnCurve2D::IsPointOnArc(arc2,ip2,endE,onE,false)){return false;}
            if(param.CalEndpt()){return true;}
            if(!arc1.IsEndpoint(ip2,endE) && !arc2.IsEndpoint(ip2,endE)){
                return true;
            }
        }
        return false;
    }
    static bool IntSegEndptCase(const numst::IntParam& param,
                                const ofts::Point& sp,
                                const ofts::Point& ep,
                                const ofts::Point& p){
        if(!param.CalEndpt() && (p.IsSamePoint2D(sp,param.EndptPre()) ||
                                 p.IsSamePoint2D(ep,param.EndptPre()))){
            return false;
        }
        return true;
    }
    static bool IntArcEndptCase(const numst::IntParam& param,
                                const ofts::DefArc& arc,
                                const ofts::Point& p){
        if(!param.CalEndpt() && (p.IsSamePoint2D(arc.GetSp(),param.EndptPre()) ||
                                 p.IsSamePoint2D(arc.GetEp(),param.EndptPre()))){
            return false;
        }
        return true;
    }
    static bool IsCurveIntersect(const ofts::DefElem& c1,
                                 const ofts::DefElem& c2,
                                 const numst::IntParam& param)
    {
        switch (c1._type) {
        case Ofte::LINETYPE:
            switch (c2._type) {
            case Ofte::LINETYPE:
                return IsSegmentIntersect(c1._segment.GetSp(),
                                          c1._segment.GetEp(),
                                          c2._segment.GetSp(),
                                          c2._segment.GetEp(),param);
            case Ofte::ARCTYPE:
                return IsSegmentArcInt(c1._segment.GetSp(),
                                       c1._segment.GetEp(),
                                       c2._arc,param);
            case Ofte::CIRCLETYPE:
                return IsSegmentCircleInt(c1._segment.GetSp(),
                                          c1._segment.GetEp(),
                                          c2._circle._cirCp,
                                          c2._circle._cirR,param);
            }
            break;
        case Ofte::ARCTYPE:
            switch (c2._type) {
            case Ofte::LINETYPE:
                return IsSegmentArcInt(c2._segment.GetSp(),
                                       c2._segment.GetEp(),
                                       c1._arc,param);
            case Ofte::ARCTYPE:
                return IsArcIntersect(c1._arc,c2._arc,param);
            case Ofte::CIRCLETYPE:
                return IsArcCircleInt(c1._arc,c2._circle._cirCp,
                                      c2._circle._cirR,param);
            }
            break;
        case Ofte::CIRCLETYPE:
            switch (c2._type) {
            case Ofte::LINETYPE:
                return IsSegmentCircleInt(c2._segment.GetSp(),
                                          c2._segment.GetEp(),
                                          c1._circle._cirCp,
                                          c1._circle._cirR,param);
            case Ofte::ARCTYPE:
                return IsArcCircleInt(c2._arc,c1._circle._cirCp,
                                      c1._circle._cirR,param);
            case Ofte::CIRCLETYPE:
                return IsCircleIntersect(c1._circle._cirCp,
                                         c1._circle._cirR,
                                         c2._circle._cirCp,
                                         c2._circle._cirR,param);
            }
            break;
        default: return false;
        }
        return false;
    }
    bool IsCurveLoopInt(const ofts::DefElem& curve,
                        const ofts::DefLoop& loop,
                        const numst::IntParam& param);
    bool IsLoopIntersect(const ofts::DefLoop& loop1,
                         const ofts::DefLoop& loop2,
                         const numst::IntParam& param) ;/*{
        for(size_t i = 0; i < loop1.Size(); ++i) {
            if(IsCurveLoopInt(loop1[i], loop2, param)) {
                return true;
            }
        }
        return false;
    }*/


};
#endif // ISCURVEINT2D_H
