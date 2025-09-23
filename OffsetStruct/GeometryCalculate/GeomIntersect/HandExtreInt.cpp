#include "HandExtreInt.h"
#if DebugModel
#pragma optimize("", off)
#endif
void HandExtreInt::SegEndpointOnArc(const Point& extrPnt,
                                    const DefArc& arc,
                                    bool isArcSpOn,
                                    bool isArcEpOn,
                                    GeomIntRes &intRes)
{
    GeomCalculate gocal;
    if(isArcSpOn && !isArcEpOn){
        if(gocal.IsIdenticalPoint2D(extrPnt,arc._arcSp,PreErr5_10)){
            intRes._intNum = 1;
            gocal.TwoPointMidpoint(extrPnt,arc._arcSp,intRes._intPnt1);
            gocal.TwoPointMidpoint(extrPnt,arc._arcSp,intRes._intPnt2);
        }
        else{
            intRes._intNum = 2;
            intRes._intPnt1 = extrPnt;
            intRes._intPnt2 = arc._arcSp;
        }
    }
    else if(!isArcSpOn && isArcEpOn){
        if(gocal.IsIdenticalPoint2D(extrPnt,arc._arcEp,PreErr5_10)){
            intRes._intNum = 1;
            gocal.TwoPointMidpoint(extrPnt,arc._arcEp,intRes._intPnt1);
            gocal.TwoPointMidpoint(extrPnt,arc._arcEp,intRes._intPnt2);
        }
        else{
            intRes._intNum = 2;
            intRes._intPnt1 = extrPnt;
            intRes._intPnt2 = arc._arcEp;
        }
    }
    else if(isArcSpOn && isArcEpOn){
        intRes._intNum = 2;
        intRes._intPnt1 = arc._arcSp;
        intRes._intPnt2 = arc._arcEp;
    }
    else{
        intRes._intNum = 1;
        intRes._intPnt1 = extrPnt;
        intRes._intPnt2 = extrPnt;
    }
}
void HandExtreInt::ArcEndpointOnSeg(const Point& extrPnt,
                                    const Point& lSp,
                                    const Point& lEp,
                                    bool isSigSpOn,
                                    bool isSigEpOn,
                                    GeomIntRes &intRes)
{
    GeomCalculate gocal;
    if(isSigSpOn && !isSigEpOn){
        if(gocal.IsIdenticalPoint2D(extrPnt,lSp,PreErr5_10)){
            intRes._intNum = 1;
            gocal.TwoPointMidpoint(extrPnt,lSp,intRes._intPnt1);
            gocal.TwoPointMidpoint(extrPnt,lSp,intRes._intPnt2);        }
        else{
            intRes._intNum = 2;
            intRes._intPnt1 = extrPnt;
            intRes._intPnt2 = lSp;
        }
    }
    if(!isSigSpOn && isSigEpOn){
        if(gocal.IsIdenticalPoint2D(extrPnt,lEp,PreErr5_10)){
            intRes._intNum = 1;
            gocal.TwoPointMidpoint(extrPnt,lEp,intRes._intPnt1);
            gocal.TwoPointMidpoint(extrPnt,lEp,intRes._intPnt2);
        }
        else{
            intRes._intNum = 2;
            intRes._intPnt1 = extrPnt;
            intRes._intPnt2 = lEp;
        }
    }
    else if(isSigSpOn && isSigEpOn){
        intRes._intNum = 2;
        intRes._intPnt1 = lSp;
        intRes._intPnt2 = lEp;
    }
    else{
        intRes._intNum = 1;
        intRes._intPnt1 = extrPnt;
        intRes._intPnt2 = extrPnt;
    }
}
void HandExtreInt::SegArcIntersectEndpoint(const Point& lSp,
                                           const Point& lEp,
                                           DefArc arc,
                                           double endptPre,
                                           GeomIntRes &intRes)
{
    bool isSigSpOn = false,isSigEpOn = false,
            isArcSpOn = false,isArcEpOn = false;
    PointOnElement pton;
    pton.IsPointOnArc(arc,lSp,endptPre,endptPre,true,isSigSpOn);
    pton.IsPointOnArc(arc,lEp,endptPre,endptPre,true,isSigEpOn);
    pton.IsPointOnSegment(lSp,lEp,arc._arcSp,endptPre,
                          endptPre,true,isArcSpOn);
    pton.IsPointOnSegment(lSp,lEp,arc._arcEp,endptPre,
                          endptPre,true,isArcEpOn);
    if(!isSigSpOn && !isSigEpOn && !isArcSpOn && !isArcEpOn){
        return;
    }
    if(isSigSpOn && !isSigEpOn){
        SegEndpointOnArc(lSp,arc,isArcSpOn,isArcEpOn,intRes);
    }
    else if(!isSigSpOn && isSigEpOn){
        SegEndpointOnArc(lEp,arc,isArcSpOn,isArcEpOn,intRes);
    }
    else if(isSigSpOn && isSigEpOn){
        intRes._intNum = 2;
        intRes._intPnt1 = lSp;
        intRes._intPnt2 = lEp;
    }
    else if(isArcSpOn && !isArcEpOn){
        ArcEndpointOnSeg(arc._arcSp,lSp,lEp,isSigSpOn,
                         isSigEpOn,intRes);
    }    else if(!isArcSpOn && isArcEpOn){
        ArcEndpointOnSeg(arc._arcEp,lSp,lEp,isSigSpOn,
                         isSigEpOn,intRes);
    }
    else if(isArcSpOn && isArcEpOn){
        intRes._intNum  = 2;
        intRes._intPnt1 = arc._arcSp;
        intRes._intPnt2 = arc._arcEp;
    }
    intRes._isExistInt = true;
}