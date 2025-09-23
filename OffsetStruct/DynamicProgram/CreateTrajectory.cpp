#include "CreateTrajectory.h"
#include "TrajectorySolve.h"
#include "EqudistArea/EquidBoundary.h"
#include "EqudistArea/EquidIsland.h"
#include "GeneratePath/MergeRegion.h"
#include "GeneratePath/MergeRegAux.h"
#include "../CommonFile/Header.h"
#if 1
#include "../../OperateView/OperateView/DisplayGeom.h"
#endif
using namespace std;
using namespace oft;
void CreateTrajectory::CreateTrajectoryInterface(GeomArea& region,
                                                 OffsetParam& param,
                                                 shared_ptr<OffsetNode>& 
pathNode)
{
    if(region.BndLoop().IsEmpty() ||
            region.IslLoop().empty() ||
            (!param._pathTech.IsBoundaryOffset() &&
             !param._offsetUnit.IsFixedOffset()))
        return;
    if(pathNode == nullptr){
        pathNode = make_shared<OffsetNode>();
    }
    pathNode->ClearData();
    region._bndLoop.SetPastOftNum(0);
    for(auto& l : region._vIslLoop){
        l.SetPastOftNum(0);
    }
    for(auto& l : region._vBlockLoop){
        l.SetPastOftNum(0);
    }
    vector<GeomArea>regions{region};
    CreateSpecifyTrajectory(regions,param,pathNode);
}
void CreateTrajectory::CreateSpecifyTrajectory(vector<GeomArea>& regions,
                                               OffsetParam param,
                                               shared_ptr<OffsetNode>& pathNode)
{
    GeometryLogic golg;BIReguOftVal oftval;
    EquidIsland eisl;MergeRegion mreg;
    TrajectorySolve tsol;MergeRegAux maux;
    int fixOftNum = param._offsetUnit.OffsetNumber();
    bool isFixOft = param._offsetUnit.IsFixedOffset(),isFirOft = true;
    double firOftVal = param._offsetUnit.FirstOffsetValue(),
            oriOftVal = param._offsetUnit.OffsetValue();
    param.SetIsFirstOffset(true);
    param._offsetUnit.SetAblePreProcess(true);
    NodeForm nodeForm;
    while(!regions.empty()){
        if(!regions[0].IsValidMatch()){
            regions.erase(regions.begin());
            continue;
        }
        GeomArea& region = regions[0];
        DefLoop bloop = region._bndLoop;        vector<DefLoop>iloops = region._vIslLoop;
        if(isFixOft && bloop.PastOffsetNum() >= fixOftNum)
            break;
        shared_ptr<OffsetNode>bNode = make_shared<OffsetNode>();
        if(param._pathTech.IsAddOriginLoop() && bloop.PastOffsetNum() == 0){
            bNode->PushBackLoop(region._oriBndLoop);
        }
        if(region.IsAddBndLoop() && param._pathTech.IsBoundaryOffset()){
            bNode->PushBackLoop(bloop);
        }
        vector<LoopNode>iNodes;
        tsol.InitialLoopNode(iloops,param,region.IsAddIslLoop(),iNodes,region);
        regions.erase(regions.begin());
        int hasOftNum = bloop.PastOffsetNum();
        while(!iloops.empty()){
            oftval.RegulateRegionOftValue(bloop,iNodes,isFirOft,
                                          firOftVal,oriOftVal,param);
            ///iNodes 中的loop每次在改变.
            vector<DefLoop>iOftLoops;
            eisl.EquidistantceIsland(param,iNodes,regions,nodeForm);
            for(auto &l : iNodes){
                golg.CircleArcLoopTransform(true,l.m_loop);
                iOftLoops.push_back(l.m_loop);
            }
#if 0
            DisplayGeom().DisplayLoops(iOftLoops,_colors.back(),1.5);
            string str = OperateOftString().ArrayLoopString(iOftLoops);
            std::cout<<str<<endl;
#endif
            iloops.clear();
            vector<DefLoop>merOutLoops;
            /// 对所有非岛屿合并外环 ,需要作为新的无岛屿边界环 .
            mreg.MergeIslandLoops(regions,iNodes,iOftLoops,
                                  iloops,merOutLoops,nodeForm);
#if 0
            DisplayGeom().DisplayLoops(iloops,_colors[0],1.5);
            DisplayGeom().DisplayLoops(iOftLoops,_colors[1],1.5);
#endif
            ///vIslNode-vIslOftLoop-vIslLoop 、vIslOutLoop-vIslNode
            /// 做岛屿环(包括跟其他岛屿环不相交的原始岛屿环 ,以及合并过后的局部外环 ),
            /// 与边界环进行相交合并计算 .
            bool isOriBndClip = false,isExiClip = false;
            mreg.MergeGeometryRegion(param,bloop,iloops,bNode,isOriBndClip,
                                     isExiClip,nodeForm,regions);
            if(isOriBndClip && !bNode->_vLoopData.empty()
                    && param._pathTech.IsBoundaryOffset()){
                bNode->_vLoopData.pop_back();
            }
            tsol.AddBndLoopToBndNode(param,bloop,iloops,
                                     isExiClip,bNode,nodeForm);/**
            做边界-岛屿环合并计算的后处理 ,判断偏置时的环iNodes(可能已经相互合
            并),是否与iloops中的匹配,iloops中的数据包括未合并的岛屿环、合并
            岛屿环后产生的局部外环、前两者与边界环合并后产生的新岛屿环 .*/
            maux.LoopNodeInArrayLoop(iloops,iNodes,nodeForm);
            /// 删除merOutLoops中不在iloops内的环.
            tsol.DeleteLoopNotInLoops(iloops,merOutLoops);/**
            此时需要把合并边界岛屿之前的，同时还没有与边界环合并的 ...岛屿间的合并外环
            merOutLoops 添加至下一轮岛屿偏置节点 iNodes中.*/
            tsol.InitialLoopNode(merOutLoops,param,true,iNodes);
            hasOftNum ++;
            isFirOft = false;
            param.SetIsFirstOffset(false);            if(!param._offsetUnit.IsFixedOffset() ||
                    (hasOftNum < param._offsetUnit.OffsetNumber()))
                continue;
            tsol.AddLoopNodeToPathNode(iNodes,IslOffsetPath,
                                       true,nodeForm._iOftNodes);
            tsol.AddRegionsToPathNode(param,regions,nodeForm);
            break;
        }
        if(!bNode->IsEmpty() && param._pathTech.IsBoundaryOffset()){
            nodeForm._bOftNodes.emplace_back(bNode);
        }
    }
#if 0
    DisplayGeom().DisplayLoops(nodeForm._bLocLoops,_colors[2],1.5);
    DisplayGeom().DisplayLoops(nodeForm._iLocLoops,_colors[2],1.5);
    DisplayGeom().DisplayLoops(nodeForm._biMerLoops,_colors[0],1.5);
    DisplayGeom().DisplayLoops(nodeForm._iiMerLoops,_colors[3],1.5);
    for(auto& d : nodeForm._bOftNodes){
        DisplayGeom().DisplayPathNode(d,false,true);
    }
    for(auto& d : nodeForm._iOftNodes){
        DisplayGeom().DisplayPathNode(d,false,true);
    }
#endif
    param._offsetUnit.SetOffsetValue(oriOftVal);
    tsol.OperateNodeForm(param,nodeForm,pathNode);
    tsol.SetNodeLoopSpecifyOrigin(param,pathNode);
}