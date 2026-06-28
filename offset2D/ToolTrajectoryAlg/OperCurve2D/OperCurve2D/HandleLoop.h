#ifndef HANDLELOOP_H
#define HANDLELOOP_H
#include "../../CommonFile/DataStructure.h"
class HandleLoop
{
public:
    HandleLoop(){}

    void ClipLoopSelIntCurve(const numst::IntParam& param,
                             ofts::DefLoop& loop);
};
#endif // HANDLELOOP_H
