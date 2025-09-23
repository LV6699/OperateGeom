#include "EquIslLoop.h"
#if DebugModel
#pragma optimize("", off)
#endif
void EquIslLoop::EquidistantIslLoop(OffsetParam offsetPara,
                                    vector<LoopNode>&vIslNode,
                                    vector<GeomArea>&vMatchLoop,
                                    vector<shared_ptr<OffsetNode>>&vIslOftNode,
                                    vector<DefLoop>&vIslLocalLoop)
{
    if(vIslNode.empty() || (offsetPara._offsetUnit.IsFixedOffset() &&
                            offsetPara._offsetUnit.OffsetNumber() == 0))
        return;
    ExicuteProgram exepro;GeometryLogic golg;
    RootNodeSolve nodsol;EquIslLoopSolve islsol;
    offsetPara._offsetUnit.SetFixedOffset(true);
    offsetPara._offsetUnit.SetOffsetNumber(1);
    offsetPara._offsetUnit.SetAblePreProcess(true);
    vector<DefLoop>vOutLoop;
    vector<LoopNode>vNewIslNode;
    vector<EquWitOutLoop>vOffsetPair;
    int i = 0;
    while (i < vIslNode.size()) {
        if(!vIslNode[i].m_loop._loopLabel._isLoopOffset){
            i ++;
            continue;
        }
        ///OperateOftString().PrintLoopData(vIslNode[i].m_loop,true,0);
        shared_ptr<OffsetNode> rootNode_;
        golg.CircleArcLoopTransform(false,vIslNode[i].m_loop);
        vIslNode[i].m_loop.SetThisOftNum(0);
        vIslNode[i].m_loop.SetLoopType(WITHINPROFILE);
        exepro.LoopOffsetNodeResult(vIslNode[i].m_loop,offsetPara,rootNode_);
        vector<DefLoop>vOftResLoop;
        nodsol.PathRootNodeLoop(rootNode_,vOftResLoop);
        if(vOftResLoop.empty()){
            islsol.AddDeleteIslNode(vIslNode,i,vIslOftNode);
            continue;
        }
        ///当vOftResLoop.size() == 1 时,vOftResLoop里面的元素不可
        ///能是外环,或包含其他岛屿环.
        if(vOftResLoop.size() == 1){
            golg.CircleArcLoopTransform(true,vOftResLoop[0]);
            vIslNode[i].m_loop = vOftResLoop[0];
            i ++;
            continue;
        }
        ///岛屿环偏移后产生多个环
        islsol.AddDeleteIslNode(vIslNode,i,vIslOftNode);
        EquWitOutLoop offsetPair;
        for(auto &lo : vOftResLoop){
            if(lo.LoopType() == WITHINPROFILE){
                shared_ptr<OffsetNode>node = make_shared<OffsetNode>();
                LoopNode data(lo,node);
                vNewIslNode.push_back(data);
                offsetPair.m_vWitLoop.push_back(lo);
            }
            else{
                offsetPair.m_vOutLoop.push_back(lo);            }
        }
        vOffsetPair.push_back(offsetPair);
    }
    islsol.HandleOutLoopIncludedLoop(vIslNode,vOffsetPair);
    vector<DefLoop>vWitLoop;
    for(auto& it : vOffsetPair){
        for(auto& loop : it.m_vOutLoop){
            vOutLoop.push_back(loop);
        }
        for(auto& loop : it.m_vWitLoop){
            vWitLoop.push_back(loop);
        }
    }
    islsol.SummarizeIslEquOutLoop(vOutLoop,vIslNode,vWitLoop,
                                  vIslOftNode,vMatchLoop);
    for(auto &data : vNewIslNode){
        vIslNode.push_back(data);
    }
    for(auto& it : vOutLoop){
        vIslLocalLoop.push_back(it);
    }
}
/**
对于outLoop中的环,其具有相关固有特性,其只能在其原始环内部、只能向内偏移等 ,因此可
以看作是边界还,所以需要考虑,满足与其作为边界-岛屿配对条件其他岛屿环 ,有三种需要处理。
1.当其没有包含其他岛屿环且与其他岛屿环不存在相交 ,其应该作为普通外环,向内偏移.
2.包含在其他岛屿环(与其同一原始环偏移产生的新岛屿节点中的环除外 ,因为outLoop中的环
  本来就包含在新岛屿节点的环中 )内,这种情况需要直接从 outLoop中删除.->被包含在vIslNode内
3.当其包含其他岛屿环 ,其与所包含的岛屿环作为新的边界 -岛屿配对环,并参与后续边界-岛屿裁
  剪计算,因为所包含的其他岛屿环不可能再与另外的岛屿环存在相交 ,所以应该从其所在的原来
  vIslNode中删除.-->包含vIslNode中的岛屿环
4.当其与其他岛屿环相交 ,其与所相交的岛屿环组成新的边界 -岛屿配对环,并参与后续边界-岛屿
  裁剪计算,同时与该外环相交的其他岛屿环 ,因为还有可能与另外的岛屿环相交 ,所以不应该从其
  所在的原来vIslNode中删除.-->相交vIslNode中的岛屿环*/