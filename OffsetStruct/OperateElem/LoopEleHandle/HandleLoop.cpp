#include "HandleLoop.h"
void HandleLoop::ResetOpenLoopEleOrder(DefLoop& loop)
{
    ElemCalculate num;
    auto NodePoint = [loop,&num](vector<Point>&vNode,int index)->void{
        for (int i = 0; i < loop.m_vElem.size(); ++i) {
            if(i == index){
                continue;
            }
            Point startPt,endPt;
            num.ElementEndpoint(loop.m_vElem[i],false,startPt);
            num.ElementEndpoint(loop.m_vElem[i],true,endPt);
            vNode.push_back(startPt);
            vNode.push_back(endPt);
        }
    };
    GeomCalculate go;
    auto IsExistPoint = [&go](const vector<Point>&vNode,
            const Point& P,bool& isExist)->void{
        isExist = false;
        for (int i = 0; i < vNode.size(); ++i) {
            if(go.IsIdenticalPoint(P,vNode[i],PreErr5_6)){
                isExist = true;
                break;
            }
        }
    };
    ///寻找环第一个元素.
    int endPtIdx;
    bool isExistStart = false;
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        Point startPt;
        vector<Point>vNode;
        NodePoint(vNode,i);
        bool isExist = false;
        num.ElementEndpoint(loop.m_vElem[i],false,startPt);
        IsExistPoint(vNode,startPt,isExist);
        if(isExist){
            continue;
        }
        endPtIdx = i;
        isExistStart = true;
        break;
    }
    if(!isExistStart){
        return;
    }
    ///寻找与当前元素结束点 endPt相连的下一元素ele.
    auto SamePoint = [&loop,&go,&num](const Point& endPt,bool& isExist,
            DefElem& ele)->void{
        for (int i = 0; i < loop.m_vElem.size(); ++i) {
            Point startPt;
            num.ElementEndpoint(loop.m_vElem[i],false,startPt);
            if(go.IsIdenticalPoint(endPt,startPt,PreErr5_6)){
                ele = loop.m_vElem[i];
                isExist = true;
                break;
            }
        }
    };
    DefLoop loop_;
    loop_.m_vElem.push_back(loop.m_vElem[endPtIdx]);    for (int i = 1; i < loop.m_vElem.size(); ++i) {
        Point endPt;
        DefElem ele = loop_.m_vElem.back(),ele_;
        num.ElementEndpoint(ele,true,endPt);
        bool isExist = false;
        SamePoint(endPt,isExist,ele_);
        if(!isExist){
            continue;
        }
        loop_.m_vElem.push_back(ele_);
    }
    loop = loop_;
}
void HandleLoop::LoopRangeIndexElement(const DefLoop& oriLoop,
                                       int sIndex,int eIndex,
                                       DefLoop& loop)
{
    if(sIndex < 0 || eIndex < 0 ||
            (sIndex > oriLoop.m_vElem.size() - 1) ||
            (eIndex > oriLoop.m_vElem.size() - 1)){
        return;
    }
    int index = sIndex;
    loop = oriLoop;
    loop.m_vElem.clear();
    ElemCalculate numcal;
    loop.m_vElem.push_back(oriLoop.m_vElem[index]);
    for (int i = 0; i < oriLoop.m_vElem.size(); ++i) {
        int nex = numcal.NextIndex(oriLoop.m_vElem.size(),index);
        if(nex == eIndex){
            break;
        }
        loop.m_vElem.push_back(oriLoop.m_vElem[nex]);
        index = nex;
    }
    loop.m_vElem.push_back(oriLoop.m_vElem[eIndex]);
}
void HandleLoop::SetLoopCurveSourceType(ToolPathType type,DefLoop& loop)
{
    for(auto& it : loop.m_vElem)
        it._elemSource = type;
}
void HandleLoop::SetLoopsCurveSource(ToolPathType type,
                                     vector<DefLoop>& loops)
{
    for(auto& it : loops)
        SetLoopCurveSourceType(type,it);
}
void HandleLoop::SetMatchEleSourceType(GeomArea& region)
{
    SetLoopCurveSourceType(BndOffsetPath,region._bndLoop);
    SetLoopsCurveSource(IslOffsetPath,region._vIslLoop);
}
void HandleLoop::SetRegionsCurveSourceType(vector<GeomArea>&regions)
{
    for(auto& it : regions)
        SetMatchEleSourceType(it);
}