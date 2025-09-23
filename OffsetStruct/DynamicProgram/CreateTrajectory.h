#ifndef CREATETRAJECTORY_H
#define CREATETRAJECTORY_H
#include<vector>
#include"../CommonFile/DataStructure.h"
class CreateTrajectory
{
public:
    CreateTrajectory(){}
    void CreateTrajectoryInterface(OffsetStruct::GeomArea& region,
                                   OffsetStruct::OffsetParam &param,
                                   std::shared_ptr<OffsetStruct::OffsetNode> 
&pathNode);
private:
    void CreateSpecifyTrajectory(std::vector<OffsetStruct::GeomArea> &regions,
                                 OffsetStruct::OffsetParam param,
                                 std::shared_ptr<OffsetStruct::OffsetNode> 
&pathNode);
};
#endif // CREATETRAJECTORY_H