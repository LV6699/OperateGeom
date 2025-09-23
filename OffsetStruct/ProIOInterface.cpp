/**
  @author: lvdong
  @date: 2023-07-12
**/
#include "ProIOInterface.h"
#include "PlaneStyleMilling/faceMill.h"
#include "EquidisCurve/GeometryFitting/CreateFitting.h"
#include "ResultTreatment/OperatePath/OpeSmallLoop.h"
#include "DynamicProgram/CreateTrajectory.h"
#include "../OperateView/OperateView/DisplayGeom.h"
#include<iostream>
#if DebugModel
#pragma optimize("", off)
#endif
bool ProIOInterface::IsValidExicuteCalculate(GeomArea &region,
                                             OffsetParam param,
                                             shared_ptr<OffsetNode>& pathNode)
{
    if(param._offsetUnit.IsFixedOffset() &&
            param._offsetUnit.OffsetNumber() < 1){
        if(param._pathTech.IsAddOriginLoop()){
            if(!region.BndLoop().IsEmpty()){
                pathNode->SetPathType(BndOffsetPath);
                pathNode->PushBackLoop(region.BndLoop());
            }
            for(auto& l : region._vIslLoop){
                if(!l.IsEmpty()){
                    shared_ptr<OffsetNode> n = make_shared<OffsetNode>();
                    n->PushBackLoop(l);
                    n->SetPathType(IslOffsetPath);
                    pathNode->PushBackNode(n);
                }
            }
        }
        return false;
    }
    return true;
}
void ProIOInterface::PreprocesseInputData(GeomArea& region,
                                          OffsetParam& param)
{
    ElemCalculate elecal;
    elecal.SetLoopElementZValue(0,region._bndLoop);
    for(auto& d : region._vIslLoop){
        elecal.SetLoopElementZValue(0,d);
    }
    DefLoop oriBLoop = region._bndLoop;
    vector<DefLoop>vOriIslLoop = region._vIslLoop;
    oriBLoop._loopLabel._loPathType = BndOffsetPath;
    for(auto& lo : vOriIslLoop){
        lo._loopLabel._loPathType = IslOffsetPath;
    }
    ElementPrecision pre;
    pre.SpecifyInputPretreatment(param._offsetUnit.IsFixedOffset(),
                                 param._offsetUnit.IsAblePreProcess(),
                                 param._offsetUnit.FirstOffsetValue(),
                                 false,true,false,false,false,
                                 region._bndLoop,
                                 region._vIslLoop);
    if(param._pathTech.IsAddOriginLoop()){
        region._bndLoop.SetCloseMode(true);        for(auto& lo : region._vIslLoop){
            lo.SetCloseMode(true);
        }
        param._offsetUnit.SetFirstOffsetValue(
                    param._offsetUnit.OffsetValue());
    }
    region._bndLoop._loopLabel._loPathType = BndOffsetPath;
    for(auto& lo : region._vIslLoop){
        lo._loopLabel._loPathType = IslOffsetPath;
    }
    region.SetOriBndLoop(oriBLoop);
    region.SetOriIslLoop(vOriIslLoop);
    double lenVal = Fitting_LenLim,
            disVal = Fitting_DisErr,angVal = Fitting_AngVarErr;
#if DebugModel
#else
    if(param._offsetUnit.IsFixedOffset() &&
            param._offsetUnit.FirstOffsetValue() <= PreprocValLim &&
            !param._offsetUnit.IsAblePreProcess()){
        return;
    }
#endif
    CreateFitting fitt;
    fitt.BndIslLoopGeomFitting(region,lenVal,disVal,angVal);
}
void ProIOInterface::BorderLoopOffsetInOutIO(GeomArea& region,
                                             OffsetParam& param,
                                             shared_ptr<OffsetNode>& pathNode)
{
    if(pathNode == nullptr)
        pathNode = make_shared<OffsetNode>();
    if(region.BndLoop().IsEmpty() ||
            (region.BndLoop().LoopType() == WITHINPROFILE &&
             !param._offsetUnit.IsFixedOffset()))
        return;
    ExicuteProgram exepro;
    if(param._pathTech.ProfOffsetStyles() == Plane_Style_Milling){
        faceMillMsgIO(region._oriBndLoop,param,pathNode);
    }else{
        region._bndLoop.SetThisOftNum(0);
        exepro.LoopOffsetNodeResult(region.BndLoop(),
                                    param,pathNode);
        if(param._pathTech.IsAddOriginLoop()){
            pathNode->InsertLoop(0,region._oriBndLoop);
        }
    }
    if(!param._pathTech.IsAddOriginLoop() &&
            param._pathTech.IsBlankBoundary() &&
            param._pathTech.IsAcceptBoundaryOut()){
        pathNode->InsertLoop(0,region._oriBndLoop);
    }
    ///ToolPathType type = bndIslLoop.BndLoop()._loopLabel._loPathType;
    RootNodeSolve().SetNodeToolPathType(BndOffsetPath,pathNode);
}
#if TimeStatistic
#include"../../../ViewWindow/MainWindow/mainwindow.h"
#endif
void ProIOInterface::InPutOutPutIO(GeomArea region,
                                   OffsetParam param,                                   shared_ptr<OffsetNode>& pathNode)
{
#if 0
    DisplayGeom().DisplayGeomArea(region,true);
#endif
    if(!IsValidExicuteCalculate(region,param,pathNode))
        return;
    if(param._offsetUnit.IsFixedOffset()){
        param._offsetUnit.SetAlterableValue(false);
    }///是否固定偏置
#if TimeStatistic
    auto sTime = std::chrono::high_resolution_clock::now();
#endif
    ElemCalculate numcal;
    if(!numcal.IsLegalCurveNumber(region.BndLoop()))
        return;
    numcal.DeleteIllegalEleNumLoop(region._vIslLoop);/// 删除非法环
    numcal.DeleteIllegalEleNumLoop(region._vBlockLoop);/// 删除无效干涉环
#if 0
    string str = OperateOftString().BndIslLoopString(region);
    std::cout<<str<<std::endl;
#endif
    double zCoord = numcal.LoopZCoordinate(region.BndLoop());
    ToolTrajectory traj;CreateTrajectory ctra;/// 计算刀路
    PreprocesseInputData(region,param);/// 拟合圆弧
    if(region._vIslLoop.empty()){
        BorderLoopOffsetInOutIO(region,param,pathNode);
    }else{
        ///traj.CreateTrajectoryInterface(region,param,pathNode);
        ctra.CreateTrajectoryInterface(region,param,pathNode);
    }
    ///EqudDebug().InformationCase(biLoop,offsetPara,rootNode);
    NodePathResultTreatment(param,zCoord,pathNode);/// 刀路结果后处理
#if TimeStatistic
    auto eTime = std::chrono::high_resolution_clock::now();
    double exeTime = std::chrono::duration<double>(eTime - sTime).count();
    Time_total += exeTime * 1000;
#endif
}
void ProIOInterface::NodePathResultTreatment(OffsetParam param,
                                             double zCoord,
                                             shared_ptr<OffsetNode> &pathNode)
{
    RootNodeSolve nodsol;
    nodsol.DeleteRootNodeEmptyLoop(pathNode);/// 删除空的无效环
    ///   圆弧端点距离、半径、弓高、是否保留有效位数、小数点位数 //删除很细小的高精度圆弧
    nodsol.PathNodeArcPrecision(PreErr5_4,PreErr5_4,
                                PreErr5_4,true,4,pathNode);
    nodsol.NodePathCircleArcTranslate(true,pathNode);/// 两段圆弧若能构成一个完整圆，
    则合并成一个圆
            PathDirection().SpecifyPathMillDirection(param,pathNode);/// 顺逆铣处理
    int index = 0;
    if(pathNode->ArrayLoop().empty()){
        index = -1;
    }
    nodsol.SetPathNodeAreaIndex(pathNode,index);
    nodsol.SetRootNodeZValue(zCoord,pathNode);
}
