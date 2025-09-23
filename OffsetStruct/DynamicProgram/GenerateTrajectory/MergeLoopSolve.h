#ifndef MergeLoopSolve_H
#define MergeLoopSolve_H
#include"../../CommonFile/Header.h"
class MergeLoopSolve
{
public:
    MergeLoopSolve(){}
    bool IsInsideOtherArea(const vector<GeomArea>& vBILoop,
                           const DefLoop& loop, int id);
    void HandleIslandMergeInside(vector<DefLoop>& vLoop,
                                 vector<DefLoop>& vIslLoop,
                                 vector<LoopNode>& vIslNode,
                                 vector<GeomArea>& vBILoop);
    void LoopsUnionBndIslMatchLoop(vector<DefLoop>& vMerLocLoop,
                                   vector<DefLoop>& vIslLoop,
                                   vector<LoopNode> &vIslNode,
                                   vector<GeomArea> &vBILoop);
    void LoopsInsideOutsideLoop(const vector<DefLoop>& vLoop,
                                    vector<DefLoop>& vInsLoop,
                                    vector<DefLoop>& vOutLoop);
    void InitialOffsetLoopNode(const vector<DefLoop> &vIslLoop,
                               OffsetParam offsetPara,bool isAddLoop,
                               vector<LoopNode>&vNode,
                               const GeomArea& biLoop = GeomArea());
    void IsLoopNodeInArrayLoop(vector<shared_ptr<OffsetNode>>&vIslOftNode,
                               const vector<DefLoop>& vLoop,
                               vector<LoopNode>& vIslNode);
    void GetLoopInArrayLoop(const vector<DefLoop>& vectLoop,
                            vector<DefLoop>& vLoop);
    void HandleInClockwiseMerLoop(vector<DefLoop>&vLoop);
};
#endif // MergeLoopSolve_H