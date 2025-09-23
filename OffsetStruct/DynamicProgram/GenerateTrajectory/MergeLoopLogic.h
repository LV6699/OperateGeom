#ifndef MERBNDISLLOGIC_H
#define MERBNDISLLOGIC_H
#include"../../CommonFile/Header.h"
class MergeLoopLogic
{
public:
    MergeLoopLogic(){}
    void MergeArrayIslandLoop(vector<GeomArea> &vBILoop,
                              vector<DefLoop> &vIslMerLoop,
                              vector<LoopNode>&vIslNode,
                              vector<DefLoop>vInputLoop,
                              vector<DefLoop>& vIslLoop,
                              vector<DefLoop>& vOutLoop);
    void MergeBoundaryIslandLoop(DefLoop &bndLoop,
                                 vector<DefLoop>& vIslLoop,
                                 OffsetParam offsetPara,
                                 bool& isOriBndClip, bool &isExistClip,
                                 bool &isBndInside,
                                 vector<shared_ptr<OffsetNode>>&vBndOftNode,
                                 vector<shared_ptr<OffsetNode>>&vIslOftNode,
                                 shared_ptr<OffsetNode> &bndNode,
                                 vector<DefLoop> &vBndLocLoop,
                                 vector<DefLoop>& vBIMerLoop,
                                 vector<GeomArea> &vBILoop);
    void ExicuteMergeBndIslLoop(DefLoop &bndLoop_,
                                vector<DefLoop> &vIslLoop_,
                                bool &isBndInside,
                                bool &isExistClip,
                                DefLoop& bndLoop,
                                vector<DefLoop>&vIslLoop,
                                vector<DefLoop>& vBIMerLoop,
                                vector<GeomArea>& vBILoop);
};
#endif // MERBNDISLLOGIC_H