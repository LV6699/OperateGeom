#ifndef EQUIDBOUNDARY_H
#define EQUIDBOUNDARY_H
#include<vector>
#include"../../CommonFile/DataStructure.h"
class EquidBoundary
{
public:
    EquidBoundary(){}
    void EquidistantceBoundary(OffsetStruct::OffsetParam param,
                               OffsetStruct::DefLoop bloop,
                               std::vector<OffsetStruct::DefLoop>& loops);
    void EquidistantceBoundary(OffsetStruct::OffsetParam param,
                               const OffsetStruct::DefLoop &bloop,
                               const std::vector<OffsetStruct::DefLoop>& iloops,
                               std::shared_ptr<OffsetStruct::OffsetNode> &bNode,
                               OffsetStruct::NodeForm &nodeForm,
                               std::vector<OffsetStruct::GeomArea>& regions);
    void IsLoopInGeomArea(const std::vector<OffsetStruct::GeomArea> &areas,
                          const std::vector<OffsetStruct::DefLoop> &iloops,
                          const std::vector<OffsetStruct::DefLoop>&noIncLoops_,
                          std::vector<OffsetStruct::DefLoop>& noIncLoops);
};
#endif // EQUIDBOUNDARY_H