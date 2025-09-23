#include "CutEntrySolve.h"
void CutEntrySolve::PointToClosetSplitLoop(const Point& P,DefLoop& loop)
{
    int index;
    Point closePt;
    CutEntrySolve entsol;
    entsol.PointToLoopClosestPoint(P,loop,index,closePt);
    entsol.PointSplitReorederLoop(closePt,index,loop);
}
///需要保证vLoop中的环是从外向内.
void CutEntrySolve::PointSplitReorederLoop(const Point& P,int index,
                                           DefLoop& loop)
{
    ElemCalculate numcal;
    DefElem preEle,nexEle;
    numcal.PointSplitElement(loop.m_vElem[index],P,true,preEle);
    numcal.PointSplitElement(loop.m_vElem[index],P,false,nexEle);
    numcal.ReorederLoopElement(index,true,false,loop);
    loop.m_vElem.insert(loop.m_vElem.begin(),nexEle);
    loop.m_vElem.push_back(preEle);
}
void CutEntrySolve::ReorederSplitElement(DefLoop& loop)
{
    Point midPnt;
    ElemCalculate numcal;
    double zCoord;
    zCoord = ElemCalculate().LoopZCoordinate(loop);
    int index = numcal.LongestElementIndex(loop);
    numcal.ElementMidpoint(loop.m_vElem[index],midPnt);
    midPnt.SetZ(zCoord);
    PointSplitReorederLoop(midPnt,index,loop);
}
void CutEntrySolve::PointToLoopClosestPoint(const Point& P,const DefLoop& loop,
                                            int& index,Point& cloPt)
{
    if(loop.m_vElem.empty()){
        return;
    }
    index = 0;
    double distance;
    GeomDistance godis;
    godis.PointToElementDistance(P,loop.m_vElem[0],distance);
    for (int i = 1; i < loop.m_vElem.size(); ++i) {
        double dis;
        godis.PointToElementDistance(P,loop.m_vElem[i],dis);
        if(dis < distance){
            index = i;
            distance = dis;
        }
    }
    ClosestPoint clopnt;
    clopnt.PointToElementClosestPoint(P,loop.m_vElem[index],cloPt);
}
void CutEntrySolve::SegLoopSpecifyIntersect(const DefLoop& loop,
                                            const Point& Sp,
                                            const Point& Ep,
                                            int& index,Point& intPnt){
    if(loop.m_vElem.empty()){
        return;
    }
    GeomIntRes intRes;
    GeomCalculate gocal;
    GeomIntersect geoint;
    DefElem ele(LINETYPE,DefSeg(Sp,Ep));
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        geoint.CalculateElementIntersect(ele,loop.m_vElem[i],true,PreErr5_9,
                                         PreErr5_9,PreErr5_9,intRes);
        if(!intRes._isExistInt){
            continue;
        }
        if(intRes._intNum == 1){
            index = i;
            intPnt = intRes._intPnt1;
            return;
        }
        index = i;
        double dis1,dis2;
        gocal.TwoPointDistance(intRes._intPnt1,Sp,dis1);
        gocal.TwoPointDistance(intRes._intPnt2,Sp,dis2);
        intPnt = dis1 <= dis2 ? intRes._intPnt1 : intRes._intPnt2;
        return;
    }
    Point P1,P2;
    ElemCalculate numcal;
    index = 0;
    numcal.ElementEndpoint(loop.m_vElem[0],false,P1);
    numcal.ElementEndpoint(loop.m_vElem[0],true,P2);
    gocal.TwoPointMidpoint(P1,P2,intPnt);
}
void CutEntrySolve::ReorederIntermediateLoop(const Point& Sp,
                                             const Point& Ep,
                                             vector<DefLoop>&vLoop)
{
    for (int i = 0; i < vLoop.size(); ++i) {
        int index;
        Point intPnt;
        SegLoopSpecifyIntersect(vLoop[i],Sp,Ep,index,intPnt);
        PointSplitReorederLoop(intPnt,i,vLoop[i]);
    }
}