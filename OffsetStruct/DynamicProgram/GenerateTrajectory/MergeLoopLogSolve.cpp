#include <tbb/tbb.h>
#include "MergeLoopLogSolve.h"
#if DebugModel
#pragma optimize("", off)
#endif
#if _ViewDebug
#include"../../OperateDebug/DisplayGeom.h"
#endif
void MergeLoopLogSolve::ClassfyClipResult(vector<DefLoop>vOriLoop,
                                          vector<DefLoop>vResLoop,
                                          vector<DefLoop>& vNotClipLoop,
                                          vector<DefLoop>& vClipLoop)
{
    vNotClipLoop.clear();
    vClipLoop.clear();
    ElemCalculate numcal;
    for (int i = 0; i < vResLoop.size(); ++i) {
        if(numcal.IsLoopInArrayLoop(vOriLoop,vResLoop[i]))
            vNotClipLoop.push_back(vResLoop[i]);
        else
            vClipLoop.push_back(vResLoop[i]);
    }
}
///仅用在边界和岛屿没有发生 bool裁剪的情况下,验证边界是否在岛屿环内部 .
bool MergeLoopLogSolve::IsBoundaryInIslandLoop(const DefLoop& bloop,
                                               const vector<DefLoop>&iloops)
{
    if(bloop.IsEmpty())
        return false;
    ///由于裁剪结果为空,说明边界岛屿环不存在相交 ,只需要验证边界环一条边是否在
    ///岛屿内部即可说明边界环是否在岛屿内部 .
    GeomContain gocon;
#if 1
    for(auto& d : iloops){
#if 0
        DisplayGeom().DisplayLoop(bloop,_colors.back(),1,false,true);
        DisplayGeom().DisplayLoop(d,_colors.back(),1.5);
        _mainwind->myOccView->fitAll();
#endif
        bool isCoorInc = false;
        gocon.IsLoopContainLoopCoord(d,bloop,PreErr5_12,isCoorInc);
        if(!isCoorInc){
            return false;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,vIslLoop.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t lo = r.begin(); lo < r.end(); ++lo) {
            bool isCoorInc = false;
            gocon.IsLoopContainLoopCoord(vIslLoop[lo],bndLoop,
                                         PreErr5_8,isCoorInc);
            if(!isCoorInc){
                return false;
            }   
        }
    });
#endif
    ElemCalculate numcal;
#if 0    Point mp;
    const auto& ele = [0];
    numcal.ElementMidpoint(ele,mp);
    return LoopSubtractSolve().IsPointInArrayLoop(
                vIslLoop,mp,true,PreErr5_10);
#else
    PointInPolygon inpoly;
    for (int i = 0; i < bloop.Size(); ++i) {
        Point mp;
        const auto& ele = bloop[i];
        if(ele.ElemeType() != CIRCLETYPE){
            numcal.ElementMidpoint(ele,mp);
        }else{
            mp = ele._circle.LeftPoint();
        }
        bool isOnBorder = false,isInPoly = false;
        for(const auto& l : iloops){
            inpoly.IsPointInsideLoop(l,mp,PreErr5_10,
                                     isOnBorder,isInPoly);
#if 0
            if(isInPoly){
                return true;
            }
#else
            if(!isOnBorder){
                return isInPoly;
            }
#endif
            isOnBorder = false;
        }
    }
    return false;
#endif
}
void MergeLoopLogSolve::AddOriIslLoopToIslNode(OffsetParam offsetPara,
                                               int hasOffsetNum,
                                               const 
vector<DefLoop>&vOriIslLoop,
                                               const vector<DefLoop>&vIslLoop,
                                               
vector<shared_ptr<OffsetNode>>&vIslOftNode)
{
    if(!offsetPara._pathTech.IsAcceptBoundaryOut() || hasOffsetNum != 0)
        return;
    ElemCalculate numcal;
    for(auto& lo : vOriIslLoop){
        if(lo.PastOffsetNum() > 1 ||
                numcal.IsLoopInArrayLoop(vIslLoop,lo))
            continue;
        shared_ptr<OffsetNode>node = make_shared<OffsetNode>();
        node->PushBackLoop(lo);
        node->_geneType = IslOffsetPath;
        vIslOftNode.push_back(node);
    }
}
void MergeLoopLogSolve::MergeOriginBoundaryLoop(DefLoop& bndLoop,
                                                vector<DefLoop>& vIslLoop,
                                                OffsetParam offsetPara,
                                                bool &isBndInside, bool 
&isExistClip,
                                                
vector<shared_ptr<OffsetNode>>&vIslOftNode,                                                vector<DefLoop>& vBIMerLoop,
                                                vector<GeomArea>&vBndIslLoop)
{
    isExistClip = false;
    BndIslMatch bimat;ExicuteProgram proexi;
    RootNodeSolve nodsol;LoopBoolSubtract losub;GeometryLogic golg;
    golg.CircleArcLoopTransform(true,bndLoop);
    for(auto& lo : vIslLoop)
        golg.CircleArcLoopTransform(true,lo);
    int hasOffsetNum = bndLoop.PastOffsetNum();
    vector<DefLoop>vOriIslLoop = vIslLoop,vResLoop,vResLoop_;
    if(offsetPara._pathTech.IsAcceptBoundaryOut()){
        DefLoop bndLoop_ = bndLoop;
        bndLoop_._thisOftNum = 0;
        bndLoop_.SetLoopType(WITHINPROFILE);
        OffsetParam offsetPara_ = offsetPara;
        offsetPara_._offsetUnit.SetFixedOffset(true);
        offsetPara_._offsetUnit.SetOffsetNumber(1);
        vector<DefLoop>vBndOftLoop_;
        shared_ptr<OffsetNode>bndNode_;
        proexi.LoopOffsetNodeResult(bndLoop_,offsetPara_,bndNode_);
        nodsol.PathRootNodeLoop(bndNode_,vBndOftLoop_);
        if(!vBndOftLoop_.empty())
            bndLoop.SetLoopElem(vBndOftLoop_[0].LoopElem());
    }
    ///ElemCalculate().PrintBndIslLoop(GeomArea(bndLoop,vIslLoop));
    losub.LoopGeomBoolSubtract(bndLoop,vIslLoop,vResLoop);
    if(vResLoop.empty()){
        if(IsBoundaryInIslandLoop(bndLoop,vIslLoop)){
            isBndInside = true;
            AddOriIslLoopToIslNode(offsetPara,hasOffsetNum,
                                   vOriIslLoop,vIslLoop,vIslOftNode);
        }
        return;
    }
    vIslLoop.clear();
    ClassfyClipResult(vOriIslLoop,vResLoop,vIslLoop,vResLoop_);
    vector<DefLoop> vNotIncLoop_;
    vector<GeomArea>vBndIslLoop_;
    /// 合并计算之后可能会存在多个边界岛屿配对环 ,产生的结果只有两种，
    /// vNotIncLoop_1 和vCorrLoop_1；
    isExistClip = true;
    for(auto& it : vResLoop_)
        it.SetPastOftNum(hasOffsetNum);
    bimat.LoopsMatchLoopsToAreas(vResLoop_,vIslLoop,
                                  vNotIncLoop_,vBndIslLoop_);
    if(vIslLoop.empty())
        for(auto& it : vResLoop_)
            vBIMerLoop.push_back(it);
    else
        for(auto& it : vNotIncLoop_)
            vBIMerLoop.push_back(it);
    ///当存在裁剪且vResLoop_为空说明bndLoop与vIslLoop所有环重合.
    if(vResLoop_.empty()){
        for(auto& lo : vIslLoop){
            lo._loopLabel._isLoopOffset = false;
            vBIMerLoop.push_back(lo);
        }
        bndLoop.ClearElem();        vIslLoop.clear();
    }
    if(!vBndIslLoop_.empty()){
        bndLoop = vBndIslLoop_[0].BndLoop();
        vIslLoop = vBndIslLoop_[0].IslLoop();
        bndLoop.SetPastOftNum(hasOffsetNum);
        ///vIslLoop 中的环可能包含原始实参 vIslLoop中的环,
        ///原始vIslLoop中环的hasOffsetNum不应该被改变.
        vBndIslLoop_.erase(vBndIslLoop_.begin());
        for(auto& it : vBndIslLoop_)
            vBndIslLoop.push_back(it);
    }
    ///由于允许刀路在边界外 ,因此需要添加vOriIslLoop中不在vIslLoop
    ///中的环(已经与边界合并的环)至vIslOftNode中.
    AddOriIslLoopToIslNode(offsetPara,hasOffsetNum,
                           vOriIslLoop,vIslLoop,vIslOftNode);
}