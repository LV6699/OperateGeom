#include "MultipleInclude.h"
void MultipleInclude::OrganizeLoopToArea(const vector<DefLoop>& vLoop,
                                         double intPre,double coinPre,
                                         double incPre,
                                         vector<GeomArea>& vBILoop)
{
    ElemCalculate elecal;
    MergeLoopSolve mersol;GeomContain goinc;
#define _ToArea 0
#define _view 0
    int i = 0;
    vector<DefLoop>loops = vLoop,loops1;
    while (!loops.empty()) {
        DefLoop loop_;
        vector<DefLoop>vLoop_;
        elecal.ExceptIndexLoop(loops,i,loop_,vLoop_);
#if _ToArea
        if(loop_._id == 6 || loop_._id == 7 || loop_._id == 8){
            int id = loop_._id;
        }
#endif
        bool isInside = false;
        goinc.IsLoopsContainLoop(vLoop_,loop_,intPre,
                                 incPre,isInside);
        if(!isInside){
            loops1.emplace_back(loop_);
            loops.erase(loops.begin() + i);
            i = 0;
        }else{
            i ++;
        }
        ///防止无限循环.
        if(i > vLoop.size()){
            loops1 = vLoop;
            break;
        }
    }
    i = 0;
    while (!loops1.empty()) {
        DefLoop loop_;
        vector<DefLoop>vLoop_;
        elecal.ExceptIndexLoop(loops1,i,loop_,vLoop_);
        vector<DefLoop>vIncLoop;
        LoopIncludeLoops(loop_,vLoop_,intPre,coinPre,
                         incPre,true,vIncLoop);
#if _ToArea
        if(loop_._id == 6){
            int id = loop_._id;
        }
#endif
        if(vIncLoop.empty()){
            bool isInside = false;
            goinc.IsLoopsContainLoop(vLoop_,loop_,intPre,
                                     incPre,isInside);
            if(!isInside){
                vBILoop.push_back(GeomArea(loop_));
                loops1.erase(loops1.begin() + i);
                i = 0;
            }else{
                i ++;
            }
            continue;
        }        vector<DefLoop>vInsLoop,vOutLoop;
        mersol.LoopsInsideOutsideLoop(vIncLoop,vInsLoop,vOutLoop);
        vBILoop.push_back(GeomArea(loop_,vOutLoop));
        vector<DefLoop>leftLoops;
        loops1.erase(loops1.begin() + i);
        for(const auto& d : loops1){
            if(!elecal.IsLoopInArrayLoop(vOutLoop,d)){
                leftLoops.emplace_back(d);
            }
        }
        loops1 = leftLoops;
        i = 0;
    }
#if _ToArea
    int t = 0;
#endif
}
void MultipleInclude::LoopMultipleInclude(const vector<DefLoop>& vLoop,
                                          double intPre,double coinPre,
                                          double incPre,
                                          vector<GeomArea>& vBILoop)
{}
void MultipleInclude::LoopIncludeLoops(const DefLoop& loop,
                                       vector<DefLoop>& vLoop,
                                       double intPre,double coinPre,
                                       double incPre,bool isDel,
                                       vector<DefLoop>& vIncLoop)
{
    vector<int>vId;
    GeomContain goinc;ElemCalculate elecal;
    for (int i = 0; i < vLoop.size(); ++i) {
        bool isInclude = false;
        goinc.IsLoopContainLoop(loop,vLoop[i],intPre,
                                coinPre,incPre,isInclude);
        if(isInclude){
            vIncLoop.push_back(vLoop[i]);
            vId.push_back(i);
        }
    }
    if(isDel){
        elecal.DeleteIndexLoop(vId,vLoop);
    }
}