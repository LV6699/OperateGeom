#include "ToolTrajectorySolve.h"
#if DebugModel
#pragma optimize("", off)
#endif
void ToolTrajectorySolve::SetNodeLoopSpecifyOrigin(OffsetParam offsetPara,
                                                   
shared_ptr<OffsetNode>&rootNode)
{
    if(rootNode == nullptr)
        return;
    int oftNumLim = 2;
    if(offsetPara._pathTech.IsAddOriginLoop()){
        oftNumLim = 1;
    }
    for (int i = 0; i < rootNode->_vLoopData.size(); ++i) {
        if(rootNode->_vLoopData[i].PastOffsetNum() < oftNumLim)
            rootNode->_vLoopData[i].SetCloseMode(true);
        else
            rootNode->_vLoopData[i].SetCloseMode(false);
    }
    for (int i = 0; i < rootNode->_vOffsetNode.size(); ++i)
        SetNodeLoopSpecifyOrigin(offsetPara,rootNode->_vOffsetNode[i]);
}
void ToolTrajectorySolve::AddBndLoopToBndNode(OffsetParam offsetPara,
                                              const DefLoop& curBndLoop,
                                              const vector<DefLoop>&vIslLoop,
                                              bool isExistClip,
                                              shared_ptr<OffsetNode>& bndNode,
                                              
vector<shared_ptr<OffsetNode>>&vBndOftNode)
{
    ///这里的curBndLoop可能是原始的,也可能是合并后处理过的 .
    if(vIslLoop.empty() || !offsetPara._pathTech.IsBoundaryOffset())
        return;
    if(isExistClip){
        bndNode->_geneType = BndOffsetPath;
        if(!bndNode->_vLoopData.empty() || !bndNode->_vOffsetNode.empty()){
            vBndOftNode.push_back(bndNode);
        }
        bndNode = make_shared<OffsetNode>();
    }
    bndNode->PushBackLoop(curBndLoop);
}
void ToolTrajectorySolve::AddLoopNodeToRootNode(const vector<LoopNode>&vIslNode,
                                                ToolPathType type,
                                                
vector<shared_ptr<OffsetNode>>&vNode)
{
    for (int i = 0; i < vIslNode.size(); ++i) {
        if(!vIslNode[i]._offsetNode->_vLoopData.empty() ||
                !vIslNode[i]._offsetNode->_vOffsetNode.empty()){
            vIslNode[i]._offsetNode->_geneType = type;
            auto node = vIslNode[i]._offsetNode;
            node->_geneType = type;
            std::reverse(node->_vLoopData.begin(),node->_vLoopData.end());
            vNode.push_back(node);
        }
    }
}void ToolTrajectorySolve::AddBndNoteToRootNode(OffsetParam offsetPar,
                                               const shared_ptr<OffsetNode>& 
bndNode,
                                               
vector<shared_ptr<OffsetNode>>&vBndOftNode)
{
    if((bndNode->_vLoopData.empty() && bndNode->_vOffsetNode.empty())
            || !offsetPar._pathTech.IsBoundaryOffset())
        return;
    vBndOftNode.push_back(bndNode);
}
void ToolTrajectorySolve::AddLocalLoopToRootNode(OffsetParam offsetPar,
                                                 const shared_ptr<OffsetNode>& 
bndNode,
                                                 const 
vector<LoopNode>&vIslNode,
                                                 const 
vector<DefLoop>&vBndLocalLoop,
                                                 const 
vector<DefLoop>&vBndIslMerLoop,
                                                 const 
vector<DefLoop>&vIslMerLoop,
                                                 const 
vector<DefLoop>&vIslLocalLoop,
                                                 
shared_ptr<OffsetNode>&rootNode)
{
    if(rootNode == nullptr)
        rootNode = make_shared<OffsetNode>();
    if((!bndNode->_vLoopData.empty() || !bndNode->_vOffsetNode.empty())
            && offsetPar._pathTech.IsBoundaryOffset()){
        bndNode->_geneType = BndOffsetPath;
        rootNode->_vOffsetNode.push_back(bndNode);
    }
    for(auto& it : vBndLocalLoop){
        ///string str = OperateOftString().LoopElementString(it);
        shared_ptr<OffsetNode> nodeData = make_shared<OffsetNode>();
        nodeData->_geneType = BndOftLocalPath;
        nodeData->PushBackLoop(it);
        rootNode->_vOffsetNode.push_back(nodeData);
    }
    for(auto& it : vBndIslMerLoop){
        ///string str = OperateOftString().LoopElementString(it);
        shared_ptr<OffsetNode> nodeData = make_shared<OffsetNode>();
        nodeData->_geneType = BndIslMerPath;
        nodeData->PushBackLoop(it);
        rootNode->_vOffsetNode.push_back(nodeData);
    }
    for(auto& it : vIslMerLoop){
        ///string str = OperateOftString().LoopElementString(it);
        shared_ptr<OffsetNode> nodeData = make_shared<OffsetNode>();
        nodeData->_geneType = IslIslMerPath;
        nodeData->PushBackLoop(it);
        rootNode->_vOffsetNode.push_back(nodeData);
    }
    for(auto& it : vIslNode){
        it._offsetNode->_geneType = IslOffsetPath;
        rootNode->_vOffsetNode.push_back(it._offsetNode);
    }
    for(auto& it : vIslLocalLoop){        ///string str = OperateOftString().LoopElementString(it);
        shared_ptr<OffsetNode> nodeData = make_shared<OffsetNode>();
        nodeData->_geneType = IslOftLocalPath;
        nodeData->PushBackLoop(it);
        rootNode->_vOffsetNode.push_back(nodeData);
    }
}
void ToolTrajectorySolve::GetLoopOffsetNode(OffsetParam offsetPara,
                                            const DefLoop& loop,
                                            shared_ptr<OffsetNode> &oftNode)
{
    if((!offsetPara._offsetUnit.IsFixedOffset()) ||
            (offsetPara._offsetUnit.IsFixedOffset() &&
             (loop.PastOffsetNum() < offsetPara._offsetUnit.OffsetNumber()))){
        offsetPara._offsetUnit.SetOffsetNumber(
                    offsetPara._offsetUnit.OffsetNumber() -
                    loop.PastOffsetNum());
        if(offsetPara._offsetUnit.IsFixedOffset() &&
                offsetPara._offsetUnit.OffsetNumber() < 0)
            return;
        ExicuteProgram proexi;
        proexi.LoopOffsetNodeResult(loop,offsetPara,oftNode);
    }
}
void ToolTrajectorySolve::OffsetMerBndIslLoop(OffsetParam offsetPara,
                                              
vector<shared_ptr<OffsetNode>>&vBndOftNode,
                                              
vector<shared_ptr<OffsetNode>>&vIslOftNode,
                                              const 
vector<DefLoop>&vBndLocalLoop,
                                              const 
vector<DefLoop>&vBndIslMerLoop,
                                              const vector<DefLoop>&vIslMerLoop,
                                              const 
vector<DefLoop>&vIslLocalLoop,
                                              shared_ptr<OffsetNode> &rootNode)
{
    offsetPara._offsetUnit.SetAblePreProcess(true);
    if(rootNode == nullptr)
        rootNode = make_shared<OffsetNode>();
    for(auto& it : vBndOftNode){
        it->_geneType = BndOffsetPath;
        rootNode->_vOffsetNode.push_back(it);
    }
    RootNodeSolve nodsol;
    for(auto& it : vBndLocalLoop){
        ///it.SetThisOftNum(0);
        shared_ptr<OffsetNode> nodeData = make_shared<OffsetNode>();
        GetLoopOffsetNode(offsetPara,it,nodeData);
        nodsol.SetNodeToolPathType(BndOftLocalPath,nodeData);
        nodeData->_vLoopData.insert(nodeData->_vLoopData.begin(),it);
        rootNode->_vOffsetNode.push_back(nodeData);
    }
    for(auto& it : vBndIslMerLoop){
        ///biCal_num->PrintLoopData(it);
        shared_ptr<OffsetNode> node = make_shared<OffsetNode>();
        if(it._loopLabel._isLoopOffset)
            GetLoopOffsetNode(offsetPara,it,node);
        nodsol.SetNodeToolPathType(BndIslMerPath,node);        node->InsertLoop(0,it);
        rootNode->_vOffsetNode.push_back(node);
    }
    for(auto& it : vIslMerLoop){
        ///ElemCalculate().PrintLoopData(it,false,0);
        shared_ptr<OffsetNode> nodeData = make_shared<OffsetNode>();
        GetLoopOffsetNode(offsetPara,it,nodeData);
        nodsol.SetNodeToolPathType(IslIslMerPath,nodeData);
        nodeData->_vLoopData.insert(nodeData->_vLoopData.begin(),it);
        rootNode->_vOffsetNode.push_back(nodeData);
    }
    for(auto& it : vIslOftNode){
        rootNode->_vOffsetNode.push_back(it);
    }
    for(auto& it : vIslLocalLoop){
        ///biCal_pre->PretreatmentLoop(it,true,true);
        shared_ptr<OffsetNode> nodeData = make_shared<OffsetNode>();
        GetLoopOffsetNode(offsetPara,it,nodeData);
        nodsol.SetNodeToolPathType(IslOftLocalPath,nodeData);
        nodeData->_vLoopData.insert(nodeData->_vLoopData.begin(),it);
        rootNode->_vOffsetNode.push_back(nodeData);
    }
}