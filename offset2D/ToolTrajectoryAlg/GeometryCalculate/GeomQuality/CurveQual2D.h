#ifndef CURVEQUAL2D_H
#define CURVEQUAL2D_H
#include"../GeomCalculate/PtOnCurve2D.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class CurveQual2D
{
public:
    CurveQual2D(){}
    static bool IsLineCircleTan(const ofts::Point& sp,const ofts::Point& ep,
                                const ofts::Point& cp,double R, double pre)
    {
        double dis = BaseGeom2D::PointToLineDist(sp,ep,cp);
        if(std::abs(dis - R) <= pre){
            return true;
        }
        return false;
    }
    static bool IsCircleTangency(const ofts::Point& cp1,double R1,
                                 const ofts::Point& cp2,double R2,double pre)
    {
        bool isTan = false;
        double maxR = (R1 > R2) ? R1 : R2;
        double minR = (R1 < R2) ? R1 : R2;
        double cirDis = cp1.Distance2D(cp2);
        if(std::abs(cirDis - (R1 + R2)) <= pre ||
                std::abs(maxR - minR - cirDis) <= pre){
            isTan = true;
        }
        return isTan;
    }
    static bool IsCurveTangency(const ofts::DefElem& c1,
                                const ofts::DefElem& c2,
                                bool isJudLine,double pre)
    {
        if(c1._type == LINETYPE && c2._type == LINETYPE){
            if(!isJudLine){return false;}
            const auto& sp1 = c1._segment.GetSp(),ep1 = c1._segment.GetEp(),
                    sp2 = c2._segment.GetSp(),ep2 = c2._segment.GetEp();
            return IsLineExistCoincide(sp1,ep1,sp2,ep2,pre);
        }
        bool isLineCir = false;
        if(c1._type == LINETYPE || c2._type == LINETYPE){
            isLineCir = true;
        }
        auto sp = c1._segment.GetSp(),ep = c1._segment.GetEp();
        if(c2._type == LINETYPE){
            sp = c2._segment.GetSp();
            ep = c2._segment.GetEp();
        }
        double R1,R2;
        ofts::Point cp1,cp2;
        if(c1._type == LINETYPE){
            if(c2._type == ARCTYPE){
                cp1 = c2._arc._arcCp;
                R1 = c2._arc._arcR;
            }else if(c2._type == CIRCLETYPE){
                cp1 = c2._circle._cirCp;
                R1 = c2._circle._cirR;
            }
        }else if(c1._type == ARCTYPE){
            cp1 = c1._arc._arcCp;
            R1 = c1._arc._arcR;
            if(c2._type == ARCTYPE){
                cp2 = c2._arc._arcCp;
                R2 = c2._arc._arcR;
            }else if(c2._type == CIRCLETYPE){
                cp2 = c2._circle._cirCp;
                R2 = c2._circle._cirR;
            }
        }else if(c1._type == CIRCLETYPE){
            cp1 = c1._circle._cirCp;
            R1 = c1._circle._cirR;
            if(c2._type == ARCTYPE){
                cp2 = c2._arc._arcCp;
                R2 = c2._arc._arcR;
            }else if(c2._type == CIRCLETYPE){
                cp2 = c2._circle._cirCp;
                R2 = c2._circle._cirR;
            }
        }
        bool isTan = false;
        if(isLineCir){
            isTan = IsLineCircleTan(sp,ep,cp1,R1,pre);
        }else{
            isTan = IsCircleTangency(cp1,R1,cp2,R2,pre);
        }
        return isTan;
    }
    static bool IsCircleCointcoincide(const ofts::Point& cp1,double r1,
                                      const ofts::Point& cp2,double r2,
                                      double pre)
    {
        return (cp1.IsSamePoint2D(cp2,pre) &&
                pnum::IsEqualNum(r1,r2,pre));
    }
    static bool IsSegExistCoincide(const ofts::Point &sp1,const ofts::Point &ep1,
                                   const ofts::Point &sp2,const ofts::Point &ep2,
                                   double pre,bool& isCoin)
    {
        if(!IsLineSlopeEqual(sp1,ep1,sp2,ep2,pre)){return false;}
        bool isOn = false;
        isOn = PtOnCurve2D::IsPointOnSegment(
                    sp2,ep2,sp1,pre,pre,true);
        if(isOn){return true;}
        isOn = PtOnCurve2D::IsPointOnSegment(sp2,ep2,ep1,pre,pre,true);
        if(isOn){return true;}
        isOn = PtOnCurve2D::IsPointOnSegment(sp1,ep1,sp2,pre,pre,true);
        if(isOn){return true;}
        isOn = PtOnCurve2D::IsPointOnSegment(sp1,ep1,ep2,pre,pre,true);
        if(isOn){return true;}
        return false;
    }
    //计算两线段是否平行,err 为角度误差为a 时的正弦值平方
    static bool IsLineAglEqual(const ofts::Point& sp1,const ofts::Point& ep1,
                               const ofts::Point& sp2,const ofts::Point& ep2,
                               double err)
    {
        /*double dx1 = ep1.X() - sp1.X();dy1 = ep1.Y() - sp1.Y();
         dx2 = ep2.X() - sp2.X();dy2 = ep2.Y() - sp2.Y();
         c = dx1 * dy2 - dy1 * dx2;return std::abs(c) < e;
        //叉积法误差较大,如果点坐标值较大计算误差也较大*/
        double dx1 = ep1.X() - sp1.X();
        double dy1 = ep1.Y() - sp1.Y();
        double dx2 = ep2.X() - sp2.X();
        double dy2 = ep2.Y() - sp2.Y();
        double l_sq1 = dx1 * dx1 + dy1 * dy1;
        double l_sq2 = dx2 * dx2 + dy2 * dy2;
        double c = dx1 * dy2 - dy1 * dx2;
        if (std::abs(c) < PreErr_10) {
            double c1 = dx1 * dy2;
            double c2 = dy1 * dx2;
            c = c1 - c2;
        }
        double c_sq = c * c;
        double threshold = err * l_sq1 * l_sq2;
        return c_sq <= threshold;
    }
    static bool IsLineAglEqual(const ofts::DefSeg& s1,const ofts::DefSeg& s2,double e)
    {
        return IsLineAglEqual(s1.GetSp(),s1.GetEp(),s2.GetSp(),s2.GetEp(),e);
    }
    static bool IsLineSlopeEqual(const ofts::Point& sp1,const ofts::Point& ep1,
                                 const ofts::Point& sp2,const ofts::Point& ep2,double e)
    {
        double s1 = BaseGeom2D::SegmentSlope(sp1,ep1);
        double s2 = BaseGeom2D::SegmentSlope(sp2,ep2);
        double e1 = std::abs(s1 - s2);
        return e1 <= e;
    }
    static bool IsLineSlopeEqual(const ofts::DefSeg& s1,
                                 const ofts::DefSeg& s2,double e)
    {
        return IsLineSlopeEqual(s1.GetSp(),s1.GetEp(),s2.GetSp(),s2.GetEp(),e);
    }
    static bool IsLineExistCoincide(const ofts::Point& sp1, const ofts::Point& ep1,
                                    const ofts::Point& sp2, const ofts::Point& ep2,
                                    double pre)
    {
        PtOnCurve2D oc;
        if(!IsLineSlopeEqual(sp1,ep1,sp2,ep2,pre)){
            return false;}
        if(oc.IsPointOnSegment(sp2,ep2,sp1,pre,pre,true)){
            return true;}
        if(oc.IsPointOnSegment(sp2,ep2,ep1,pre,pre,true)){
            return true;}
        if(oc.IsPointOnSegment(sp1,ep1,sp2,pre,pre,true)){
            return true;}
        if(oc.IsPointOnSegment(sp1,ep1,ep2,pre,pre,true)){
            return true;}
        return false;
    }
    static bool IsLineCompleteCoincide(const ofts::Point& sp1,const ofts::Point& ep1,
                                       const ofts::Point& sp2,const ofts::Point& ep2,
                                       double pre)
    {
        if(!IsLineSlopeEqual(sp1,ep1,sp2,ep2,pre))
            return false;
        if(sp1.IsSamePoint2D(sp2,pre) &&
                ep1.IsSamePoint2D(ep2,pre))
            return true;
        if(sp1.IsSamePoint2D(ep2,pre) &&
                ep1.IsSamePoint2D(sp2,pre))
            return true;
        return false;
    }
    static bool IsCoincideSeg(const ofts::DefSeg& s1,
                              const ofts::DefSeg& s2,double err)
    {
        return (s1.GetSp().IsSamePoint2D(s2.GetSp(),err) &&
                s1.GetEp().IsSamePoint2D(s2.GetEp(),err)) ||
                (s1.GetSp().IsSamePoint2D(s2.GetEp(),err) &&
                 s1.GetEp().IsSamePoint2D(s2.GetSp(),err));
    }
    static bool IsLineLocatedOnLine(const ofts::Point& refSp,
                                    const ofts::Point& refEp,
                                    const ofts::Point& sp,const ofts::Point& ep,
                                    double pre)
    {
        if(!IsLineSlopeEqual(refSp,refEp,sp,ep,pre))
            return false;
        PtOnCurve2D oc;
        bool isSpOn = false,isEpOn = false;
        isSpOn = oc.IsPointOnSegment(refSp,refEp,sp,pre,pre,true);
        isEpOn = oc.IsPointOnSegment(refSp,refEp,ep,pre,pre,true);
        return (isSpOn && isEpOn);
    }
    static bool IsArcExistCoincide(const ofts::DefArc& arc1,
                                   const ofts::DefArc& arc2,double pre)
    {
        if(!arc1._arcCp.IsSamePoint2D(arc2._arcCp,pre) ||
                !pnum::IsEqualNum(arc1._arcR,arc2._arcR,pre))
            return false;
        PtOnCurve2D oc;
        bool sp1on = false,ep1on = false;
        sp1on = oc.IsPointOnArc(arc2,arc1._arcSp,pre,PreErr5_10,false);
        ep1on = oc.IsPointOnArc(arc2,arc1._arcEp,pre,PreErr5_10,false);
        if(sp1on || ep1on)
            return true;
        return false;
    }
    static bool IsArcCompleteCoincide(const ofts::DefArc& arc1,
                                      const ofts::DefArc& arc2,double pre)
    {
        if(!arc1._arcCp.IsSamePoint2D(arc2._arcCp,pre))
            return false;
        if(!pnum::IsEqualNum(arc1._arcR,arc2._arcR,pre))
            return false;
        if(arc1._arcSp.IsSamePoint2D(arc2._arcSp,pre) &&
                arc1._arcEp.IsSamePoint2D(arc2._arcEp,pre) &&
                arc1.IsCW() == arc2.IsCW())
            return true;
        if(arc1._arcSp.IsSamePoint2D(arc2._arcEp,pre) &&
                arc1._arcEp.IsSamePoint2D(arc2._arcSp,pre) &&
                arc1.IsCW() != arc2.IsCW())
            return true;
        return false;
    }
    static bool IsCoincideArc(const ofts::DefArc& a1,
                              const ofts::DefArc& a2,double err)
    {
        if(a1.IsCW() != a2.IsCW()){return false;}
        if(!a1.GetCp().IsSamePoint2D(a2.GetCp(),err)){return false;}
        return (a1.GetSp().IsSamePoint2D(a2.GetSp(),err) &&
                a1.GetEp().IsSamePoint2D(a2.GetEp(),err)) ||
                (a1.GetSp().IsSamePoint2D(a2.GetEp(),err) &&
                 a1.GetEp().IsSamePoint2D(a2.GetSp(),err));
    }
    static bool IsArcLocatedOnArc(const ofts::DefArc &refArc,
                                  const ofts::DefArc &arc,double pre)
    {
        PtOnCurve2D pton;
        if(!refArc._arcCp.IsSamePoint2D(arc._arcCp,pre))
            return false;
        if(!pnum::IsEqualNum(refArc._arcR,arc._arcR,pre))
            return false;
        bool isSpOn = false,isEpOn = false,isMidOn = false;
        isSpOn = pton.IsPointOnArc(refArc,arc._arcSp,pre,pre,false);
        if(isSpOn){
            isEpOn = pton.IsPointOnArc(refArc,arc._arcEp,pre,pre,false);
            if(isEpOn){
                auto mp = BaseGeom2D().ArcMidpoint(arc);
                isMidOn = pton.IsPointOnArc(refArc,mp,pre,pre,false);
                if(isMidOn){return true;}
            }
        }
        return false;
    }
    static bool IsCircleCoincide(const ofts::Point& cp1,double r1,
                                 const ofts::Point& cp2,double r2,
                                 double pre)
    {
        return (cp1.IsSamePoint2D(cp2,pre) &&
                pnum::IsEqualNum(r1,r2,pre));
    }
    static bool IsCoincideCircle(const ofts::DefCircle& c1,
                                 const ofts::DefCircle& c2,double err)
    {
        return pnum::IsEqualNum(c1.R(),c2.R(),err) &&
                c1.GetCp().IsSamePoint2D(c2.GetCp(),err);
    }
    static bool IsCurveCoincide(const ofts::DefElem& c1,
                                const ofts::DefElem& c2,
                                bool isComplete,double pre)
    {
        if(c1._type != c2._type){return false;}
        if(c1._type == LINETYPE){
            if(isComplete){
                return IsLineCompleteCoincide(c1._segment.GetSp(),
                                              c1._segment.GetEp(),
                                              c2._segment.GetSp(),
                                              c2._segment.GetEp(),pre);
            }
            return IsLineExistCoincide(c1._segment.GetSp(),
                                       c1._segment.GetEp(),
                                       c2._segment.GetSp(),
                                       c2._segment.GetEp(),pre);
        }else if(c1._type == ARCTYPE){
            if(isComplete){
                return IsArcCompleteCoincide(c1._arc,c2._arc,pre);
            }
            return IsArcExistCoincide(c1._arc,c2._arc,pre);
        }else if(c1._type == CIRCLETYPE){
            return IsCircleCoincide(c1._circle._cirCp,
                                    c1._circle._cirR,
                                    c2._circle._cirCp,
                                    c2._circle._cirR,pre);
        }
        return false;
    }
    static bool IsCoincideCurve(const ofts::DefElem& c1,
                                const ofts::DefElem& c2,double err)
    {
        if(c1._type != c2._type){return false;}
        switch (c1._type) {
        case Ofte::LINETYPE:
            return IsCoincideSeg(c1.GetSeg(),c2.GetSeg(),err);
        case Ofte::ARCTYPE:
            return IsCoincideArc(c1.GetArc(),c2.GetArc(),err);
        case Ofte::CIRCLETYPE:
            return IsCoincideCircle(c1.GetCircle(),c2.GetCircle(),err);
        default:
            return false;
        }
        return false;
    }
    static bool IsCurveExistCoincide(const ofts::DefElem& c1,
                                     const ofts::DefElem& c2,double pre)
    {
        if(c1._type == LINETYPE && c2._type == LINETYPE){
            return IsLineExistCoincide(c1._segment.GetSp(),
                                       c1._segment.GetEp(),
                                       c2._segment.GetSp(),
                                       c2._segment.GetEp(),pre);
        }
        else if(c1._type == ARCTYPE && c2._type == ARCTYPE){
            return IsArcExistCoincide(c1._arc,c2._arc,pre);
        }
        return false;
    }
    static bool IsIsCurveLocatedIsCurve(const ofts::DefElem& rc,
                                        const ofts::DefElem& c,double pre)
    {
        if(rc._type != c._type){return false;}
        if(rc._type == LINETYPE){
            return IsLineLocatedOnLine(rc._segment.GetSp(),
                                       rc._segment.GetEp(),
                                       c._segment.GetSp(),
                                       c._segment.GetEp(),
                                       pre);
        }
        else if(rc._type == ARCTYPE){
            return IsArcLocatedOnArc(rc._arc,c._arc,pre);
        }
        else if(rc._type == CIRCLETYPE){
            return IsCircleCointcoincide(rc._circle._cirCp,
                                         rc._circle._cirR,
                                         c._circle._cirCp,
                                         c._circle._cirR,pre);
        }
        return false;
    }
    static bool IsHorizontalLine(const ofts::Point& sp,const ofts::Point& ep, double pre)
    {
        double agl = BaseGeom2D::SegmentAngle(sp,ep);
        if(pnum::IsEqualNum(agl,0,pre) ||
                pnum::IsEqualNum(agl,180,pre) ||
                pnum::IsEqualNum(agl,360,pre))
            return true;
        return false;
    }
    static bool IsVerticalLine(const ofts::Point& sp,const ofts::Point& ep,double pre)
    {
        double agl = BaseGeom2D::SegmentAngle(sp,ep);
        if(pnum::IsEqualNum(agl,90,pre) ||
                pnum::IsEqualNum(agl,270,pre))
            return true;
        return false;
    }
    static bool IsValidGeomSeg(const ofts::DefSeg& seg,double err)
    {
        return !seg.GetSp().IsEqual(seg.GetEp(),err,seg.Is3D());
    }
    static bool IsValidGeomArc(const ofts::DefArc& arc,double endVal,
                               bool isR = false,double minR = PreErr5_10,
                               bool isBh = false,double H = PreErr5_10,
                               bool isDecNum = false,int number = 6)
    {
        if(arc.GetSp().IsEqual(arc.GetEp(),endVal,arc.Is3D())){
            double agl = BaseGeom2D::ArcCenterAngle(arc);
            if(agl < 180){
                return false;
            }
        }
        if(isR){
            double R = arc.GetOptR(PreErr5_2);
            if(R < minR){return false;}
        }
        if(isBh){
            double bh = BaseGeom2D::ArcBowHeight(arc);
            if(bh < H){
                return false;
            }
        }
        if(!isDecNum){
            return true;
        }
        double angle = BaseGeom2D::ArcCenterAngle(arc);
        double sx_ = arc.GetSp().X(),sy_ = arc.GetSp().Y(),
                ex_ = arc.GetEp().X(),ey_ = arc.GetEp().Y();
        double sx = pnum::NumDecimalPlace(sx_,number);
        double sy = pnum::NumDecimalPlace(sy_,number);
        double ex = pnum::NumDecimalPlace(ex_,number);
        double ey = pnum::NumDecimalPlace(ey_,number);
        double endPre = 1 / std::pow(10.0,number);
        double vx_ = std::abs(sx_ - ex_);
        double vy_ = std::abs(sy_ - ey_);
        double vx = std::abs(sx - ex);
        double vy = std::abs(sy - ey);
        if(((vx_ > PreErr5_10 && vx < endPre) ||
            (vy_ > PreErr5_10 && vy < endPre)) && angle < 180){
            return false;
        }
        return true;
    }

    static bool IsValidGeomCurve(const ofts::DefElem& curve,double segErr,double aEnd,
                                 bool isR = false,double minR = PreErr5_10,
                                 bool isBh = false,double bH = PreErr5_10,
                                 bool aIsDecNum = false,int aNum = 6)
    {
        if(curve._type == Ofte::LINETYPE){
            return IsValidGeomSeg(curve.GetSeg(),segErr);
        }else if(curve._type == Ofte::ARCTYPE){
            return IsValidGeomArc(curve.GetArc(),aEnd,isR,minR,
                                  isBh,bH,aIsDecNum,aNum);
        }
        return curve._circle.R() >= minR;
    }
    bool IsCurveParallel(const ofts::DefElem& c0,const ofts::DefElem& c1,
                         const numst::FitParam& param)
    {
        if(c0.Type() != c1.Type()){return false;}
        if(c0.Type() == Ofte::LINETYPE){
            const auto& s0 = c0.GetSeg();
            const auto& s1 = c1.GetSeg();
            if(!CurveQual2D::IsLineAglEqual(c0.GetSeg(),c1.GetSeg(),
                                            param.LinearSin())){
                return false;
            }
            return true;
        }
        if(c0.Type() == Ofte::ARCTYPE){
            if(!c0.GetArc().GetCp().IsEqual(c1.GetArc().GetCp(),param.CpVar()) ||
                    !pnum::IsEqualNum(c0.GetArc().GetOptR(),c1.GetArc().GetOptR(),param.RVar())){
                return false;
            }
            return true;
        }
        if(c0.Type() == Ofte::CIRCLETYPE){
            if(!c0.GetCircle().GetCp().IsEqual(c1.GetCircle().GetCp(),param.CpVar()) ||
                    !pnum::IsEqualNum(c0.GetCircle().R(),c1.GetCircle().R(),param.RVar())){
                return false;
            }
            return true;
        }
        return false;
    }
};
#endif // CURVEQUAL2D_H
