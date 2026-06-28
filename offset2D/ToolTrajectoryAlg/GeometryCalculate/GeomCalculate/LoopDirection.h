#ifndef LOOPROTDIRE_H
#define LOOPROTDIRE_H
#include"../../CommonFile/Header.h"
class LoopDirection
{
public:
    LoopDirection(){}
    bool IsClockwiseLoop(const ofts::DefLoop& inloop);
};
#endif // LOOPROTDIRE_H
