#include "MergeRegLogic.h"
#include "MergeRegAux.h"
#include"../../CommonFile/Header.h"
#if 0
#pragma optimize("", off)
#endif
#if 1
#include"../../../OperateView/OperateView/DisplayGeom.h"
#endif
using namespace std;
using namespace oft;
void MergeRegLogic::HandleIslandMergeInside(vector<DefLoop>& inloops,
                                            vector<DefLoop>& iloops,
                                            vector<LoopNode>& iNodes,
                                            vector<GeomArea>& areas)
{
    MultipleInclude mulinc;MergeRegAux maux;
    GeomContain gocon;ElemCalculate elecal;
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
            d.SetAddIslLoop(true);
        }
    }
    inloops.clear();
    for (int b = 0; b < areas_.size(); ++b) {
        vector<int>ids;
        vector<DefLoop>loops;
        auto& area = areas_[b];
        auto& bloop = area._bndLoop;
        area.SetAddBndLoop(true);
        for (int i = 0; i < iloops.size(); ++i) {
            if(iloops[i].IsEmpty())
                continue;
            bool isInclude = false;
            gocon.IsLoopContainIntersectLoop(bloop,iloops[i],PreErr_5,
                                             PreErr_5,PreErr_10,false,
                                             isInclude);
            if(!isInclude || maux.IsInsideOtherArea(areas_,iloops[i],b))
                continue;
            iloops[i].SetLoopType(WITHINPROFILE);
            loops.emplace_back(iloops[i]);
            ids.emplace_back(i);
        }
        if(loops.empty())
            continue;
        elecal.DeleteIndexLoop(ids,iloops);
        for(auto& l : loops){
            const auto& ele = l.m_vElem[0];
            for (int n = 0; n < iNodes.size(); ++n) {
                if(iNodes[n].m_loop.IsEmpty())
                    continue;
                const auto& ele_ = iNodes[n].m_loop.m_vElem[0];                bool isSame = false;
                isSame = elecal.IsIdenticalElement(ele,ele_,isSame);
                if(isSame && (!iNodes[n]._offsetNode->IsLoopsEmpty() ||
                              !iNodes[n]._offsetNode->IsNodeEmpty())){
                    iNodes[n]._offsetNode->PushBackLoop(l);
                    l._loopLabel.m_hasExiNode = iNodes[n]._offsetNode;
                    iNodes.erase(iNodes.begin() + n);
                    break;
                }
            }
        }
        for(auto& d : loops){
            area._vIslLoop.emplace_back(d);
        }
        bloop.SetLoopType(OUTSIDEPROFILE);
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
    BndIslMatch bimat;LoopBoolSubtract losub;
    MergeRegAux maux;
    int hasOftNum = bloop.PastOffsetNum();
    vector<DefLoop>oriILoops = iloops,rloops,rloops_;
    losub.LoopGeomBoolSubtract(bloop,iloops,rloops);
    if(rloops.empty()){
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
    /// 合并计算之后可能会存在多个边界岛屿配对环 ,产生的结果只有两种，
    /// vNotIncLoop_ 和regions_；
    isExistClip = true;
    for(auto& d : rloops_){
        d.SetPastOftNum(hasOftNum);
    }
    bimat.LoopsMatchLoopsToAreas(rloops_,iloops,
                                 vNotIncLoop_,regions_);
    if(iloops.empty()){
        for(auto& d : rloops_){            nodeForm._biMerLoops.emplace_back(d);
        }
    }else{
        for(auto& d : vNotIncLoop_){
            nodeForm._biMerLoops.emplace_back(d);
        }
    }
    ///当存在裁剪且rloops_为空说明bloop与iloops所有环重合.
    if(rloops_.empty()){
        for(auto& d : iloops){
            d._loopLabel._isLoopOffset = false;
            nodeForm._biMerLoops.emplace_back(d);
        }
        bloop.ClearElem();
        iloops.clear();
    }
    if(!regions_.empty()){
        bloop = regions_[0].BndLoop();
        iloops = regions_[0].IslLoop();
        bloop.SetPastOftNum(hasOftNum);
        ///iloops 中的环可能包含原始实参 iloops中的环,
        ///原始iloops中环的hasOftNum不应该被改变.
        regions_.erase(regions_.begin());
        for(auto& d : regions_){
            regions.emplace_back(d);
        }
    }
    ///由于允许刀路在边界外 ,因此需要添加oriILoops中不在iloops
    ///中的环(已经与边界合并的环)至nodeForm._iOftNodes 中.
    maux.AddOriIslLoopToNode(param,hasOftNum,oriILoops,
                             iloops,nodeForm);
}
/**
功能:
1-对area中边界岛屿进行合并 .2-对合并结果进行处理(添加相关合并结果等).3-重置合并区域.*/
void MergeRegLogic::ExicuteMergeRegion(GeomArea& area,
                                       bool& isExistClip,
                                       DefLoop& bloop,
                                       vector<DefLoop>& iLoops,
                                       NodeForm& nodeForm,
                                       vector<GeomArea>& regions)
{
    BndIslMatch bimat;
    LoopBoolSubtract losub;MergeLoopLogSolve bilg;
    vector<DefLoop>rloops;
    losub.LoopGeomBoolSubtract(area._bndLoop,area._vIslLoop,rloops);
#if 0
    DisplayGeom().DisplayGeomArea(GeomArea(area._bndLoop,area._vIslLoop),
                                  _colors.back(),1,true);
    DisplayGeom().DisplayLoops(rloops,_colors[1],1.5);
    _mainwind->myOccView->fitAll();
#endif
    if(rloops.empty()){
        bloop = area._bndLoop;
        iLoops = area._vIslLoop;
        bool isBndInside = false;
#if 0
        DisplayGeom().DisplayLoop(area._bndLoop,_colors[1],1.5,true,true);
        DisplayGeom().DisplayLoops(area._vIslLoop,_colors[2],1.5,false,false);
        _mainwind->myOccView->fitAll();
#endif        isBndInside = bilg.IsBoundaryInIslandLoop(area._bndLoop,
                                                  area._vIslLoop);
        if(isBndInside){
            bloop.ClearElem();
            iLoops.clear();
        }
        return;
    }
    isExistClip = true;
    MergeRegAux().ClassfyClipResult(area._vIslLoop,rloops,
                                    area._vIslLoop,rloops);
    ///当存在裁剪且rloops为空说明bloop_与iLoops_所有环重合.
    if(rloops.empty()){
        for(auto& d : iLoops){
            nodeForm._biMerLoops.emplace_back(d);
        }
        bloop.ClearElem();
        iLoops.clear();
        return;
    }
    int hasOftNum = bloop.PastOffsetNum();
    for(auto& d : rloops){
        d.SetPastOftNum(hasOftNum + 1);
    }
    vector<GeomArea>areas;
    vector<DefLoop>notIncLoops;
    ///合并计算之后可能会存在多个边界岛屿配对环 .
    bimat.LoopsMatchLoopsToAreas(rloops,area._vIslLoop,
                                 notIncLoops,areas);
    for(auto& d : areas){
        d.SetAddBndLoop(true);
        d.SetAddIslLoop(true);
    }
    bloop = area._bndLoop;
    iLoops = area._vIslLoop;
    ///当岛屿环全部合并时 ,则所有局部环都是外轮廓 .
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
    for (int i = 0; i < areas.size(); ++i) {        DefLoop bloop_;
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
    bloop.ClearElem();
    for(auto& d : regions){
        d.SetAddIslLoop(true);
        d.SetAddBndLoop(true);
    }
    if(!regions.empty()){
        bloop = regions[0].BndLoop();
        iLoops = regions[0].IslLoop();
        regions.erase(regions.begin());
    }
}