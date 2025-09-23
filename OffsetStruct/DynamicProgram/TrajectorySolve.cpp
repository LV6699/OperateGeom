#include "TrajectorySolve.h"
#include"../CommonFile/Header.h"
#if 0
#pragma optimize("", off)
#endif
#if _ViewDebug
#include"../../../OperateView/OperateView/DisplayGeom.h"
#endif
using namespace std;
using namespace oft;
void TrajectorySolve::SetNodeLoopSpecifyOrigin(OffsetParam param,
                                               shared_ptr<OffsetNode>& pathNode)
{
    if(pathNode == nullptr)
        return;
    int oftNumLim = 2;
    if(param._pathTech.IsAddOriginLoop()){
        oftNumLim = 1;
    }
    for(auto& d : pathNode->_vLoopData){
        if(d.PastOffsetNum() < oftNumLim){
            d.SetCloseMode(true);
        }else{
            d.SetCloseMode(false);
        }
    }
    for (auto& d : pathNode->_vOffsetNode){
        SetNodeLoopSpecifyOrigin(param,d);
    }
}
void TrajectorySolve::AddBndLoopToBndNode(OffsetParam param,
                                          const DefLoop& bloop,
                                          const vector<DefLoop>&iloops,
                                          bool isExistClip,
                                          shared_ptr<OffsetNode>& bNode,
                                          NodeForm &nodeForm)
{
    ///这里的curBndLoop可能是原始的,也可能是合并后处理过的 .
    if(iloops.empty() || !param._pathTech.IsBoundaryOffset())
        return;
    if(isExistClip){
        bNode->_geneType = BndOffsetPath;
        if(!bNode->_vLoopData.empty() || !bNode->_vOffsetNode.empty()){
            nodeForm._bOftNodes.emplace_back(bNode);
        }
        bNode = make_shared<OffsetNode>();
    }
    bNode->PushBackLoop(bloop);
}
void TrajectorySolve::DeleteLoopNotInLoops(const vector<DefLoop>& loops,
                                           vector<DefLoop>& loops_)
{
    ///删除loops_中不在loops内的环；
    if(loops.empty()){
        loops_.clear();
        return;
    }
    vector<int>ids;
    ElemCalculate numcal;    for (int i = 0; i < loops_.size(); ++i) {
        if(!numcal.IsLoopInArrayLoop(loops,loops_[i])){
            ids.emplace_back(i);
        }
    }
    numcal.DeleteIndexLoop(ids,loops_);
}
void TrajectorySolve::InitialLoopNode(const vector<DefLoop> &loops,
                                      OffsetParam param,bool isAdd,
                                      vector<LoopNode>&nodes,
                                      const GeomArea& area)
{
    for (int i = 0; i < loops.size(); ++i) {
        shared_ptr<OffsetNode> node = loops[i]._loopLabel.m_hasExiNode;
        if(node == nullptr){
            node = make_shared<OffsetNode>();
        }
        DefLoop loop_ = loops[i];
        if(param._pathTech.IsAddOriginLoop() &&
                loop_.PastOffsetNum() == 0 &&
                area._vOriIslLoop.size() == loops.size()){
            loop_ = area._vOriIslLoop[i];
            isAdd = true;
        }
        else if(area.IsAddIslLoop()){
            isAdd = true;
        }
        LoopNode node_(loops[i],node);
        if(isAdd){
            node_._offsetNode->PushBackLoop(loop_);
        }
        nodes.emplace_back(node_);
    }
}
void TrajectorySolve::AddLoopNodeToPathNode(const vector<LoopNode>&lNodes,
                                            ToolPathType type,bool isReverse,
                                            
vector<shared_ptr<OffsetNode>>&nodes)
{
    for (size_t i = 0; i < lNodes.size(); ++i) {
        if(!lNodes[i]._offsetNode->_vLoopData.empty() ||
                !lNodes[i]._offsetNode->_vOffsetNode.empty()){
            lNodes[i]._offsetNode->_geneType = type;
            auto n = lNodes[i]._offsetNode;
            n->_geneType = type;
            if(isReverse){
                std::reverse(n->_vLoopData.begin(),n->_vLoopData.end());
            }
            nodes.emplace_back(n);
        }
    }
}
void TrajectorySolve::AddRegionsToPathNode(OffsetParam param,
                                           vector<GeomArea>&areas,
                                           NodeForm& nodeForm)
{
    for(auto &d : areas){
        if(d.BndLoop().PastOffsetNum() <
                param._offsetUnit.OffsetNumber())
            continue;
        shared_ptr<OffsetNode> bnode = make_shared<OffsetNode>(                    BndOffsetPath,vector<DefLoop>{d.BndLoop()});
        shared_ptr<OffsetNode> inode = make_shared<OffsetNode>(
                    IslOffsetPath,d.IslLoop());
        nodeForm._bOftNodes.emplace_back(bnode);
        nodeForm._iOftNodes.emplace_back(inode);
        d.SetValidMatch(false);
    }
}
void TrajectorySolve::GetLoopOffsetNode(OffsetParam param,
                                        const DefLoop& loop,
                                        shared_ptr<OffsetNode>& node)
{
    if((!param._offsetUnit.IsFixedOffset()) ||
            (param._offsetUnit.IsFixedOffset() &&
             (loop.PastOffsetNum() < param._offsetUnit.OffsetNumber()))){
        param._offsetUnit.SetOffsetNumber(
                    param._offsetUnit.OffsetNumber() -
                    loop.PastOffsetNum());
        if(param._offsetUnit.IsFixedOffset() &&
                param._offsetUnit.OffsetNumber() < 0)
            return;
        ExicuteProgram proexi;
        proexi.LoopOffsetNodeResult(loop,param,node);
    }
}
void TrajectorySolve::OperateNodeForm(OffsetParam param,
                                      NodeForm& nodeForm,
                                      shared_ptr<OffsetNode>& pathNode)
{
    param._offsetUnit.SetAblePreProcess(true);
    if(pathNode == nullptr){
        pathNode = make_shared<OffsetNode>();
    }
    for(auto& it : nodeForm._bOftNodes){
        it->_geneType = BndOffsetPath;
        pathNode->_vOffsetNode.emplace_back(it);
    }
    RootNodeSolve nodsol;
    for(auto& d : nodeForm._bLocLoops){
#if 0
        string str = OperateOftString().LoopElementString(d);
        std::cout<<str<<endl;
#endif
        shared_ptr<OffsetNode> node = make_shared<OffsetNode>();
        GetLoopOffsetNode(param,d,node);
        nodsol.SetNodeToolPathType(BndOftLocalPath,node);
        node->_vLoopData.insert(node->_vLoopData.begin(),d);
        pathNode->_vOffsetNode.emplace_back(node);
    }
    for(auto& d : nodeForm._biMerLoops){
#if 0
        string str = OperateOftString().LoopElementString(d);
        std::cout<<str<<endl;
#endif
        shared_ptr<OffsetNode> node = make_shared<OffsetNode>();
        if(d._loopLabel._isLoopOffset)
            GetLoopOffsetNode(param,d,node);
        nodsol.SetNodeToolPathType(BndIslMerPath,node);
        node->InsertLoop(0,d);
        pathNode->_vOffsetNode.emplace_back(node);
    }
    for(auto& d : nodeForm._iiMerLoops){#if 0
        string str = OperateOftString().LoopElementString(d);
        std::cout<<str<<endl;
#endif
        shared_ptr<OffsetNode> node = make_shared<OffsetNode>();
        GetLoopOffsetNode(param,d,node);
        nodsol.SetNodeToolPathType(IslIslMerPath,node);
        node->_vLoopData.insert(node->_vLoopData.begin(),d);
        pathNode->_vOffsetNode.emplace_back(node);
    }
    for(auto& d : nodeForm._iOftNodes){
        pathNode->_vOffsetNode.emplace_back(d);
    }
    for(auto& d : nodeForm._iLocLoops){
#if 0
        string str = OperateOftString().LoopElementString(d);
        std::cout<<str<<endl;
#endif
        shared_ptr<OffsetNode> node = make_shared<OffsetNode>();
        GetLoopOffsetNode(param,d,node);
        nodsol.SetNodeToolPathType(IslOftLocalPath,node);
        node->_vLoopData.insert(node->_vLoopData.begin(),d);
        pathNode->_vOffsetNode.emplace_back(node);
    }
}