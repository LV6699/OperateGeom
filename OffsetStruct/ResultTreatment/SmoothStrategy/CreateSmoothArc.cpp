#include "CreateSmoothArc.h"
void CreateSmoothArc::CreateSegmentSegmentSmoothArc(const Point& l1Sp,
                                                    const Point& l1Ep,
                                                    const Point& l2Sp,
                                                    const Point& l2Ep,
                                                    double radius,
                                                    double strechLimit,
                                                    bool &isExiSmooth,
                                                    DefArc& smoothArc)
{
    isExiSmooth = false;
    if(TupleQuality().IsLineLineParallel(l1Sp,l1Ep,l2Sp,l2Ep,PreErr5_10)){
        return;
    }
    Point centerPt;
    bool isExiCenter = false;
    GeomCalculate gocal;
    SmoothArcCenter amoarc;
    amoarc.SegmentSegmentSmoothArcCenter(l1Sp,l1Ep,l2Sp,l2Ep,radius,
                                         isExiCenter,centerPt);
    if(!isExiCenter){
        return;
    }
    if(gocal.TwoPointDistance(l1Ep,centerPt) > strechLimit){
        return;
    }
    ///RotDir arcRot;
    ///crtArc_go->TriangleRotDirection(l1Sp,l1Ep,l2Ep,arcRot);
    ///arc._arcRot = arcRot;
    gocal.PointOnLineProjection(centerPt,l1Sp,l1Ep,
                                smoothArc._arcSp);
    gocal.PointOnLineProjection(centerPt,l2Sp,l2Ep,
                                smoothArc._arcEp);
    bool isSamePnt = false;
    gocal.IsIdenticalPoint(smoothArc.ArcStartPt(),smoothArc.ArcEndPt(),
                           SmoothExtrDistance,isSamePnt);
    if(isSamePnt){
        return;
    }
    smoothArc._arcCp = centerPt;
    smoothArc._arcR = radius;
    ConnectArcRotDir arcrot;
    arcrot.ArcRotDirBaseOriginLine(l1Sp,smoothArc.ArcStartPt(),true,
                                smoothArc,smoothArc._arcRot);
    isExiSmooth = true;
}
void CreateSmoothArc::CreateSegmentArcSmoothArc(const Point& lSp,
                                                const Point& lEp,
                                                const DefArc& arc,
                                                double radius,
                                                double strechLimit,
                                                bool isLineFont,
                                                bool &isExiSmooth,
                                                DefArc &smoothArc)
{
    isExiSmooth = false;
    Point center;
    bool isExiCenter = false;    SmoothArcCenter arccen;
    arccen.SegmentArcSmoothArcCenter(lSp,lEp,arc,isLineFont,
                                     radius,isExiCenter,center);
    if(!isExiCenter){
        return;
    }
    Point lastPt = lEp;
    if(!isLineFont){
        lastPt = arc.ArcEndPt();
    }
    GeomCalculate gocal;
    if(gocal.TwoPointDistance(lastPt,center) > strechLimit){
        return;
    }
    ///计算原始圆弧所在圆与连接 intPnt与圆弧圆心的段的交点
    Point cirIntPnt;
    GeomIntRes intRes;
    GeomIntersect goint;
    GeometryLogic golg;
    ConnectArcRotDir arcrot;
    goint.CalculateLineCircleInt(arc._arcCp,center,
                                 arc._arcCp,arc._arcR,
                                 PreErr_10,PreErr_10,intRes);
    if(!intRes._isExistInt){
        return;
    }
    ///此处不需要考虑目标点在射线 (arc._arcCp,center) 正方向上,
    ///因为arc._arcR > radius.
    golg.ClosetPointFromTwoPoint(center,intRes._intPnt1,
                                 intRes._intPnt2,cirIntPnt);
    Point sigIntPnt;
    gocal.PointOnLineProjection(center,lSp,lEp,sigIntPnt);
    bool isSamePnt = false;
    gocal.IsIdenticalPoint(cirIntPnt,sigIntPnt,
                           SmoothExtrDistance,isSamePnt);
    if(isSamePnt){
        return;
    }
    if(isLineFont){
        smoothArc._arcSp = sigIntPnt;
        smoothArc._arcEp = cirIntPnt;
    }
    else{
        smoothArc._arcSp = cirIntPnt;
        smoothArc._arcEp = sigIntPnt;
    }
    smoothArc._arcCp = center;
    bool isNex = false;
    Point temSp = lSp,temEp = smoothArc._arcSp;
    if(isLineFont){
        isNex = true;
    }
    else{
        temSp = smoothArc._arcEp;
        temEp = lEp;
    }
    arcrot.ArcRotDirBaseOriginLine(temSp,temEp,isNex,smoothArc,
                                smoothArc._arcRot);
    double R1,R2;
    gocal.TwoPointDistance(center,sigIntPnt,R1);
    gocal.TwoPointDistance(center,cirIntPnt,R2);
    smoothArc._arcR = R1 <= R2 ? R1 : R2;    isExiSmooth = true;
}
void CreateSmoothArc::CreateArcArcSmoothArc(const DefArc& arc1,
                                            const DefArc& arc2,
                                            double radius,
                                            double strechLimit,
                                            bool& isExiSmooth,
                                            DefArc& smoothArc)
{
    isExiSmooth = false;
    Point center;
    bool isExiCenter = false;
    SmoothArcCenter arccen;
    arccen.ArcArcSmoothArcCenter(arc1,arc2,radius,isExiCenter,center);
    if(!isExiCenter){
        return;
    }
    ///计算原始圆弧arc1所在圆与连接centerPnt与圆弧圆心的段的交点
    Point cirIntPnt1;
    GeometryLogic golg;
    GeomCalculate gocal;
    GeomIntersect goInt;
    ConnectArcRotDir arcrot;
    GeomIntRes intRes1,intRes2;
    goInt.CalculateLineCircleInt(arc1._arcCp,center,
                                 arc1._arcCp,arc1._arcR,
                                 PreErr_10,PreErr_10,intRes1);
    if(!intRes1._isExistInt){
        return;
    }
    if(gocal.TwoPointDistance(arc1.ArcEndPt(),center) > strechLimit){
        return;
    }
    golg.ClosetPointFromTwoPoint(center,intRes1._intPnt1,
                                 intRes1._intPnt2,cirIntPnt1);
    ///计算原始圆弧arc2所在圆与连接centerPnt与圆弧圆心的段的交点
    Point cirIntPnt2;
    goInt.CalculateLineCircleInt(arc2._arcCp,center,
                                 arc2._arcCp,arc2._arcR,
                                 PreErr_10,PreErr_10,intRes2);
    if(!intRes2._isExistInt){
        return;
    }
    golg.ClosetPointFromTwoPoint(center,intRes2._intPnt1,
                                 intRes2._intPnt2,cirIntPnt2);
    bool isSamePnt = false;
    gocal.IsIdenticalPoint(cirIntPnt1,cirIntPnt2,
                           SmoothExtrDistance,isSamePnt);
    if(isSamePnt){
        return;
    }
    smoothArc._arcSp = cirIntPnt1;
    smoothArc._arcEp = cirIntPnt2;
    smoothArc._arcCp = center;
    double R1,R2;
    gocal.TwoPointDistance(center,cirIntPnt1,R1);
    gocal.TwoPointDistance(center,cirIntPnt2,R2);
    smoothArc._arcR = R1 <= R2 ? R1 : R2;    DefArc temArc = arc1;
    temArc._arcEp = cirIntPnt1;
    arcrot.ArcRotDirBaseOriginArc(temArc,true,smoothArc,smoothArc._arcRot);
    /*crtArc_go->TriangleRotDirection(newArc._arcCp,newArc._arcSp,
                                 newArc._arcEp,newArc._arcRot);*/
    isExiSmooth = true;
}