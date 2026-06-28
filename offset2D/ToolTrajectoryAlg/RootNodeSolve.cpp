#include "RootNodeSolve.h"
#include "GeometryCalculate/RelativeContain/LimitCoord.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
void RootNodeSolve::PathNodeFirstLoop(const shared_ptr<OffsetNode> &pathNode,
                                      std::vector<DefLoop>& loops)
{
    if(pathNode == nullptr){return;}
    if(!pathNode->Loops().empty()){
        loops.emplace_back(pathNode->IndexLoop(0));loops;
    }
    for(const auto& d : pathNode->_nodes){
        if(!d->Loops().empty()){
            loops.emplace_back(d->IndexLoop(0));
        }
    }
}
void RootNodeSolve::DeleteSmallPathLoop(const double l,shared_ptr<OffsetNode> &pathNode)
{
    if(pathNode == nullptr)
        return;
    vector<size_t>vId;
    for (size_t i = 0; i < pathNode->_loops.size(); i++){
        DefCircle circle;
        LimitCoord2D::LoopMinCircumcircle(pathNode->_loops[i],circle);
        if(circle._cirR < l * 0.9){ /// 乘以0.9 处理计算误差
            vId.push_back(i);
        }
    }
    ElemCalculate().DeleteIndexLoop(vId,pathNode->_loops);
    for (size_t i = 0; i < pathNode->_nodes.size(); i++){
        DeleteSmallPathLoop(l,pathNode->_nodes[i]);
    }
}
void RootNodeSolve::DelEmptyOffsetNode(std::shared_ptr<ofts::OffsetNode>&rootNode)
{
#if DebugModel
    return;
#endif
    if(rootNode == nullptr){return;}
    size_t i = 0;
    while (i < rootNode->_loops.size()) {
        if(rootNode->_loops[i].m_vElem.empty()){
            rootNode->_loops.erase(rootNode->_loops.begin() + i);
            continue;
        }
        i ++;
    }
    for (size_t i = 0; i < rootNode->_nodes.size(); ++i){
        DelEmptyOffsetNode(rootNode->_nodes[i]);
    }
}
void RootNodeSolve::OffsetNodeLoopNum(const std::shared_ptr<ofts::OffsetNode>&rootNode,
                                      int &num)
{
    num = rootNode->_loops.size();
    for(int i = 0;i < rootNode->_nodes.size();i++){
        int num_ = 0;
        OffsetNodeLoopNum(rootNode->_nodes[i],num_);
        num = num + num_;
    }
}
void RootNodeSolve::SetOffsetNodeAreaId(std::shared_ptr<ofts::OffsetNode> &pathNode,
                                        int &index)
{
    if(pathNode == nullptr)
        return;
    pathNode->SetId(index);
    for (int i = 0; i < pathNode->_nodes.size(); ++i) {
        index ++;
        SetOffsetNodeAreaId(pathNode->_nodes[i],index);
    }
}
void RootNodeSolve::SetNodeToolPathType(Ofte::ToolPathType type,
                                        std::shared_ptr<ofts::OffsetNode>&node)
{
    if(node == nullptr)
        return;
    node->_geneType = type;
    for(auto& it : node->_nodes)
        SetNodeToolPathType(type,it);
}
void RootNodeSolve::AddNodeToNodes(std::shared_ptr<ofts::OffsetNode> &node,
                                   Ofte::ToolPathType pathType,bool resetNode,
                                   std::vector<std::shared_ptr<ofts::OffsetNode>>&vNode)
{
    if(!node->_loops.empty() || !node->_nodes.empty()){
        std::shared_ptr<ofts::OffsetNode>node_ = node;
        node_->_geneType = pathType;
        vNode.push_back(node_);
    }
    if(resetNode)
        node = make_shared<ofts::OffsetNode>();
}
void RootNodeSolve::PathRootNodeLoop(const std::shared_ptr<ofts::OffsetNode>&rootNode,
                                     std::vector<ofts::DefLoop>& vResLoop)
{
    if(rootNode == nullptr)
        return;
    vResLoop.clear();
    for(auto& it : rootNode->_loops)
        vResLoop.push_back(it);
    for(int i = 0; i < rootNode->_nodes.size();i ++){
        std::vector<ofts::DefLoop> vLoop;
        PathRootNodeLoop(rootNode->_nodes[i],vLoop);
        for(auto& it : vLoop)
            vResLoop.push_back(it);
    }
}
void RootNodeSolve::RootNodeAreaLoop(const std::shared_ptr<ofts::OffsetNode> &rootNode,
                                     std::vector<std::vector<ofts::DefLoop>>&vAreaLoop)
{
    if(rootNode == nullptr || (rootNode->_loops.empty() &&
                               rootNode->_nodes.empty())){
        return;
    }
    vAreaLoop.push_back(rootNode->_loops);
    for (int i = 0; i < rootNode->_nodes.size(); ++i)
        RootNodeAreaLoop(rootNode->_nodes[i],vAreaLoop);
}
void RootNodeSolve::SetOffsetNodeZ(const double& zValue,
                                   std::shared_ptr<ofts::OffsetNode> &rootNode)
{
    if(rootNode == nullptr)
        return;
    ElemCalculate().SetArrayLoopElementZValue(zValue,rootNode->_loops);
    for(int i = 0; i < rootNode->_nodes.size();i ++){
        if(rootNode->_nodes[i]->_loops.empty() &&
                rootNode->_nodes[i]->_nodes.empty()){
            continue;
        }
        SetOffsetNodeZ(zValue,rootNode->_nodes[i]);
    }
}
void RootNodeSolve::OffsetNodeCircleArcTrans(bool isTranToArc,
                                             std::shared_ptr<ofts::OffsetNode> &rootNode)
{
#if DebugModel
    return;
#endif
    if(rootNode == nullptr)
        return;
    GeometryLogic golg;
    for (int lo = 0; lo < rootNode->_loops.size(); ++lo) {
        golg.CircleArcLoopTransform(isTranToArc,rootNode->_loops[lo]);
    }
    for(int i = 0; i < rootNode->_nodes.size();i ++)
        OffsetNodeCircleArcTrans(isTranToArc,rootNode->_nodes[i]);
}
void RootNodeSolve::OffsetNodeArcPrecision(double arcEndpre,double R,
                                           double H,bool isDecNum,
                                           int number,std::shared_ptr<ofts::OffsetNode>&rootNode)
{
#if DebugModel
    return;
#endif
    if(rootNode == nullptr)
        return;
    ElemCalculate elecal;
    for (int lo = 0; lo < rootNode->_loops.size(); ++lo) {
        elecal.InitialLoopArcRadius(rootNode->_loops[lo]);
        LoopArcPrecision(arcEndpre,R,H,isDecNum,number,
                         rootNode->_loops[lo]);
    }
    for(int i = 0; i < rootNode->_nodes.size();i ++){
        OffsetNodeArcPrecision(arcEndpre,R,H,isDecNum,number,
                               rootNode->_nodes[i]);
    }
}
void RootNodeSolve::LoopArcPrecision(double arcEndpre,double R,
                                     double H,bool isDecNum,
                                     int number,ofts::DefLoop& loop)
{
#if DebugModel
    return;
#endif
    ElemCalculate elecal;
    for (int i = 0; i < loop.Size(); ++i) {
        if(loop.IndexElem(i).Type() != ARCTYPE)
            continue;
        DefArc arc = loop.IndexElem(i).GetArc();
        if(elecal.IsValidGeomArc(arc,arcEndpre,R,H,isDecNum,number)){
            continue;
        }
        DefElem ele_(LINETYPE,DefSeg(arc.GetSp(),arc.GetEp()));
        loop.SetIdElem(ele_,i);
    }
}
void RootNodeSolve::SetLoopTreeDire(std::shared_ptr<ofts::LoopTree>& node)
{
    bool iscw = LoopDirection().IsClockwiseLoop(node->GetLoop());
    node->SetCW(iscw);
    for(auto& d : node->_nodes){
        SetLoopTreeDire(d);
    }
}
std::vector<ofts::GeomArea> RootNodeSolve::LoopTreesRegions(const std::shared_ptr<ofts::LoopTree>& node)
{
    std::vector<ofts::GeomArea> regions;
    auto& chs = node->_nodes;
    std::vector<ofts::DefLoop> ls;ls.reserve(chs.size());
    for(auto& d : chs){ls.emplace_back(d->GetLoop());}
    ofts::GeomArea region(node->GetLoop(),ls);
    regions.emplace_back(region);
    for (size_t i = 0; i < chs.size(); i++){
        auto& ch = chs[i];  //ch 与ch1 之间区域的填充区域为0
        for (size_t j = 0; j < ch->_nodes.size(); j++){
            auto& ch1 = ch->_nodes[j];
            auto rs = LoopTreesRegions(ch1);
            regions.insert(regions.end(),rs.begin(),rs.end());
            //std::cout<<"";
        }
    }
    return regions;
}
void RootNodeSolve::SubNodesToZezo(std::shared_ptr<ofts::LoopTree>& node)
{
    //仅修改node 中无效的子节点,不会修改node 本身
    auto ResetChildes = [](std::shared_ptr<ofts::LoopTree>& node){
        /*std::vector<std::pair<int,int>>ids;
        if(node->Id() == 1){
            int tem = 1;
        }*/
        bool invalid = false;
        std::vector<std::shared_ptr<ofts::LoopTree>>nodes;
        for(auto& ch : node->_nodes){
            if(ch->WindNum() == 0){continue;}
            ch->SetValid(false);
            invalid = true;
            //ids.push_back(std::make_pair(ch->_id,ch->WindNum()));
            for(auto& c : ch->_nodes){
                nodes.emplace_back(c);  //添加子节点的子节点
            }
        }
        if(!invalid){return false;}
        auto& chs = node->_nodes;
        chs.erase(std::remove_if(chs.begin(),chs.end(),[](
                                 std::shared_ptr<ofts::LoopTree>& c){
                      return !c->IsValid();}),chs.end());
        chs.reserve(chs.size() + nodes.size());
        for(auto& d : nodes){chs.emplace_back(d);}
        return true;
    };
    //重置node 中无效子节点,直至所有子节点均有效
    while (true){
        bool isReset = ResetChildes(node);
        if(!isReset){break;}
    }
    //std::cout<<"";
}
std::vector<ofts::GeomArea> RootNodeSolve::LoopTreeToRegions(const std::shared_ptr<ofts::LoopTree>& itree,
                                                             bool no_zezo)
{
    auto tree = itree;
    tree->InitParent();
    tree->InitWindNum();
    if(no_zezo){
        //递归设置node 中的子节点,使得每一子节点均有效
        auto ResetTree = [&](std::shared_ptr<ofts::LoopTree>& node,auto&& self)->void{
            SubNodesToZezo(node);
            auto& chs = node->_nodes;
            //这里不会导致索引超出范围,因为先重置子节点,再遍历子节点
            for (size_t i = 0; i < chs.size(); i++){
                auto& ch = chs[i];
                auto& chs1 = ch->_nodes;
                //因为这里node 子节点内部填充区域已经是0,即下一有效区域应该
                //是node 子节点的子节点内部,即应该处理node 子节点的子节点
                for (size_t j = 0; j < chs1.size(); j++){
                    auto& ch1 = chs1[j];
                    self(ch1,self);
                    //std::cout<<"";
                }
            }
        };
        ResetTree(tree,ResetTree);
        std::vector<ofts::GeomArea> regions;
        regions.reserve(tree->_nodes.size() * 2);
        regions = LoopTreesRegions(tree);
        return regions;
    }
    std::vector<std::shared_ptr<ofts::LoopTree>> trees{tree};

    //递归查找环绕数为正的子节点,当前子节点有效时即返回
    auto GetValidChs = [&](std::shared_ptr<ofts::LoopTree>& node,auto&& self)->
            std::vector<std::shared_ptr<ofts::LoopTree>>{
        std::vector<std::shared_ptr<ofts::LoopTree>> nodes;
        nodes.reserve(node->_nodes.size());
        for(auto& ch : node->_nodes){
            if(ch->WindNum() > 0){
                nodes.emplace_back(ch);continue;
            }
            auto ns = self(ch,self);
            nodes.insert(nodes.end(),ns.begin(), ns.end());
        }
        return nodes;
    };
    //使得trees 中每一根点填充区域为正(不修改其子节点)
    for (size_t i = 0; i < trees.size(); i++){
        auto& node = trees[i];
        if(node->WindNum() > 0){continue;}
        auto ns = GetValidChs(node,GetValidChs);
        trees.insert(trees.end(),ns.begin(), ns.end());
    }
    trees.erase(std::remove_if(trees.begin(),trees.end(),[](
                               std::shared_ptr<ofts::LoopTree>& d){
                    return d->WindNum() < 1;}),trees.end());
    //设置node 子节点的子节点填充区域为正,不会修改node 及其子节点
    auto SetRootValid = [&](std::shared_ptr<ofts::LoopTree>& node,auto&& self)->void{
        SubNodesToZezo(node);     //先设置有效子节点
        auto& ns = node->_nodes;  //ns 填充区域已经为0
        for (size_t i = 0; i < ns.size(); i++){
            auto& n = ns[i];
            auto& ns1 = n->_nodes;
            //处理node 子节点的子节点n1(不包括n1 子节点),使得所有n1 填充区域为正
            for (size_t j = 0; j < ns1.size(); j++) {
                auto& n1 = ns1[j];  //node 子节点的子节
                if(n1->WindNum() > 0){continue;}  //不用处理
                auto ns2 = GetValidChs(n1,GetValidChs);
                ns1.insert(ns1.end(),ns2.begin(), ns2.end());
            }
            ns1.erase(std::remove_if(ns1.begin(),ns1.end(),[](
                                     std::shared_ptr<ofts::LoopTree>& d){
                          return d->WindNum() < 1;}),ns1.end());
            for (size_t j = 0; j < ns1.size(); j++) {
                auto& n1 = ns1[j];
                self(n1,self);
                //std::cout<<"";
            }
        }
    };
    for (size_t i = 0; i < trees.size(); i++){
        auto& node = trees[i];
        SetRootValid(node,SetRootValid);
    }
    std::vector<ofts::GeomArea> regions;
    for (size_t i = 0; i < trees.size(); i++){
        auto& node = trees[i];
        auto rs = LoopTreesRegions(node);
        regions.insert(regions.end(),rs.begin(),rs.end());
    }
    return regions;
}
