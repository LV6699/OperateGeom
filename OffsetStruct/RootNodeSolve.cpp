#include "RootNodeSolve.h"
#if DebugModel
#pragma optimize("", off)
#endif
void RootNodeSolve::DeleteRootNodeEmptyLoop(shared_ptr<OffsetNode>&rootNode)
{
#if DebugModel
    return;
#endif
    if(rootNode == nullptr)
        return;
    int lo = 0;
    while (lo < rootNode->_vLoopData.size()) {
        if(rootNode->_vLoopData[lo].m_vElem.empty()){
            rootNode->_vLoopData.erase(rootNode->_vLoopData.begin() + lo);
            continue;
        }
        lo ++;
    }
    for (int i = 0; i < rootNode->_vOffsetNode.size(); ++i)
        DeleteRootNodeEmptyLoop(rootNode->_vOffsetNode[i]);
}
void RootNodeSolve::SetLoopIslEle(DefLoop& loop,int start,int end)
{
    if(start < 0 || end > loop.m_vElem.size() - 1)
        return;
    for (int i = start; i < end + 1; ++i)
        loop.m_vElem[i]._elemLabel._isIslElem = true;
}
void RootNodeSolve::GetRootNodeLoopNum(const shared_ptr<OffsetNode>&rootNode,
                                       int &num)
{
    num = rootNode->_vLoopData.size();
    for(int i = 0;i < rootNode->_vOffsetNode.size();i++){
        int num_ = 0;
        GetRootNodeLoopNum(rootNode->_vOffsetNode[i],num_);
        num = num + num_;
    }
}
void RootNodeSolve::SetPathNodeAreaIndex(shared_ptr<OffsetNode> &pathNode,
                                         int &index)
{
    if(pathNode == nullptr)
        return;
    pathNode->SetNodeIndex(index);
    for (int i = 0; i < pathNode->_vOffsetNode.size(); ++i) {
        index ++;
        SetPathNodeAreaIndex(pathNode->_vOffsetNode[i],index);
    }
}
void RootNodeSolve::SetNodeToolPathType(ToolPathType type,
                                        shared_ptr<OffsetNode>&node)
{
    if(node == nullptr)
        return;
    node->_geneType = type;
    for(auto& it : node->_vOffsetNode)
        SetNodeToolPathType(type,it);}
void RootNodeSolve::AddNodeToNodes(shared_ptr<OffsetNode> &node,
                                   ToolPathType pathType,bool resetNode,
                                   vector<shared_ptr<OffsetNode>>&vNode)
{
    if(!node->_vLoopData.empty() || !node->_vOffsetNode.empty()){
        shared_ptr<OffsetNode>node_ = node;
        node_->_geneType = pathType;
        vNode.push_back(node_);
    }
    if(resetNode)
        node = make_shared<OffsetNode>();
}
void RootNodeSolve::PathRootNodeLoop(const shared_ptr<OffsetNode>&rootNode,
                                     vector<DefLoop>& vResLoop)
{
    if(rootNode == nullptr)
        return;
    vResLoop.clear();
    for(auto& it : rootNode->_vLoopData)
        vResLoop.push_back(it);
    for(int i = 0; i < rootNode->_vOffsetNode.size();i ++){
        vector<DefLoop> vLoop;
        PathRootNodeLoop(rootNode->_vOffsetNode[i],vLoop);
        for(auto& it : vLoop)
            vResLoop.push_back(it);
    }
}
void RootNodeSolve::GetRootNodeAreaLoop(const shared_ptr<OffsetNode> &rootNode,
                                        vector<vector<DefLoop>>&vAreaLoop)
{
    if(rootNode == nullptr || (rootNode->_vLoopData.empty() &&
                               rootNode->_vOffsetNode.empty())){
        return;
    }
    vAreaLoop.push_back(rootNode->_vLoopData);
    for (int i = 0; i < rootNode->_vOffsetNode.size(); ++i)
        GetRootNodeAreaLoop(rootNode->_vOffsetNode[i],vAreaLoop);
}
void RootNodeSolve::SetRootNodeZValue(const double& zValue,
                                      shared_ptr<OffsetNode> &rootNode)
{
    if(rootNode == nullptr)
        return;
    ElemCalculate().SetArrayLoopElementZValue(zValue,rootNode->_vLoopData);
    for(int i = 0; i < rootNode->_vOffsetNode.size();i ++){
        if(rootNode->_vOffsetNode[i]->_vLoopData.empty() &&
                rootNode->_vOffsetNode[i]->_vOffsetNode.empty()){
            continue;
        }
        SetRootNodeZValue(zValue,rootNode->_vOffsetNode[i]);
    }
}
void RootNodeSolve::PathRootNodeSmooth(OffsetParam offsetPara,bool isAddOriLoop,
                                       shared_ptr<OffsetNode> &rootNode)
{
    if(rootNode == nullptr)        return;
    PathSmooth patsmo;
    patsmo.ArrayLoopSmooth(offsetPara,isAddOriLoop,rootNode->_vLoopData);
    for(int i = 0; i < rootNode->_vOffsetNode.size();i ++){
        PathRootNodeSmooth(offsetPara,isAddOriLoop,rootNode->_vOffsetNode[i]);
    }
}
void RootNodeSolve::NodePathCircleArcTranslate(bool isTranToArc,
                                               shared_ptr<OffsetNode> &rootNode)
{
#if DebugModel
    return;
#endif
    if(rootNode == nullptr)
        return;
    GeometryLogic golg;
    for (int lo = 0; lo < rootNode->_vLoopData.size(); ++lo) {
        golg.CircleArcLoopTransform(isTranToArc,rootNode->_vLoopData[lo]);
    }
    for(int i = 0; i < rootNode->_vOffsetNode.size();i ++)
        NodePathCircleArcTranslate(isTranToArc,rootNode->_vOffsetNode[i]);
}
void RootNodeSolve::PathNodeArcPrecision(double arcEndpre,double R,
                                         double H,bool isDecNum,
                                         int
                                         number,shared_ptr<OffsetNode>&rootNode)
{
#if DebugModel
    return;
#endif
    if(rootNode == nullptr)
        return;
    ElemCalculate elecal;
    for (int lo = 0; lo < rootNode->_vLoopData.size(); ++lo) {
        elecal.InitialLoopArcRadius(rootNode->_vLoopData[lo]);
        ProcessLoopArcPrecision(arcEndpre,R,H,isDecNum,number,
                                rootNode->_vLoopData[lo]);
    }
    for(int i = 0; i < rootNode->_vOffsetNode.size();i ++){
        PathNodeArcPrecision(arcEndpre,R,H,isDecNum,number,
                             rootNode->_vOffsetNode[i]);
    }
}
void RootNodeSolve::ProcessLoopArcPrecision(double arcEndpre,double R,
                                            double H,bool isDecNum,
                                            int number,DefLoop& loop)
{
#if DebugModel
    return;
#endif
    ElemCalculate elecal;
    for (int i = 0; i < loop.ElemNum(); ++i) {
        if(loop.IndexElem(i).ElemeType() != ARCTYPE)
            continue;
        DefArc arc = loop.IndexElem(i).ElemArc();
        if(elecal.IsValidGeomArc(arc,arcEndpre,R,H,isDecNum,number)){
            continue;
        }
        DefElem ele_(LINETYPE,DefSeg(arc.ArcStartPt(),arc.ArcEndPt()));
        loop.SetIndexElem(ele_,i);    }
}
void RootNodeSolve::PathNodeFirstLoop(const shared_ptr<OffsetNode> &pathNode,
                                      std::vector<DefLoop>&vLoop)
{
    if(!pathNode->ArrayLoop().empty()){
        vLoop.push_back(pathNode->IndexLoop(0));
        return;
    }
    for(const auto& d : pathNode->_vOffsetNode){
        if(!d->ArrayLoop().empty()){
            vLoop.push_back(d->IndexLoop(0));
        }
    }
}
void RootNodeSolve::DeleteSmallPathLoop(const double l,
                                        shared_ptr<OffsetNode> &pathNode)
{
    if(pathNode == nullptr)
        return;
    vector<int>vId;
    for (size_t i = 0; i < pathNode->_vLoopData.size(); i++){
        DefCircle circle;
        LimitCoordinate().LoopMinCircumcircle(pathNode->_vLoopData[i],circle);
        if(circle._cirR < l * 0.9){ /// 乘以0.9处理计算误差
            vId.push_back(i);
        }
    }
    ElemCalculate().DeleteIndexLoop(vId,pathNode->_vLoopData);
    for (size_t i = 0; i < pathNode->_vOffsetNode.size(); i++){
        DeleteSmallPathLoop(l,pathNode->_vOffsetNode[i]);
    }
    
}
void RootNodeSolve::ReverseOffsetNode(shared_ptr<OffsetNode> &pathNode)
{
    if(pathNode == nullptr){
        return;
    }
    std::reverse(pathNode->_vLoopData.begin(),pathNode->_vLoopData.end());
    for(auto& d : pathNode->_vOffsetNode){
        ReverseOffsetNode(d);
    }
}
