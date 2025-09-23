#include "ClosestPoint.h"
#if DebugModel
#pragma optimize("", off)
#endif
void ClosestPoint::PointToSegmentClosestPoint(const Point &P,
                                              const Point &lSp,
                                              const Point &lEp,
                                              Point& cloPt)
{
    Point proPt;
    GeomCalculate gocal;
    gocal.PointOnLineProjection(P,lSp,lEp,proPt);
    bool isOnSeg = false;
    PointOnElement().IsPointOnSegment(lSp,lEp,proPt,
                                      PreErr5_10,PreErr5_10,
                                      false,isOnSeg);
    if(isOnSeg){
        cloPt = proPt;
        cloPt.SetZ(proPt.Z());
    }else{
        double dis1 = P.DistSquare2D(lSp),
                dis2 = P.DistSquare2D(lEp);
        cloPt = dis1 <= dis2 ? lSp : lEp;
        double z = dis1 <= dis2 ? lSp.Z() : lEp.Z();
        cloPt.SetZ(z);
    }
}
void ClosestPoint::PointToArcClosestPoint(const Point &P,
                                          const DefArc &arc,
                                          Point& cloPt)
{
    GeomCalculate gocal;
    double startAgl,endAgl;
    gocal.ArcEndpointAngle(arc._arcSp,arc._arcEp,
                               arc._arcCp,arc._arcRot,
                               startAgl,endAgl);
    double theta = atan2((P._y - arc._arcCp._y),
                         (P._x - arc._arcCp._x))
            * 180.0 / M_PI;
    theta = (theta < 0) ? (theta += 360) : theta;
    bool isIn = false;
    gocal.AngleIsBetweenAngles(startAgl,endAgl,arc._arcRot,
                               theta,PreErr_6,isIn);
    if(isIn){
        GeomIntRes intRes;
        GeomIntersect().CalculateLineCircleInt(P,arc.ArcCenterPt(),
                                              arc.ArcCenterPt(),
                                              arc.ArcRadius(),
                                              PreErr5_10,
                                              PreErr5_10,intRes);
        double dis1,dis2;
        gocal.TwoPointDistance(P,intRes._intPnt1,dis1);
        gocal.TwoPointDistance(P,intRes._intPnt2,dis2);
        cloPt = dis1 <= dis2 ? intRes._intPnt1 : intRes._intPnt2;
    }
    else {
        double dis1,dis2;
        gocal.TwoPointDistance(P,arc._arcSp,dis1);
        gocal.TwoPointDistance(P,arc._arcEp,dis2);
        cloPt = dis1 <= dis2 ? arc.ArcStartPt() : arc.ArcEndPt();    }
    cloPt.SetZ(arc.ArcCenterPt().Z());
}
void ClosestPoint::PointToCircleClosestPoint(const Point &P,
                                             const Point &cp,
                                             double R,
                                             Point& cloPt)
{
    GeomCalculate gocal;
    GeomIntRes intRes;
    GeomIntersect().CalculateLineCircleInt(P,cp,cp,R,PreErr5_10,
                                          PreErr5_10,intRes);
    double dis1,dis2;
    gocal.TwoPointDistance(P,intRes._intPnt1,dis1);
    gocal.TwoPointDistance(P,intRes._intPnt2,dis2);
    cloPt = dis1 <= dis2 ? intRes._intPnt1 : intRes._intPnt2;
    cloPt.SetZ(cp.Z());
}
void ClosestPoint::PointToElementClosestPoint(const Point &P,
                                              const DefElem &element,
                                              Point& cloPt)
{
    switch (element._elemType) {
    case LINETYPE:
        PointToSegmentClosestPoint(P,element._segment.SegStartPt(),
                                   element._segment.SegEndPt(),cloPt);
        break;
    case ARCTYPE:
        PointToArcClosestPoint(P,element._arc,cloPt);
        break;
    case CIRCLETYPE:
        PointToCircleClosestPoint(P,element._circle._cirCp,
                                  element._circle._cirR,cloPt);
        break;
    default:
        break;
    }
}
void ClosestPoint::SegToSegClosestPoint(const Point &l1Sp,
                                        const Point &l1Ep,
                                        const Point &l2Sp,
                                        const Point &l2Ep,
                                        Point& cloPt1,
                                        Point& cloPt2)
{
    GeomCalculate gocal;
    double ssDis,seDis,esDis,eeDis;
    gocal.TwoPointDistance(l1Sp,l2Sp,ssDis);
    gocal.TwoPointDistance(l1Sp,l2Ep,seDis);
    gocal.TwoPointDistance(l1Ep,l2Sp,esDis);
    gocal.TwoPointDistance(l1Ep,l2Ep,eeDis);
    Point P1 = l1Sp,P2 = ssDis <= seDis ? l2Sp : l2Ep;
    Point P3 = l1Ep,P4 = esDis <= eeDis ? l2Sp : l2Ep;
    double endPtDis1 = ssDis <= seDis ? ssDis : seDis;
    double endPtDis2 = esDis <= eeDis ? esDis : eeDis;
    if(endPtDis1 <= endPtDis2){
        cloPt1 = P1;
        cloPt2 = P2;
    }    else{
        cloPt1 = P3;
        cloPt2 = P4;
    }
    Point l1SpPro,l1EpPro,l2SpPro,l2EpPro;
    gocal.PointOnLineProjection(l1Sp,l2Sp,l2Ep,l1SpPro);
    gocal.PointOnLineProjection(l1Ep,l2Sp,l2Ep,l1EpPro);
    gocal.PointOnLineProjection(l2Sp,l1Sp,l1Ep,l2SpPro);
    gocal.PointOnLineProjection(l2Ep,l1Sp,l1Ep,l2EpPro);
    PointOnElement onele;
    bool isOn1 = false,isOn2 = false,isOn3 = false,isOn4 = false;
    onele.IsPointOnSegment(l2Sp,l2Ep,l1SpPro,PreErr5_10,
                           PreErr5_10,false,isOn1);
    onele.IsPointOnSegment(l2Sp,l2Ep,l1EpPro,PreErr5_10,
                           PreErr5_10,false,isOn2);
    onele.IsPointOnSegment(l1Sp,l1Ep,l2SpPro,PreErr5_10,
                           PreErr5_10,false,isOn3);
    onele.IsPointOnSegment(l1Sp,l1Ep,l2EpPro,PreErr5_10,
                           PreErr5_10,false,isOn4);
    double dis1,dis2,dis3,dis4;
    double distance = endPtDis1 <= endPtDis2 ? endPtDis1 : endPtDis2;
    if(isOn1){
        gocal.TwoPointDistance(l1Sp,l1SpPro,dis1);
        if(dis1 < distance){
            cloPt1 = l1Sp;
            cloPt2 = l1SpPro;
            distance = dis1;
        }
    }
    if(isOn2){
        gocal.TwoPointDistance(l1Ep,l1EpPro,dis2);
        if(dis2 < distance){
            cloPt1 = l1Ep;
            cloPt2 = l1EpPro;
            distance = dis2;
        }
    }
    if(isOn3){
        gocal.TwoPointDistance(l2Sp,l2SpPro,dis3);
        if(dis3 < distance){
            cloPt1 = l2Sp;
            cloPt2 = l2SpPro;
            distance = dis3;
        }
    }
    if(isOn4){
        gocal.TwoPointDistance(l2Ep,l2EpPro,dis4);
        if(dis4 < distance){
            cloPt1 = l2Ep;
            cloPt2 = l2EpPro;
        }
    }
}
void ClosestPoint::SegmentToArcClosestPoint(const Point &lSp,
                                            const Point &lEp,
                                            const DefArc &arc,
                                            Point &cloPt1,
                                            Point &cloPt2)
{    GeomIntRes intRes;
    GeomIntersect goint;
    goint.CalculateSegmentArcInt(lSp,lEp,arc,PreErr5_10,
                                 PreErr5_10,PreErr5_10,intRes);
    if(intRes._isExistInt){
        cloPt1 = intRes._intPnt1;
        cloPt2 = intRes._intPnt1;
        return;
    }
    Point proPt;
    Point arcSp = arc.ArcStartPt(),arcEp = arc.ArcEndPt(),
            arcCp = arc.ArcCenterPt();
    GeomCalculate gocal;
    PointOnElement onele;
    gocal.PointOnLineProjection(arcCp,lSp,lEp,proPt);
    bool isOnSeg = false;
    onele.IsPointOnSegment(lSp,lEp,proPt,PreErr5_10,
                           PreErr5_10,false,isOnSeg);
    if(isOnSeg){
        GeomIntRes intRes_;
        goint.CalculateLineCircleInt(proPt,arcCp,arcCp,
                                     arc.ArcRadius(),PreErr5_10,
                                     PreErr5_10,intRes_);
        if(intRes_._isExistInt){
            cloPt1 = proPt;
            double dis1,dis2;
            gocal.TwoPointDistance(proPt,intRes._intPnt1,dis1);
            gocal.TwoPointDistance(proPt,intRes._intPnt2,dis2);
            cloPt2 = dis1 <= dis2 ? intRes_._intPnt1 : intRes_._intPnt2;
            return;
        }
    }
    Point toSegPt1,toSegPt2,toArcPt1,toArcPt2;
    PointToSegmentClosestPoint(arcSp,lSp,lEp,toSegPt1);
    PointToSegmentClosestPoint(arcEp,lSp,lEp,toSegPt2);
    PointToArcClosestPoint(lSp,arc,toArcPt1);
    PointToArcClosestPoint(lEp,arc,toArcPt2);
    double dis1,dis2,dis3,dis4,distance;
    gocal.TwoPointDistance(arcSp,toSegPt1,dis1);
    gocal.TwoPointDistance(arcEp,toSegPt2,dis2);
    gocal.TwoPointDistance(lSp,toArcPt1,dis3);
    gocal.TwoPointDistance(lEp,toArcPt2,dis4);
    if(dis1 <= dis2){
        cloPt1 = toSegPt1;
        cloPt2 = arcSp;
        distance = dis1;
    }
    else{
        cloPt1 = toSegPt2;
        cloPt2 = arcEp;
        distance = dis2;
    }
    if(dis3 <= distance){
        cloPt1 = toArcPt1;
        cloPt2 = lSp;
        distance = dis3;
    }
    if(dis4 <= distance){
        cloPt1 = toArcPt2;
        cloPt2 = lEp;    }
}
void ClosestPoint::ArcToArcClosestPoint(const DefArc &arc1,
                                        const DefArc &arc2,
                                        Point &cloPt1,
                                        Point &cloPt2)
{
    GeomIntRes intRes;
    GeomIntersect goint;
    goint.CalculateArcArcInt(arc1,arc2,PreErr5_10,PreErr5_10,
                             PreErr5_10,intRes);
    if(intRes._isExistInt){
        cloPt1 = intRes._intPnt1;
        cloPt2 = intRes._intPnt1;
        return;
    }
    GeomCalculate gocal;
    Point arcSp1 = arc1.ArcStartPt(),arcEp1 = arc1.ArcEndPt(),
            arcCp1 = arc1.ArcCenterPt(),arcSp2 = arc2.ArcStartPt(),
            arcEp2 = arc2.ArcEndPt(),arcCp2 = arc2.ArcCenterPt();
    double ssDis,seDis,esDis,eeDis;
    gocal.TwoPointDistance(arcSp1,arcSp2,ssDis);
    gocal.TwoPointDistance(arcSp1,arcEp2,seDis);
    gocal.TwoPointDistance(arcEp1,arcSp2,esDis);
    gocal.TwoPointDistance(arcEp1,arcEp2,eeDis);
    Point P1 = arcSp1,P2 = ssDis <= seDis ? arcSp2 : arcEp2;
    Point P3 = arcEp1,P4 = esDis <= eeDis ? arcSp2 : arcEp2;
    double endPtDis1 = ssDis <= seDis ? ssDis : seDis;
    double endPtDis2 = esDis <= eeDis ? esDis : eeDis;
    if(endPtDis1 <= endPtDis2){
        cloPt1 = P1;
        cloPt2 = P2;
    }
    else{
        cloPt1 = P3;
        cloPt2 = P4;
    }
}