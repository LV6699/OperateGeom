#ifndef BNDISLLOGICAUX_H
#define BNDISLLOGICAUX_H
#include"../../CommonFile/Header.h"
class MergeLoopLogSolve
{
public:
    MergeLoopLogSolve(){}
    void ClassfyClipResult(vector<DefLoop>vOriLoop,
                           vector<DefLoop>vResLoop,
                           vector<DefLoop>& vNotClipLoop,
                           vector<DefLoop>& vClipLoop);
    bool IsBoundaryInIslandLoop(const DefLoop &bloop,
                                const vector<DefLoop> &iloops);
    void AddOriIslLoopToIslNode(OffsetParam offsetPara,
                                int hasOffsetNum,
                                const vector<DefLoop> &vOriIslLoop,
                                const vector<DefLoop> &vIslLoop,
                                vector<shared_ptr<OffsetNode>>&vIslOftNode);
    void MergeOriginBoundaryLoop(DefLoop& bndLoop,
                                 vector<DefLoop>& vIslLoop,
                                 OffsetParam offsetPara,
                                 bool &isBndInside,
                                 bool &isExistClip,
                                 vector<shared_ptr<OffsetNode>>&vIslOftNode,
                                 vector<DefLoop>& vBIMerLoop,
                                 vector<GeomArea>&vBndIslLoop);
};
#endif // BNDISLLOGICAUX_H