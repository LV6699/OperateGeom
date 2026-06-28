#ifndef CURVEDIST2D_H
#define CURVEDIST2D_H
#include "../GeomIntersect/IsCurveInt2D.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class CurveDist2D
{
public:
    CurveDist2D(){}
    ///需要保证点在圆弧上.
    static double PointToArcEndptDist(ofts::DefArc arc,const ofts::Point& P,bool isCloseSp)
    {
        double dis = 0;
        double theta = std::atan2((P.Y() - arc.GetCp().Y()),(P.X() - arc.GetCp().X()));
        theta = theta * _AlgRad;
        if(theta < 0){
            theta = theta + 360;
        }
        bool isOnArc = false;
        double sAgl_,eAgl_;
        BaseGeom2D::ArcEndpointAngle(arc,sAgl_,eAgl_);
        isOnArc = pnum::AglIsBetweenAgls(sAgl_,eAgl_,arc.IsCW(),theta,PreErr_6);
        ///如果点在圆弧上,计算目标点至圆弧端点最近距离,且不考虑旋向,
        ///否则需要按照旋向计算目标点到圆弧端点距离.
        if(isOnArc){
            if(isCloseSp){arc._arcEp = P;}
            else{arc._arcSp = P;}
        }else{
            ofts::Point arcSp_ = arc._arcSp;
            arc._arcSp = P;
            if(isCloseSp){arc._arcEp = arcSp_;}
        }
        double sAgl,eAgl,aglAbs;
        BaseGeom2D::ArcEndpointAngle(arc,sAgl,eAgl);
        aglAbs = pnum::TwoAglIncludedAgl(sAgl,eAgl,arc._isCW);
        dis = aglAbs;
        if(dis == 360){dis = 0;}
        return dis;
    }
    static double PointToCurveEndptDist(const ofts::DefElem& c,
                                        const ofts::Point& P,
                                        bool isCloseSp)
    {
        switch (c._type) {
        case Ofte::LINETYPE :{
            if(isCloseSp){return P.Distance2D(c._segment._segSp);}
            else{return P.Distance2D(c._segment._segEp);}
        }
        case Ofte::ARCTYPE:{
            return PointToArcEndptDist(c._arc,P,isCloseSp);
        }
        default:break;
        }
        return 0;
    }
    static double PointToCircleMaxDist(const ofts::Point& p,
                                       const ofts::DefCircle& c)
    {
        double dis = p.Distance2D(c.GetCp());
        return dis + c.R();
    }
    static double PointToSegMaxDist(const ofts::Point& p,
                                    const ofts::Point& sp,
                                    const ofts::Point& ep)
    {
        double d1 = p.DistSquare2D(sp),d2 = p.DistSquare2D(ep);
        return d1 > d2 ? std::sqrt(d1) : std::sqrt(d2);
    }
    static double PointToSegmentDist(const ofts::Point &p,
                                     const ofts::Point &sp,
                                     const ofts::Point &ep)
    {
        ofts::Point proPt;
        BaseGeom2D::PointOnLineProj(p,sp,ep,proPt);
        bool isOn = false;
        ///BaseGeom2D().IsPointOnSegment(sp,ep,proPt,OnSeg_Err,false,isOnSeg);
        isOn = PtOnCurve2D::IsPointOnSegment(sp,ep,proPt,PreErr_10,
                                             PreErr_10,false);
        double dist = 0;
        if(isOn){
            dist = BaseGeom2D::PointToLineDist(sp,ep,p);
        }else{
            double d1 = p.DistSquare2D(sp),
                    d2 = p.DistSquare2D(ep);
            dist = d1 < d2 ? std::sqrt(d1) : std::sqrt(d2);
        }
        return dist;
    }
    static double PointToArcDist(const ofts::Point &p,const ofts::DefArc &arc)
    {
        double sAgl,eAgl,dist = 0;
        BaseGeom2D::ArcEndpointAngle(arc,sAgl,eAgl);
        double theta = std::atan2((p.Y() - arc._arcCp.Y()),
                                  (p.X() - arc._arcCp.X())) * _AlgRad;
        if(theta < 0){theta += 360;}
        bool isIn = false;
        isIn = pnum::AglIsBetweenAgls(sAgl,eAgl,arc.IsCW(),
                                      theta,PreErr_6);
        if(isIn){
            double toArcDis = p.Distance2D(arc._arcCp);
            dist = std::abs(toArcDis - arc._arcR);
        }else {
            double d1 = p.DistSquare2D(arc.GetSp());
            double d2 = p.DistSquare2D(arc.GetEp());
            dist = d1 < d2 ? std::sqrt(d1) : std::sqrt(d2);
        }
        return dist;
    }
    static double PointToCircleDist(const ofts::Point p,
                                    const ofts::Point &cp,
                                    double r)
    {
        return std::abs(p.Distance2D(cp) - r);
    }
    static double CurveEndptDist(const ofts::Point &sp1,
                                 const ofts::Point &ep1,
                                 const ofts::Point &sp2,
                                 const ofts::Point &ep2,
                                 double &dist)
    {
        double ssDis = sp1.DistSquare2D(sp2);
        double seDis = sp1.DistSquare2D(ep2);
        double esDis = ep1.DistSquare2D(sp2);
        double eeDis = ep1.DistSquare2D(ep2);
        double d1 = ssDis < seDis ? ssDis : seDis;
        double d2 = esDis < eeDis ? esDis : eeDis;
        return d1 < d2 ? std::sqrt(d1) : std::sqrt(d2);
    }
    static double CurveEndptDistSquare(const ofts::Point &sp1,
                                       const ofts::Point &ep1,
                                       const ofts::Point &sp2,
                                       const ofts::Point &ep2)
    {
        double ssDis = sp1.DistSquare2D(sp2);
        double seDis = sp1.DistSquare2D(ep2);
        double esDis = ep1.DistSquare2D(sp2);
        double eeDis = ep1.DistSquare2D(ep2);
        double d1 = ssDis < seDis ? ssDis : seDis;
        double d2 = esDis < eeDis ? esDis : eeDis;
        return d1 < d2 ? d1 : d2;
    }
    static double SegmentDistance(const ofts::Point &sp1,
                                  const ofts::Point &ep1,
                                  const ofts::Point &sp2,
                                  const ofts::Point &ep2)
    {
        double endDis = CurveEndptDistSquare(sp1,ep1,sp2,ep2);
        ofts::Point l1SpPro,l1EpPro,l2SpPro,l2EpPro;
        BaseGeom2D::PointOnLineProj(sp1,sp2,ep2,l1SpPro);
        BaseGeom2D::PointOnLineProj(ep1,sp2,ep2,l1EpPro);
        BaseGeom2D::PointOnLineProj(sp2,sp1,ep1,l2SpPro);
        BaseGeom2D::PointOnLineProj(ep2,sp1,ep1,l2EpPro);
        bool isOn1 = false,isOn2 = false,isOn3 = false,isOn4 = false;
        isOn1 = PtOnCurve2D::IsPointOnSegment(sp2,ep2,l1SpPro,
                                              PreErr_10,PreErr_10,false);
        isOn2 = PtOnCurve2D::IsPointOnSegment(sp2,ep2,l1EpPro,
                                              PreErr_10,PreErr_10,false);
        isOn3 = PtOnCurve2D::IsPointOnSegment(sp1,ep1,l2SpPro,
                                              PreErr_10,PreErr_10,false);
        isOn4 = PtOnCurve2D::IsPointOnSegment(sp1,ep1,l2EpPro,
                                              PreErr_10,PreErr_10,false);
        double dist = endDis;
        if(isOn1){
            double d = sp1.DistSquare2D(l1SpPro);
            if(d < dist){dist = d;}
        }
        if(isOn2){
            double d = ep1.DistSquare2D(l1EpPro);
            if(d < dist){dist = d;}
        }
        if(isOn3){
            double d = sp2.DistSquare2D(l2SpPro);
            if(d < dist){dist = d;}
        }
        if(isOn4){
            double d = ep2.DistSquare2D(l2EpPro);
            if(d < dist){dist = d;}
        }
        return std::sqrt(dist);
    }
    static double SegmentArcDist(const ofts::Point &sp,
                                 const ofts::Point &ep,
                                 const ofts::DefArc &arc)
    {
        numst::IntParam param;
        param._calParalLine = true;
        param._calParalCir = true;
        bool isExiInt = false;
        isExiInt = IsCurveInt2D::IsSegmentArcInt(sp,ep,arc,param);
        if(isExiInt){return 0;}
        ofts::Point proPt;
        const auto& arcSp = arc._arcSp,arcEp = arc._arcEp,arcCp = arc._arcCp;
        BaseGeom2D::PointOnLineProj(arcCp,sp,ep,proPt);
        if(PtOnCurve2D::IsPointOnSegment(sp,ep,proPt,PreErr5_10,
                                         PreErr5_10,false)){
            isExiInt = IsCurveInt2D::IsSegmentArcInt(arcCp,proPt,arc,param);
            if(isExiInt){
                double dis = BaseGeom2D::PointToLineDist(sp,ep,arcCp);
                return std::abs(dis - arc._arcR);
            }
        }
        double aSpSegDis = PointToSegmentDist(arcSp,sp,ep);
        double aEpSegDis = PointToSegmentDist(arcEp,sp,ep);
        double sArcDis = PointToArcDist(sp,arc);
        double eArcDis = PointToArcDist(ep,arc);
        double aEndSegDis = aSpSegDis < aEpSegDis ? aSpSegDis : aEpSegDis;
        double segEndADis = sArcDis < eArcDis ? sArcDis : eArcDis;
        return aEndSegDis < segEndADis ? aEndSegDis : segEndADis;
    }
    static double SegmentCircleDist(const ofts::Point &sp,const ofts::Point &ep,
                                    const ofts::Point &cp,double R)
    {
        ofts::Point proPt;
        BaseGeom2D::PointOnLineProj(cp,sp,ep,proPt);
        if(PtOnCurve2D::IsPointOnSegment(sp,ep,proPt,
                                         OnSeg_Err,OnSeg_Err,false)){
            double dis = BaseGeom2D::PointToLineDist(sp,ep,proPt);
            return std::abs(dis - R);
        }
        double d1 = cp.DistSquare2D(sp),d2 = cp.DistSquare2D(ep);
        double dis = d1 < d2 ? std::sqrt(d1) : std::sqrt(d2);
        return std::abs(dis - R);
    }
    static double ArcEndptToArcDist(const ofts::DefArc &arc1,
                                    const ofts::DefArc &arc2)
    {
        double dis1 = PointToArcDist(arc1._arcSp,arc2);
        double dis2 = PointToArcDist(arc1._arcEp,arc2);
        double dis3 = PointToArcDist(arc2._arcSp,arc1);
        double dis4 = PointToArcDist(arc2._arcEp,arc1);
        double d1 = dis1 < dis2 ? dis1 : dis2;
        double d2 = dis3 < dis4 ? dis3 : dis4;
        return d1 < d2 ? d1 : d2;
    }
    static double ArcCircleDist(const ofts::DefArc &arc,
                                const ofts::Point &cp,double R)
    {
        double dist = 0;
        const auto& arcCp = arc._arcCp;
        bool isExiIn = false;
        numst::IntParam param;
        param._calParalLine = true;
        param._calParalCir = true;
        isExiIn = IsCurveInt2D::IsLineArcInt(arcCp,cp,arc,param);
        if(isExiIn){
            double dis = arcCp.Distance2D(cp);
            return std::abs(dis - arc._arcR - R);
        }
        double d1 = arc._arcSp.DistSquare2D(cp),
                d2 = arc._arcEp.DistSquare2D(cp);
        double dis = d1 < d2 ?  std::sqrt(d1) :  std::sqrt(d2);
        return std::abs(dis - arc._arcR - R);
    }
    static double ArcDistance(const ofts::DefArc &arc1,
                              const ofts::DefArc &arc2)
    {
        bool isInt = false;
        numst::IntParam param;
        param._calParalLine = true;
        param._calParalCir = true;
        isInt = IsCurveInt2D::IsArcIntersect(arc1,arc2,param);
        if(isInt){return 0;}
        const auto& sp1 = arc1._arcSp;
        const auto& ep1 = arc1._arcEp;
        const auto& cp1 = arc1._arcCp;
        const auto& sp2 = arc2._arcSp;
        const auto& ep2 = arc2._arcEp;
        const auto& cp2 = arc2._arcCp;
        ofts::GeomIntRes iRes1,iRes2;
        CurveInt2D::GetLineArcInt(cp1,cp2,arc1,param,iRes1);
        CurveInt2D::GetLineArcInt(cp1,cp2,arc2,param,iRes2);
        double minNoEndDis;
        if(iRes1.IsInt() && iRes2.IsInt()){
            if(iRes1.Num() == 1){
                if(iRes2.Num() == 1){
                    minNoEndDis = iRes1.P1().Distance2D(iRes2.P1());
                    double ccDis = cp1.Distance2D(cp2);
                    bool isExiInt1_ = false,isExiInt2_ = false;
                    isExiInt1_ = IsCurveInt2D::IsSegmentArcInt(cp1,cp2,arc1,param);
                    isExiInt2_ = IsCurveInt2D::IsSegmentArcInt(cp1,cp2,arc2,param);
                    if(isExiInt1_ && isExiInt2_ && (ccDis + PreErr5_10) >
                            (arc1._arcR + arc2._arcR)){
                        return std::abs(ccDis - (arc1._arcR + arc2._arcR));
                    }
                }else{
                    double d1 = iRes1.P1().DistSquare2D(iRes2.P1());
                    double d2 = iRes1.P1().DistSquare2D(iRes2.P2());
                    minNoEndDis = d1 < d2 ? std::sqrt(d1) : std::sqrt(d2);
                }
            }else{
                double minD1,minD2;
                if(iRes2._intNum == 1){
                    minD1 = iRes1.P1().DistSquare2D(iRes2.P1());
                    minD2 = iRes1.P2().DistSquare2D(iRes2.P1());
                }else{
                    double d1 = iRes1.P1().DistSquare2D(iRes2.P1());
                    double d2 = iRes1.P1().DistSquare2D(iRes2.P2());
                    double d3 = iRes1.P2().DistSquare2D(iRes2.P1());
                    double d4 = iRes1.P2().DistSquare2D(iRes2.P2());
                    minD1 = d1 < d2 ? d1 : d2;
                    minD2 = d3 < d4 ? d3 : d4;
                }
                minNoEndDis = minD1 < minD2 ? std::sqrt(minD1) : std::sqrt(minD2);
            }
        }
        double sp1ArcD = PointToArcDist(sp1,arc2);
        double ep1ArcD = PointToArcDist(ep1,arc2);
        double sp2ArcD = PointToArcDist(sp2,arc1);
        double ep2ArcD = PointToArcDist(ep2,arc1);
        double minD1 = sp1ArcD < ep1ArcD ? sp1ArcD : ep1ArcD;
        double minD2 = sp2ArcD < ep2ArcD ? sp2ArcD : ep2ArcD;
        double dist = minD1 < minD2 ? minD1 : minD2;
        if(iRes1.IsInt() && iRes2.IsInt() && dist > minNoEndDis){
            dist = minNoEndDis;
        }
        return dist;
    }
    static double PointToCurveDist(const ofts::Point& p,
                                   const ofts::DefElem& c)
    {
        switch (c._type) {
        case Ofte::LINETYPE:
            return PointToSegmentDist(p,c._segment._segSp,
                                      c._segment._segEp);
        case Ofte::ARCTYPE:
            return PointToArcDist(p,c._arc);
        case Ofte::CIRCLETYPE:
            return PointToCircleDist(p,c._circle._cirCp,
                                     c._circle._cirR);
        default:break;
        }
        return 0;
    }
    static double CurveDistance(const ofts::DefElem &c1,
                                const ofts::DefElem &c2)
    {
        switch (c1._type) {
        case LINETYPE:
            switch (c2._type) {
            case LINETYPE:
                return SegmentDistance(c1._segment._segSp,
                                       c1._segment._segEp,
                                       c2._segment._segSp,
                                       c2._segment._segEp);
            case ARCTYPE:
                return SegmentArcDist(c1._segment._segSp,
                                      c1._segment._segEp,
                                      c2._arc);
            default:break;}
            break;
        case ARCTYPE:
            switch (c2._type) {
            case LINETYPE:
                return SegmentArcDist(c2._segment._segSp,
                                      c2._segment._segEp,
                                      c1._arc);
            case ARCTYPE:
                return ArcDistance(c1._arc,c2._arc);
            default:break;}
        default:break;
        }
        return 0;
    }
    double PointToLoopDist(const ofts::DefLoop &loop,const ofts::Point& p);
    double CurveToLoopDist(const ofts::DefLoop& loop,const ofts::DefElem& curve);
    double LoopDistance(const ofts::DefLoop& loop1,const ofts::DefLoop& loop2);
};
#endif // CURVEDIST2D_H
