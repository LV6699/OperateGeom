#ifndef MERGEREGION_H
#define MERGEREGION_H
#include<vector>
#include"../../CommonFile/DataStructure.h"
class MergeRegion
{
public:
    MergeRegion(){}
    void MergeIslandLoops(std::vector<OffsetStruct::GeomArea> &regions,
                          std::vector<OffsetStruct::LoopNode>&iNodes,
                          std::vector<OffsetStruct::DefLoop>inLoops,
                          std::vector<OffsetStruct::DefLoop>& iloops,
                          std::vector<OffsetStruct::DefLoop>& outLoops,
                          OffsetStruct::NodeForm& nodeForm);
    void MergeGeometryRegion(OffsetStruct::OffsetParam param,
                             OffsetStruct::DefLoop& bloop,
                             std::vector<OffsetStruct::DefLoop>& iLoops,
                             std::shared_ptr<OffsetStruct::OffsetNode> &bNode,
                             bool& isOriBndClip,
                             bool& isExistClip,
                             OffsetStruct::NodeForm& nodeForm,
                             std::vector<OffsetStruct::GeomArea>& regions);
};
#endif // MERGEREGION_H