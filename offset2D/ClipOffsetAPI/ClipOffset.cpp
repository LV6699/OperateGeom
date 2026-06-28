#include "ClipOffset.h"
#include "ExicuteOffset.h"
#include "ToolTrajectoryAlg/ProIOInterface.h"
#include "ToolTrajectoryAlg/GeometryClip/GeomBoolUnion/LoopBoolUnio.h"
#include "ToolTrajectoryAlg/OperCurve2D/OperCurve2D/LayoutRegion.h"
namespace boolean_offset_api{
std::vector<ofts::DefLoop> NodesToLoops(const std::vector<std::shared_ptr<ofts::OffsetNode>>& ns1,
                                        const std::vector<std::shared_ptr<ofts::OffsetNode>>& ns2)
{
    std::vector<ofts::DefLoop> ls;
    for(auto& d : ns1){auto ls1 = d->AllLoops();ls.insert(ls.end(),ls1.begin(),ls1.end());}
    for(auto& d : ns2){auto ls1 = d->AllLoops();ls.insert(ls.end(),ls1.begin(),ls1.end());}
    return ls;
}
//目前n 里面的数据只能是偏置一次后的结果
PolyTree NodeToPolyTree(const std::shared_ptr<ofts::OffsetNode>& n)
{
    PolyTree tree;
    if(n == nullptr){return tree;}
    const auto& loops = n->Loops();
    for(const auto& l : loops){
        auto t = PolyTree::BuildTree(l);
        tree.AddNode(t);
    }
    return tree;
}
void Offseter::Execute(double delta, Paths& soltions)
{
    std::vector<std::shared_ptr<ofts::OffsetNode>>close_ns,open_ns;
    const auto& loops = _loops;
    ofts::OffsetParam param(true,1,delta,true,true,false);
    ExicuteOffset().OffsetInterface(loops,delta,NoZezo(),close_ns,open_ns);
    soltions = NodesToLoops(close_ns,open_ns);
    if((close_ns.size() < 2 && open_ns.empty()) || (open_ns.size() < 2 && close_ns.empty())){
        return;
    }
    LoopBoolUnio lbu;
    std::vector<ofts::DefLoop>rloops;
    lbu.LoopsBoolUnion(soltions,rloops);
    if(!lbu.HasClip() || rloops.empty()){return;}
    soltions = rloops;
}
void Offseter::Execute(double delta,PolyTree& polytree)
{
    std::vector<std::shared_ptr<ofts::OffsetNode>>close_ns,open_ns;
    const auto& loops = _loops;
    ofts::OffsetParam param(true,1,delta,true,true,false);
    ExicuteOffset().OffsetInterface(loops,delta,NoZezo(),close_ns,open_ns);

    if(close_ns.empty() && open_ns.empty()){return;}
    if((close_ns.size() < 2 && open_ns.empty())){
        polytree = NodeToPolyTree(close_ns[0]);return;
    }
    if((open_ns.size() < 2 && close_ns.empty())){
        polytree = NodeToPolyTree(open_ns[0]);return;
    }
    LoopBoolUnio lbu;
    std::vector<ofts::DefLoop>loops1,rloops;
    loops1 = NodesToLoops(close_ns,open_ns);
    lbu.LoopsBoolUnion(loops1,rloops);
    if(!lbu.HasClip() || rloops.empty()){rloops = loops1;};

    numst::ContParam cpa(PreErr_10,PreErr_10,PreErr_10,true);
    auto trees = LayoutRegion().LoopsToLoopTrees(cpa,rloops);
    for (size_t i = 0; i < trees.size(); i++){
        polytree.AddNode(trees[i]);
    }
}
};
