#include "ExicuteOffset.h"
#include "ToolTrajectoryAlg/RootNodeSolve.h"
#include "ToolTrajectoryAlg/ProIOInterface.h"
#include "ToolTrajectoryAlg/OperCurve2D/OperCurve2D/LayoutRegion.h"
#include "ToolTrajectoryAlg/GeometryClip/GeomBoolUnion/LoopBoolUnio.h"
#include "ToolTrajectoryAlg/GeometryCalculate/GeomCalculate/LoopDirection.h"
#include "../offset2D/OperateView/OperateView/DisplayGeom.h"
namespace boolean_offset_api{
void ExicuteOffset::OffsetCloseRegion(std::vector<ofts::DefLoop>& loops,double oftD,bool no_zezo,
                                      std::vector<std::shared_ptr<ofts::OffsetNode>>& nodes)
{
    ofts::OffsetParam param(true,1,std::abs(oftD),true,true,false);
    numst::ContParam cpa(PreErr_10,PreErr_10,PreErr_10,true);
    auto trees = LayoutRegion().LoopsToLoopTrees(cpa,loops);
    /*for (size_t i = 0; i < trees.size(); i++){
        DisplayGeom().DrawLoopTree(trees[i],1.5,true,0,true);
    }*/
    nodes.reserve(trees.size());
    for (size_t i = 0; i < trees.size(); i++){
        auto& tree = trees[i];
        RootNodeSolve().SetLoopTreeDire(tree);
        auto regions = RootNodeSolve().LoopTreeToRegions(tree,no_zezo);
        for (size_t j = 0; j < regions.size(); j++){
            auto& r = regions[j];
            bool iscw = r.BLoop().IsCW();
            if(!iscw){r._bndLoop.Reverse();}
            for(auto& l : r){if(!l.IsCW()){l.Reverse();}}
            std::shared_ptr<ofts::OffsetNode>node;
            //DisplayGeom().DisplayGeomArea(r,_colors.back(),1,1.5,true);
            if(oftD < 0){
                r._bndLoop.SetLeftward(false);
                for(auto& l : r){l.SetLeftward(true);}
                ProIOInterface().InPutOutPutIO(r,param,node);
                if(!iscw){ReverseNodeDire(node);}
                nodes.emplace_back(node);
                //DisplayGeom().DisplayPathNode(node);
                continue;
            }
            auto& bloop = r.BLoop();bloop.SetLeftward(true);
            ProIOInterface().InPutOutPutIO(ofts::GeomArea(bloop),param,node);
            //DisplayGeom().DisplayPathNode(node);
            if(!iscw){ReverseNodeDire(node);}
            nodes.emplace_back(node);
            node = std::make_shared<ofts::OffsetNode>();;
            for(auto& l : r){
                l.SetLeftward(false);
                std::shared_ptr<ofts::OffsetNode>node1;
                ProIOInterface().InPutOutPutIO(ofts::GeomArea(l),param,node1);
                for(auto& d : node1->Loops()){
                    std::vector<DefLoop>ls{d};
                    node->AddNode(std::make_shared<ofts::OffsetNode>(ls));
                }
                if(!iscw){ReverseNodeDire(node);}
                //DisplayGeom().DisplayPathNode(node);
                nodes.emplace_back(node);
                node = std::make_shared<ofts::OffsetNode>();
            }
        }
        //std::cout<<"";
    }
    //std::cout<<"";
}
void ExicuteOffset::OffsetOpenLoops(const std::vector<ofts::DefLoop>& loops,double oftD,
                                    std::vector<std::shared_ptr<ofts::OffsetNode>>& nodes)
{
    ofts::OffsetParam param(true,1,std::abs(oftD),true,true,false);
    for (size_t i = 0; i < loops.size(); i++){
        const auto& loop = loops[i];
        std::shared_ptr<ofts::OffsetNode>node;
        //DisplayGeom().DisplayLoop(loop,_colors.back(),1,true);
        ProIOInterface().InPutOutPutIO(ofts::GeomArea(loop),param,node);
        nodes.emplace_back(node);
        //DisplayGeom().DisplayPathNode(node);
        //std::cout<<"";
    }
    //std::cout<<"";
}
void ExicuteOffset::OffsetInterface(const std::vector<ofts::DefLoop>& loops,
                                    double oftD,bool no_zezo,
                                    std::vector<std::shared_ptr<ofts::OffsetNode>>& close_ns,
                                    std::vector<std::shared_ptr<ofts::OffsetNode>>& open_ns)
{
    std::vector<ofts::DefLoop>closes,opens;
    for(auto& d : loops){
        if(d.GetEndType() == Ofte::EndType::Polygon){closes.emplace_back(d);}
        else{opens.emplace_back(d);}
    }
    OffsetCloseRegion(closes,oftD,no_zezo,close_ns);
    OffsetOpenLoops(opens,oftD,close_ns);
}
}
