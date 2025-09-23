#include "GeomIntSolve.h"
#if DebugModel
#pragma optimize("", off)
#endif
void GeomIntSolve::JudgeArcRadius(DefArc& arc)
{
    if(!(MathCalculate().IsLegalValue(arc._arcR)) ||
            (arc._arcR <= PreErr5_5)){
        GeomCalculate().InitialArcRadius(arc);
    }
}
void GeomIntSolve::SetHorVerLineIntPnt(const Point& lSp,
                                       const Point& lEp,
                                       Point& P)
{
    TupleQuality tup;
    if(tup.IsVerticalLine(lSp,lEp,PreErr5_12))
        P.SetX(lSp.X());
    else if(tup.IsHorizontalLine(lSp,lEp,PreErr5_12))
        P.SetY(lSp.Y());
}
void GeomIntSolve::ExicuteEndpointOnTuple(bool isSpOn,
                                          bool isEpOn,
                                          const Point &sPnt,
                                          const Point &ePnt,
                                          bool& isEndpntOn,
                                          Point& onEndpnt,
                                          GeomIntRes &intRes)
{
    if(isSpOn && isSpOn){
        intRes._intPnt1 = sPnt;
        intRes._intPnt2 = ePnt;
        intRes._intNum = 2;
        intRes._isExistInt = true;
        intRes._isExiGeoInt = true;
        return;
    }
    if(isSpOn){
        isEndpntOn = true;
        onEndpnt = sPnt;
    }
    if(isEpOn){
        isEndpntOn = true;
        onEndpnt = ePnt;
    }
}
void GeomIntSolve::HandleEndpointOnTuple(bool isEndpntOn,
                                         const Point &onEndpnt,
                                         const GeomIntRes& intRes_,
                                         GeomIntRes& intRes)
{
    if(!isEndpntOn)
        return;
    if(intRes._intNum == 0 || intRes._intNum == 1){
        intRes._intPnt1 = onEndpnt;
        intRes._intPnt2 = onEndpnt;
        intRes._intNum = 1;
    }
    else{        GeometryLogic golg;
        intRes._intPnt1 = onEndpnt;
        golg.AnotherFromTwoPoint(intRes_._intPnt1,intRes_._intPnt2,
                                 onEndpnt,intRes._intPnt2);
    }
}
void GeomIntSolve::IntersectPointNotOnArc(const DefArc &arc,
                                          Point &intPnt)
{
    double startAgl,endAgl;
    GeomCalculate gocal;
    gocal.ArcEndpointAngle(arc._arcSp,arc._arcEp,
                           arc._arcCp,arc._arcRot,
                           startAgl,endAgl);
    double theta = atan2((intPnt._y - arc._arcCp._y),
                         (intPnt._x - arc._arcCp._x))
            * 180.0 / M_PI;
    theta = (theta < 0) ? (theta += 360) : theta;
    bool isOnArc = false;
    gocal.AngleIsBetweenAngles(startAgl,endAgl,arc._arcRot,
                               theta,0,isOnArc);
    if(!isOnArc){
        double dis1,dis2;
        gocal.TwoPointDistance(intPnt,arc._arcSp,dis1);
        gocal.TwoPointDistance(intPnt,arc._arcEp,dis2);
        //gocal.CalPntToArcExtremePntDis(arc,intPnt,true,dis1);
        //gocal.CalPntToArcExtremePntDis(arc,intPnt,false,dis2);
        intPnt = dis1 <= dis2 ? arc._arcSp : arc._arcEp;
    }
}