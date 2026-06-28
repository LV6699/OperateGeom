#include "EquidIsland.h"
#include "EquidIslandAux.h"
#include "../../GeometryCalculate/RelativeContain/LoopContain.h"
#include "../../CommonFile/Header.h"
#include"../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
using namespace std;
using namespace OffsetStruct;
ofts::InOutLoop EquidIsland::GetInOutLoop(std::vector<ofts::DefLoop>& loops,
                                          std::vector<ofts::LoopNode>& newNodes)
{
    ofts::InOutLoop ioloop;
    std::vector<ofts::DefLoop>others;
    others.reserve(loops.size()-1);
    numst::ContParam cpa(PreErr5_8,PreErr5_8,PreErr5_10);
    for (size_t i = 0; i < loops.size(); ++i) {
        for (size_t j = 0; j < loops.size(); j++){
            if(i == j){continue;}
            others.emplace_back(loops[j]);
        }
        auto& loop = loops[i];
        /*DisplayGeom().DisplayLoop(loop,_colors.back(),2,true,true);
        DisplayGeom().DisplayLoops(others,_colors.back(),1,true);
        auto lim = LimitCoord2D::LoopLimitCoord(loop);*/
        if(!LoopContain::IsLoopsContLoop(others,loop,cpa)){
            ioloop._inLoops.emplace_back(loop);
            newNodes.emplace_back(ofts::LoopNode(loop,make_shared<ofts::OffsetNode>()));
            continue;
        }
        //loop.SetLoopType(Ofte::OUTSIDEPROFILE);
        loop.SetLeftward(false);
        ioloop._outLoops.emplace_back(loop);
        if(loops.size() == 2 && i == 0){
            ioloop._inLoops.emplace_back(loop);
            newNodes.emplace_back(ofts::LoopNode(loops[1],make_shared<ofts::OffsetNode>()));
            break;
        }
        others.clear();
    }
    return ioloop;
}
void EquidIsland::EquidistantceIsland(OffsetParam param,
                                      vector<LoopNode>& iNodes,
                                      vector<GeomArea>& regions,
                                      NodeForm& nodeForm)
{
    if((param._offsetUnit.IsFixedOffset() &&
        param._offsetUnit.OffsetNumber() == 0) || iNodes.empty()){return;}
    ExicuteProgram exepro;GeometryLogic golg;EquidIslandAux islsol;
    vector<LoopNode>newNodes;
    vector<InOutLoop>ioLoops;
    param._offsetUnit.SetFixedOffset(true);
    param._offsetUnit.SetOffsetNumber(1);
    param._offsetUnit.SetAblePreProcess(true);
    for (size_t i = 0; i < iNodes.size(); ++i) {
        if(!iNodes[i].m_loop.IsOffset()){continue;}
        shared_ptr<OffsetNode> node;
        golg.CircleArcLoopTransform(false,iNodes[i].m_loop);
        iNodes[i].m_loop.SetThisOftNum(0);
        //iNodes[i].m_loop.SetLoopType(WITHINPROFILE);
        iNodes[i].m_loop.SetLeftward(true);
        exepro.LoopOffsetNodeResult(iNodes[i].m_loop,param,node);
        auto rLoops = node->AllLoops();
#if 0
        DisplayGeom().DisplayLoop(iNodes[i].m_loop,_colors.back(),1,true,true);
        DisplayGeom().DisplayLoops(rLoops,_colors[1],1,true);
        auto s = OperateOftString().LoopElementString(iNodes[i].m_loop);
        std::cout<<s<<std::endl;
#endif
        ///当rLoops.size() == 1 时,rLoops 里面的元素不可
        ///能是外环,或包含其他岛屿环.
        if(rLoops.size() == 1){
            golg.CircleArcLoopTransform(true,rLoops[0]);
            iNodes[i].m_loop = rLoops[0];
            continue;
        }
        if(rLoops.empty()){
            islsol.AddDeleteIslNode(iNodes,i,nodeForm);
            i --;continue;
        }
        ///岛屿环偏移后产生多个环
        islsol.AddDeleteIslNode(iNodes,i,nodeForm);
        auto ioLoop = GetInOutLoop(rLoops,newNodes);/*
        for(auto &l : rLoops){
            if(l.LoopType() == WITHINPROFILE){
                LoopNode data(l,make_shared<OffsetNode>());
                newNodes.emplace_back(data);
                ioLoop._inLoops.emplace_back(l);
            }else{
                ioLoop._outLoops.emplace_back(l);
            }
        }*/
        ioLoops.emplace_back(ioLoop);
        i --;
    }
    EquidIslandAux aux;
    vector<DefLoop>inloops,outloops;
    aux.DeleteInsidedOutLoop(iNodes,ioLoops);
    for(auto& d : ioLoops){
        for(auto& l : d._outLoops){
            outloops.emplace_back(l);
        }
        for(auto& l : d._inLoops){
            inloops.emplace_back(l);
        }
    }
    aux.OperateEquidOutLoop(inloops,outloops,iNodes,nodeForm,regions);
    for(auto &d : newNodes){
        iNodes.emplace_back(d);
    }
    for(auto& d : outloops){
        nodeForm._iLocLoops.emplace_back(d);
    }
}
