#include "EquidBoundary.h"
#include "../GeneratePath/MergeRegAux.h"
#include"../../CommonFile/Header.h"
#include"../../GeometryCalculate/RelativeContain/LoopContain.h"
#include"../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
using namespace std;
using namespace OffsetStruct;
void EquidBoundary::EquidistantceBoundary(OffsetParam param,
                                          DefLoop bloop,
                                          vector<DefLoop>& loops)
{
    if(!bloop.IsOffset()){
        loops.emplace_back(bloop);
        return;
    }
    GeometryLogic golg;RootNodeSolve nodsol;
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
    string s = OperateOftString().LoopElementString(bloop);
    std::cout<<s<<endl;
    DisplayGeom().DisplayLoop(bloop,_colors.back(),1);
    DisplayGeom().DisplayLoops(loops,_colors.back(),2,true);
    _mainwind->myOccView->fitAll();
    string str = OperateOftString().ArrayLoopString(loops);
    std::cout<<str<<endl;
#endif
    for(auto& l : loops){for(auto& c : l){c.SetSourceType(BndOffsetPath);}}
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
#if 0
    DisplayGeom().DisplayLoop(bloop,_colors.back(),1,true,true);
    DisplayGeom().DisplayLoops(loops,_colors[1],2);
    DisplayGeom().DisplayLoops(iloops,_colors.back(),1.5);
#endif
    meraux.LoopsMatchLoopsToAreas(loops,iloops,notIncLs,regions);
#if 0
    for(auto& d : regions){
        DisplayGeom().DisplayGeomArea(d,1,2);
    }
    for(auto& d : notIncLs){
        DisplayGeom().DisplayLoop(d,_colors[1],2);
    }
#endif
    IsLoopInGeomArea(regions,iloops,notIncLs,notIncLs_);
#if 0
    for(auto& d : notIncLs_){
        DisplayGeom().DisplayLoop(d,_colors[1],2);
    }
#endif
    if(!notIncLs.empty()){
        ///说明bloop 偏移产生了多个环.
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
    numst::ContParam cpa(PreErr5_10,PreErr5_10,Base_LoInLo_Err);

    for (int i = 0; i < noIncLoops_.size(); ++i) {
        bool matchRes = false;
        auto loop = noIncLoops_[i];
        LimitCoord2D::SetLoopLimCoord(loop);
        for (int j = 0; j < barLoops.size(); ++j){
            auto& l = barLoops[j];
            LimitCoord2D::SetLoopLimCoord(l);
            matchRes = LoopContain::IsLoopContainLoop(l,loop,cpa);
            if(matchRes){break;}
        }
        if(matchRes){continue;}
        noIncLoops.push_back(loop);
    }
}
