#include <tbb/tbb.h>
#include "LoopSubtractSolve.h"
#include <iostream>
#if _ViewDebug
#include"../../OperateDebug/DisplayGeom.h"
#endif
bool LoopSubtractSolve::IsPointInArrayLoop(const vector<DefLoop>& vLoop,
                                           const Point& P,bool isTemplate,double
bErr)
{
#if 0
    string s = OperateOftString().ArrayLoopString(vLoop);
    std::cout<<s<<endl;
#endif
    PointInPolygon inpoly;
    for (int i = 0; i < vLoop.size(); ++i) {
        bool isOnBorder = false,isInPoly = false;///PreErr5_10
        inpoly.IsPointInsideLoop(vLoop[i],P,bErr,
                                 isOnBorder,isInPoly);
        if(isOnBorder){
            if(isTemplate){
                return false;
            }
            else{
                return true;
            }
        }
        if(isInPoly){
            return true;
        }
    }
    return false;
}
void LoopSubtractSolve::IntersectPointSplitLoop(const DefLoop& tempLoop,
                                                const vector<DefLoop>& vLoop,
                                                bool &isExisClip,
                                                DefLoop &resLoop,
                                                vector<DefLoop> &vResLoop)
{
    LoopUnionSolve unio;
    unio.LoopLoopsIntersectPoint(tempLoop,vLoop,isExisClip,resLoop);
    if(!isExisClip){
        vResLoop = vLoop;
        resLoop = tempLoop;
        return;
    }
#if 0
    for (int i = 0; i < vLoop.size(); ++i) {
        DefLoop resLoop_;
        bool isExisClip_ = false;
        vector<DefLoop>vTemLoop{tempLoop};
        unio.LoopLoopsIntersectPoint(vLoop[i],vTemLoop,
                                     isExisClip_,resLoop_);
        if(isExisClip_){
            resLoop_._loopLabel._loopIntersect = true;
            vResLoop.push_back(resLoop_);
        }
        else{
            vResLoop.push_back(vLoop[i]);
        }    }
#else
    vResLoop.resize(vLoop.size());
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, vLoop.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            DefLoop resLoop_;
            bool isExisClip_ = false;
            vector<DefLoop>vTemLoop{tempLoop};
            unio.LoopLoopsIntersectPoint(vLoop[i],vTemLoop,
                                         isExisClip_,resLoop_);
            if(isExisClip_){
                resLoop_._loopLabel._loopIntersect = true;
                vResLoop[i] = resLoop_;
            }
            else{
                vResLoop[i] = vLoop[i];
            }
        }
    });
#endif
}
///tempLoop和vLoop中的元素要分别与clipLoop和vClipLoop中的元素一一对应.
void LoopSubtractSolve::DeleteInvalidClipElement(const DefLoop& tempLoop,
                                                 const vector<DefLoop>& vLoop,
                                                 bool& isExiDelete,
                                                 DefLoop& clipLoop,
                                                 vector<DefLoop>& vClipLoop)
{
    if(vLoop.size() != vClipLoop.size())
        return;
    ElemCalculate numcal;
    vector<int>vIndex;
#if 0
    string s = OperateOftString().BndIslLoopString(GeomArea(clipLoop,vLoop));
    std::cout<<s<<endl;
#endif
    LoopInvalidElement(clipLoop,vLoop,true,isExiDelete,vIndex);
    if(!isExiDelete){
        clipLoop = tempLoop;
        vClipLoop = vLoop;
        return;
    }
    ///对于在tempLoop外部(包括部分或完全与tempLoop重合)的环应该被删除,
    ///而在tempLoop内部的环需要另外存放 ,且其元素不在参与组成新的环 .
    vector<vector<int>>vArrIndex;
    vector<DefLoop>vTemLoop{tempLoop};
#if 0
    string s = 
OperateOftString().BndIslLoopString(GeomArea(tempLoop,vClipLoop));
    std::cout<<s<<endl;
#endif
    for (int lo = 0; lo < vClipLoop.size(); ++lo) {
        vector<int>vIndex_;
        if(!vClipLoop[lo]._loopLabel._loopIntersect){
            vArrIndex.push_back(vIndex_);
            continue;
        }
        bool isExiDel_ = false;
        LoopInvalidElement(vClipLoop[lo],vTemLoop,false,
                           isExiDel_,vIndex_);
        ///在tempLoop内部或外部的环都应从 vClipLoop中删除.
        if(!isExiDel_){            vClipLoop[lo] = vLoop[lo];
            vClipLoop[lo]._loopLabel._loopIntersect = false;
        }
        vArrIndex.push_back(vIndex_);
    }
    bool isEmpty = true;
    for (int i = 0; i < vArrIndex.size(); ++i) {
        if(!vArrIndex[i].empty()){
            isEmpty = false;
        }
        numcal.DeleteIndexElement(vArrIndex[i],vClipLoop[i].m_vElem);
    }
    if(isEmpty){
        vIndex.clear();
        isExiDelete = false;
    }
    numcal.DeleteIndexElement(vIndex,clipLoop.m_vElem);
}
void LoopSubtractSolve::LoopInvalidElement(DefLoop& loop,
                                           const vector<DefLoop>& vLoop,
                                           bool isTemplate,
                                           bool& isExiDelete,
                                           vector<int>& vIndex)
{
#if 0
        DisplayGeom().DisplayLoop(loop,_colors.back(),1,false,true);
        DisplayGeom().DisplayLoops(vLoop,_colors[1],1);
        _mainwind->myOccView->fitAll();
        string s = OperateOftString().BndIslLoopString(GeomArea(loop,vLoop));
        std::cout<<s<<endl;
#endif
    ElemCalculate numcal;
#if 0
    for (int i = 0; i < loop.ElemNum(); ++i) {/**
        if(i == 34){
            int tem = 1;
        }*/
        Point mp;
        DefElem ele = loop.IndexElem(i);
        numcal.ElementMidpoint(ele,mp);
#if 0
        DisplayGeom().DisplayElement(ele,_colors[2],2);
        DisplayGeom().DisplayPoint(mp,_colors[1],true,i);
#endif
        if((isTemplate && !IsPointInArrayLoop(vLoop,mp,true)) ||
                (!isTemplate && IsPointInArrayLoop(vLoop,mp,false)))
            continue;
        vIndex.push_back(i);
        isExiDelete = true;
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop.ElemNum()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            Point mp;
            const DefElem& ele = loop[i];
            numcal.ElementMidpoint(ele,mp);
            if((isTemplate && !IsPointInArrayLoop(vLoop,mp,true,PreErr5_10)) ||
                    (!isTemplate && 
IsPointInArrayLoop(vLoop,mp,false,PreErr5_10)))
                continue;
            loop.m_vElem[i]._elemLabel._isValidity = false;            isExiDelete = true;
        }
    });
    for (int i = 0; i < loop.ElemNum(); ++i) {
        if(!loop.IndexElem(i)._elemLabel._isValidity){
            vIndex.push_back(i);
        }
    }
#endif
}
void LoopSubtractSolve::ClassfyClipArrayLoop(const vector<DefLoop>& vLoop,
                                             vector<DefLoop>& vNotCliLoop,
                                             vector<DefLoop>& vClipLoop)
{
    for (int i = 0; i < vLoop.size(); ++i) {
        if(!vLoop[i]._loopLabel._loopIntersect){
            vNotCliLoop.push_back(vLoop[i]);
        }
        else{
            vClipLoop.push_back(vLoop[i]);
        }
    }
}
void LoopSubtractSolve::CloseLoopCase(vector<DefElem>& vElement)
{
    if(!ElemCalculate().IsCloseLoop(DefLoop(vElement),PreErr5_8)){
        vElement.clear();
    }
}
bool LoopSubtractSolve::IsTerminateSearch(const Point& endpoint,
                                          const Point& curPnt,
                                          vector<DefElem>& vElement)
{
    if(GeomCalculate().IsIdenticalPoint2D(endpoint,curPnt,
                                          PreErr5_8)){
        CloseLoopCase(vElement);
        return true;
    }
    return false;
}
void LoopSubtractSolve::TempateConnectElement(const DefLoop& tempLoop,
                                              const Point& refPnt,
                                              bool& isFound,int& index)
{
    isFound = false;
    GeomCalculate gocal;
    ElemCalculate numcal;
    for (int i = 0; i < tempLoop.ElemNum(); ++i) {
        if(tempLoop.IndexElem(i)._elemLabel._eleHasMer)
            continue;
        const DefElem& ele = tempLoop.IndexElem(i);
        Point sp = numcal.ElementEndpoint(ele,false);
        if(gocal.IsIdenticalPoint(refPnt,sp,PreErr5_8)){
            index = i;
            isFound = true;
            return;
        }
    }
}void LoopSubtractSolve::ArrayLoopConnectElement(const vector<DefLoop> &vLoop,
                                                const Point& refPnt,
                                                bool& isFound,int& loIndex,
                                                int& index)
{
    isFound = false;
    GeomCalculate gocal;
    ElemCalculate numcal;
    for (int lo = 0; lo < vLoop.size(); ++lo) {
        for (int i = 0; i < vLoop[lo].ElemNum(); ++i) {
            const DefElem& ele = vLoop[lo].IndexElem(i);
            Point ep = numcal.ElementEndpoint(ele,true);
            if(gocal.IsIdenticalPoint(refPnt,ep,PreErr5_8)){
                index = i;
                loIndex = lo;
                isFound = true;
                return;
            }
        }
    }
}
///ele不能是引用传参.
void LoopSubtractSolve::AddReverseElement(DefElem ele,
                                          vector<DefElem> &vElement)
{
    ElemCalculate().ReverseEleDirection(ele);
    vElement.push_back(ele);
}