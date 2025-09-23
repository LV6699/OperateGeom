#include "ConnectAreaSolve.h"
void ConnectAreaSolve::CreateConnectAreaLoopArc(const Point& preEp,
                                                const Point& nexSp,
                                                bool isToOutside,
                                                bool isPreCW,bool isNexCW,
                                                DefArc& arc1,DefArc& arc2)
{
    Point midPnt;
    GeomCalculate gocal;
    gocal.TwoPointMidpoint(preEp,nexSp,midPnt);
    if(isPreCW != isNexCW){
        double R = gocal.TwoPointDistance(preEp,nexSp);
        RotDir rotDir = isPreCW == true ? ANTICLOCKWISE : CLOCKWISE;
        DefArc arc(preEp,nexSp,midPnt,R,rotDir);
        arc1 = arc;
        arc2 = arc;
        return;
    }
    Point centerPt1,centerPt2;
    gocal.TwoPointMidpoint(preEp,midPnt,centerPt1);
    gocal.TwoPointMidpoint(midPnt,nexSp,centerPt2);
    double R = gocal.TwoPointDistance(preEp,nexSp) / 4;
    RotDir rotDir1,rotDir2;
    if(isToOutside){
        rotDir1 = isPreCW == true ? ANTICLOCKWISE : CLOCKWISE,
                rotDir2 = isPreCW == true ? CLOCKWISE : ANTICLOCKWISE;
    }
    else{
        rotDir1 = isPreCW == true ? CLOCKWISE : ANTICLOCKWISE,
                rotDir2 = isPreCW == true ? ANTICLOCKWISE : CLOCKWISE;
    }
    DefArc arc1_(preEp,midPnt,centerPt1,R,rotDir1),
            arc2_(midPnt,nexSp,centerPt2,R,rotDir2);
    arc1 = arc1_;
    arc2 = arc2_;
}
void ConnectAreaSolve::ConnectTwoLoopEndpoint(const DefLoop& preLoop,
                                              const DefLoop& nexLoop,
                                              bool isSmooth,
                                              DefLoop& resLoop)
{
    if(preLoop.m_vElem.empty() || nexLoop.m_vElem.empty()){
        return;
    }
    ElemCalculate numcal;
    Point preEp,nexSp;
    numcal.LoopEndpoint(preLoop,true,preEp);
    numcal.LoopEndpoint(nexLoop,false,nexSp);
    if(!isSmooth){
        resLoop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(preEp,nexSp)));
        return;
    }
    DefArc arc1,arc2;
    bool isPreCW = preLoop._loopLabel._loRotDir ==
            CLOCKWISE ? true : false;
    bool isNexCW = nexLoop._loopLabel._loRotDir ==
            CLOCKWISE ? true : false;
    numcal.ElementEndpoint(nexLoop.m_vElem[0],false,nexSp);    CreateConnectAreaLoopArc(preEp,nexSp,true,
                             isPreCW,isNexCW,arc1,arc2);
    resLoop.m_vElem.push_back(DefElem(ARCTYPE,arc1));
    if(isPreCW == isNexCW){
        resLoop.m_vElem.push_back(DefElem(ARCTYPE,arc2));
    }
}