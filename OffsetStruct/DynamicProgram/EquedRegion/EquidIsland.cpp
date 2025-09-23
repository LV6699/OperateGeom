#include "EquidIsland.h"
#include "EquidIslandAux.h"
#include "../../CommonFile/Header.h"
#include"../../../OperateView/OperateView/DisplayGeom.h"
using namespace std;
using namespace oft;
void EquidIsland::EquidistantceIsland(OffsetParam param,
                                      vector<LoopNode>& iNodes,
                                      vector<GeomArea>& regions,
                                      NodeForm& nodeForm)
{
    if((param._offsetUnit.IsFixedOffset() &&
        param._offsetUnit.OffsetNumber() == 0) || iNodes.empty())
        return;
    ExicuteProgram exepro;GeometryLogic golg;
    RootNodeSolve nodsol;EquidIslandAux islsol;
    vector<LoopNode>newNodes;
    vector<InOutLoop>ioLoops;
    param._offsetUnit.SetFixedOffset(true);
    param._offsetUnit.SetOffsetNumber(1);
    param._offsetUnit.SetAblePreProcess(true);
    for (size_t i = 0; i < iNodes.size(); ++i) {
        if(!iNodes[i].m_loop.IsOffset())
            continue;
        shared_ptr<OffsetNode> node;
        golg.CircleArcLoopTransform(false,iNodes[i].m_loop);
        iNodes[i].m_loop.SetThisOftNum(0);
        iNodes[i].m_loop.SetLoopType(WITHINPROFILE);
        
///DisplayGeom().DisplayLoop(iNodes[i].m_loop,_colors.back(),1,true,true);
        exepro.LoopOffsetNodeResult(iNodes[i].m_loop,param,node);
        vector<DefLoop>rLoops;
        nodsol.PathRootNodeLoop(node,rLoops);
        ///当rLoops.size() == 1 时,rLoops里面的元素不可
        ///能是外环,或包含其他岛屿环.
        if(rLoops.size() == 1){
            golg.CircleArcLoopTransform(true,rLoops[0]);
            iNodes[i].m_loop = rLoops[0];
            continue;
        }
        if(rLoops.empty()){
            islsol.AddDeleteIslNode(iNodes,i,nodeForm);
            i --;
            continue;
        }
        ///岛屿环偏移后产生多个环
        islsol.AddDeleteIslNode(iNodes,i,nodeForm);
        InOutLoop ioLoop;
        for(auto &l : rLoops){
            if(l.LoopType() == WITHINPROFILE){
                shared_ptr<OffsetNode>node = make_shared<OffsetNode>();
                LoopNode data(l,node);
                newNodes.push_back(data);
                ioLoop._inLoops.push_back(l);
            }else{
                ioLoop._outLoops.push_back(l);
            }
        }        ioLoops.push_back(ioLoop);
        i --;
    }
    EquidIslandAux aux;
    vector<DefLoop>inloops,outloops;
    aux.DeleteInsidedOutLoop(iNodes,ioLoops);
    for(auto& d : ioLoops){
        for(auto& l : d._outLoops){
            outloops.push_back(l);
        }
        for(auto& l : d._inLoops){
            inloops.push_back(l);
        }
    }
    aux.OperateEquidOutLoop(inloops,outloops,
                            iNodes,nodeForm,regions);
    for(auto &d : newNodes){
        iNodes.push_back(d);
    }
    for(auto& d : outloops){
        nodeForm._iLocLoops.push_back(d);
    }
}