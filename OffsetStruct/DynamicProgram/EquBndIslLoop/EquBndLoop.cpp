#include "EquBndLoop.h"
#if DebugModel
#pragma optimize("", off)
#endif
#if _ViewDebug
#include"../../OperateDebug/DisplayGeom.h"
#endif
/**
在岛屿环和边界环进行偏置求交合并计算中，有如下几种合并情况：
1-不同岛屿环之间的合并可能会产生多个外环 (外轮廓)和多个内环(内轮廓)，如果一
  个环其元素不在合并后的其他所有环内，则该环是外环，否则是内环；
2-单个岛屿环在偏置之后产生的多个环，由于岛屿环向外偏置，因此产生的多个环可
  根据环的旋转方向确定其是内环还是外环，顺时针的环是外环，否则是内环；
3-边界环和岛屿环合并可能会产生多个环，在这些环中，如果某个环包含有其他岛屿
  环，则该环与其所包含的岛屿环作为新的待偏置计算的配对环，需要添加至 vCorrLoop
  中，否则会作为局部外环；
4-边界环向内偏置产生的多个环，在这些环中同样需要跟所有岛屿环作配对计算，
  处理方式跟第3点一样；*/
void EquBndLoop::EquidistantBndLoop(DefLoop bndLoop,
                                    const vector<DefLoop>& vIslLoop,
                                    OffsetParam offsetPar,
                                    vector<shared_ptr<OffsetNode>>&vBndOftNode,
                                    shared_ptr<OffsetNode>& bndNode,
                                    vector<DefLoop>& vNoIntLoop,
                                    vector<GeomArea>& vMatchLoop)
{
    BndIslMatch bimat;GeometryLogic golg;
    RootNodeSolve nodsol;HandleLoop bisol;
    OffsetParam bndPara = offsetPar;
    bndPara._offsetUnit.SetFixedOffset(true);
    bndPara._offsetUnit.SetOffsetNumber(1);
    bndPara._offsetUnit.SetAblePreProcess(true);/**
    if(bndLoop._pastOftNum == 1)
        int tem = 1;
    */
    vector<DefLoop>vBndOffset;
    shared_ptr<OffsetNode> bndNode_;
    golg.CircleArcLoopTransform(false,bndLoop);
    bndLoop.SetThisOftNum(0);
    ExicuteProgram().LoopOffsetNodeResult(bndLoop,bndPara,bndNode_);
    nodsol.PathRootNodeLoop(bndNode_,vBndOffset);
    for (int i = 0; i < vBndOffset.size(); ++i){
        golg.CircleArcLoopTransform(true,vBndOffset[i]);
    }
#if 0
        DisplayGeom().DisplayLoops(vBndOffset,_colors.back(),1.5,true);
        _mainwind->myOccView->fitAll();
#endif
    ///ElemCalculate().PrintBndIslLoop(GeomArea(vBndOffset[0],vIslLoop));
    bisol.SetLoopsCurveSource(BndOffsetPath,vBndOffset);
    vector<DefLoop> vNoIntLoop_;
    bimat.LoopsMatchLoopsToAreas(vBndOffset,vIslLoop,
                                  vNoIntLoop_,vMatchLoop);
#if 0
    _mainwind->myOccView->getContext()->RemoveAll(true);
    for(auto& d : vMatchLoop){
        DisplayGeom().DisplayGeomArea(d,_colors.back(),1.5);
    }
    for(auto& d : vNoIntLoop_){
        DisplayGeom().DisplayLoop(d,_colors[1],1.5);    }
#endif
    IsLoopInMathLoop(vMatchLoop,vIslLoop,vNoIntLoop_,vNoIntLoop);
#if 0
    for(auto& d : vNoIntLoop){
        DisplayGeom().DisplayLoop(d,_colors[1],1.5);
    }
#endif
    if(!vNoIntLoop_.empty()){
        ///说明bndLoop偏移产生了多个环.
        nodsol.AddNodeToNodes(bndNode,BndOffsetPath,true,vBndOftNode);
    }
}
void EquBndLoop::MerMultipleMatchLoop(int hasOffsetNum,
                                      DefLoop& bndLoop,
                                      vector<DefLoop>& vIslLoop,
                                      vector<GeomArea>& vInputMatch,
                                      vector<DefLoop>& vNoIncLoop,
                                      vector<GeomArea>& vMatchLoop)
{
    vector<DefLoop>vNoIncLoop_;
    vector<GeomArea>vMatchLoop_;
    MergeLoopLogSolve bilg;
    BndIslMatch bimat;LoopBoolSubtract losub;
    for (int i = 0; i < vInputMatch.size(); ++i) {
        DefLoop curBndLoop = vInputMatch[i].BndLoop();
        vector<DefLoop>vCurIslLoop = vInputMatch[i].IslLoop(),
                vResLoop;
        losub.LoopGeomBoolSubtract(curBndLoop,vCurIslLoop,
                                       vResLoop);
        if(vResLoop.empty() &&
                !bilg.IsBoundaryInIslandLoop(curBndLoop,
                                            vCurIslLoop)){
            vMatchLoop_.push_back(vInputMatch[i]);
            continue;
        }
        for(auto &it : vResLoop){
            it.SetPastOftNum(hasOffsetNum + 1);
        }
        vector<DefLoop>vNotIncLoop;
        vector<GeomArea>vMatchLoop__;
        bimat.LoopsMatchLoopsToAreas(vResLoop,vCurIslLoop,
                                      vNotIncLoop,vMatchLoop__);
        IsLoopInMathLoop(vMatchLoop__,vIslLoop,vNotIncLoop,vNoIncLoop_);
        for(auto& it : vMatchLoop__){
            vMatchLoop_.push_back(it);
        }
    }
    vIslLoop.clear();
    for(auto& it : vNoIncLoop_){
        vNoIncLoop.push_back(it);
    }
    if(!vMatchLoop_.empty()){
        bndLoop = vMatchLoop_[0].BndLoop();
        vIslLoop = vMatchLoop_[0].IslLoop();
        vMatchLoop_.erase(vMatchLoop_.begin());
    }
    for(auto& it : vMatchLoop_){
        vMatchLoop.push_back(it);
    }}
void EquBndLoop::MergeArrayMatchLoop(DefLoop &bndLoop,
                                     vector<DefLoop> &vIslLoop,
                                     bool& isExistClip,
                                     vector<GeomArea> &vInputMatch,
                                     vector<DefLoop> &vNoIncLoop,
                                     vector<GeomArea> &vMatchLoop)
{
    MergeLoopLogic merlg;
    for (int i = 0; i < vInputMatch.size(); ++i) {
        bool isBndInside = false,isExistClip_ = false;
        DefLoop bndLoop_ = vInputMatch[i].BndLoop(),bndLoop__;
        vector<DefLoop>vIslLoop_ = vInputMatch[i].IslLoop(),
                vIslLoop__;
        vector<DefLoop>vNoIncLoop_;
        vector<GeomArea>vBILoop_;
        merlg.ExicuteMergeBndIslLoop(bndLoop_,vIslLoop_,isBndInside,
                                     isExistClip_,bndLoop__,vIslLoop__,
                                     vNoIncLoop_,vBILoop_);
        ///isBndInside 不可能为真.
        if(!isExistClip_){
            vMatchLoop.push_back(vInputMatch[i]);
            continue;
        }
        isExistClip = true;
        for(auto& lo : vNoIncLoop_)
            vNoIncLoop.push_back(lo);
        if(!bndLoop__.IsEmpty() && !vIslLoop__.empty())
            vMatchLoop.push_back(GeomArea(bndLoop__,vIslLoop__));
        for(auto& d : vBILoop_)
            vMatchLoop.push_back(d);
    }
    vIslLoop.clear();
    bndLoop.ClearElem();
    for(auto& b : vMatchLoop){
        b.SetAddIslLoop(true);
        b.SetAddBndLoop(true);
    }
    if(!vMatchLoop.empty()){
        bndLoop = vMatchLoop[0].BndLoop();
        vIslLoop = vMatchLoop[0].IslLoop();
        vMatchLoop.erase(vMatchLoop.begin());
    }
}
void EquBndLoop::IsLoopInMathLoop(const vector<GeomArea>& vMatchLoop,
                                  const vector<DefLoop>& vIslLoop,
                                  const vector<DefLoop>& vNoIncLoop_,
                                  vector<DefLoop>& vNoIncLoop)
{
    vector<DefLoop>vBarLoop;
    for(auto& it : vMatchLoop)
        for(auto& lo : it.IslLoop())
            vBarLoop.push_back(lo);
    for(auto& lo : vIslLoop)
        vBarLoop.push_back(lo);
    ElemCalculate().DeduplicationLoop(vBarLoop);
    GeomContain goInc;    for (int i = 0; i < vNoIncLoop_.size(); ++i) {
        bool matchRes = false;
        for (int j = 0; j < vBarLoop.size(); ++j){
            goInc.IsLoopContainLoop(vBarLoop[j],vNoIncLoop_[i],
                                    PreErr5_10,PreErr5_10,
                                    Base_LoInLo_Err,matchRes);
            if(matchRes)
                break;
        }
        if(matchRes)
            continue;
        vNoIncLoop.push_back(vNoIncLoop_[i]);
    }
}