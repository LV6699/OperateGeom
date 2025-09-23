#ifndef EQUIDISLAND_H
#define EQUIDISLAND_H

#include<vector>
#include"../../CommonFile/DataStructure.h"

class EquidIsland{
public:
    void EquidistantceIsland(OffsetParam param,
                             vector<LoopNode>& iNodes,
                             vector<GeomArea>& regions,
                             NodeForm& nodeForm);


}
#endif // EQUIDISLAND_H
