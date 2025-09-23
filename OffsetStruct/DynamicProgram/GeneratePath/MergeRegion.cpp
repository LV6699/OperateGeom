#include"MergeRegion.h"
#include"MergeRegLogic.h"
#include"MergeRegAux.h"
#include"../../CommonFile/Header.h"
#include"../EqudistArea/EquidBoundary.h"
#if 0
#pragma optimize("", off)
#endif
#if _ViewDebug
#include"../../../OperateView/OperateView/DisplayGeom.h"
#endif
using namespace std;
void MergeRegion::MergeIslandLoops(vector<GeomArea>& regions,
                                   vector<LoopNode>& iNodes,
                                   vector<DefLoop>inLoops,
                                   vector<DefLoop>& iloops,
                                   vector<DefLoop>& outLoops,
                                   oft::NodeForm& nodeForm)
{
#if 0
    for(auto& d : regions){
        DisplayGeom().DisplayGeomArea(d,_colors.back(),1.5);
        string str = OperateOftString().BndIslLoopString(d);
        std::cout<<str<<endl;
    }
#endif
    LoopBoolUnion unio;MergeRegAux maux;
    HandleLoop bisol;
    ///对所有岛屿环进行相交合并计算 ,从这里开始产生新的 iloops，
    ///其数据来源于未合并的 inLoops以及合并的vLocOutLoop；
    bisol.SetLoopsCurveSource(IslOffsetPath,inLoops);
    vector<DefLoop>rloops,rloops_,barLoops,insideLoops;
    unio.LoopGeomBooleanUnion(inLoops,barLoops,rloops);
    bisol.SetLoopsCurveSource(IslOffsetPath,rloops);
    maux.ClassfyClipResult(inLoops,rloops,iloops,rloops_);
    if(rloops_.empty()){
        iloops = inLoops;
        return;
    }
    ///对所有非合并外环的其他岛屿合并环 ,需要作为新的无岛屿边界环 ,即
    ///rloops_中,如果一个环在其他环内 ,则存放至insideLoops中,
    ///否则存放在outLoops中.
    ///这里的outLoops是合并外环,insideLoops是合并内环.
    maux.LoopsInsideOutsideLoop(rloops_,insideLoops,outLoops);
    maux.AdjustNotCWLoop(insideLoops);
    ///对于所有岛屿合并非外环 ,其可能是新的边界环,如果其完全包含原始岛屿环 ,则其与
    ///所包含的岛屿环组成新的 vBndIslLoop成员.
#if 0
    string str = OperateOftString().ArrayLoopString(rloops_);
    std::cout<<str<<endl;
#endif
    MergeRegLogic().HandleIslandMergeInside(insideLoops,iloops,
                                            iNodes,regions);
    for(auto& it : outLoops){
        iloops.push_back(it);
    }
    for(auto& it : insideLoops){
        nodeForm._iiMerLoops.push_back(it);
    }}
void MergeRegion::MergeGeometryRegion(OffsetParam param,
                                      DefLoop& bloop,
                                      vector<DefLoop>& iLoops,
                                      shared_ptr<OffsetNode>&bNode,
                                      bool& isOriBndClip,
                                      bool& isExistClip,
                                      NodeForm& nodeForm,
                                      vector<GeomArea>& regions)
{
#if 0
    DisplayGeom().DisplayGeomArea(GeomArea(bndLoop,vIslLoop),
                                  _colors.back(),1.5,true);
    _mainwind->myOccView->fitAll();
#endif
    if(!param._pathTech.IsBoundaryOffset() ||
            iLoops.empty() || bloop.IsEmpty())
        return;
    GeometryLogic golg;HandleLoop mersol;
    ElementPrecision elepre;MergeRegLogic merlg;
    ///numcal.ResetLoopEleParameters(bndLoop,false,bndLoop);
    ///golg.CircleArcLoopTransform(true,bloop);
    for(auto& d : iLoops){
        golg.CircleArcLoopTransform(true,d);
    }
    mersol.SetLoopCurveSourceType(BndOffsetPath,bloop);
    mersol.SetLoopsCurveSource(IslOffsetPath,iLoops);
    elepre.LoopElementEndToEnd(bloop);
    bool isBndInside_ = false;
    if(param._pathTech.IsBlankBoundary() && bloop.PastOffsetNum() == 0){
        merlg.MergeOriginBoundary(bloop,iLoops,param,isBndInside_,
                                  isOriBndClip,nodeForm,regions);
        if(!isOriBndClip && param._pathTech.
                IsAcceptBoundaryOut() && !isBndInside_){
            bNode->InsertLoop(0,bloop);
        }
    }
    if(isOriBndClip || isBndInside_){
        isExistClip = true;
        return;
    }
    vector<GeomArea>areas;
    if(param._pathTech.IsBoundaryOffset() && bloop.IsOffset()){
        EquidBoundary ebnd;
        ebnd.EquidistantceBoundary(param,bloop,iLoops,
                                   bNode,nodeForm,areas);
    }else{
        areas.emplace_back(GeomArea(bloop,iLoops));
    }
    if(areas.empty()){  /// 说明边界环在岛屿环内部 .
        iLoops.clear();
        bloop.ClearElem();
        return;
    }
    mersol.SetRegionsCurveSourceType(areas);
    if(areas.size() == 1){
        MergeRegLogic().ExicuteMergeRegion(areas[0],
                isExistClip,bloop,iLoops,nodeForm,regions);
        return;
    }    ///偏置单个边界环后,产生的边界岛屿配对环大于 1个的情况,需要对所有配对环进行合并计算 .
    RootNodeSolve().AddNodeToNodes(bNode,BndOffsetPath,true,
                                   nodeForm._bOftNodes);
    MergeRegLogic().MergeMultipleRegions(bloop,iLoops,isExistClip,
                                         areas,nodeForm,regions);
}