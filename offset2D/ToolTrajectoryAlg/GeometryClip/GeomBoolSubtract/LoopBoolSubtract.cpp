#include "LoopBoolSubtract.h"
#include "../GeomBoolUnion/OrganClipLoop.h"
#include <iostream>
#include"../../../OperateView/OperateView/DisplayGeom.h"
void LoopBoolSubtract::InputPreprocesse(DefLoop& tempLoop,
                                        vector<DefLoop>& vLoop,
                                        vector<DefLoop>& vResLoop)
{
    GeomClipSolve().HandleSubtractCoincide(tempLoop,vLoop);
    if(vLoop.empty()){
        vResLoop.push_back(tempLoop);
        return;
    }
    LoopSelIntersect().LoopClipPreprocesse(tempLoop);
    for(auto& d : vLoop){
        LoopSelIntersect().LoopClipPreprocesse(d);
    }
}
///如果vResLoop 为空,则说明没有发生裁剪.
void LoopBoolSubtract::LoopGeomBoolSubtract(DefLoop tempLoop,
                                            vector<DefLoop> vLoop,
                                            vector<DefLoop>& vResLoop)
{
#if TimeStatistic
    auto sTime = std::chrono::high_resolution_clock::now();
#endif
#if 0
    DisplayGeom().DisplayLoop(tempLoop,_colors.back(),1,true);
    DisplayGeom().DisplayLoops(vLoop,_colors.back(),1);
    _mainwind->myOccView->fitAll();
#endif
    if(vLoop.empty() || tempLoop.IsEmpty())
        return;
    LoopSubtractSolve solve;
    InputPreprocesse(tempLoop,vLoop,vResLoop);
    if(vLoop.empty())
        return;
    DefLoop clipLoop;
    vector<DefLoop> vClipLoop;
    bool isExisClip = false,isExiDelete = false;
    solve.IntersectPointSplitLoop(tempLoop,vLoop,isExisClip,
                                  clipLoop,vClipLoop);
    if(!isExisClip)
        return;
#if 0
    DisplayGeom().DisplayLoop(clipLoop,_colors.back(),1,true,true);
    DisplayGeom().DisplayLoops(vClipLoop,_colors[1],1.5);
    _mainwind->myOccView->fitAll();
    string s = OperateOftString().BndIslLoopString(GeomArea(clipLoop,vClipLoop));
    std::cout<<s<<endl;
#endif
    solve.DeleteInvalidClipElement(tempLoop,vLoop,isExiDelete,
                                   clipLoop,vClipLoop);
    if(!isExiDelete)
        return;
#if 0
    DisplayGeom().DisplayLoop(clipLoop,_colors.back(),1,true,true);
    DisplayGeom().DisplayLoops(vClipLoop,_colors[1],1.5);
    _mainwind->myOccView->fitAll();
#endif
    vector<DefLoop>vNotCliLoop,vClipLoop_;
    solve.ClassfyClipArrayLoop(vClipLoop,vNotCliLoop,vClipLoop_);
#if 0
    DisplayGeom().DisplayLoops(vNotCliLoop,_colors[1],1.5);
    DisplayGeom().DisplayLoops(vClipLoop_,_colors[1],1.5);
    _mainwind->myOccView->fitAll();
#endif
    IteratorTemplateLoop(clipLoop,vClipLoop_,vResLoop);
#if 0
    DisplayGeom().DisplayLoops(vResLoop,_colors[1],1.5);
    _mainwind->myOccView->fitAll();
#endif
    for(auto& d : vResLoop){
        LoopSelIntersect().LoopClipPreprocesse(d);
    }
    for (int i = 0; i < vNotCliLoop.size(); ++i){
        vResLoop.push_back(vNotCliLoop[i]);
    }
#if 0
    DisplayGeom().DisplayLoops(vResLoop,_colors[0],1.5);
    _mainwind->myOccView->fitAll();
#endif
#if TimeStatistic
    auto eTime = std::chrono::high_resolution_clock::now();
    double exeTime = std::chrono::duration<double>(eTime - sTime).count();
    Time_boolSub += exeTime * 1000;
    Time_boolClip += exeTime * 1000;
#endif
}
void LoopBoolSubtract::IteratorTemplateLoop(DefLoop& tempLoop,
                                            vector<DefLoop>& vLoop,
                                            vector<DefLoop>& vResLoop)
{
    ElemCalculate numcal;OrganSubtrLoop orglo;
    DefLoop loop = tempLoop;
    loop.Clear();
    for (int i = 0; i < tempLoop.Size(); ++i) {
        if(tempLoop.IndexElem(i)._label._hasMerge)
            continue;
        DefElem ele = tempLoop.IndexElem(i);
        vector<DefElem> vElement;
        Point endpoint = numcal.ElementEndpoint(ele,false);
        orglo.TemplateLoopForwardIterate(tempLoop,vLoop,
                                         endpoint,i,vElement);
        if(!vElement.empty()){
            loop.SetCurves(vElement);
            vResLoop.push_back(loop);
        }
    }
    for (int lo = 0; lo < vLoop.size(); ++lo) {
        bool isExiMerge = false;
        for (int i = 0; i < vLoop[lo].Size(); ++i) {
            if(vLoop[lo].IndexElem(i)._label._hasMerge){
                isExiMerge = true;
                break;
            }
        }
        if(!isExiMerge && !vLoop[lo].IsEmpty()){
            vResLoop.push_back(vLoop[lo]);
        }
    }
}
