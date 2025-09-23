#include "PathSmooth.h"
void PathSmooth::SetSmoothOffsetParam(OffsetParam& offsetPara)
{
    offsetPara._offsetUnit.SetFixedOffset(true);
    offsetPara._offsetUnit.SetOffsetNumber(1);
    offsetPara._offsetUnit.SetAlterableValue(false);
    double R = offsetPara._offsetUnit.PathSmoothValue();
    if(R >= offsetPara._offsetUnit.ToolRadius() - PreErr5_6)
        R = offsetPara._offsetUnit.ToolRadius() - PreErr5_6;
    offsetPara._offsetUnit.SetOffsetValue(R);
    offsetPara._offsetUnit.SetFirstOffsetValue(R);
    offsetPara._offsetUnit.SetToolRadius(R * 2);
}
void PathSmooth::ArrayLoopSmooth(OffsetParam offsetPara,bool isAddOriLoop,
                                 vector<DefLoop>& vLoop)
{
    SmoothStrategy smostr;CreateFitting fitt;
    vector<DefLoop> vArrLoop;
    for(const auto& it : vLoop)
        vArrLoop.push_back(it);
    vLoop.clear();
    SetSmoothOffsetParam(offsetPara);
    int limOftNum = isAddOriLoop ? 0 : 1;
    for (int i = 0; i < vArrLoop.size(); ++i) {
        if(vArrLoop[i].PastOffsetNum() > limOftNum){
            vLoop.push_back(vArrLoop[i]);
            continue;
        }
        if(smostr.CircleTranslateArcSmooth(vLoop,vArrLoop[i]))
            continue;
        DefLoop loop = vArrLoop[i];
        if(vArrLoop[i].IsCloseMode())
            fitt.LoopGeometryFitting(vArrLoop[i],loop,
                                     Fitting_LenLim,
                                     Fitting_DisErr / 5,
                                     Fitting_AngVarErr / 2);
        vector<DefLoop> vLoop_;
        ExicuteLoopSmooth(offsetPara,loop,vLoop_);
        for(auto& lo : vLoop_)
            vLoop.push_back(lo);
    }
}
///对一个环进行光顺,目前存在欠切情况,后续需要优化.
void PathSmooth::ExicuteLoopSmooth(OffsetParam offsetPara,
                                   const DefLoop& loop,
                                   vector<DefLoop>& vLoop)
{
    ExicuteProgram exeoft;RootNodeSolve nodsol;
    shared_ptr<OffsetNode> node;
    exeoft.LoopOffsetNodeResult(loop,offsetPara,node);
    vector<DefLoop>vLoop_;
    nodsol.PathRootNodeLoop(node,vLoop_);
    for (int i = 0; i < vLoop_.size(); ++i) {
        vector<DefLoop>vLoop__;
        shared_ptr<OffsetNode> node_;
        vLoop_[i].SetLoopType(loop.LoopType() == OUTSIDEPROFILE ?
                                  WITHINPROFILE : OUTSIDEPROFILE);
        exeoft.LoopOffsetNodeResult(vLoop_[i],offsetPara,node_);        nodsol.PathRootNodeLoop(node_,vLoop__);
        for(auto& lo : vLoop__)
            vLoop.push_back(lo);
    }
    for(auto& d : vLoop){
        d.SetPastOftNum(loop.PastOffsetNum());
        d.SetThisOftNum(loop.ThisOffsetNum());
        d.SetLoopType(loop.LoopType());
        d._loopLabel._loPathType = loop._loopLabel._loPathType;
    }
}