/**
  @author: lvdong
  @date: 2023-07-12
**/
#include "ProIOInterface.h"
//#include "PlaneStyleMilling/faceMill.h"
#include "GeometryCalculate/GeomFitting2D/CreateFitting.h"
#include "ResultTreatment/OperatePath/OpeSmallLoop.h"
#include "DynamicProgram/CreateTrajectory.h"
#include "../OperateView/OperateView/DisplayGeom.h"
#include  <iostream>

#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif

bool ProIOInterface::IsValidExicute(ofts::GeomArea &region,
                                    ofts::OffsetParam param,
                                    std::shared_ptr<ofts::OffsetNode>& pathNode)
{
    if(param._offsetUnit.IsFixedOffset() &&
            param._offsetUnit.OffsetNumber() < 1){
        if(param._pathTech.IsAddOriginLoop()){
            if(!region.BndLoop().IsEmpty()){
                pathNode->SetPathType(BndOffsetPath);
                pathNode->AddLoop(region.BndLoop());
            }
            for(auto& l : region._vIslLoop){
                if(!l.IsEmpty()){
                    std::shared_ptr<ofts::OffsetNode> n = make_shared<ofts::OffsetNode>();
                    n->AddLoop(l);
                    n->SetPathType(IslOffsetPath);
                    pathNode->AddNode(n);
                }
            }
        }
        return false;
    }
    return true;
}
void ProIOInterface::PreprocesseInput(ofts::GeomArea& region,
                                      ofts::OffsetParam& param)
{
    region._bndLoop.SetZCoord(0);
    for(auto& l : region._vIslLoop){l.SetZCoord(0);}
    auto obloop = region._bndLoop;
    auto oils = region._vIslLoop;
    obloop._label._loPathType = BndOffsetPath;
    for(auto& l : oils){l._label._loPathType = IslOffsetPath;}
    const auto& uni = param._offsetUnit;
    PrepOption preOpt(uni.FirstOftVal(),false,true,true,false);
  
    if(region.BLoop().GetEndType() == Ofte::EndType::Joined){
        //DisplayGeom().DisplayLoop(region._bndLoop,_colors.back(),1,true,false);
        region._bndLoop.MakeClose(PreErr5_14);
        //DisplayGeom().DisplayLoop(region._bndLoop,_colors.back(),1.5,true,false);
    }
    ofts::OffsetParam param_ = param;
    param_._prepOpti = preOpt;
    PreprocLoop().OptInputPreprocess(param_,region._bndLoop,region._vIslLoop);
    param._prepOpti.SetOftValue(uni.FirstOftVal());
    if(param._pathTech.IsAddOriginLoop()){
        region._bndLoop.SetCloseMode(true);
        for(auto& l : region._vIslLoop){l.SetCloseMode(true);}
        param._offsetUnit.SetFirstOffsetValue(param._offsetUnit.OffsetValue());
    }
    region._bndLoop._label._loPathType = BndOffsetPath;
    for(auto& l : region._vIslLoop){
        l._label._loPathType = IslOffsetPath;
    }
    region.SetOriBLoop(obloop);
    region.SetOriILoop(oils);
#if DebugModel
#else
    if(param._offsetUnit.IsFixedOffset() &&
            param._offsetUnit.FirstOftVal() <= PreprocValLim &&
            !param._offsetUnit.IsAblePreProcess()){
        return;
    }
#endif
    if(param._prepOpti.IsFitting()) {
        CreateFitting fitt;
        //fitt.BndIslLoopGeomFitting(region,lenVal,disVal,angVal);
        fitt.FittingGeomRegion(numst::FitParam(8e-3,10),region);
    }
}
void ProIOInterface::BorderLoopOffset(ofts::GeomArea& region,
                                      ofts::OffsetParam& param,
                                      std::shared_ptr<ofts::OffsetNode>& pathNode)
{
    if(pathNode == nullptr){pathNode = make_shared<ofts::OffsetNode>();}
    auto& loop = region.BLoop();
    if(loop.IsEmpty() || (loop.Leftward() && !param._offsetUnit.IsFixedOffset())){
        return;
    }
    ExicuteProgram exepro;
    if(param._pathTech.ProfOffsetStyles() == Plane_Style_Milling){
        //faceMillMsgIO(region._obLoop,param,pathNode);
        RootNodeSolve().SetNodeToolPathType(BndOffsetPath,pathNode);
        return;
    }
    loop.SetThisOftNum(0);
    exepro.LoopOffsetNodeResult(loop,param,pathNode);
    if(param._pathTech.IsAddOriginLoop()){
        pathNode->InsertLoop(0,region._obLoop);
    }
    if(!param._pathTech.IsAddOriginLoop() &&
            param._pathTech.IsBlankBoundary() &&
            param._pathTech.IsAcceptBoundaryOut()){
        pathNode->InsertLoop(0,region._obLoop);
    }
    //Ofte::ToolPathType type = bndIslLoop.BndLoop()._label._loPathType;
    RootNodeSolve().SetNodeToolPathType(BndOffsetPath,pathNode);
}
void ProIOInterface::InPutOutPutIO(ofts::GeomArea region,
                                   ofts::OffsetParam param,
                                   std::shared_ptr<ofts::OffsetNode>& pathNode)
{
#if 0
    DisplayGeom().DisplayGeomArea(region,true);
#endif
    if(!IsValidExicute(region,param,pathNode)){return;}
    if(param._offsetUnit.IsFixedOffset()){
        param._offsetUnit.SetAlterableValue(false);
    }
#if 0
    auto sTime = std::chrono::high_resolution_clock::now();
#endif
    if(!region.BndLoop().IsLegalNum()){return;}
    auto& ils = region._vIslLoop;
    ils.erase(std::remove_if(ils.begin(),ils.end(),[](const ofts::DefLoop& l){
        return !l.IsLegalNum();}),ils.end());
#if 0
    string str = OperateOftString().BndIslLoopString(region);
    std::cout<<str<<std::endl;
#endif
    double zCoord = region.BndLoop().ZCoord();
    try {
        CreateTrajectory ctra;
        PreprocesseInput(region,param);
        if(ils.empty()){
            BorderLoopOffset(region,param,pathNode);
        }else{
            ctra.CreateTrajectoryInterface(region,param,pathNode);
        }
        NodePathAfterprocess(param,zCoord,pathNode);
    }catch (const std::exception& e){
        std::cerr << "偏置产生了异常: " << e.what() << std::endl;
    }catch(...){
        std::cerr << "偏置产生了未知异常" << std::endl;
    }
#if 0
    std::vector<ofts::DefLoop>loops;
    Point p0(-70.72471076250076,46.52969264984131,-5.499);
    RootNodeSolve().PathRootNodeLoop(pathNode,loops);
    for (size_t i = 0; i < loops.size(); i++){
        auto& l = loops[i];
        if(l.IsExistObjPt(p0,PreErr5_6)){
            int tem = 1;
        }
    }
#endif
  
#if 0
    auto eTime = std::chrono::high_resolution_clock::now();
    double exeTime = std::chrono::duration<double>(eTime - sTime).count();
    Time_total += exeTime * 1000;
#endif
}
void ProIOInterface::NodePathAfterprocess(ofts::OffsetParam param,double zCoord,
                                          std::shared_ptr<ofts::OffsetNode> &pathNode)
{
    RootNodeSolve nodsol;
    if(param.IsAfterProc()) {
        nodsol.DelEmptyOffsetNode(pathNode);
        nodsol.OffsetNodeArcPrecision(PreErr5_4, PreErr5_4,
                                      PreErr5_4, true, 4, pathNode);
        nodsol.OffsetNodeCircleArcTrans(true, pathNode);
        PathDirection().SpecifyPathMillDirection(param, pathNode);
        int index = 0;
        if(pathNode->Loops().empty()) {
            index = -1;
        }
        nodsol.SetOffsetNodeAreaId(pathNode, index);
    }
    nodsol.SetOffsetNodeZ(zCoord, pathNode);
}
