#ifndef MERGEREGLOGIC_H
#define MERGEREGLOGIC_H
#include<vector>
#include"../../CommonFile/DataStructure.h"
class MergeRegLogic
{
public:
    MergeRegLogic(){}
    void MergeOriginBoundary(ofts::DefLoop& bloop,
                             std::vector<ofts::DefLoop>& iloops,
                             ofts::OffsetParam param,
                             bool &isBndInside,
                             bool &isExistClip,
                             ofts::NodeForm& nodeForm,
                             std::vector<ofts::GeomArea>& regions);
    void ExicuteMergeRegion(ofts::GeomArea &area,
                            bool& isExistClip,
                            ofts::DefLoop &bloop,
                            std::vector<ofts::DefLoop> &iLoops,
                            ofts::NodeForm& nodeForm,
                            std::vector<ofts::GeomArea>& regions);
    void MergeMultipleRegions(ofts::DefLoop& bloop,
                              std::vector<ofts::DefLoop>&iLoops,
                              bool &isExistClip,
                              std::vector<ofts::GeomArea>& areas,
                              ofts::NodeForm& nodeForm,
                              std::vector<ofts::GeomArea>&regions);
    void HandleIslandMergeInside(std::vector<ofts::DefLoop>& inloops,
                                 std::vector<ofts::DefLoop>& iloops,
                                 std::vector<ofts::LoopNode>& iNodes,
                                 std::vector<ofts::GeomArea>& areas);
    bool IsBLoopInIslLoop(const ofts::DefLoop& bloop,
                          std::vector<ofts::DefLoop>&iloops);
};
#endif // MERGEREGLOGIC_H
