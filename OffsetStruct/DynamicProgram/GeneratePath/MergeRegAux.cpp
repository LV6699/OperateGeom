#include "MergeRegAux.h"
#include "../../CommonFile/Header.h"
#if 0
#pragma optimize("", off)
#endif
#if 1 ///_ViewDebug
#include "../../../OperateView/OperateView/DisplayGeom.h"
#endif
using namespace std;
using namespace oft;
void MergeRegAux::ClassfyClipResult(vector<DefLoop>oriLoops,
                                    vector<DefLoop>rLoops,
                                    vector<DefLoop>& notClipLoops,
                                    vector<DefLoop>& clipLoops)
{
    notClipLoops.clear();
    clipLoops.clear();
    ElemCalculate numcal;
    for(auto& d : rLoops){
        if(numcal.IsLoopInArrayLoop(oriLoops,d)){
            notClipLoops.push_back(d);
        }else{
            clipLoops.push_back(d);
        }
    }
}
void MergeRegAux::AddOriIslLoopToNode(OffsetParam param,
                                      int hasOftNum,
                                      const vector<DefLoop> &oriIloops,
                                      const vector<DefLoop> &iloops,
                                      NodeForm& nodeForm)
{
    if(!param._pathTech.IsAcceptBoundaryOut() || hasOftNum != 0)
        return;
    ElemCalculate numcal;
    for(auto& d : oriIloops){
        if(d.PastOffsetNum() > 1 ||
                numcal.IsLoopInArrayLoop(iloops,d))
            continue;
        shared_ptr<OffsetNode>node = make_shared<OffsetNode>();
        node->PushBackLoop(d);
        node->_geneType = IslOffsetPath;
        nodeForm._iOftNodes.emplace_back(node);
    }
}
void MergeRegAux::LoopNodeInArrayLoop(const vector<oft::DefLoop>& 
iloops,
                                      std::vector<oft::LoopNode>& 
iNodes,
                                      oft::NodeForm& nodeForm)
{
    ElemCalculate numcal;
    for (size_t i = 0; i < iNodes.size(); ++i) {
        bool isExist = false;
        ///iNodes[i] 中的环loop还在iloops中,则需要把loop添加至iNodes[i]的历史偏置环中.
        isExist = numcal.IsLoopInArrayLoop(iloops,iNodes[i].m_loop);
        if(isExist){
            iNodes[i]._offsetNode->PushBackLoop(iNodes[i].m_loop);
            continue;
        }        ///iNodes[i] 中的环loop不在iloops中,说明loop已经合并了(包括合并后产生
        ///多个区域或其他类型 ),则需要把iNodes[i]添加至结果中,并删除iNodes[i].
        if(!iNodes[i]._offsetNode->IsLoopsEmpty()){
            std::reverse(iNodes[i]._offsetNode->_vLoopData.begin(),
                         iNodes[i]._offsetNode->_vLoopData.end());
            iNodes[i]._offsetNode->_geneType = IslOffsetPath;
            nodeForm._iOftNodes.push_back(iNodes[i]._offsetNode);
        }
        iNodes.erase(iNodes.begin() + i);
        i --;
    }
}
bool MergeRegAux::IsBoundaryInsideIsland(const DefLoop &bloop,
                                         const vector<DefLoop> &iloops)
{
    if(bloop.IsEmpty())
        return false;
    ///仅用在边界和岛屿没有发生 bool裁剪的情况下,验证边界是否在岛屿环内部 .
    ///由于裁剪结果为空,只需要验证边界环一条边是否在岛屿内部即可说明边界环是否
    ///在岛屿内部.
    GeomContain gocon;
#if 1
    for (size_t i = 0; i < iloops.size(); ++i) {
#if 0
        DisplayGeom().DisplayLoop(bloop,_colors.back(),1,false,true);
        DisplayGeom().DisplayLoop(iloops[lo],_colors.back(),1.5);
        _mainwind->myOccView->fitAll();
        string str = OperateOftString().LoopElementString(bloop);
        string strs = OperateOftString().ArrayLoopString(iloops);
        std::cout<<str<<strs<<endl;
#endif
        bool isCoorInc = false;
        gocon.IsLoopContainLoopCoord(iloops[i],bloop,
                                     PreErr5_12,isCoorInc);
        if(!isCoorInc){
            return false;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,iloops.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t lo = r.begin(); lo < r.end(); ++lo) {
            bool isCoorInc = false;
            gocon.IsLoopContainLoopCoord(iloops[lo],bloop,
                                         PreErr5_8,isCoorInc);
            if(!isCoorInc){
                return false;
            }
        }
    });
#endif
    Point mp;
    const DefElem& ele = bloop.m_vElem[0];
    ElemCalculate().ElementMidpoint(ele,mp);
    return LoopSubtractSolve().IsPointInArrayLoop(iloops,mp,true,PreErr5_10);
}
void MergeRegAux::LoopsMatchLoopsToAreas(const vector<DefLoop>& bLoops,
                                         const vector<DefLoop>& iLoops,
                                         vector<DefLoop>& notIncLoops,
                                         vector<GeomArea>& regions){
#if 0
    vector<DefLoop>vLoop = vBndLoop;
    for(auto& d : vIslLoop){vLoop.emplace_back(d);}
    string str = OperateOftString().ArrayLoopString(vLoop);
    std::cout <<str<<endl;
#endif
    GeomContain gocon;
    for(int i = 0;i < bLoops.size();i++){
        vector<DefLoop>rloops;
        for (int l = 0; l < iLoops.size(); ++l) {
            if(iLoops[l].IsEmpty())
                continue;
            bool resValue = false;
            gocon.IsLoopContainIntersectLoop(bLoops[i],iLoops[l],
                                             PreErr_5,PreErr_5,PreErr_5,
                                             false,resValue);
            ///resValue = true;
            if(resValue){
                rloops.push_back(iLoops[l]);
            }
        }
        if(!rloops.empty()){
            GeomArea data(bLoops[i],rloops);
            regions.push_back(data);
        }else{
            notIncLoops.push_back(bLoops[i]);
        }
    }
}
void MergeRegAux::LoopsInsideOutsideLoop(const vector<DefLoop>& loops,
                                         vector<DefLoop>& inloops,
                                         vector<DefLoop>& outloops)
{
    if(loops.empty())
        return;
    inloops.clear();
    outloops.clear();
    if(loops.size() == 1){
        outloops.push_back(loops[0]);
        return;
    }
    ///近适用于loops是环做布尔并的结果.
    ElemCalculate numcal;GeoContainSolve consol;
    for (int i = 0; i < loops.size(); ++i) {
        if(loops[i].IsEmpty())
            continue;
        DefLoop loop;
        vector<DefLoop>loops_;
        numcal.ExceptIndexLoop(loops,i,loop,loops_);
        bool isOutLoop = true;
        const auto& ele = loop.m_vElem[0];
        for (int j = 0; j < loops_.size(); ++j) {
            bool isInclude = false;
            consol.IsLoopContainCurve(loops_[j],ele,
                                      PreErr5_6,PreErr5_6,
                                      isInclude);
            if(isInclude){
                isOutLoop = false;                break;
            }
        }
        if(!isOutLoop){
            loop.SetLoopType(OUTSIDEPROFILE);
            inloops.push_back(loop);
            continue;
        }
        loop.SetLoopType(WITHINPROFILE);
        outloops.push_back(loop);
    }
}
void MergeRegAux::AdjustNotCWLoop(vector<DefLoop>&loops)
{
    LoopDirection lodir;ElemCalculate numcal;
    for(auto& d : loops){
        RotDir r = lodir.LoopRotateDirection(d);
        if(r != CLOCKWISE){
            numcal.ReverseLoopDirection(d);
        }
    }
}
bool MergeRegAux::IsInsideOtherArea(const vector<GeomArea>& areas,
                                    const DefLoop& loop, int id)
{
    ElemCalculate numcal;CheckGeomInt cheint;
    double coinPre = PreErr5_10,tanPre = PreErr5_10,intPre = PreErr5_10;
    Point p1 = numcal.ElementEndpoint(loop.IndexElem(0),false),
            p2 = numcal.ElementEndpoint(areas[id].BndLoop().
                                        IndexElem(0),false);
    DefElem ele(LINETYPE,DefSeg(p1,p2));
    for (int i = 0; i < areas.size(); ++i) {
        if(i == id)
            continue;
        bool isIntersect = false;
        cheint.IsLoopElemSpecifyIntersect(areas[i].BndLoop(),
                                          ele,coinPre,tanPre,
                                          intPre,isIntersect);
        if(isIntersect)
            return true;
    }
    return false;
}