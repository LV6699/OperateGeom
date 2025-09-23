#ifndef LOOPDIRSYMBOL_H
#define LOOPDIRSYMBOL_H
#include "PathDirection.h"
class LoopDirSymbol
{
public:
    LoopDirSymbol(){}
    void SetNodeDirectionSymbol(const double toolRadius,
                                const shared_ptr<OffsetNode> &rootNode,
                                shared_ptr<OffsetNode> &symNode);
    void NodePathDirectionSymbol(const double toolRadius,
                                 const shared_ptr<OffsetNode> &rootNode,
                                 vector<DefLoop> &vLoop);
    void ElementDirectionSymbol(const DefElem ele,
                                const double toolRadius,
                                bool &isExiSym,
                                DefSeg& segment1,
                                DefSeg& segment2);
};
#endif // LOOPDIRSYMBOL_H