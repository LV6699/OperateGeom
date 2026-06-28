#include "MergeRegAux.h"
#include "../../CommonFile/Header.h"
#include "../../GeometryCalculate/RelativeContain/LoopContain.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
using namespace std;
using namespace OffsetStruct;
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
        if(d.PastOftNum() > 1 ||
                numcal.IsLoopInArrayLoop(iloops,d))
            continue;
        shared_ptr<OffsetNode>node = make_shared<OffsetNode>();
        node->AddLoop(d);
        node->_geneType = IslOffsetPath;
        nodeForm._iOftNodes.emplace_back(node);
    }
}
void MergeRegAux::LoopNodeInArrayLoop(const vector<OffsetStruct::DefLoop>& iloops,
                                      vector<OffsetStruct::LoopNode>& iNodes,
                                      OffsetStruct::NodeForm& nodeForm)
{
    ElemCalculate numcal;
    for (size_t i = 0; i < iNodes.size(); ++i) {
        bool isExist = false;
        ///iNodes[i]中的环loop 还在iloops 中,则需要把loop 添加至iNodes[i]的历史偏置环中.
        isExist = numcal.IsLoopInArrayLoop(iloops,iNodes[i].m_loop);
        if(isExist){
            iNodes[i]._offsetNode->AddLoop(iNodes[i].m_loop);
            continue;
        }
        ///iNodes[i]中的环loop 不在iloops 中,说明loop 已经合并了(包括合并后产生
        ///多个区域或其他类型),则需要把iNodes[i]添加至结果中,并删除iNodes[i].
        if(!iNodes[i]._offsetNode->IsLoopsEmpty()){
            reverse(iNodes[i]._offsetNode->_loops.begin(),
                    iNodes[i]._offsetNode->_loops.end());
            iNodes[i]._offsetNode->_geneType = IslOffsetPath;
            nodeForm._iOftNodes.push_back(iNodes[i]._offsetNode);
        }
        iNodes.erase(iNodes.begin() + i);
        i --;
    }
}
bool MergeRegAux::IsBoundaryInsideIsland(const DefLoop &bloop,
                                         vector<DefLoop> &iloops)
{
    if(bloop.IsEmpty()){return false;}
    ///仅用在边界和岛屿没有发生bool 裁剪的情况下,验证边界是否在岛屿环内部.
    ///由于裁剪结果为空,只需要验证边界环一条边是否在岛屿内部即可说明边界环是否
    ///在岛屿内部.
#if 1
    for (size_t i = 0; i < iloops.size(); ++i) {
#if 0
        DisplayGeom().DisplayLoop(bloop,_colors.back(),1,false,true);
        DisplayGeom().DisplayLoop(iloops[lo],_colors.back(),1.5);
        _mainwind->myOccView->fitAll();
        string str = OperateOftString().LoopElementString(bloop);
        string strs = OperateOftString().ArrayLoopString(iloops);
        cout<<str<<strs<<endl;
#endif
        bool isCoorInc = false;
        isCoorInc = LoopContain::IsLoopContLoopCoord(iloops[i],bloop,PreErr5_12);
        if(!isCoorInc){
            return false;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<size_t>(0,iloops.size()),
                      [&](tbb::blocked_range<size_t>& r) {
        for (size_t lo = r.begin(); lo < r.end(); ++lo) {
            bool isCoorInc = false;
            gocon.IsLoopContainLoopCoord(iloops[lo],bloop,PreErr5_8,isCoorInc);
            isCoorInc = LoopContain::IsLoopContLoopCoord(iloops[lo],bloop,PreErr5_8);
            if(!isCoorInc){
                return false;
            }
        }
    });
#endif
    const auto& c = bloop[0];
    auto mp = BaseGeom2D::CurveMidpoint(c);
    return LoopSubtractSolve().IsPointInArrayLoop(iloops,mp,true,PreErr5_10);
}
void MergeRegAux::LoopsMatchLoopsToAreas(const vector<DefLoop>& bLoops,
                                         const vector<DefLoop>& iLoops,
                                         vector<DefLoop>& notIncLoops,
                                         vector<GeomArea>& regions)
{
#if 0
    vector<DefLoop>vLoop = vBndLoop;
    for(auto& d : vIslLoop){vLoop.emplace_back(d);}
    string str = OperateOftString().ArrayLoopString(vLoop);
    cout <<str<<endl;
#endif
    numst::ContParam cpa(PreErr_5,PreErr_5,PreErr_5,false);
    for(size_t i = 0;i < bLoops.size();i++){
        vector<DefLoop>rloops;
        auto bloop = bLoops[i];
        LimitCoord2D::SetLoopLimCoord(bloop);
        for (size_t j = 0; j < iLoops.size(); ++j) {
            auto l = iLoops[j];
            if(l.IsEmpty()){continue;}
#if 0
            DisplayGeom().DisplayLoop(bloop,_colors.back(),1,true,true);
            DisplayGeom().DisplayLoop(l,_colors[0],1,true);
#endif 
            LimitCoord2D::SetLoopLimCoord(l);
            if(LoopContain::IsLoopContIntLoop(bloop,l,cpa)){
                rloops.push_back(l);
            }
        }
        if(!rloops.empty()){
            regions.push_back(GeomArea(bloop,rloops));
        }else{
            notIncLoops.push_back(bloop);
        }
    }
}
void MergeRegAux::LoopsInsideOutsideLoop(const vector<DefLoop>& loops,
                                         vector<DefLoop>& inloops,
                                         vector<DefLoop>& outloops)
{
    if(loops.empty()){return;}
    inloops.clear();
    outloops.clear();
    if(loops.size() == 1){
        outloops.push_back(loops[0]);return;
    }
    //仅适用于loops 是环做布尔并的结果.
    numst::ContParam cpa(PreErr5_6,PreErr5_6,PreErr5_6);
    for (size_t i = 0; i < loops.size(); ++i) {
        if(loops[i].IsEmpty()){continue;}
        auto loop = loops[i];
        vector<DefLoop>ls;
        for (size_t j = 0; j < loops.size(); ++j) {
            if(i != j){ls.push_back(loops[j]);}
        }
#if 0
        DisplayGeom().DisplayLoops(ls,_colors.back(),1,true,true);
        DisplayGeom().DisplayLoop(loop,_colors[0],1,true);
#endif
        bool isOutLoop = true;
        auto& curve = loop[0];
        LimitCoord2D::SetCurveLimitCoord(curve);
        for (size_t j = 0; j < ls.size(); ++j) {/**
            consol.IsLoopContainCurve(ls[j],ele,PreErr5_6,PreErr5_6,isInclude);*/
            auto& l = ls[j];
            l.SetLimCoord(LimitCoord2D::LoopLimitCoord(l));
            if(LoopContAux::IsLoopContCurve(l,curve,cpa,true)){
                isOutLoop = false;break;
            }
        }
        if(!isOutLoop){
            //loop.SetLoopType(OUTSIDEPROFILE);
            loop.SetLeftward(false);
            inloops.push_back(loop);
            continue;
        }
        //loop.SetLoopType(WITHINPROFILE);
        loop.SetLeftward(true);
        outloops.push_back(loop);
    }
}
void MergeRegAux::AdjustNotCWLoop(vector<DefLoop>&loops)
{
    for(auto& d : loops){
        bool iscw = LoopDirection().IsClockwiseLoop(d);
        if(!iscw){
            d.Reverse();
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
