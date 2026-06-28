#ifndef MERGEREGAUX_H
#define MERGEREGAUX_H
#include<vector>
#include"../../CommonFile/DataStructure.h"
class MergeRegAux
{
public:
    MergeRegAux(){}
    void ClassfyClipResult(std::vector<ofts::DefLoop> oriLoops,
                           std::vector<ofts::DefLoop> rLoops,
                           std::vector<ofts::DefLoop>& notClipLoops,
                           std::vector<ofts::DefLoop>& clipLoops);
    void AddOriIslLoopToNode(ofts::OffsetParam param,
                             int hasOftNum,
                             const std::vector<ofts::DefLoop> &oriIloops,
                             const std::vector<ofts::DefLoop> &iloops,
                             ofts::NodeForm& nodeForm);
    void LoopNodeInArrayLoop(const std::vector<ofts::DefLoop>& iloops,
                             std::vector<ofts::LoopNode>& iNodes,
                             ofts::NodeForm& nodeForm);
    void AddBndLoopToBndNode(ofts::OffsetParam offsetPara,
                             const ofts::DefLoop &curBndLoop,
                             const std::vector<ofts::DefLoop> &vIslLoop,
                             bool isExiClip,
                             std::shared_ptr<ofts::OffsetNode>& bndNode,
                             std::vector<std::shared_ptr<ofts::OffsetNode>>&vBndOftNode);
    void LoopsMatchLoopsToAreas(const std::vector<ofts::DefLoop> &bLoops,
                                const std::vector<ofts::DefLoop> &iLoops,
                                std::vector<ofts::DefLoop> &notIncLoops,
                                std::vector<ofts::GeomArea> &regions);
    bool IsBoundaryInsideIsland(const ofts::DefLoop &bloop,
                                std::vector<ofts::DefLoop> &iloops);
    void LoopsInsideOutsideLoop(const std::vector<ofts::DefLoop>& loops,
                                std::vector<ofts::DefLoop>& inloops,
                                std::vector<ofts::DefLoop>& outloops);
    void AdjustNotCWLoop(std::vector<ofts::DefLoop>& loops);
    bool IsInsideOtherArea(const std::vector<ofts::GeomArea>& areas,
                           const ofts::DefLoop& loop, int id);
};
#endif // MERGEREGAUX_H
