#include <tbb/tbb.h>
#include "GeomDistance.h"
#if DebugModel
#pragma optimize("", off)
#endif
///需要保证点在圆弧上 ,返回单位是角度.
void GeomDistance::PointToArcEndpointDistance(DefArc arc,
                                              const Point& P,
                                              bool isCloseSp,
                                              double& dis)
{
    double theta = std::atan2((P._y - arc._arcCp._y),
                              (P._x - arc._arcCp._x));
    theta = theta * 180.0 / M_PI;
    if(theta < 0){
        theta = theta + 360;
    }
    bool isOnArc = false;
    double startAgl_,endAgl_;
    GeomCalculate gocal;
    gocal.ArcEndpointAngle(arc,startAgl_,endAgl_);
    gocal.AngleIsBetweenAngles(startAgl_,endAgl_,arc._arcRot,
                               theta,Base_AngBetweenAng_Err,isOnArc);
    ///如果点在圆弧上,计算目标点至圆弧端点最近距离 ,且不考虑旋向,
    ///否则需要按照旋向计算目标点到圆弧端点距离 .
    if(isOnArc){
        if(isCloseSp){
            arc._arcEp = P;
        }
        else{
            arc._arcSp = P;
        }
    }
    else{
        Point arcSp_ = arc._arcSp;
        arc._arcSp = P;
        if(isCloseSp){
            arc._arcEp = arcSp_;
        }
    }
    double startAgl,endAgl,aglAbs;
    gocal.ArcEndpointAngle(arc._arcSp,arc._arcEp,
                           arc._arcCp,arc._arcRot,
                           startAgl,endAgl);
    gocal.TwoAngleIncludedAngle(startAgl,endAgl,
                                arc._arcRot,aglAbs);
    dis = aglAbs;
    if(dis == 360){
        dis = 0;
    }
}
void GeomDistance::PointToEleEndpointDistance(const DefElem &ele,
                                              const Point& P,
                                              bool isCloseSp,
                                              double& dis)
{
    GeomCalculate gocal;
    if(ele._elemType == LINETYPE){
        if(isCloseSp){
            gocal.TwoPointDistance(P,ele._segment._segSp,dis);
        }        else{
            gocal.TwoPointDistance(P,ele._segment._segEp,dis);
        }
    }
    else if(ele._elemType == ARCTYPE){
        PointToArcEndpointDistance(ele._arc,P,isCloseSp,dis);
    }
}
void GeomDistance::PointToSegmentDistance(const Point &P,
                                          const Point &lSp,
                                          const Point &lEp,
                                          double &distance)
{
    Point proPt;
    GeomCalculate gocal;PointOnElement pton;
    gocal.PointOnLineProjection(P,lSp,lEp,proPt);
    bool isOnSeg = false;
    ///gocal.IsPointOnSegment(lSp,lEp,proPt,OnSeg_Err,false,isOnSeg);
    pton.IsPointOnSegment(lSp,lEp,proPt,PreErr_10,
                          PreErr_10,false,isOnSeg);
    if(isOnSeg){
        gocal.PointToLineDistance(lSp,lEp,P,distance);
    }
    else{
        double dis1,dis2;
        gocal.TwoPointDistance(lSp,P,dis1);
        gocal.TwoPointDistance(lEp,P,dis2);
        distance = dis1 <= dis2 ? dis1 : dis2;
    }
}
double GeomDistance::PointToSegMaxDistance(const Point &P,
                                           const Point &lSp,
                                           const Point &lEp)
{
    double dis1 = P.Distance2D(lSp),dis2 = P.Distance2D(lEp);
    return dis1 >= dis2 ? dis1 : dis2;
}
double GeomDistance::PointToArcMaxDistance(const Point &P,
                                           const DefArc &arc)
{
    vector<Point> vPoint;
    GeomCalculate().DiscreteArcToPoint(arc,PreErr_80_,vPoint);
    double distance = 0;
    for (int i = 0; i < vPoint.size(); ++i) {
        double dis = P.Distance2D(vPoint[i]);
        if(dis > distance)
            distance = dis;
    }
    return distance;
}
double GeomDistance::PointToCircleMaxDistance(const Point &P,
                                              const DefCircle &circle)
{
    double dis = P.Distance2D(circle._cirCp);
    return dis + circle.CircleRadius();
}
void GeomDistance::PointToArcDistance(const Point &P,                                      const DefArc &arc,
                                      double &distance)
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
        double toArcDis;
        gocal.TwoPointDistance(P,arc._arcCp,toArcDis);
        distance = std::abs(toArcDis - arc._arcR);
    }
    else {
        double dis1 = P.Distance2D(arc._arcSp),
                dis2 = P.Distance2D(arc._arcEp);
        distance = dis1 <= dis2 ? dis1 : dis2;
    }
}
void GeomDistance::PointToCircleDistance(const Point &P,
                                         const Point &cp,
                                         double radius,
                                         double &distance)
{
    double distance_ = P.Distance2D(cp);
    distance = std::abs(distance_ - radius);
}
void GeomDistance::ElementEndpointDistance(const Point &ele1Sp,
                                           const Point &ele1Ep,
                                           const Point &ele2Sp,
                                           const Point &ele2Ep,
                                           double &distance)
{
    double ssDis = ele1Sp.Distance2D(ele2Sp),
            seDis = ele1Sp.Distance2D(ele2Ep),
            esDis = ele1Ep.Distance2D(ele2Sp),
            eeDis = ele1Ep.Distance2D(ele2Ep);
    double dis1 = ssDis <= seDis ? ssDis : seDis;
    double dis2 = esDis <= eeDis ? esDis : eeDis;
    distance = dis1 <= dis2 ? dis1 : dis2;
}
void GeomDistance::SegmentSegmentDistance(const Point &l1Sp,
                                          const Point &l1Ep,
                                          const Point &l2Sp,
                                          const Point &l2Ep,
                                          double &distance)
{
    double extrDis;
    ElementEndpointDistance(l1Sp,l1Ep,l2Sp,l2Ep,extrDis);
    GeomCalculate gocal;PointOnElement onele;
    ///vector<double>vDistance;
    ///vDistance.push_back(extrDis);    Point l1SpPro,l1EpPro,l2SpPro,l2EpPro;
    gocal.PointOnLineProjection(l1Sp,l2Sp,l2Ep,l1SpPro);
    gocal.PointOnLineProjection(l1Ep,l2Sp,l2Ep,l1EpPro);
    gocal.PointOnLineProjection(l2Sp,l1Sp,l1Ep,l2SpPro);
    gocal.PointOnLineProjection(l2Ep,l1Sp,l1Ep,l2EpPro);
    bool isOn1 = false,isOn2 = false,isOn3 = false,isOn4 = false;
    onele.IsPointOnSegment(l2Sp,l2Ep,l1SpPro,OnSeg_Err,OnSeg_Err,false,isOn1);
    onele.IsPointOnSegment(l2Sp,l2Ep,l1EpPro,OnSeg_Err,OnSeg_Err,false,isOn2);
    onele.IsPointOnSegment(l1Sp,l1Ep,l2SpPro,OnSeg_Err,OnSeg_Err,false,isOn3);
    onele.IsPointOnSegment(l1Sp,l1Ep,l2EpPro,OnSeg_Err,OnSeg_Err,false,isOn4);
    distance = extrDis;
    if(isOn1){
        double dis1 = l1Sp.Distance2D(l1SpPro);
        if(dis1 < distance)
            distance = dis1;
        ///vDistance.push_back(dis1);
    }
    if(isOn2){
        double dis2 = l1Ep.Distance2D(l1EpPro);
        if(dis2 < distance)
            distance = dis2;
        ///vDistance.push_back(dis2);
    }
    if(isOn3){
        double dis3 = l2Sp.Distance2D(l2SpPro);
        ///vDistance.push_back(dis3);
        if(dis3 < distance)
            distance = dis3;
    }
    if(isOn4){
        double dis4 = l2Ep.Distance2D(l2EpPro);
        ///vDistance.push_back(dis4);
        if(dis4 < distance)
            distance = dis4;
    }
    ///distance = *min_element(vDistance.begin(),vDistance.end());
}
void GeomDistance::SegmentArcDistance(const Point &lSp,
                                      const Point &lEp,
                                      const DefArc &arc,
                                      double &distance)
{
    ///CreateCurveSolve().ArcGeomInfo(arc);
    bool isExiInt = false;
    CheckGeomInt chInt;GeomCalculate gocal;PointOnElement pton;
    chInt.IsSegmentArcIntersect(lSp,lEp,arc,PreErr5_10,isExiInt);
    if(isExiInt){
        distance = 0;
        return;
    }
    Point proPt;
    Point arcSp = arc._arcSp,arcEp = arc._arcEp,arcCp = arc._arcCp;
    gocal.PointOnLineProjection(arcCp,lSp,lEp,proPt);
    bool isOnSeg = false;
    pton.IsPointOnSegment(lSp,lEp,proPt,PreErr5_10,
                          PreErr5_10,false,isOnSeg);
    if(isOnSeg){
        chInt.IsSegmentArcIntersect(arcCp,proPt,arc,PreErr5_10,isExiInt);
        if(isExiInt){
            double dis;            gocal.PointToLineDistance(lSp,lEp,arcCp,dis);
            distance = std::abs(dis - arc._arcR);
            return;
        }
    }
    double aSpSigDis,aEpSigDis,sPArcDis,ePArcDis;
    PointToSegmentDistance(arcSp,lSp,lEp,aSpSigDis);
    PointToSegmentDistance(arcEp,lSp,lEp,aEpSigDis);
    PointToArcDistance(lSp,arc,sPArcDis);
    PointToArcDistance(lEp,arc,ePArcDis);
    double aExtSigDis = aSpSigDis <= aEpSigDis ? aSpSigDis : aEpSigDis;
    double sigExtADis = sPArcDis <= ePArcDis ? sPArcDis : ePArcDis;
    distance = aExtSigDis <= sigExtADis ? aExtSigDis : sigExtADis;
}
void GeomDistance::SegmentCircleDistance(const Point &lSp,
                                         const Point &lEp,
                                         const Point &cp,
                                         double R,
                                         double &distance)
{
    Point proPt;
    GeomCalculate gocal;
    gocal.PointOnLineProjection(cp,lSp,lEp,proPt);
    bool isOnSeg = false;
    PointOnElement().IsPointOnSegment(lSp,lEp,proPt,OnSeg_Err,
                                      OnSeg_Err,false,isOnSeg);
    if(isOnSeg){
        double dis;
        gocal.PointToLineDistance(lSp,lEp,proPt,dis);
        distance = std::abs(dis - R);
        return;
    }
    double distance1 = cp.Distance2D(lSp),distance2 = cp.Distance2D(lEp);
    double dis = distance1 <= distance2 ? distance1 : distance2;
    distance = std::abs(dis - R);
}
void GeomDistance::ArcExtrToArcDistance(const DefArc &arc1,
                                        const DefArc &arc2,
                                        double &distance)
{
    double dis1,dis2,dis3,dis4;
    PointToArcDistance(arc1._arcSp,arc2,dis1);
    PointToArcDistance(arc1._arcEp,arc2,dis2);
    PointToArcDistance(arc2._arcSp,arc1,dis3);
    PointToArcDistance(arc2._arcEp,arc1,dis4);
    double minDis1 = dis1 <= dis2 ? dis1 : dis2;
    double minDis2 = dis3 <= dis4 ? dis3 : dis4;
    distance = minDis1 <= minDis2 ? minDis1 : minDis2;
}
void GeomDistance::ArcCircleDistance(const DefArc &arc,
                                     const Point &cp,
                                     double R,
                                     double &distance)
{
    GeomCalculate gocal;
    CheckGeomInt cheint;
    Point arcCp = arc._arcCp;    bool isExiIn = false;
    cheint.IsLineArcIntersect(arcCp,cp,arc,PreErr_10,isExiIn);
    if(isExiIn){
        double dis = arcCp.Distance2D(cp);
        distance = std::abs(dis - arc._arcR - R);
        return;
    }
    double dis1 = arc._arcSp.Distance2D(cp),
            dis2 = arc._arcEp.Distance2D(cp);
    double dis = dis1 <= dis2 ? dis1 : dis2;
    distance = std::abs(dis - arc._arcR - R);
}
void GeomDistance::ArcArcDistance(const DefArc &arc1,
                                  const DefArc &arc2,
                                  double &distance)
{
    bool isCCExiInt = false;
    CheckGeomInt chInt;
    chInt.IsArcArcIntersect(arc1,arc2,PreErr_10,isCCExiInt);
    if(isCCExiInt){
        distance = 0;
        return;
    }
    Point arcSp1 = arc1._arcSp,arcEp1 = arc1._arcEp,
            arcCp1 = arc1._arcCp,arcSp2 = arc2._arcSp,
            arcEp2 = arc2._arcEp,arcCp2 = arc2._arcCp;
    GeomIntersect goInt;
    GeomIntRes intRes1,intRes2;
    goInt.CalculateLineArcInt(arcCp1,arcCp2,arc1,PreErr_10,
                              PreErr_10,PreErr_10,intRes1);
    goInt.CalculateLineArcInt(arcCp1,arcCp2,arc2,PreErr_10,
                              PreErr_10,PreErr_10,intRes2);
    double minNoExtDis;
    if(intRes1._isExistInt && intRes2._isExistInt){
        if(intRes1._intNum == 1){
            if(intRes2._intNum == 1){
                minNoExtDis = intRes1._intPnt1.Distance2D(intRes2._intPnt1);
                double ccDis = arcCp1.Distance2D(arcCp2);
                bool isExiInt1_ = false,isExiInt2_ = false;
                chInt.IsSegmentArcIntersect(arcCp1,arcCp2,arc1,
                                            PreErr_10,isExiInt1_);
                chInt.IsSegmentArcIntersect(arcCp1,arcCp2,arc2,
                                            PreErr_10,isExiInt2_);
                if(isExiInt1_ && isExiInt2_ && (ccDis + PreErr5_10) >
                        (arc1._arcR + arc2._arcR)){
                    distance = std::abs(ccDis - (arc1._arcR +
                                                 arc2._arcR));
                    return;
                }
            }
            else{
                double dis1 = intRes1._intPnt1.Distance2D(intRes2._intPnt1),
                        dis2 = intRes1._intPnt1.Distance2D(intRes2._intPnt2);
                minNoExtDis = dis1 <= dis2 ? dis1 : dis2;
            }
        }
        else{
            double minDis1,minDis2;
            if(intRes2._intNum == 1){
                minDis1 = intRes1._intPnt1.Distance2D(intRes2._intPnt1);
                minDis2 = intRes1._intPnt2.Distance2D(intRes2._intPnt1);
            }            else{
                double dis1 = intRes1._intPnt1.Distance2D(intRes2._intPnt1),
                        dis2 = intRes1._intPnt1.Distance2D(intRes2._intPnt2),
                        dis3 = intRes1._intPnt2.Distance2D(intRes2._intPnt1),
                        dis4 = intRes1._intPnt2.Distance2D(intRes2._intPnt2);
                minDis1 = dis1 <= dis2 ? dis1 : dis2;
                minDis2 = dis3 <= dis4 ? dis3 : dis4;
            }
            minNoExtDis = minDis1 <= minDis2 ? minDis1 : minDis2;
        }
    }
    double a1SpArcDis,a1EpArcDis,a2SpArcDis,a2EpArcDis;
    PointToArcDistance(arcSp1,arc2,a1SpArcDis);
    PointToArcDistance(arcEp1,arc2,a1EpArcDis);
    PointToArcDistance(arcSp2,arc1,a2SpArcDis);
    PointToArcDistance(arcEp2,arc1,a2EpArcDis);
    double minDist1 = a1SpArcDis <= a1EpArcDis ? a1SpArcDis : a1EpArcDis;
    double minDist2 = a2SpArcDis <= a2EpArcDis ? a2SpArcDis : a2EpArcDis;
    distance = minDist1 <= minDist2 ? minDist1 : minDist2;
    if(intRes1._isExistInt && intRes2._isExistInt){
        distance = distance <= minNoExtDis ? distance : minNoExtDis;
    }
}
void GeomDistance::PointToElementDistance(const Point& P,
                                          const DefElem& ele,
                                          double& distance)
{
    if(ele._elemType == LINETYPE){
        PointToSegmentDistance(P,ele._segment._segSp,
                               ele._segment._segEp,distance);
    }
    else if(ele._elemType == ARCTYPE){
        PointToArcDistance(P,ele._arc,distance);
    }
    else if(ele._elemType == CIRCLETYPE){
        PointToCircleDistance(P,ele._circle._cirCp,
                              ele._circle._cirR,distance);
    }
}
void GeomDistance::ElementToElementDistance(const DefElem &ele1,
                                            const DefElem &ele2,
                                            double &distance)
{
    switch (ele1.ElemeType()) {
    case LINETYPE:
        switch (ele2.ElemeType()) {
        case LINETYPE:
            SegmentSegmentDistance(ele1._segment._segSp,
                                   ele1._segment._segEp,
                                   ele2._segment._segSp,
                                   ele2._segment._segEp,distance);
            break;
        case ARCTYPE:
            SegmentArcDistance(ele1._segment._segSp,
                               ele1._segment._segEp,
                               ele2._arc,distance);
            break;
        default:
            break;
        }        break;
    case ARCTYPE:
        switch (ele2.ElemeType()) {
        case LINETYPE:
            SegmentArcDistance(ele2._segment._segSp,
                               ele2._segment._segEp,
                               ele1._arc,distance);
            break;
        case ARCTYPE:
            ArcArcDistance(ele1._arc,ele2._arc,
                           distance);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
void GeomDistance::PointToElemMaxDistance(const Point &P,
                                          const DefElem &ele,
                                          double &distance)
{
    if(ele._elemType == LINETYPE)
        distance = PointToSegMaxDistance(P,ele._segment._segSp,
                                         ele._segment._segEp);
    else if(ele._elemType == ARCTYPE)
        distance = PointToArcMaxDistance(P,ele.ElemArc());
    else if(ele._elemType == CIRCLETYPE)
        distance = PointToCircleMaxDistance(P,ele.ElemCircle());
    else
        distance = 0;
}
void GeomDistance::PointToLoopMaxDistance(const DefLoop &loop,
                                          const Point& P,
                                          double& distance)
{
    distance = 0;
    for (int i = 0; i < loop.ElemNum(); ++i) {
        double distance_;
        PointToElemMaxDistance(P,loop.IndexElem(i),distance_);
        if(distance_ > distance)
            distance = distance_;
    }
}
void GeomDistance::PointToLoopDistance(const DefLoop &loop,
                                       const Point &P,
                                       double& distance)
{
    if(loop.IsEmpty()){
        distance = 0;
        return;
    }
#if 0
    vector<double>vDistance;
    for (int i = 0; i < loop.ElemNum(); ++i) {
        double dis;
        PointToElementDistance(P,loop.IndexElem(i),dis);
        vDistance.push_back(dis);    }
    distance = *std::min_element(vDistance.begin(),vDistance.end());
#else
    distance = 10e8;
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop.ElemNum()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            double distance_;
            PointToElementDistance(P,loop.IndexElem(i),distance_);
            if(distance_ < distance){
                distance = distance_;
            }
        }
    });
#endif
}
void GeomDistance::ElementToLoopDistance(const DefLoop& loop,
                                         const DefElem& ele,
                                         double& distance)
{
    if(loop.IsEmpty()){
        distance = 0;
        return;
    }
#if 0
    ElementToElementDistance(ele,loop.m_vElem[0],distance);
    for (int i = 1; i < loop.m_vElem.size(); ++i) {
        double curDistance;
        ElementToElementDistance(ele,loop.m_vElem[i],curDistance);
        if(curDistance < distance){
            distance = curDistance;
        }
    }
#else
    distance = 10e8;
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop.ElemNum()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            double distance_;
            ElementToElementDistance(ele,loop.IndexElem(i),distance_);
            if(distance_ < distance){
                distance = distance_;
            }
        }
    });
#endif
}
void GeomDistance::LoopToLoopDistance(const DefLoop& loop1,
                                      const DefLoop& loop2,
                                      double& distance)
{
    if(loop1.IsEmpty() || loop2.IsEmpty()){
        distance = 0;
        return;
    }
    ElementToElementDistance(loop1.m_vElem[0],
            loop2.m_vElem[0],distance);
#if 0    for (int i = 1; i < loop1.ElemNum(); ++i) {
        DefElem ele = loop1.IndexElem(i);
        double distance_;
        ElementToLoopDistance(loop2,ele,distance_);
        if(distance_ < distance){
            distance = distance_;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop1.ElemNum()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            DefElem ele = loop1.IndexElem(i);
            double distance_;
            ElementToLoopDistance(loop2,ele,distance_);
            if(distance_ < distance){
                distance = distance_;
            }
        }
    });
#endif
}