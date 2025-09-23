#ifndef ROOTNODESOLVE_H
#define ROOTNODESOLVE_H
#include"CommonFile/Header.h"
class RootNodeSolve
{
public:
    RootNodeSolve(){}
    void DeleteRootNodeEmptyLoop(shared_ptr<OffsetNode>&rootNode);
    void SetLoopIslEle(DefLoop& loop, int start, int end);
    void GetRootNodeLoopNum(const shared_ptr<OffsetNode>&rootNode,
                            int &num);
    void SetPathNodeAreaIndex(shared_ptr<OffsetNode> &pathNode,int &index);
    void SetNodeToolPathType(ToolPathType type,
                             shared_ptr<OffsetNode>&node);
    void AddNodeToNodes(shared_ptr<OffsetNode>& node,
                        ToolPathType pathType, bool resetNode,
                        vector<shared_ptr<OffsetNode>>&vNode);
    void PathRootNodeLoop(const shared_ptr<OffsetNode> &rootNode,
                          vector<DefLoop>& vResLoop);
    void GetRootNodeAreaLoop(const shared_ptr<OffsetNode> &rootNode,
                             vector<vector<DefLoop>>&vAreaLoop);
    void SetRootNodeZValue(const double& zValue,
                           shared_ptr<OffsetNode> &rootNode);
    void PathRootNodeSmooth(OffsetParam offsetPara,
                            bool isAddOriLoop,
                            shared_ptr<OffsetNode> &rootNode);
    void NodePathCircleArcTranslate(bool isTranToArc,
                                    shared_ptr<OffsetNode> &rootNode);
    void PathNodeArcPrecision(double arcEndpre,double R,
                              double H,bool isDecNum,int number,
                              shared_ptr<OffsetNode> &rootNode);
    void ProcessLoopArcPrecision(double arcEndpre, double R,
                                 double H,bool isDecNum,
                                 int number,DefLoop& loop);
    void PathNodeFirstLoop(const shared_ptr<OffsetNode> &pathNode,
                           std::vector<DefLoop>&vLoop);
    
    void DeleteSmallPathLoop(const double l,shared_ptr<OffsetNode> &pathNode);
    void ReverseOffsetNode(shared_ptr<OffsetNode> &pathNode);
};
#endif // ROOTNODESOLVE_H