#include "MergeLoopSolve.h"
#if DebugModel
#pragma optimize("", off)
#endif
///处理vIslLoop中的环在vLoop中环的内部的情况,如果在内部,则
///vLoop中环与vIslLoop中的环组成新的边界岛屿配对环 .
///对于所有岛屿合并外环 ,其可能是新的边界环,如果其完全包含原始岛屿环 ,则其与
///所包含的岛屿环组成新的 vMatchLoop成员;
/// LoopsUnionBndIslMatch
bool MergeLoopSolve::IsInsideOtherArea(const vector<GeomArea>& vBILoop,
                                       const DefLoop& loop,int id)
{
    double coinPre = PreErr5_10,
            tanPre = PreErr5_10,intPre = PreErr5_10;
    ElemCalculate numcal;CheckGeomInt cheint;
    Point p1 = numcal.ElementEndpoint(loop.IndexElem(0),false),
            p2 = numcal.ElementEndpoint(vBILoop[id].BndLoop().
                                        IndexElem(0),false);
    DefElem ele(LINETYPE,DefSeg(p1,p2));
    for (int i = 0; i < vBILoop.size(); ++i) {
        if(i == id)
            continue;
        bool isIntersect = false;
        cheint.IsLoopElemSpecifyIntersect(vBILoop[i].BndLoop(),
                                          ele,coinPre,tanPre,
                                          intPre,isIntersect);
        if(isIntersect)
            return true;
    }
    return false;
}
void MergeLoopSolve::HandleIslandMergeInside(vector<DefLoop>& vLoop,
                                             vector<DefLoop>& vIslLoop,
                                             vector<LoopNode>& vIslNode,
                                             vector<GeomArea>& vBILoop)
{
    MultipleInclude mulinc;
    GeomContain gocon;ElemCalculate elecal;
    vector<GeomArea>vBILoop_;
#if 0
    OperateOftString opeoft;
    string str = opeoft.ArrayLoopString(vLoop);
    std::cout<<str<<endl;
#endif
    double intPre = PreErr5_8,coinPre = PreErr5_8,incPre = PreErr5_8;
    mulinc.OrganizeLoopToArea(vLoop,intPre,coinPre,incPre,vBILoop_);
    for(auto& d : vBILoop_){
        if(!d.IslLoop().empty()){
            d.SetAddIslLoop(true);
        }
    }
    vLoop.clear();
    for (int b = 0; b < vBILoop_.size(); ++b) {
        vector<int>vId_;
        vector<DefLoop>vLoop_;
        vBILoop_[b].SetAddBndLoop(true);
        for (int i = 0; i < vIslLoop.size(); ++i) {
            if(vIslLoop[i].IsEmpty())
                continue;
            bool isInclude = false;            gocon.IsLoopContainIntersectLoop(vBILoop_[b]._bndLoop,
                                             vIslLoop[i],PreErr_5,
                                             PreErr_5,PreErr_10,
                                             false,isInclude);
            if(!isInclude || IsInsideOtherArea(vBILoop_,vIslLoop[i],b))
                continue;
            vIslLoop[i].SetLoopType(WITHINPROFILE);
            vLoop_.push_back(vIslLoop[i]);
            vId_.push_back(i);
        }
        if(vLoop_.empty())
            continue;
        elecal.DeleteIndexLoop(vId_,vIslLoop);
        for(auto& lo : vLoop_){
            int n = 0;
            DefElem ele = lo.IndexElem(0);
            while (n < vIslNode.size()) {
                DefElem ele_ = vIslNode[n].m_loop.IndexElem(0);
                bool isSame = false;
                isSame = elecal.IsIdenticalElement(ele,ele_,isSame);
                if(isSame && (!vIslNode[n]._offsetNode->IsLoopsEmpty() ||
                              !vIslNode[n]._offsetNode->IsNodeEmpty())){
                    vIslNode[n]._offsetNode->PushBackLoop(lo);
                    lo._loopLabel.m_hasExiNode = vIslNode[n]._offsetNode;
                    vIslNode.erase(vIslNode.begin() + n);
                    break;
                }
                n ++;
            }
        }
        for(auto& d : vLoop_){
            vBILoop_[b]._vIslLoop.push_back(d);
        }
        vBILoop_[b]._bndLoop.SetLoopType(OUTSIDEPROFILE);
    }
    for (int b = 0; b < vBILoop_.size(); ++b) {
        if(vBILoop_[b].IslLoop().empty()){
            vLoop.push_back(vBILoop_[b].BndLoop());
        }
        else{
            vBILoop.push_back(vBILoop_[b]);
        }
    }
}
void MergeLoopSolve::LoopsUnionBndIslMatchLoop(vector<DefLoop>& vLoop,
                                               vector<DefLoop>& vIslLoop,
                                               vector<LoopNode>& vIslNode,
                                               vector<GeomArea>& vBILoop)
{
    ///vIslLoop 中的环来源于vIslNode.
    GeomContain gocon;ElemCalculate numcal;
    vector<int>vIndex;
    for (int i = 0; i < vLoop.size(); ++i) {
        vector<int>vIndex_;
        vector<DefLoop>vLoop_;
        for (int lo = 0; lo < vIslLoop.size(); ++lo) {
            if(vIslLoop[lo].IsEmpty())
                continue;
            bool resValue = false;
            gocon.IsLoopContainIntersectLoop(vLoop[i],vIslLoop[lo],
                                             PreErr_5,PreErr_5,                                             PreErr_10,false,resValue);
            if(!resValue)
                continue;
            vIslLoop[lo].SetLoopType(WITHINPROFILE);
            vLoop_.push_back(vIslLoop[lo]);
            vIndex_.push_back(lo);
        }
        if(vLoop_.empty())
            continue;
        numcal.DeleteIndexLoop(vIndex_,vIslLoop);
        for(auto& lo : vLoop_){
            int n = 0;
            DefElem ele = lo.IndexElem(0);
            while (n < vIslNode.size()) {
                DefElem ele_ = vIslNode[n].m_loop.IndexElem(0);
                bool isSame = false;
                isSame = numcal.IsIdenticalElement(ele,ele_,isSame);
                if(isSame && (!vIslNode[n]._offsetNode->IsLoopsEmpty() ||
                              !vIslNode[n]._offsetNode->IsNodeEmpty())){
                    vIslNode[n]._offsetNode->PushBackLoop(lo);
                    lo._loopLabel.m_hasExiNode = vIslNode[n]._offsetNode;
                    vIslNode.erase(vIslNode.begin() + n);
                    break;
                }
                n ++;
            }
        }
        vLoop[i].SetLoopType(OUTSIDEPROFILE);
        GeomArea data(vLoop[i],vLoop_);
        data.SetAddBndLoop(true);
        vBILoop.push_back(data);
        vIndex.push_back(i);
    }
    numcal.DeleteIndexLoop(vIndex,vLoop);
}
void MergeLoopSolve::LoopsInsideOutsideLoop(const vector<DefLoop>& vLoop,
                                                vector<DefLoop>& vInsLoop,
                                                vector<DefLoop>& vOutLoop)
{
    if(vLoop.empty())
        return;
    vInsLoop.clear();
    vOutLoop.clear();
    if(vLoop.size() == 1){
        vOutLoop.push_back(vLoop[0]);
        return;
    }
    ElemCalculate numcal;GeoContainSolve consol;
    for (int i = 0; i < vLoop.size(); ++i) {
        if(vLoop[i].IsEmpty())
            continue;
        DefLoop loop;
        vector<DefLoop>vLoop_;
        numcal.ExceptIndexLoop(vLoop,i,loop,vLoop_);
        bool isOutLoop = true;
        DefElem ele = loop.IndexElem(0);
        for (int lo = 0; lo < vLoop_.size(); ++lo) {
            bool isInclude = false;            consol.IsLoopContainCurve(vLoop_[lo],ele,
                                        PreErr5_6,PreErr5_6,
                                        isInclude);
            if(isInclude){
                isOutLoop = false;
                break;
            }
        }
        if(isOutLoop){
            loop.SetLoopType(WITHINPROFILE);
            vOutLoop.push_back(loop);
            continue;
        }
        loop.SetLoopType(OUTSIDEPROFILE);
        vInsLoop.push_back(loop);
    }
}
void MergeLoopSolve::InitialOffsetLoopNode(const vector<DefLoop>& vIslLoop,
                                           OffsetParam offsetPara,
                                           bool isAddLoop,
                                           vector<LoopNode>& vNode,
                                           const GeomArea& biLoop)
{
    for (int i = 0; i < vIslLoop.size(); ++i) {
        shared_ptr<OffsetNode> node = vIslLoop[i]._loopLabel.m_hasExiNode;
        if(node == nullptr)
            node = make_shared<OffsetNode>();
        DefLoop loop_ = vIslLoop[i];
        if(offsetPara._pathTech.IsAddOriginLoop() &&
                loop_.PastOffsetNum() == 0 &&
                biLoop._vOriIslLoop.size() == vIslLoop.size()){
            loop_ = biLoop._vOriIslLoop[i];
            isAddLoop = true;
        }
        else if(biLoop.IsAddIslLoop()){
            isAddLoop = true;
        }
        LoopNode node_(vIslLoop[i],node);
        if(isAddLoop){
            node_._offsetNode->PushBackLoop(loop_);
        }
        vNode.push_back(node_);
    }
}
void 
MergeLoopSolve::IsLoopNodeInArrayLoop(vector<shared_ptr<OffsetNode>>&vIslOftNode
,
                                           const vector<DefLoop>& vIslLoop,
                                           vector<LoopNode>& vIslNode)
{
    int i = 0;
    ElemCalculate numcal;
    while (i < vIslNode.size()) {
        bool isExist = false;
        isExist = numcal.IsLoopInArrayLoop(vIslLoop,vIslNode[i].m_loop);
        if(isExist){
            vIslNode[i]._offsetNode->PushBackLoop(vIslNode[i].m_loop);
            i ++;
            continue;        }
        if(!vIslNode[i]._offsetNode->IsLoopsEmpty()){
            std::reverse(vIslNode[i]._offsetNode->_vLoopData.begin(),
                         vIslNode[i]._offsetNode->_vLoopData.end());
            vIslNode[i]._offsetNode->_geneType = IslOffsetPath;
            vIslOftNode.push_back(vIslNode[i]._offsetNode);
        }
        vIslNode.erase(vIslNode.begin() + i);
    }
}
void MergeLoopSolve::GetLoopInArrayLoop(const vector<DefLoop>& vectLoop,
                                        vector<DefLoop>& vLoop)
{
    ///删除vLoop中不在vectLoop内的环；
    if(vectLoop.empty()){
        vLoop.clear();
        return;
    }
    int i = 0;
    ElemCalculate numcal;
    while (i < vLoop.size()) {
        if(!numcal.IsLoopInArrayLoop(vectLoop,vLoop[i])){
            vLoop.erase(vLoop.begin() + i);
            continue;
        }
        i ++;
    }
}
void MergeLoopSolve::HandleInClockwiseMerLoop(vector<DefLoop>&vLoop)
{
    LoopDirection lodir;
    ElemCalculate numcal;
    for(auto& lo : vLoop){
        RotDir rotDir = lodir.LoopRotateDirection(lo);
        if(rotDir != CLOCKWISE){
            numcal.ReverseLoopDirection(lo);
        }
    }
}