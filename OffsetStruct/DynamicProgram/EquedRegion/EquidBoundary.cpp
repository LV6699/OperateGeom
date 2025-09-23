#include "EquidBoundary.h"
#include "../GeneratePath/MergeRegAux.h"
#include"../../CommonFile/Header.h"
#if 1///_ViewDebug
#include"../../../OperateView/OperateView/DisplayGeom.h"
#endif
using namespace std;
using namespace oft;
void EquidBoundary::EquidistantceBoundary(OffsetParam param,
                                          DefLoop bloop,
                                          vector<DefLoop>& loops)
{
    if(!bloop.IsOffset()){
        loops.emplace_back(bloop);
        return;
    }
    GeometryLogic golg;
    RootNodeSolve nodsol;HandleLoop bisol;
    param._offsetUnit.SetFixedOffset(true);
    param._offsetUnit.SetOffsetNumber(1);
    param._offsetUnit.SetAblePreProcess(true);
    shared_ptr<OffsetNode>node;
    golg.CircleArcLoopTransform(false,bloop);
    bloop.SetThisOftNum(0);
    ExicuteProgram().LoopOffsetNodeResult(bloop,param,node);
    nodsol.PathRootNodeLoop(node,loops);
    for (int i = 0; i < loops.size(); ++i){
        golg.CircleArcLoopTransform(true,loops[i]);
    }
#if 0
    DisplayGeom().DisplayLoops(loops,_colors.back(),1.5,true);
    _mainwind->myOccView->fitAll();
    string str = OperateOftString().ArrayLoopString(loops);
    std::cout<<str<<endl;
#endif
    bisol.SetLoopsCurveSource(BndOffsetPath,loops);
}
void EquidBoundary::EquidistantceBoundary(OffsetParam param,
                                          const DefLoop& bloop,
                                          const vector<DefLoop>& iloops,
                                          shared_ptr<OffsetNode>& bNode,
                                          NodeForm& nodeForm,
                                          vector<GeomArea>& regions)
{
    vector<DefLoop>loops;
    MergeRegAux meraux;RootNodeSolve nodsol;
    EquidistantceBoundary(param,bloop,loops);
    vector<DefLoop> notIncLs,notIncLs_;
    meraux.LoopsMatchLoopsToAreas(loops,iloops,notIncLs,regions);
#if 0
    for(auto& d : regions){
        DisplayGeom().DisplayGeomArea(d,_colors.back(),1.5,false,true);
    }
    for(auto& d : notIncLs){
        DisplayGeom().DisplayLoop(d,_colors[1],1.5);
    }
#endif
    IsLoopInGeomArea(regions,iloops,notIncLs,notIncLs_);#if 0
    for(auto& d : notIncLs_){
        DisplayGeom().DisplayLoop(d,_colors[1],1.5);
    }
#endif
    if(!notIncLs.empty()){
        ///说明bloop偏移产生了多个环.
        nodsol.AddNodeToNodes(bNode,BndOffsetPath,true,
                              nodeForm._bOftNodes);
    }
    for(auto& d : notIncLs_){
        nodeForm._bLocLoops.emplace_back(d);
    }
}
void EquidBoundary::IsLoopInGeomArea(const vector<GeomArea>& areas,
                                     const vector<DefLoop>& iloops,
                                     const vector<DefLoop>& noIncLoops_,
                                     vector<DefLoop>& noIncLoops)
{
    vector<DefLoop>barLoops;
    for(auto& d : areas){
        for(auto& l : d.IslLoop()){
            barLoops.push_back(l);
        }
    }
    for(auto& l : iloops){
        barLoops.push_back(l);
    }
    ElemCalculate().DeduplicationLoop(barLoops);
    GeomContain goInc;
    for (int i = 0; i < noIncLoops_.size(); ++i) {
        bool matchRes = false;
        for (int j = 0; j < barLoops.size(); ++j){
            goInc.IsLoopContainLoop(barLoops[j],noIncLoops_[i],
                                    PreErr5_10,PreErr5_10,
                                    Base_LoInLo_Err,matchRes);
            if(matchRes)
                break;
        }
        if(matchRes)
            continue;
        noIncLoops.push_back(noIncLoops_[i]);
    }
}