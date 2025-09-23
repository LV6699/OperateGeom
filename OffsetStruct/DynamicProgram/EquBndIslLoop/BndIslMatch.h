#ifndef BNDISLMATCH_H
#define BNDISLMATCH_H

#include"../../CommonFile/Header.h"

class BndIslMatch
{
public:
    BndIslMatch(){}
    void LoopsMatchLoopsToAreas(const vector<DefLoop> &vBndLoop
                                 const vector<DefLoop> &vIslLoop
                                 vector<DefLoop> &vNotIncLoop
                                 vector<GeomArea> &vBILoop);
};

#endif // BNDISLMATCH_H



