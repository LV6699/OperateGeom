#ifndef MERGEREGLOGIC_H
#define MERGEREGLOGIC_H
#include<vector>
#include"../../CommonFile/DataStructure.h"
class MergeRegLogic
{
public:
    MergeRegLogic(){}
    void MergeOriginBoundary(OffsetStruct::DefLoop& bloop,
                             std::vector<OffsetStruct::DefLoop>& iloops,
                             OffsetStruct::OffsetParam param,
                             bool &isBndInside,
                             bool &isExistClip,
                             OffsetStruct::NodeForm& nodeForm,
                             std::vector<OffsetStruct::GeomArea>& regions);
    void ExicuteMergeRegion(OffsetStruct::GeomArea &area,
                            bool& isExistClip,
                            OffsetStruct::DefLoop &bloop,
                            std::vector<OffsetStruct::DefLoop> &iLoops,
                            OffsetStruct::NodeForm& nodeForm,
                            std::vector<OffsetStruct::GeomArea>& regions);
    void MergeMultipleRegions(OffsetStruct::DefLoop& bloop,
                              std::vector<OffsetStruct::DefLoop>&iLoops,
                              bool &isExistClip,
                              std::vector<OffsetStruct::GeomArea>& areas,
                              OffsetStruct::NodeForm& nodeForm,
                              std::vector<OffsetStruct::GeomArea>&regions);
    void HandleIslandMergeInside(std::vector<OffsetStruct::DefLoop>& inloops,
                                 std::vector<OffsetStruct::DefLoop>& iloops,
                                 std::vector<OffsetStruct::LoopNode>& iNodes,
                                 std::vector<OffsetStruct::GeomArea>& areas);
};
#endif // MERGEREGLOGIC_H