#include "EqudDebug.h"
#include <iostream>
#include "../GeometryCalculate/RelativeContain/LimitCoord.h"
#pragma optimize("", off)
void EqudDebug::EquedElementDistance(const vector<DefElem>& vOriElem,
                                     const vector<DefElem>& vElem,
                                     double offsetVal,
                                     vector<int> &vIndex)
{
    vIndex.clear();
    GeomDistance godis;
    vector<double>vDistance;
    for (int i = 0; i < vElem.size(); ++i) {
        for (int j = 0; j < vOriElem.size(); ++j) {
            if(i == 12 && j == 2){
                int tem = 1 + 1;
            }
            double distance;
            godis.ElementToElementDistance(vElem[i],vOriElem[j],distance);
            vDistance.push_back(distance);
            if(distance + SeIn_Dis_Err < offsetVal){
                vIndex.push_back(i);
                break;
            }
        }
    }
}
void EqudDebug::SameEndpointIndex(const vector<DefElem>& vElem,
                                  double pre,
                                  vector<int>&vIndex)
{
    ElemCalculate elecal;GeomCalculate gocal;
    for (int i = 0; i < vElem.size(); ++i) {
        Point sp = elecal.ElementEndpoint(vElem[i],false),
                ep = elecal.ElementEndpoint(vElem[i],true);
        if(gocal.TwoPointDistance2D(sp,ep) < pre){
            vIndex.push_back(i);
        }
    }
}
void EqudDebug::NotConnectElemIndex(const vector<DefElem>& vElem,
                                    double pre,
                                    vector<int>&vIndex)
{
    ElemCalculate elecal;GeomCalculate gocal;
    for (int i = 0; i < vElem.size(); ++i) {
        int nex = elecal.NexId(vElem.size(),i);
        Point ep = elecal.ElementEndpoint(vElem[i],true),
                sp = elecal.ElementEndpoint(vElem[nex],false);
        if(gocal.TwoPointDistance2D(sp,ep) >= pre){
            vIndex.push_back(i);
        }
    }
}
void EqudDebug::ArcGeomInfo(const DefArc& arc, double &d1, double &d2)
{
    GeomCalculate gocal;
    Point sp = arc.GetSp(),ep = arc.GetEp(),
            cp = arc.GetCp();
    d1 = gocal.TwoPointDistance2D(sp,cp);
    d2 = gocal.TwoPointDistance2D(ep,cp);
    double startAgl,endAgl,angle;
    gocal.ArcEndpointAngle(arc._arcSp,arc._arcEp,
                           arc._arcCp, arc._isCW,
                           startAgl,endAgl);
    gocal.TwoAngleIncludedAngle(startAgl,endAgl,
                                arc._isCW,angle);
}
void EqudDebug::NotMeetArcIndex(const DefLoop &loop,
                                double e,vector<int>&vIndex)
{
    for (int i = 0; i < loop.Size(); ++i) {
        if(loop.IndexElem(i).Type() != ARCTYPE)
            continue;
        double d1 = 0,d2 = 0;
        ArcGeomInfo(loop.IndexElem(i).GetArc(),d1,d2);
        if(std::abs(d1 - d2) > e){
            vIndex.push_back(i);
        }
    }
}
void EqudDebug::InformationCase(GeomArea biLoop,
                                OffsetParam offsetPara,
                                shared_ptr<OffsetNode>rootNode)
{
    double z = ElemCalculate().LoopZCoordinate(biLoop.BndLoop());
    string biStr = OperateOftString().BndIslLoopString(biLoop);
    string strb = OperateOftString().LoopElementString(biLoop.BndLoop());
    string stri = OperateOftString().ArrayLoopString(biLoop.IslLoop());
    if(std::abs(z - 3.15) < PreErr5_2){
        int tem = 1;
    }
    std::cout<<biStr<<std::endl;
    vector<DefLoop>vResLoop;
    RootNodeSolve().PathRootNodeLoop(rootNode,vResLoop);
    if(vResLoop.empty())
        return;
    string resStr = OperateOftString().ArrayLoopString(vResLoop);
    std::cout<<resStr<<std::endl;
    DefCircle circle;
    LimitCoord2D::LoopMinCircumcircle(vResLoop[0],circle);
    double minX = 0,maxX = 0,minY = 0,maxY = 0,w = 0,h = 0,l = 0;
    //LimitCoord2D::LoopLimitCoord(vResLoop[0],minX,maxX,minY,maxY);
    w = maxX - minX;
    h = maxY - minY;
    l = w > h ? w : h;
    int tem = 1;
}
void EqudDebug::PathNodeDebug(const shared_ptr<OffsetNode>&rootNode)
{
    if(rootNode == nullptr)
        return;
    for (int lo = 0; lo < rootNode->_loops.size(); ++lo) {
        LoopDataDebug(rootNode->_loops[lo]);
    }
    for(int i = 0; i < rootNode->_nodes.size();i ++)
        PathNodeDebug(rootNode->_nodes[i]);
}
void EqudDebug::LoopDataDebug(const DefLoop &loop)
{
    ElemCalculate elecal;GeomCalculate gocal;
    for (int i = 0; i < loop.Size(); ++i) {
        if(loop.IndexElem(i).Type() != ARCTYPE)
            continue;
        DefArc arc = loop.IndexElem(i).GetArc();
        arc.InitR();
        Point sp = arc.GetSp(),ep = arc.GetEp(),
                cp = arc.GetCp();
        double ra = arc.R();
        bool isSame = false,isSame1 = false,isSame2 = false;
        gocal.IsIdenticalPoint2D(sp,Point(-2.096,-0.404),
                                 PreErr5_1,isSame);
        gocal.IsIdenticalPoint2D(ep,Point(-1.672, 0),
                                 PreErr5_1,isSame1);
        gocal.IsIdenticalPoint2D(cp,Point(6.5, -8.999),
                                 PreErr5_1,isSame2);
        if(isSame || isSame1 || isSame2 || std::abs(ra - 12.15) < 0.5){
            int tem = 1;
            std::cout<<tem<<std::endl;
        }
    }
}
bool EqudDebug::IsLoopExistPoint(const DefLoop& loop,const Point& P,double err)
{
    ElemCalculate elecal;
    for (int i = 0; i < loop.Size(); ++i) {
        Point sp = elecal.ElementEndpoint(loop.IndexElem(i),false),
                ep = elecal.ElementEndpoint(loop.IndexElem(i),true);
        if(P.IsSamePoint2D(sp,err) ||
                P.IsSamePoint2D(ep,err))
            return true;
    }
    return false;
}