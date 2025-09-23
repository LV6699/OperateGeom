#ifndef EQUIDISLAND_H
#define EQUIDISLAND_H
#include<vector>
#include"../../CommonFile/DataStructure.h"
class EquidIsland
{
public:
    EquidIsland(){}
    void EquidistantceIsland(OffsetStruct::OffsetParam param
                             std::vector<OffsetStruct::LoopNode>& iNodes
                             std::vector<OffsetStruct::GeomArea>& regions
                             OffsetStruct::NodeForm& nodeForm);
};
#endif // EQUIDISLAND_H
