#include "ToolTrajectory.h"
#if DebugModel
#pragma optimize("", off)
#endif
#if _ViewDebug
#include"../../OperateDebug/DisplayGeom.h"
#endif
shared_ptr<ElemCalculate>bio_num = make_shared<ElemCalculate>();
shared_ptr<GeometryLogic>bio_golg = make_shared<GeometryLogic>();
shared_ptr<ToolTrajectorySolve>bio_biAux = make_shared<ToolTrajectorySolve>();
shared_ptr<EquIslLoop>bio_equIs = make_shared<EquIslLoop>();
shared_ptr<MergeLoopLogic>bio_mergl = make_shared<MergeLoopLogic>();
shared_ptr<BIReguOftVal>bio_oftVal = make_shared<BIReguOftVal>();
void ToolTrajectory::CreateTrajectoryInterface(GeomArea& biLoop,
                                               OffsetParam &offsetPara,
                                               shared_ptr<OffsetNode> &rootNode)
{
    if(biLoop.BndLoop().IsEmpty() ||
            biLoop.IslLoop().empty() ||
            (!offsetPara._pathTech.IsBoundaryOffset() &&
             !offsetPara._offsetUnit.IsFixedOffset()))
        return;
    if(rootNode == nullptr)
        rootNode = make_shared<OffsetNode>();
    biLoop._bndLoop.SetPastOftNum(0);
    for(auto& lo : biLoop._vIslLoop)
        lo.SetPastOftNum(0);
    for(auto& lo : biLoop._vBlockLoop)
        lo.SetPastOftNum(0);
    rootNode->_vLoopData.clear();
    rootNode->_vOffsetNode.clear();
    vector<GeomArea>vBndIslLoop{biLoop};
    CreateSpecifyTrajectory(vBndIslLoop,offsetPara,rootNode);
}
/**
在岛屿环和边界环进行偏置求交合并计算中，有如下几种合并情况：
1 不同岛屿环之间的合并可能会产生多个外环 (外轮廓)和多个内环(内轮廓)，如果一
  个环其元素不在合并后的其他所有环内，则该环是外环，否则是内环；
2 单个岛屿环在偏置之后产生的多个环，由于岛屿环向外偏置，因此产生的多个环可
  根据环的旋转方向确定其是内环还是外环，顺时针的环是外环，否则是内环；
3 边界环和岛屿环合并可能会产生多个环，在这些环中，如果某个环包含有其他岛屿
  环，则该环与其所包含的岛屿环作为新的待偏置计算的配对环，需要添加至 vCorrLoop
  中，否则会作为局部外环；
4 边界环向内偏置产生的多个环，在这些环中同样需要跟所有岛屿环作配对计算，
  处理方式跟第3点一样；*/
void ToolTrajectory::CreateSpecifyTrajectory(vector<GeomArea>vBILoop,
                                             OffsetParam offsetPara,
                                             shared_ptr<OffsetNode> &rootNode)
{
    GeometryLogic golg;EquIslLoop equisl;BIReguOftVal oftval;
    MergeLoopSolve islcal;MergeLoopLogic merlg;
    ToolTrajectorySolve trasol;
    bool isFirOffset = true;
    double firOffsetVal = offsetPara._offsetUnit.FirstOffsetValue(),
            oriOffsetVal = offsetPara._offsetUnit.OffsetValue();
    offsetPara.SetIsFirstOffset(true);    offsetPara._offsetUnit.SetAblePreProcess(true);
    vector<DefLoop>vBndLocalLoop,vBndIslMerLoop,
            vIslMerLoop,vIslLocalLoop;
    vector<shared_ptr<OffsetNode>>vBndOftNode,vIslOftNode;
    while(!vBILoop.empty()){
        if(!vBILoop[0].IsValidMatch()){
            vBILoop.erase(vBILoop.begin());
            continue;
        }
        DefLoop bndLoop = vBILoop[0]._bndLoop;
        vector<DefLoop> vIslLoop = vBILoop[0]._vIslLoop;
        if(offsetPara._offsetUnit.IsFixedOffset() &&
                bndLoop.PastOffsetNum() >=
                offsetPara._offsetUnit.OffsetNumber())
            break;
        shared_ptr<OffsetNode>bndNode = make_shared<OffsetNode>();
        if(offsetPara._pathTech.IsAddOriginLoop() &&
                bndLoop.PastOffsetNum() == 0){
            bndNode->PushBackLoop(vBILoop[0]._oriBndLoop);
        }
        if(vBILoop[0].IsAddBndLoop() && offsetPara.
                _pathTech.IsBoundaryOffset()){
            bndNode->PushBackLoop(bndLoop);
        }
        vector<LoopNode>vIslNode;
        islcal.InitialOffsetLoopNode(vIslLoop,offsetPara,
                                     vBILoop[0].IsAddIslLoop(),
                                     vIslNode,vBILoop[0]);
        vBILoop.erase(vBILoop.begin());
        int iter = 0,hasOffsetNum = bndLoop.PastOffsetNum();
        while(!vIslLoop.empty()){
            oftval.RegulateRegionOftValue(bndLoop,vIslNode,
                                           isFirOffset,firOffsetVal,
                                           oriOffsetVal,offsetPara);
            ///vIslNode 中的loop每次在改变.
            vector<DefLoop>vIslOftLoop;
            equisl.EquidistantIslLoop(offsetPara,vIslNode,vBILoop,
                                      vIslOftNode,vIslLocalLoop);
            for(auto &loop : vIslNode){
                golg.CircleArcLoopTransform(true,loop.m_loop);
                vIslOftLoop.push_back(loop.m_loop);
            }
#if 0
        DisplayGeom().DisplayLoops(vIslOftLoop,_colors.back(),1.5);
        _mainwind->myOccView->fitAll();
#endif
            vIslLoop.clear();
            vector<DefLoop>vIslOutLoop;
            /// 对所有非合并外环的其他岛屿合并环 ,需要作为新的无岛屿边界环 .
            merlg.MergeArrayIslandLoop(vBILoop,vIslMerLoop,vIslNode,
                                       vIslOftLoop,vIslLoop,vIslOutLoop);
#if 0
        DisplayGeom().DisplayLoops(vIslLoop,_colors[0],1.5);
        DisplayGeom().DisplayLoops(vIslOutLoop,_colors[1],1.5);
        _mainwind->myOccView->fitAll();
#endif
            ///vIslNode-vIslOftLoop-vIslLoop 、vIslOutLoop-vIslNode
            /// 做岛屿环(包括跟其他岛屿环不相交的原始岛屿环 ,以及合并过后的局部外环 ),
            /// 与边界环进行相交合并计算 .
            bool isOriBndClip = false,isExistClip = false,                    isBndInside = false;
            merlg.MergeBoundaryIslandLoop(bndLoop,vIslLoop,offsetPara,
                                          isOriBndClip,isExistClip,
                                          isBndInside,vBndOftNode,vIslOftNode,
                                          bndNode,vBndLocalLoop,vBndIslMerLoop,
                                          vBILoop);
            if(isOriBndClip && !bndNode->_vLoopData.empty()
                    && offsetPara._pathTech.IsBoundaryOffset()){
                bndNode->_vLoopData.pop_back();
            }
            trasol.AddBndLoopToBndNode(offsetPara,bndLoop,vIslLoop,
                                       isExistClip,bndNode,vBndOftNode);/**
            做边界-岛屿环合并计算的后处理 ,判断偏置时的环vIslNode(可能已经相互合
            并),是否与vIslLoop中的匹配,vIslLoop中的数据包括未合并的岛屿环、合并
            岛屿环后产生的局部外环、前两者与边界环合并后产生的新岛屿环 .*/
            islcal.IsLoopNodeInArrayLoop(vIslOftNode,vIslLoop,vIslNode);
            /// 删除vNewIslLoop中不在vIslLoop内的环.
            islcal.GetLoopInArrayLoop(vIslLoop,vIslOutLoop);/**
            此时需要把合并边界岛屿之前的，同时还没有与边界环合并的 ...岛屿间的合并外环
            vNewIslLoop 添加至下一轮岛屿偏置节点 vIslNode中.*/
            islcal.InitialOffsetLoopNode(vIslOutLoop,offsetPara,true,vIslNode);
            iter ++;
            hasOffsetNum ++;
            isFirOffset = false;
            offsetPara.SetIsFirstOffset(false);
            if(!offsetPara._offsetUnit.IsFixedOffset() ||
                    (hasOffsetNum < offsetPara._offsetUnit.OffsetNumber()))
                continue;
            trasol.AddLoopNodeToRootNode(vIslNode,IslOffsetPath,
                                         vIslOftNode);
            for(auto &b : vBILoop){
                if(b.BndLoop().PastOffsetNum() <
                        offsetPara._offsetUnit.OffsetNumber())
                    continue;
                shared_ptr<OffsetNode> bNode = make_shared<OffsetNode>(
                            BndOffsetPath,vector<DefLoop>{b.BndLoop()});
                shared_ptr<OffsetNode> iNode = make_shared<OffsetNode>(
                            IslOffsetPath,b.IslLoop());
                vBndOftNode.push_back(bNode);
                vIslOftNode.push_back(iNode);
                b.SetValidMatch(false);
            }
            break;
        }
        trasol.AddBndNoteToRootNode(offsetPara,bndNode,vBndOftNode);
    }
    offsetPara._offsetUnit.SetOffsetValue(oriOffsetVal);
    trasol.OffsetMerBndIslLoop(offsetPara,vBndOftNode,
                               vIslOftNode,vBndLocalLoop,
                               vBndIslMerLoop,vIslMerLoop,
                               vIslLocalLoop,rootNode);
    ///ElemCalculate().PrintArrayLoop(vBndIslMerLoop);
    trasol.SetNodeLoopSpecifyOrigin(offsetPara,rootNode);
}