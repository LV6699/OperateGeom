#include"MergeRegion.h"
#include"MergeRegLogic.h"
#include"MergeRegAux.h"
#include"../../CommonFile/Header.h"
#include"../EqudistArea/EquidBoundary.h"
#include"../../GeometryClip/GeomBoolUnion/LoopBoolUnio.h"
#include"../../OperCurve2D/OperCurve2D/OperateLoop.h"
#include"../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
using namespace std;
void MergeRegion::MergeIslandLoops(vector<GeomArea>& regions,
                                   vector<LoopNode>& iNodes,
                                   vector<DefLoop>inLoops,
                                   vector<DefLoop>& iloops,
                                   vector<DefLoop>& outLoops,
                                   OffsetStruct::NodeForm& nodeForm)
{
    ///对所有岛屿环进行相交合并计算,从这里开始产生新的iloops，
    ///其数据来源于未合并的inLoops 以及合并的vLocOutLoop；
    for(auto& l : inLoops){for(auto& c : l){c.SetSourceType(IslOffsetPath);}}
    vector<DefLoop>rloops,rloops_,insideLoops;
    ///unio.LoopGeomBooleanUnion(inLoops,barLoops,rloops);
    LoopBoolUnio lbu;
    lbu.LoopsBoolUnion(inLoops,rloops);
#if 0
    DisplayGeom().DisplayLoops(inLoops,_colors.back(),1,false,true);
    DisplayGeom().DisplayLoops(rloops,_colors[1],2,true);
    string s = OperateOftString().ArrayLoopString(inLoops);
    std::cout<<s<<std::endl;
#endif
    for(auto& l : rloops){for(auto& c : l){c.SetSourceType(IslOffsetPath);}}
    MergeRegAux().ClassfyClipResult(inLoops,rloops,iloops,rloops_);
    if(!lbu.HasClip() || rloops_.empty()){
        iloops = inLoops;return;
    }
    ///对所有非合并外环的其他岛屿合并环,需要作为新的无岛屿边界环,即
    ///rloops_中,如果一个环在其他环内,则存放至insideLoops 中,
    ///否则存放在outLoops 中.
    ///这里的outLoops 是合并外环,insideLoops 是合并内环.
    MergeRegAux().LoopsInsideOutsideLoop(rloops_,insideLoops,outLoops);
    MergeRegAux().AdjustNotCWLoop(insideLoops);
    ///对于所有岛屿合并非外环,其可能是新的边界环,如果其完全包含原始岛屿环,则其与
    ///所包含的岛屿环组成新的vBndIslLoop 成员.
#if 0
    string str = OperateOftString().ArrayLoopString(rloops_);
    std::cout<<str<<endl;
#endif
    MergeRegLogic().HandleIslandMergeInside(insideLoops,iloops,iNodes,regions);
    for(auto& it : outLoops){iloops.push_back(it);}
    for(auto& it : insideLoops){nodeForm._iiMerLoops.push_back(it);}
}
void MergeRegion::MergeGeometryRegion(OffsetParam param,
                                      DefLoop& bloop,
                                      vector<DefLoop>& iLoops,
                                      shared_ptr<OffsetNode>&bNode,
                                      bool& isOriBndClip,
                                      bool& isExistClip,
                                      NodeForm& nodeForm,
                                      vector<GeomArea>& regions)
{
#if 0
    DisplayGeom().DisplayGeomArea(GeomArea(bloop,iLoops),
                                  _colors.back(),1.5,true);
    _mainwind->myOccView->fitAll();
#endif
    /*if(param._offsetUnit.OffsetValue() == 2.6666666666666665){
        int tem = 1;
    }*/
    if(!param._pathTech.IsBoundaryOffset() ||
            iLoops.empty() || bloop.IsEmpty()){return;}
    for(auto& d : iLoops){OperateLoop().CircleArcLoopTrans(true,d);}
    for(auto& d : bloop){d.SetSourceType(BndOffsetPath);}
    for(auto& l : iLoops){for(auto& c : l){c.SetSourceType(IslOffsetPath);}}
    OperateLoop().LoopCurveEndToEnd(bloop);
    bool isBndInside_ = false;
    if(param._pathTech.IsBlankBoundary() && bloop.PastOftNum() == 0){
        MergeRegLogic().MergeOriginBoundary(bloop,iLoops,param,isBndInside_,
                                            isOriBndClip,nodeForm,regions);
        if(!isOriBndClip && param._pathTech.IsAcceptBoundaryOut() && !isBndInside_){
            bNode->InsertLoop(0,bloop);
        }
    }
    if(isOriBndClip || isBndInside_){
        isExistClip = true;return;
    }
    vector<GeomArea>areas;
    if(param._pathTech.IsBoundaryOffset() && bloop.IsOffset()){
        EquidBoundary().EquidistantceBoundary(param,bloop,iLoops,bNode,nodeForm,areas);
    }else{
        areas.emplace_back(GeomArea(bloop,iLoops));
    }
    if(areas.empty()){  ///说明边界环在岛屿环内部.
        iLoops.clear();bloop.Clear();return;
    }
    for(auto& d : areas){
        for(auto& c : d._bndLoop){c.SetSourceType(BndOffsetPath);}
        for(auto& l : d._vIslLoop){for(auto& c : l){c.SetSourceType(IslOffsetPath);}}
    }
    if(areas.size() == 1){
        MergeRegLogic().ExicuteMergeRegion(areas[0],isExistClip,
                bloop,iLoops,nodeForm,regions);return;
    }
    ///偏置单个边界环后,产生的边界岛屿配对环大于1 个的情况,需要对所有配对环进行合并计算.
    RootNodeSolve().AddNodeToNodes(bNode,BndOffsetPath,true,nodeForm._bOftNodes);
    MergeRegLogic().MergeMultipleRegions(bloop,iLoops,isExistClip,areas,nodeForm,regions);
}
