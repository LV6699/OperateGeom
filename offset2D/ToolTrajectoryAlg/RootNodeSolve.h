#ifndef ROOTNODESOLVE_H
#define ROOTNODESOLVE_H
#include"CommonFile/Header.h"
class RootNodeSolve
{
public:
    RootNodeSolve(){}

    void PathNodeFirstLoop(const shared_ptr<OffsetNode> &pathNode,
                           std::vector<DefLoop>&loops);
    void DeleteSmallPathLoop(const double l,shared_ptr<OffsetNode> &pathNode);
    void DelEmptyOffsetNode(std::shared_ptr<ofts::OffsetNode>&rootNode);
    void OffsetNodeLoopNum(const std::shared_ptr<ofts::OffsetNode>&rootNode,int &num);
    void SetOffsetNodeAreaId(std::shared_ptr<ofts::OffsetNode> &pathNode,int &index);
    void SetNodeToolPathType(Ofte::ToolPathType type,
                             std::shared_ptr<ofts::OffsetNode>&node);
    void AddNodeToNodes(std::shared_ptr<ofts::OffsetNode>& node,
                        Ofte::ToolPathType pathType, bool resetNode,
                        std::vector<std::shared_ptr<ofts::OffsetNode>>&vNode);
    void PathRootNodeLoop(const std::shared_ptr<ofts::OffsetNode> &rootNode,
                          std::vector<ofts::DefLoop>& vResLoop);
    void RootNodeAreaLoop(const std::shared_ptr<ofts::OffsetNode> &rootNode,
                          std::vector<std::vector<ofts::DefLoop>>&vAreaLoop);
    void SetOffsetNodeZ(const double& zValue,
                        std::shared_ptr<ofts::OffsetNode> &rootNode);
    void OffsetNodeCircleArcTrans(bool isTranToArc,
                                  std::shared_ptr<ofts::OffsetNode> &rootNode);
    void OffsetNodeArcPrecision(double arcEndpre,double R,
                                double H,bool isDecNum,int number,
                                std::shared_ptr<ofts::OffsetNode> &rootNode);
    void LoopArcPrecision(double arcEndpre, double R,
                          double H,bool isDecNum,
                          int number,ofts::DefLoop& loop);
    void SetLoopTreeDire(std::shared_ptr<ofts::LoopTree>& node);
    void SubNodesToZezo(std::shared_ptr<ofts::LoopTree>& node);
    std::vector<ofts::GeomArea> LoopTreesRegions(const std::shared_ptr<ofts::LoopTree>& node);
    std::vector<ofts::GeomArea> LoopTreeToRegions(const std::shared_ptr<ofts::LoopTree>& itree,bool no_zezo);


};
#endif // ROOTNODESOLVE_H
