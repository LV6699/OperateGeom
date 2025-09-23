#include "MergeLoopLogic.h"
#if DebugModel
#pragma optimize("", off)
#endif
#if _ViewDebug
#include"../../OperateDebug/DisplayGeom.h"
#endif
void MergeLoopLogic::MergeArrayIslandLoop(vector<GeomArea>& vBILoop,
                                          vector<DefLoop>& vIslMerLoop,
                                          vector<LoopNode>& vIslNode,
                                          vector<DefLoop>vInputLoop,
                                          vector<DefLoop>& vIslLoop,
                                          vector<DefLoop>& vOutLoop)
{
#if 0
    for(auto& d : vBILoop){
        DisplayGeom().DisplayGeomArea(d,_colors.back(),1.5);
    }
#endif
    LoopBoolUnion unio;MergeLoopSolve mersol;
    MergeLoopLogSolve biaux;HandleLoop bisol;
    ///对所有岛屿环进行相交合并计算 ,从这里开始产生新的 vIslLoop，
    ///其数据来源于未合并的 vIslOffsetLoop以及合并的vLocOutLoop；
    bisol.SetLoopsCurveSource(IslOffsetPath,vInputLoop);
    vector<DefLoop>vResLoop,vResLoop_,vBarLoop,vInsOutLoop;
    unio.LoopGeomBooleanUnion(vInputLoop,vBarLoop,vResLoop);
    bisol.SetLoopsCurveSource(IslOffsetPath,vResLoop);
    biaux.ClassfyClipResult(vInputLoop,vResLoop,vIslLoop,vResLoop_);
    if(vResLoop_.empty()){
        vIslLoop = vInputLoop;
        return;
    }
    ///对所有非合并外环的其他岛屿合并环 ,需要作为新的无岛屿边界环 ,即
    ///vResLoop_ 中,如果一个环在其他环内 ,则存放至vInsOutLoop中,
    ///否则存放在vOutLoop中.
    mersol.LoopsInsideOutsideLoop(vResLoop_,vInsOutLoop,vOutLoop);
    mersol.HandleInClockwiseMerLoop(vInsOutLoop);
    ///对于所有岛屿合并非外环 ,其可能是新的边界环,如果其完全包含原始岛屿环 ,则其与
    ///所包含的岛屿环组成新的 vBndIslLoop成员.
#if 0
    OperateOftString opeoft;
    string str = opeoft.ArrayLoopString(vResLoop_);
    std::cout<<str<<endl;
#endif
    mersol.HandleIslandMergeInside(vInsOutLoop,vIslLoop,
                                   vIslNode,vBILoop);
    for(auto& it : vOutLoop){
        vIslLoop.push_back(it);
    }
    for(auto& it : vInsOutLoop){
        vIslMerLoop.push_back(it);
    }
    
}
void MergeLoopLogic::MergeBoundaryIslandLoop(DefLoop& bndLoop,
                                             vector<DefLoop>&vIslLoop,
                                             OffsetParam offsetPara,
                                             bool &isOriBndClip,
                                             bool &isExistClip,
                                             bool &isBndInside,                                             
vector<shared_ptr<OffsetNode>>&vBndOftNode,
                                             
vector<shared_ptr<OffsetNode>>&vIslOftNode,
                                             shared_ptr<OffsetNode>&bndNode,
                                             vector<DefLoop>& vBndLocLoop,
                                             vector<DefLoop>& vBIMerLoop,
                                             vector<GeomArea>& vBILoop)
{
#if 0
    DisplayGeom().DisplayGeomArea(GeomArea(bndLoop,vIslLoop),
                                  _colors.back(),1.5,true);
    _mainwind->myOccView->fitAll();
#endif
    isOriBndClip = false;
    isExistClip = false;
    isBndInside = false;
    if(!offsetPara._pathTech.IsBoundaryOffset() ||
            vIslLoop.empty() || bndLoop.IsEmpty())
        return;
    EquBndLoop equbnd;ElemCalculate numcal;
    MergeLoopLogSolve bilg;HandleLoop mersol;
    RootNodeSolve nodsol;ElementPrecision elepre;
    numcal.ResetLoopEleParameters(bndLoop,false,bndLoop);
    mersol.SetLoopCurveSourceType(BndOffsetPath,bndLoop);
    mersol.SetLoopsCurveSource(IslOffsetPath,vIslLoop);
    elepre.LoopElementEndToEnd(bndLoop);
    bool isBndInside_ = false;
    if(offsetPara._pathTech.IsBlankBoundary() &&
            bndLoop.PastOffsetNum() == 0){
        bilg.MergeOriginBoundaryLoop(bndLoop,vIslLoop,offsetPara,
                                     isBndInside_,isOriBndClip,
                                     vIslOftNode,vBIMerLoop,vBILoop);
        if(!isOriBndClip && offsetPara._pathTech.
                IsAcceptBoundaryOut() && !isBndInside_)
            bndNode->InsertLoop(0,bndLoop);
    }
    if(isOriBndClip || isBndInside_){
        isExistClip = true;
        return;
    }
    vector<DefLoop>vBndNoIncLoop;
    vector<GeomArea>vBndIslLoop;
    numcal.ResetLoopEleParameters(bndLoop,false,bndLoop);
    if(offsetPara._pathTech.IsBoundaryOffset() &&
            bndLoop._loopLabel._isLoopOffset){
        equbnd.EquidistantBndLoop(bndLoop,vIslLoop,offsetPara,
                                  vBndOftNode,bndNode,vBndNoIncLoop,
                                  vBndIslLoop);
    }
    else{
        vBndIslLoop.push_back(GeomArea(bndLoop,vIslLoop));
    }
    ///return;
    for(auto& it : vBndNoIncLoop){
        vBndLocLoop.push_back(it);
    }
    if(vBndIslLoop.empty()){
        ///说明边界还在岛屿环内部 .
        vIslLoop.clear();
        bndLoop.ClearElem();
        return;    }
    mersol.SetRegionsCurveSourceType(vBndIslLoop);
    if(vBndIslLoop.size() == 1){
        ExicuteMergeBndIslLoop(vBndIslLoop[0]._bndLoop,
                vBndIslLoop[0]._vIslLoop,isBndInside,
                isExistClip,bndLoop,vIslLoop,vBIMerLoop,vBILoop);
        return;
    }
    ///偏置单个边界环后,产生的边界岛屿配对环对于大于 1个的情况,需要对所有配对环进行合并计算 .
    nodsol.AddNodeToNodes(bndNode,BndOffsetPath,true,vBndOftNode);
    equbnd.MergeArrayMatchLoop(bndLoop,vIslLoop,isExistClip,
                               vBndIslLoop,vBIMerLoop,vBILoop);
}
void MergeLoopLogic::ExicuteMergeBndIslLoop(DefLoop& bndLoop_,
                                            vector<DefLoop>& vIslLoop_,
                                            bool& isBndInside,
                                            bool& isExistClip,
                                            DefLoop& bndLoop,
                                            vector<DefLoop>& vIslLoop,
                                            vector<DefLoop>& vBIMerLoop,
                                            vector<GeomArea>& vBILoop)
{
    int hasOffsetNum = bndLoop.PastOffsetNum();
    vector<DefLoop>vResLoop;BndIslMatch bimat;
    LoopBoolSubtract losub;MergeLoopLogSolve bilg;
    losub.LoopGeomBoolSubtract(bndLoop_,vIslLoop_,vResLoop);
#if 0
    DisplayGeom().DisplayGeomArea(GeomArea(bndLoop_,vIslLoop_),
                                  _colors.back(),1,true);
    DisplayGeom().DisplayLoops(vResLoop,_colors[1],1.5);
    _mainwind->myOccView->fitAll();
#endif
    if(vResLoop.empty()){
        bndLoop = bndLoop_;
        vIslLoop = vIslLoop_;
        isBndInside = bilg.IsBoundaryInIslandLoop(bndLoop_,vIslLoop_);
        if(isBndInside){
            bndLoop.ClearElem();
            vIslLoop.clear();
        }
        return;
    }
    isExistClip = true;
    bilg.ClassfyClipResult(vIslLoop_,vResLoop,vIslLoop_,vResLoop);
    ///当存在裁剪且vResLoop为空说明bndLoop_与vIslLoop_所有环重合.
    if(vResLoop.empty()){
        for(auto& lo : vIslLoop){
            lo._loopLabel._isLoopOffset = false;
            vBIMerLoop.push_back(lo);
        }
        bndLoop.ClearElem();
        vIslLoop.clear();
        return;
    }
    for(auto& it : vResLoop){
        it.SetPastOftNum(hasOffsetNum + 1);
    }
    vector<DefLoop> vNotIncLoop;
    vector<GeomArea>vBndIslLoop;
    ///合并计算之后可能会存在多个边界岛屿配对环 .
    bimat.LoopsMatchLoopsToAreas(vResLoop,vIslLoop_,                                  vNotIncLoop,vBndIslLoop);
    for(auto& it : vBndIslLoop){
        it.SetAddBndLoop(true);
    }
    ///在vCurIslLoop中与边界环不相交的岛屿环依然存在 vIslLoop_中.
    bndLoop = bndLoop_;
    vIslLoop = vIslLoop_;
    ///当岛屿环全部合并时，则所有局部环都是外轮廓 .
    if(vIslLoop_.empty()){
        for(auto& it : vResLoop){
            vBIMerLoop.push_back(it);
        }
    }
    else{
        for(auto& it : vNotIncLoop){
            vBIMerLoop.push_back(it);
        }
    }
    if(!vBndIslLoop.empty()){
        bndLoop = vBndIslLoop[0].BndLoop();
        vIslLoop = vBndIslLoop[0].IslLoop();
        vBndIslLoop.erase(vBndIslLoop.begin());
        for(auto& it : vBndIslLoop){
            it.SetAddIslLoop(true);
            vBILoop.push_back(it);
        }
    }
}