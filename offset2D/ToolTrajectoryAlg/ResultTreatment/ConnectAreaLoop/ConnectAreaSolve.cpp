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
        bool iscw = !isPreCW == true;
        DefArc arc(preEp,nexSp,midPnt,R,iscw);
        arc1 = arc;
        arc2 = arc;
        return;
    }
    Point centerPt1,centerPt2;
    gocal.TwoPointMidpoint(preEp,midPnt,centerPt1);
    gocal.TwoPointMidpoint(midPnt,nexSp,centerPt2);
    double R = gocal.TwoPointDistance(preEp,nexSp) / 4;
    bool iscw1,iscw2;
    if(isToOutside){
        iscw1 = !isPreCW;
        iscw2 = isPreCW;
    }
    else{
        iscw1 = isPreCW;
        iscw2 = !isPreCW;
    }
    DefArc arc1_(preEp,midPnt,centerPt1,R,iscw1),
            arc2_(midPnt,nexSp,centerPt2,R,iscw2);
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
    bool isPreCW = preLoop._label._isCW;
    bool isNexCW = nexLoop._label._isCW;
    numcal.ElementEndpoint(nexLoop.m_vElem[0],false,nexSp);
    CreateConnectAreaLoopArc(preEp,nexSp,true,
                             isPreCW,isNexCW,arc1,arc2);
    resLoop.m_vElem.push_back(DefElem(ARCTYPE,arc1));
    if(isPreCW == isNexCW){
        resLoop.m_vElem.push_back(DefElem(ARCTYPE,arc2));
    }
}
