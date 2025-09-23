#include "ConnectAreaLoop.h"
void ConnectAreaLoop::AreaLoopStraightConnect(vector<DefLoop> vLoop,
                                              vector<DefLoop> &vResLoop)
{
    if(vLoop.empty()){
        return;
    }
    ElemCalculate numcal;
    for (int i = 0; i < vLoop.size() - 1; ++i) {
        ///numcal.AddElementToLoop(vLoop[i].m_vElem,loop);
        vResLoop.push_back(vLoop[i]);
        Point startPt,endPt;
        int nex = numcal.NextIndex(vLoop.size(),i);
        numcal.ElementEndpoint(vLoop[i].m_vElem.back(),true,startPt);
        numcal.ElementEndpoint(vLoop[nex].m_vElem[0],false,endPt);
        DefLoop loop;
        loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(startPt,endPt)));
        vResLoop.push_back(loop);
    }
    vResLoop.push_back(vLoop.back());
}
///需要保证vLoop中的环是从外向内.
void ConnectAreaLoop::ConnectAreaLoopToLoop(vector<DefLoop>vLoop,
                                            bool isSmoothConnect,
                                            bool isToOutside,
                                            vector<DefLoop> &vResLoop)
{
    if(vLoop.empty()){
        return;
    }
    if(isToOutside){
        std::reverse(vLoop.begin(),vLoop.end());
    }
    ElemCalculate().DeleteArrayEmptyLoop(vLoop);
    if(isSmoothConnect)
        AreaLoopArcConnect(vLoop,isToOutside,vResLoop);
    else
        AreaLoopStraightConnect(vLoop,vResLoop);
}
///创建相邻环之间的连接圆弧 .
void ConnectAreaLoop::AreaLoopArcConnect(vector<DefLoop>vLoop,
                                         bool isToOutside,
                                         vector<DefLoop> &vResLoop)
{
    if(vLoop.empty()){
        return;
    }
    ElemCalculate numcal;
    for (int i = 0; i < vLoop.size() - 1; ++i) {
        ///numcal.AddElementToLoop(vLoop[i].m_vElem,loop);
        vResLoop.push_back(vLoop[i]);
        int nex = numcal.NextIndex(vLoop.size(),i);
        bool isPreCW = vLoop[i]._loopLabel._loRotDir ==
                CLOCKWISE ? true : false;
        bool isNexCW = vLoop[nex]._loopLabel._loRotDir ==
                CLOCKWISE ? true : false;
        DefArc arc1,arc2;
        Point preEp,nexSp;
        numcal.ElementEndpoint(vLoop[i].m_vElem.back(),true,preEp);
        numcal.ElementEndpoint(vLoop[nex].m_vElem[0],false,nexSp);
        CreateConnectAreaLoopArc(preEp,nexSp,isToOutside,
                                 isPreCW,isNexCW,arc1,arc2);        DefLoop loop;
        loop.m_vElem.push_back(DefElem(ARCTYPE,arc1));
        if(isPreCW == isNexCW){
            loop.m_vElem.push_back(DefElem(ARCTYPE,arc2));
        }
        vResLoop.push_back(loop);
    }
    vResLoop.push_back(vLoop.back());
    ///numcal.AddElementToLoop(vLoop.back().m_vElem,loop);
}
void ConnectAreaLoop::CreateConnectAreaLoopArc(const Point& preEp,
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