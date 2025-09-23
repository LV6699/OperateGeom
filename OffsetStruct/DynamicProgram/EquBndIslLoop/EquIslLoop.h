#ifndef EQUISLLOOP_H
#define EQUISLLOOP_H
#include"../../CommonFile/Header.h"
class EquIslLoop
{
public:
    EquIslLoop(){}
    void EquidistantIslLoop(OffsetParam offsetPara
                            vector<LoopNode>&vIslNode
                            vector<GeomArea> &vMatchLoop
                            vector<shared_ptr<OffsetNode>>&vIslOftNode
                            vector<DefLoop>&vIslLocalLoop);
};
#endif // EQUISLLOOP_H
