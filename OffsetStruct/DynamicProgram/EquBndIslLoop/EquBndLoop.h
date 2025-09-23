#ifndef EQUBNDLOOP_H
#define EQUBNDLOOP_H
#include"../../CommonFile/Header.h"
class EquBndLoop
{
public:
    EquBndLoop(){}
    void EquidistantBndLoop(DefLoop bndLoop,
                            const vector<DefLoop>& vIslLoop,
                            OffsetParam offsetPar,
                            vector<shared_ptr<OffsetNode>>&vBndOftNode,
                            shared_ptr<OffsetNode> &bndNode,
                            vector<DefLoop>& vNoIntLoop,
                            vector<GeomArea>& vMatchLoop);
    void MerMultipleMatchLoop(int hasOffsetNum,
                              DefLoop& bndLoop,
                              vector<DefLoop>&vIslLoop,
                              vector<GeomArea>&vInputMatch,
                              vector<DefLoop>& vNoIncLoop,
                              vector<GeomArea>&vMatchLoop);
    void MergeArrayMatchLoop(DefLoop& bndLoop,
                             vector<DefLoop>&vIslLoop,
                             bool &isExistClip,
                             vector<GeomArea>&vInputMatch,
                             vector<DefLoop>& vNoIncLoop,
                             vector<GeomArea>&vMatchLoop);
    void IsLoopInMathLoop(const vector<GeomArea> &vMatchLoop,
                          const vector<DefLoop> &vIslLoop,
                          const vector<DefLoop>&vNoIncLoop_,
                          vector<DefLoop>& vNoIncLoop);
};
#endif // EQUBNDLOOP_H