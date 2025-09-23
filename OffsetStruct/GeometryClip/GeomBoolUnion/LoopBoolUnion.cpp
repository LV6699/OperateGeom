#include "LoopBoolUnion.h"
#include "OrganClipLoop.h"
#if TimeStatistic
#include"../../../ViewWindow/MainWindow/mainwindow.h"
#endif
void LoopBoolUnion::LoopGeomBooleanUnion(vector<DefLoop> vInputLoop,
                                             vector<DefLoop> vBarLoop,
                                             vector<DefLoop> &vResLoop)
{
#if TimeStatistic
    auto sTime = std::chrono::high_resolution_clock::now();
#endif
    ///return
    if(vInputLoop.size() < 2)
        return;/**
    if(vOriLoop[0].PastOffsetNum() == 5){
        ElemCalculate().PrintBndIslLoop(biLoop);
    }*/
    OrganClipLoop orgcli;LoopUnionSolve unisol;
    LoopSelIntersect().ArrayLoopClipPreprocesse(vInputLoop);
    LoopSelIntersect().ArrayLoopClipPreprocesse(vBarLoop);
    vector<DefLoop> vClipLoop;
    bool isExistInt = false,isExiDelete = false;
    unisol.IntersectPointSplitLoop(vInputLoop,isExistInt,vClipLoop);
    if(!isExistInt){
        vResLoop = vInputLoop;
        return;
    }/**
    vResLoop = vClipLoop;
    return;*/
    unisol.DeleteInvalidClipElement(vBarLoop,isExiDelete,vClipLoop);
    if(!isExiDelete){
        vResLoop = vInputLoop;
        return;
    }/**
    vResLoop = vClipLoop;
    return;*/
    vector<vector<ClipIndex>>vArrIndex;
    TraverseArrayLoopConnect(vClipLoop,vArrIndex);
    orgcli.LoopClipIndexElement(vClipLoop,vArrIndex,vResLoop);
    LoopSelIntersect().ArrayLoopClipPreprocesse(vResLoop);
#if TimeStatistic
    auto eTime = std::chrono::high_resolution_clock::now();
    double exeTime = std::chrono::duration<double>(eTime - sTime).count();
    Time_boolUni += exeTime * 1000;
    Time_boolClip += exeTime * 1000;
#endif
}
void LoopBoolUnion::TraverseArrayLoopConnect(vector<DefLoop> vLoop,
                                             vector<vector<ClipIndex>>& 
vArrIndex)
{
    ElemCalculate numcal;
    for (int lo = 0; lo < vLoop.size(); ++lo) {/**
        if(lo == 9){
            int tem = 1;
        }*/
        if(vLoop[lo].IsEmpty())            continue;
        if(!vLoop[lo]._loopLabel._loopIntersect){
            vector<ClipIndex>vIndex;
            for (int i = 0; i < vLoop[lo].ElemNum(); ++i) {
                vIndex.push_back(ClipIndex(lo,i));
            }
            vArrIndex.push_back(vIndex);
            continue;
        }
        int index = 0;
        bool isExiNotMer = false;
        for (int i = 0; i < vLoop[lo].ElemNum(); ++i) {
            if(!vLoop[lo].IndexElem(i)._elemLabel._eleHasMer){
                index = i;
                isExiNotMer = true;
                break;
            }
        }
        if(!isExiNotMer)
            continue;
        vector<ClipIndex> vIndex;
        DefElem ele = vLoop[lo].IndexElem(index);
        Point endpoint = numcal.ElementEndpoint(ele,false);
        CurrentLoopConnectLoop(vArrIndex,vIndex,endpoint,
                               vLoop,true,lo,index);
    }
}
void LoopBoolUnion::CurrentLoopConnectLoop(vector<vector<ClipIndex>>& vArrIndex,
                                           vector<ClipIndex>& vIndex,
                                           Point endpoint,
                                           vector<DefLoop>& vLoop,
                                           const bool isFirstInto,
                                           const int loIndex,
                                           const int eleIndex)
{
    if(vLoop[loIndex].IsEmpty())
        return;
    GeomCalculate gocal;
    ElemCalculate numcal;
    OrganClipLoop orgcli;
    DefLoop loop = vLoop[loIndex];
    ///vector<ClipIndex>vIndex{ClipIndex(loIndex,eleIndex)};
    vIndex.push_back(ClipIndex(loIndex,eleIndex));
    loop.m_vElem[eleIndex]._elemLabel._eleHasMer = true;
    DefElem startEle = loop.IndexElem(eleIndex);
    if(!isFirstInto){
        Point eleEp = numcal.ElementEndpoint(startEle,true);
        if(orgcli.IsTerminateSearch(endpoint,eleEp,
                                    vArrIndex,vIndex)){
            vLoop[loIndex] = loop;
            return;
        }
    }
    Point preEp,curSp;
    numcal.ElementEndpoint(startEle,true,preEp);
    int i = numcal.NextIndex(loop.ElemNum(),eleIndex);
    while (i < loop.ElemNum()) {
        DefElem ele = loop.IndexElem(i);
        if(ele._elemLabel._eleHasMer){
            bool isFound__ = false;            int loIndex__,eleIndex__;
            vLoop[loIndex] = loop;
            orgcli.SpecifyConnectElement(vLoop,preEp,loIndex,isFound__,
                                         loIndex__,eleIndex__);
            if(!isFound__)
                return;
            CurrentLoopConnectLoop(vArrIndex,vIndex,endpoint,vLoop,
                                   false,loIndex__,eleIndex__);
            return;
        }
        numcal.ElementEndpoint(ele,false,curSp);
        if(gocal.IsIdenticalPoint(preEp,curSp,PreErr5_8)){
            vIndex.push_back(ClipIndex(loIndex,i));
            numcal.ElementEndpoint(ele,true,preEp);
            loop.m_vElem[i]._elemLabel._eleHasMer = true;
            Point curEp = numcal.ElementEndpoint(ele,true);
            if(!isFirstInto && orgcli.IsTerminateSearch(
                        endpoint,curEp,vArrIndex,vIndex)){
                vLoop[loIndex] = loop;
                return;
            }
            i = numcal.NextIndex(loop.ElemNum(),i);/**
            if(isFirstInto)
                i ++;
            else
                i = numcal.NextIndex(loop.ElemNum(),i);*/
            continue;
        }
        vLoop[loIndex] = loop;
        bool isFound_ = false;
        int loIndex_,eleIndex_;
        orgcli.SpecifyConnectElement(vLoop,preEp,loIndex,isFound_,
                                     loIndex_,eleIndex_);
        if(!isFound_)
            return;
        CurrentLoopConnectLoop(vArrIndex,vIndex,endpoint,vLoop,
                               false,loIndex_,eleIndex_);
        if(!isFirstInto)
            return;
        loop = vLoop[loIndex];
        ///更新endpoint和preEp,同时ele作为新的环起始元素.
        numcal.ElementEndpoint(ele,false,endpoint);
        numcal.ElementEndpoint(ele,true,preEp);
        vIndex.push_back(ClipIndex(loIndex,i));
        i ++;
    }
}