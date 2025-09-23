#ifndef ToolTrajectorySolve_H
#define ToolTrajectorySolve_H
#include"../../CommonFile/Header.h"
class ToolTrajectorySolve
{
public:
    ToolTrajectorySolve(){}
    void SetNodeLoopSpecifyOrigin(OffsetParam offsetPara,
                                  shared_ptr<OffsetNode>&rootNode);
    void AddBndLoopToBndNode(OffsetParam offsetPara,
                             const DefLoop &curBndLoop,
                             const vector<DefLoop> &vIslLoop,
                             bool isExistClip,
                             shared_ptr<OffsetNode>& bndNode,
                             vector<shared_ptr<OffsetNode> > &vBndOftNode);
    void AddLoopNodeToRootNode(const vector<LoopNode>&vIslNode, ToolPathType 
type,
                               vector<shared_ptr<OffsetNode>>&vNode);
    void AddBndNoteToRootNode(OffsetParam offsetPar,
                              const shared_ptr<OffsetNode>& bndNode,
                              vector<shared_ptr<OffsetNode>>&vBndOftNode);
    void AddLocalLoopToRootNode(OffsetParam offsetPar,
                                const shared_ptr<OffsetNode>& bndNode,
                                const vector<LoopNode>&vIslNode,
                                const vector<DefLoop> &vBndLocalLoop,
                                const vector<DefLoop>&vBndIslMerLoop,
                                const vector<DefLoop>&vIslMerLoop,
                                const vector<DefLoop> &vIslLocalLoop,
                                shared_ptr<OffsetNode>&rootNode);
    void GetLoopOffsetNode(OffsetParam offsetPara,
                           const DefLoop& loop,
                           shared_ptr<OffsetNode> &oftNode);
    void OffsetMerBndIslLoop(OffsetParam offsetPara,
                             vector<shared_ptr<OffsetNode>>&vBndOftNode,
                             vector<shared_ptr<OffsetNode>>&vIslOftNode,
                             const vector<DefLoop> &vBndLocalLoop,
                             const vector<DefLoop> &vBndIslMerLoop,
                             const vector<DefLoop> &vIslMerLoop,
                             const vector<DefLoop> &vIslLocalLoop,
                             shared_ptr<OffsetNode> &rootNode);
};
#endif // ToolTrajectorySolve_H