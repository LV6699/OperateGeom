#ifndef MERGEREGAUX_H
#define MERGEREGAUX_H
#include<vector>
#include"../../CommonFile/DataStructure.h"
class MergeRegAux
{
public:
    MergeRegAux(){}
    void ClassfyClipResult(std::vector<OffsetStruct::DefLoop>oriLoops,
                           std::vector<OffsetStruct::DefLoop>rLoops,
                           std::vector<OffsetStruct::DefLoop>& notClipLoops,
                           std::vector<OffsetStruct::DefLoop>& clipLoops);
    void AddOriIslLoopToNode(OffsetStruct::OffsetParam param,
                             int hasOftNum,
                             const std::vector<OffsetStruct::DefLoop> 
&oriIloops,
                             const std::vector<OffsetStruct::DefLoop> &iloops,
                             OffsetStruct::NodeForm& nodeForm);
    void LoopNodeInArrayLoop(const std::vector<OffsetStruct::DefLoop>& iloops,
                             std::vector<OffsetStruct::LoopNode>& iNodes,
                             OffsetStruct::NodeForm& nodeForm);
    void AddBndLoopToBndNode(OffsetStruct::OffsetParam offsetPara,
                             const OffsetStruct::DefLoop &curBndLoop,
                             const std::vector<OffsetStruct::DefLoop> &vIslLoop,
                             bool isExiClip,
                             std::shared_ptr<OffsetStruct::OffsetNode>& bndNode,
                             
std::vector<std::shared_ptr<OffsetStruct::OffsetNode>>&vBndOftNode);
    void LoopsMatchLoopsToAreas(const std::vector<OffsetStruct::DefLoop> 
&bLoops,
                                const std::vector<OffsetStruct::DefLoop> 
&iLoops,
                                std::vector<OffsetStruct::DefLoop> &notIncLoops,
                                std::vector<OffsetStruct::GeomArea> &regions);
    bool IsBoundaryInsideIsland(const OffsetStruct::DefLoop &bloop,
                                const std::vector<OffsetStruct::DefLoop> 
&iloops);
    void LoopsInsideOutsideLoop(const std::vector<OffsetStruct::DefLoop>& loops,
                                std::vector<OffsetStruct::DefLoop>& inloops,
                                std::vector<OffsetStruct::DefLoop>& outloops);
    void AdjustNotCWLoop(std::vector<OffsetStruct::DefLoop>& loops);
    bool IsInsideOtherArea(const std::vector<OffsetStruct::GeomArea>& areas,
                           const OffsetStruct::DefLoop& loop, int id);
};
#endif // MERGEREGAUX_H