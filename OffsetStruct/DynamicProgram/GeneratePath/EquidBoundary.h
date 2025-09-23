#ifndef EQUIDBOUNDARY_H
#define EQUIDBOUNDARY_H

#include<vector>
#include"../../CommonFile/DataStructure.h"

class EquidBoundary{
public:
    EquidBoundary(){}
    void EquidistantceBoundary(OffsetParam param,
                               DefLoop bloop,
                               vector<DefLoop>& loops);
    void EquidistantceBoundary(OffsetParam param,
                               const DefLoop& bloop,
                               const vector<DefLoop>& iloops,
                               shared_ptr<OffsetNode>& bNode,
                               NodeForm& nodeForm,
                               vector<GeomArea>& regions);
    void IsLoopInGeomArea(const vector<GeomArea>& areas,
                          const vector<DefLoop>& iloops,
                          const vector<DefLoop>& noIncLoops_,
                          vector<DefLoop>& noIncLoops);


};

#endif // EQUIDBOUNDARY_H
