#ifndef TRAJECTORYSOLVE_H
#define TRAJECTORYSOLVE_H
#include<vector>
#include"../CommonFile/DataStructure.h"
class TrajectorySolve
{
public:
    TrajectorySolve(){}
    void SetNodeLoopSpecifyOrigin(OffsetStruct::OffsetParam param,
                                  
std::shared_ptr<OffsetStruct::OffsetNode>&pathNode);
    void AddBndLoopToBndNode(OffsetStruct::OffsetParam param,
                             const OffsetStruct::DefLoop &bloop,
                             const std::vector<OffsetStruct::DefLoop> &iloops,
                             bool isExiClip,
                             std::shared_ptr<OffsetStruct::OffsetNode>& bNode,
                             OffsetStruct::NodeForm& nodeForm);
    void DeleteLoopNotInLoops(const std::vector<OffsetStruct::DefLoop>& loops,
                              std::vector<OffsetStruct::DefLoop>& loops_);
    void InitialLoopNode(const std::vector<OffsetStruct::DefLoop> &loops,
                         OffsetStruct::OffsetParam param,bool isAdd,
                         std::vector<OffsetStruct::LoopNode>&nodes,
                         const OffsetStruct::GeomArea& area = 
OffsetStruct::GeomArea());
    void AddLoopNodeToPathNode(const std::vector<OffsetStruct::LoopNode>&lNodes,
                               ToolPathType type, bool isReverse,
                               
std::vector<std::shared_ptr<OffsetStruct::OffsetNode>>&nodes);
    void AddRegionsToPathNode(OffsetStruct::OffsetParam param,
                              std::vector<OffsetStruct::GeomArea>&areas,
                              OffsetStruct::NodeForm& nodeForm);
    void OperateNodeForm(OffsetStruct::OffsetParam param,
                         OffsetStruct::NodeForm& nodeForm,
                         std::shared_ptr<OffsetStruct::OffsetNode> &pathNode);
    void GetLoopOffsetNode(OffsetStruct::OffsetParam param,
                           const OffsetStruct::DefLoop& loop,
                           std::shared_ptr<OffsetStruct::OffsetNode>& node);
};
#endif // TRAJECTORYSOLVE_H