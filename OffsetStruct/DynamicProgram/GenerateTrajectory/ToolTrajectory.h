Ft5ztRnC4DrSpsSWEJr7Yu4kNV3q
Page 1#ifndef ToolTrajectory_H
#define ToolTrajectory_H
#include"../../CommonFile/Header.h"
class CAMALGORITHM_EXPORT ToolTrajectory
{
public:
    ToolTrajectory(){}
    void CreateTrajectoryInterface(GeomArea& biLoop
                                   OffsetParam &offsetPara
                                   shared_ptr<OffsetNode> &rootNode);
    void CreateSpecifyTrajectory(vector<GeomArea> vBILoop
                                 OffsetParam offsetPara
                                 shared_ptr<OffsetNode> &rootNode);
};
#endif // ToolTrajectory_H