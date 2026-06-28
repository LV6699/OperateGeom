#include "MergeRegLogic.h"
#include "MergeRegAux.h"
#include"../../CommonFile/Header.h"
#include"../../GeometryClip/GeomBoolSubtract/LoopBoolSubt.h"
#include "../../GeometryCalculate/RelativeContain/LoopContain.h"
#include "../../OperCurve2D/OperCurve2D/LayoutRegion.h"
#include"../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
using namespace std;
using namespace OffsetStruct;
void MergeRegLogic::HandleIslandMergeInside(vector<DefLoop>& inloops,
                                            vector<DefLoop>& iloops,
                                            vector<LoopNode>& iNodes,
                                            vector<GeomArea>& areas)
{
    LayoutRegion mulinc;MergeRegAux maux;
    ElemCalculate elecal;
#if 0
    OperateOftString opeoft;
    string str = opeoft.ArrayLoopString(vLoop);
    std::cout<<str<<endl;
#endif
    vector<GeomArea>areas_;
    double intPre = PreErr5_8,coinPre = PreErr5_8,incPre = PreErr5_8;
    mulinc.OrganizeLoopToArea(inloops,intPre,coinPre,incPre,areas_);
    for(auto& d : areas_){
        if(!d.IslLoop().empty()){
            d.SetAddILoop(true);
        }
    }
    inloops.clear();
    numst::ContParam cpa(PreErr_10,PreErr_5,PreErr_5,false);
    for (size_t b = 0; b < areas_.size(); ++b) {
        vector<size_t>ids;
        vector<DefLoop>loops;
        auto& area = areas_[b];
        auto& bloop = area._bndLoop;
        bloop.SetLimCoord(LimitCoord2D::LoopLimitCoord(bloop));
        area.SetAddBLoop(true);
        for (size_t i = 0; i < iloops.size(); ++i) {
            auto& l = iloops[i];
            l.SetLimCoord(LimitCoord2D::LoopLimitCoord(l));
            if(l.IsEmpty()){continue;}
            bool isInclude = false;
            isInclude = LoopContain::IsLoopContIntLoop(bloop,l,cpa);
            if(!isInclude || maux.IsInsideOtherArea(areas_,l,b)){
                continue;
            }
            //l.SetLoopType(WITHINPROFILE);
            l.SetLeftward(true);
            loops.emplace_back(l);
            ids.emplace_back(i);
        }
        if(loops.empty())
            continue;
        elecal.DeleteIndexLoop(ids,iloops);
        for(auto& l : loops){
            const auto& ele = l.m_vElem[0];
            for (size_t n = 0; n < iNodes.size(); ++n) {
                if(iNodes[n].m_loop.IsEmpty())
                    continue;
                const auto& ele_ = iNodes[n].m_loop.m_vElem[0];
                bool isSame = false;
                isSame = elecal.IsIdenticalElement(ele,ele_,isSame);
                if(isSame && (!iNodes[n]._offsetNode->IsLoopsEmpty() ||
                              !iNodes[n]._offsetNode->IsNodeEmpty())){
                    iNodes[n]._offsetNode->AddLoop(l);
                    l._label.m_hasExiNode = iNodes[n]._offsetNode;
                    iNodes.erase(iNodes.begin() + n);
                    break;
                }
            }
        }
        for(auto& d : loops){
            area._vIslLoop.emplace_back(d);
        }
        //bloop.SetLoopType(OUTSIDEPROFILE);
        bloop.SetLeftward(false);
    }
    for(auto& d : areas_){
        if(d.IslLoop().empty()){
            inloops.emplace_back(d.BndLoop());
        }else{
            areas.emplace_back(d);
        }
    }
}
void MergeRegLogic::MergeOriginBoundary(DefLoop& bloop,
                                        vector<DefLoop>& iloops,
                                        OffsetParam param,
                                        bool &isBndInside,
                                        bool &isExistClip,
                                        NodeForm& nodeForm,
                                        vector<GeomArea>& regions)
{
    MergeRegAux maux;
    int hasOftNum = bloop.PastOftNum();
    vector<DefLoop>oriILoops = iloops,rloops,rloops_;
    LoopBoolSubt lbs;
    lbs.LoopBoolSubtract(bloop,iloops,rloops);
    if(/*rloops.empty()*/!lbs.HasClip()){
        if(maux.IsBoundaryInsideIsland(bloop,iloops)){
            isBndInside = true;
            maux.AddOriIslLoopToNode(param,hasOftNum,oriILoops,
                                     iloops,nodeForm);
        }
        return;
    }
    iloops.clear();
    MergeRegAux().ClassfyClipResult(oriILoops,rloops,iloops,rloops_);
    vector<GeomArea>regions_;
    vector<DefLoop> vNotIncLoop_;
    /// 合并计算之后可能会存在多个边界岛屿配对环,产生的结果只有两种，
    /// vNotIncLoop_和regions_；
    isExistClip = true;
    for(auto& d : rloops_){
        d.SetPastOftNum(hasOftNum);
    }
    maux.LoopsMatchLoopsToAreas(rloops_,iloops,vNotIncLoop_,regions_);
    if(iloops.empty()){
        for(auto& d : rloops_){
            nodeForm._biMerLoops.emplace_back(d);
        }
    }else{
        for(auto& d : vNotIncLoop_){
            nodeForm._biMerLoops.emplace_back(d);
        }
    }
    ///当存在裁剪且rloops_为空说明bloop 与iloops 所有环重合.
    if(rloops_.empty()){
        for(auto& d : iloops){
            d._label._isOffset = false;
            nodeForm._biMerLoops.emplace_back(d);
        }
        bloop.Clear();
        iloops.clear();
    }
    if(!regions_.empty()){
        bloop = regions_[0].BndLoop();
        iloops = regions_[0].IslLoop();
        bloop.SetPastOftNum(hasOftNum);
        ///iloops 中的环可能包含原始实参iloops 中的环,
        ///原始iloops 中环的hasOftNum 不应该被改变.
        regions_.erase(regions_.begin());
        for(auto& d : regions_){
            regions.emplace_back(d);
        }
    }
    ///由于允许刀路在边界外,因此需要添加oriILoops 中不在iloops
    ///中的环(已经与边界合并的环)至nodeForm._iOftNodes 中.
    maux.AddOriIslLoopToNode(param,hasOftNum,oriILoops,
                             iloops,nodeForm);
}
/**
功能:
1-对area 中边界岛屿进行合并.2-对合并结果进行处理(添加相关合并结果等).3-重置合并区域.*/
void MergeRegLogic::ExicuteMergeRegion(GeomArea& area,
                                       bool& isExistClip,
                                       DefLoop& bloop,
                                       vector<DefLoop>& iLoops,
                                       NodeForm& nodeForm,
                                       vector<GeomArea>& regions)
{
    vector<DefLoop>rloops;
    MergeRegAux maux;LoopBoolSubt lbs;
    lbs.LoopBoolSubtract(area._bndLoop,area._vIslLoop,rloops);
#if 0
    DisplayGeom().DisplayGeomArea(area,1,1.5,true);
    DisplayGeom().DisplayLoops(rloops,_colors[0],2,true);
    string s = OperateOftString().BndIslLoopString(area);
    std::cout<<s<<std::endl;
#endif
    if(!lbs.HasClip() || rloops.empty()){
        bloop = area._bndLoop;
        iLoops = area._vIslLoop;
        bool isBndInside = false;
#if 0
        DisplayGeom().DisplayLoop(area._bndLoop,_colors[1],1.5,true,true);
        DisplayGeom().DisplayLoops(area._vIslLoop,_colors[2],1.5,false,false);
        _mainwind->myOccView->fitAll();
#endif
        if(rloops.empty()){
            isBndInside = true;
        }
        ///isBndInside = IsBLoopInIslLoop(area.BndLoop(),area.IslLoop());
        if(isBndInside){
            bloop.Clear();iLoops.clear();
        }
        return;
    }
    isExistClip = true;
    maux.ClassfyClipResult(area._vIslLoop,rloops,area._vIslLoop,rloops);
    ///当存在裁剪且rloops 为空说明bloop_与iLoops_所有环重合.
    if(rloops.empty()){
        for(auto& d : iLoops){
            nodeForm._biMerLoops.emplace_back(d);
        }
        bloop.Clear();iLoops.clear();
        return;
    }
    int hasOftNum = bloop.PastOftNum();
    for(auto& d : rloops){
        d.SetPastOftNum(hasOftNum + 1);
    }
    vector<GeomArea>areas;
    vector<DefLoop>notIncLoops;
    ///合并计算之后可能会存在多个边界岛屿配对环.
    maux.LoopsMatchLoopsToAreas(rloops,area._vIslLoop,notIncLoops,areas);
    for(auto& d : areas){
        d.SetAddBLoop(true);
        d.SetAddILoop(true);
    }
    bloop = area._bndLoop;
    iLoops = area._vIslLoop;
    ///当岛屿环全部合并时,则所有局部环都是外轮廓.
    if(area._vIslLoop.empty()){
        for(auto& d : rloops){
            nodeForm._biMerLoops.emplace_back(d);
        }
    }else{
        for(auto& d : notIncLoops){
            nodeForm._biMerLoops.emplace_back(d);
        }
    }
    if(!areas.empty()){
        bloop = areas[0].BndLoop();
        iLoops = areas[0].IslLoop();
        areas.erase(areas.begin());
        for(auto& d : areas){
            regions.emplace_back(d);
        }
    }
}
void MergeRegLogic::MergeMultipleRegions(DefLoop& bloop,
                                         vector<DefLoop>& iLoops,
                                         bool& isExistClip,
                                         vector<GeomArea>& areas,
                                         NodeForm& nodeForm,
                                         vector<GeomArea>& regions)
{
    for (int i = 0; i < areas.size(); ++i) {
        DefLoop bloop_;
        vector<DefLoop>iLoops_,noIncLoops_;
        vector<GeomArea>areas_;
        bool isExistClip_ = false;
        ExicuteMergeRegion(areas[i],isExistClip_,
                           bloop_,iLoops_,nodeForm,areas_);
        ///isBndInside 不可能为真.
        if(!isExistClip_){
            regions.emplace_back(areas[i]);
            continue;
        }
        isExistClip = true;
        for(auto& d : noIncLoops_){
            nodeForm._biMerLoops.emplace_back(d);
        }
        if(!bloop_.IsEmpty() && !iLoops_.empty()){
            regions.emplace_back(GeomArea(bloop_,iLoops_));
        }
        for(auto& d : areas_){
            regions.emplace_back(d);
        }
    }
    iLoops.clear();
    bloop.Clear();
    for(auto& d : regions){
        d.SetAddILoop(true);
        d.SetAddBLoop(true);
    }
    if(!regions.empty()){
        bloop = regions[0].BndLoop();
        iLoops = regions[0].IslLoop();
        regions.erase(regions.begin());
    }
}
///仅用在边界和岛屿没有发生bool 裁剪的情况下,验证边界是否在岛屿环内部.
bool MergeRegLogic::IsBLoopInIslLoop(const ofts::DefLoop& bloop,
                                     std::vector<ofts::DefLoop>& iloops)
{
    if(bloop.IsEmpty()){
        return false;
    }
    ///由于裁剪结果为空,说明边界岛屿环不存在相交,只需要验证边界环一条边是否在
    ///岛屿内部即可说明边界环是否在岛屿内部.
#if 1
    for(auto& d : iloops){
#if 0
        DisplayGeom().DisplayLoop(bloop,_colors.back(),1,false,true);
        DisplayGeom().DisplayLoop(d,_colors.back(),1.5);
        _mainwind->myOccView->fitAll();
#endif
        bool isCoorInc = false;
        ///gocon.IsLoopContainLoopCoord(d,bloop,PreErr5_12,isCoorInc);
        isCoorInc = LoopContain::IsLoopContLoopCoord(d,bloop,PreErr5_12);
        if(!isCoorInc){
            return false;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,vIslLoop.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t lo = r.begin(); lo < r.end(); ++lo) {
            bool isCoorInc = false;
            gocon.IsLoopContainLoopCoord(vIslLoop[lo],bndLoop,
                                         PreErr5_8,isCoorInc);
            if(!isCoorInc){
                return false;
            }
        }
    });
#endif
    ElemCalculate numcal;
#if 0
    Point mp;
    const auto& ele = [0];
    numcal.ElementMidpoint(ele,mp);
    return LoopSubtractSolve().IsPointInArrayLoop(
                vIslLoop,mp,true,PreErr5_10);
#else
    for (int i = 0; i < bloop.Size(); ++i) {
        Point mp;
        const auto& ele = bloop[i];
        if(ele.Type() != CIRCLETYPE){
            numcal.ElementMidpoint(ele,mp);
        }else{
            mp = ele._circle.LeftPt();
        }
        bool isOnBorder = false,isInPoly = false;
        for(auto l : iloops){
            LimitCoord2D::SetLoopLimCoord(l);
            PointInLoop().GetPointInsideLoop(l,mp,PreErr5_10,
                                             isOnBorder,isInPoly);
#if 0
            if(isInPoly){
                return true;
            }
#else
            if(!isOnBorder){
                return isInPoly;
            }
#endif
            isOnBorder = false;
        }
    }
    return false;
#endif
}
