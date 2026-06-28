#include "TrajectorySolve.h"
#include"../CommonFile/Header.h"
#include"../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
using namespace std;
using namespace OffsetStruct;
void TrajectorySolve::SetNodeLoopSpecifyOrigin(OffsetParam param,
                                               shared_ptr<OffsetNode>& pathNode)
{
    if(pathNode == nullptr){return;}
    int oftNumLim = 2;
    if(param._pathTech.IsAddOriginLoop()){
        oftNumLim = 1;
    }
    for(auto& d : pathNode->_loops){
        if(d.PastOftNum() < oftNumLim){
            d.SetCloseMode(true);
        }else{
            d.SetCloseMode(false);
        }
    }
    for (auto& d : pathNode->_nodes){
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
    ///这里的curBndLoop 可能是原始的,也可能是合并后处理过的.
    if(iloops.empty() || !param._pathTech.IsBoundaryOffset()){return;}
    if(isExistClip){
        bNode->_geneType = BndOffsetPath;
        if(!bNode->_loops.empty() || !bNode->_nodes.empty()){
            nodeForm._bOftNodes.emplace_back(bNode);
        }
        bNode = make_shared<OffsetNode>();
    }
    bNode->AddLoop(bloop);
}
void TrajectorySolve::DeleteLoopNotInLoops(const vector<DefLoop>& loops,
                                           vector<DefLoop>& loops_)
{
    ///删除loops_中不在loops 内的环；
    if(loops.empty()){loops_.clear();return;}
    vector<size_t>ids;
    ElemCalculate numcal;
    for (size_t i = 0; i < loops_.size(); ++i) {
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
    for (size_t i = 0; i < loops.size(); ++i) {
        shared_ptr<OffsetNode> node = loops[i]._label.m_hasExiNode;
        if(node == nullptr){
            node = make_shared<OffsetNode>();
        }
        DefLoop loop_ = loops[i];
        if(param._pathTech.IsAddOriginLoop() &&
                loop_.PastOftNum() == 0 &&
                area._vOriIslLoop.size() == loops.size()){
            loop_ = area._vOriIslLoop[i];
            isAdd = true;
        }
        else if(area.IsAddIslLoop()){
            isAdd = true;
        }
        LoopNode node_(loops[i],node);
        if(isAdd){
            node_._offsetNode->AddLoop(loop_);
        }
        nodes.emplace_back(node_);
    }
}
void TrajectorySolve::AddLoopNodeToPathNode(const vector<LoopNode>&lNodes,
                                            ToolPathType type,bool isReverse,
                                            vector<shared_ptr<OffsetNode>>&nodes)
{
    for (size_t i = 0; i < lNodes.size(); ++i) {
        if(!lNodes[i]._offsetNode->_loops.empty() ||
                !lNodes[i]._offsetNode->_nodes.empty()){
            lNodes[i]._offsetNode->_geneType = type;
            auto n = lNodes[i]._offsetNode;
            n->_geneType = type;
            if(isReverse){
                std::reverse(n->_loops.begin(),n->_loops.end());
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
        if(d.BndLoop().PastOftNum() <
                param._offsetUnit.OffsetNumber())
            continue;
        shared_ptr<OffsetNode> bnode = make_shared<OffsetNode>(
                    BndOffsetPath,vector<DefLoop>{d.BndLoop()});
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
             (loop.PastOftNum() < param._offsetUnit.OffsetNumber()))){
        param._offsetUnit.SetOffsetNumber(param._offsetUnit.OffsetNumber() - loop.PastOftNum());
        if(param._offsetUnit.IsFixedOffset() && param._offsetUnit.OffsetNumber() < 0){return;}
        ExicuteProgram().LoopOffsetNodeResult(loop,param,node);
    }
}
void TrajectorySolve::OperateNodeForm(OffsetParam param,
                                      NodeForm& nodeForm,
                                      shared_ptr<OffsetNode>& pathNode)
{
    param._offsetUnit.SetAblePreProcess(true);
    if(pathNode == nullptr){pathNode = make_shared<OffsetNode>();}
    for(auto& it : nodeForm._bOftNodes){
        it->_geneType = BndOffsetPath;
        pathNode->_nodes.emplace_back(it);
    }
    RootNodeSolve nodsol;
    for(auto& d : nodeForm._bLocLoops){
#if 0
        string str = OperateOftString().LoopElementString(d);
        std::cout<<str<<endl;
#endif
        shared_ptr<OffsetNode> node;
        GetLoopOffsetNode(param,d,node);
        nodsol.SetNodeToolPathType(BndOftLocalPath,node);
        node->_loops.insert(node->_loops.begin(),d);
        pathNode->_nodes.emplace_back(node);
    }
    for(auto& d : nodeForm._biMerLoops){
        shared_ptr<OffsetNode> node;
        if(d._label._isOffset){
            //DisplayGeom().DisplayLoop(d,_colors.back(),1,true,true);
            GetLoopOffsetNode(param,d,node);
        }
#if 0
        bool iscw = LoopDirection().IsClockwiseLoop(d);
        string str = OperateOftString().LoopElementString(d);
        std::cout<<str<<endl;
        DisplayGeom().DisplayLoop(d,_colors.back(),1,true,true);
        auto ls = node->AllLoops();
        DisplayGeom().DisplayLoops(ls,_colors[1],1);
#endif
        nodsol.SetNodeToolPathType(BndIslMerPath,node);
        node->InsertLoop(0,d);
        pathNode->_nodes.emplace_back(node);
    }
    for(auto& d : nodeForm._iiMerLoops){
#if 0
        string str = OperateOftString().LoopElementString(d);
        std::cout<<str<<endl;
#endif
        shared_ptr<OffsetNode> node;
        GetLoopOffsetNode(param,d,node);
        nodsol.SetNodeToolPathType(IslIslMerPath,node);
        node->_loops.insert(node->_loops.begin(),d);
        pathNode->_nodes.emplace_back(node);
    }
    for(auto& d : nodeForm._iOftNodes){
        pathNode->_nodes.emplace_back(d);
    }
    for(auto& d : nodeForm._iLocLoops){
        shared_ptr<OffsetNode> node;
        GetLoopOffsetNode(param,d,node);
#if 0
        bool iscw = LoopDirection().IsClockwiseLoop(d);
        string str = OperateOftString().LoopElementString(d);
        std::cout<<str<<endl;
        DisplayGeom().DisplayLoop(d,_colors.back(),1,true,true);
        auto ls = node->AllLoops();
        DisplayGeom().DisplayLoops(ls,_colors[1],1);
#endif
        nodsol.SetNodeToolPathType(IslOftLocalPath,node);
        node->_loops.insert(node->_loops.begin(),d);
        pathNode->_nodes.emplace_back(node);
    }
}
