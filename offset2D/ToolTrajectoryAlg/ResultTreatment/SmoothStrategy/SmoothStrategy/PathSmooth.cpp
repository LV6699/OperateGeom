#include<iostream>
#include "PathSmooth.h"
#include "../../../GeometryCalculate/GeomFitting2D/CreateFitting.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
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
    numst::FitParam fpa(8e-3,10);
    for (int i = 0; i < vArrLoop.size(); ++i) {
        if(vArrLoop[i].PastOftNum() > limOftNum){
            vLoop.push_back(vArrLoop[i]);
            continue;
        }
        if(smostr.CircleTranslateArcSmooth(vLoop,vArrLoop[i]))
            continue;
        DefLoop loop = vArrLoop[i];
        if(vArrLoop[i].IsCloseMode()){
            auto loop = vArrLoop[i];
            fitt.FittingLoopCurves(fpa,loop);
        }
        vector<DefLoop> vLoop_;
        ExicuteLoopSmooth(offsetPara,loop,vLoop_);
        for(auto& lo : vLoop_)
            vLoop.push_back(lo);
    }
}
///对一个环进行光顺,目前存在欠切情况,后续需要优化.
void PathSmooth::ExicuteLoopSmooth(OffsetParam offsetPara,
                                   const DefLoop& loop,
                                   vector<DefLoop>& loops)
{
    ExicuteProgram exeoft;RootNodeSolve nodsol;
    shared_ptr<OffsetNode> node;
    exeoft.LoopOffsetNodeResult(loop,offsetPara,node);
    vector<DefLoop>vLoop_;
    nodsol.PathRootNodeLoop(node,vLoop_);
    for (int i = 0; i < vLoop_.size(); ++i) {
        vector<DefLoop>vLoop__;
        shared_ptr<OffsetNode> node_;
        vLoop_[i].SetLeftward(!loop.Leftward());
        exeoft.LoopOffsetNodeResult(vLoop_[i],offsetPara,node_);
        nodsol.PathRootNodeLoop(node_,vLoop__);
        for(auto& lo : vLoop__)
            loops.push_back(lo);
    }
    for(auto& d : loops){
        d.SetPastOftNum(loop.PastOftNum());
        d.SetThisOftNum(loop.ThisOftNum());
        d.SetLeftward(loop.Leftward());
        d._label._loPathType = loop._label._loPathType;
    }
}
void PathSmooth::CalculateLoopSmooth(const DefLoop& loop,double R,
                                     vector<DefLoop>& loops,bool isOne)
{
    OffsetParam param;
    param._offsetUnit.SetFixedOffset(true);
    param._offsetUnit.SetOffsetNumber(1);
    param._offsetUnit.SetAlterableValue(false);
    param._offsetUnit.SetOffsetValue(R);
    param._offsetUnit.SetFirstOffsetValue(R);
    param._offsetUnit.SetToolRadius(R * 2);
    param._offsetUnit.SetToolRadius(R * 2);
#if 0
    string str = OperateOftString().LoopElementString(loop);
    std::cout<<str<<endl;
#endif
    vector<DefLoop>loops_;
    shared_ptr<OffsetNode> node;
    ExicuteProgram exeoft;RootNodeSolve nodsol;
    exeoft.LoopOffsetNodeResult(loop,param,node);
    nodsol.PathRootNodeLoop(node,loops_);
    for(size_t i = 0;i < loops_.size();++i){
        vector<DefLoop>ls;
        shared_ptr<OffsetNode> n;
        loops_[i].SetLeftward(!loop.Leftward());
        exeoft.LoopOffsetNodeResult(loops_[i],param,n);
        nodsol.PathRootNodeLoop(n,ls);
        for(auto& l : ls){
            loops.push_back(l);
        }
    }
    for(auto& d : loops){
        d.SetPastOftNum(loop.PastOftNum());
        d.SetThisOftNum(loop.ThisOftNum());
        d.SetLeftward(loop.Leftward());
        d._label._loPathType = loop._label._loPathType;
    }
    if(!isOne || loops.size() < 2)
        return;
    ElemCalculate elecal;
    vector<double>values;
    for(size_t i = 0;i < loops.size();++i){
        DefCircle circle;
        elecal.LoopMinCircumcircle(loops[i],true,4,circle);
        values.push_back(circle.R());
    }
    auto max_it = std::max_element(values.begin(), values.end());
    size_t id = std::distance(values.begin(), max_it);
    auto l = loops[id];
    loops.clear();
    loops.push_back(l);
}
