#ifndef LOOPCLIPAUX_H
#define LOOPCLIPAUX_H
#include"../../CommonFile/DataStructure.h"
class LoopClipAux
{
public:
    LoopClipAux(){}
    bool IsPointInLoops(std::vector<ofts::DefLoop>& loops,
                        const ofts::Point& P, bool isBoundary, double bErr);
};
#endif // LOOPCLIPAUX_H
