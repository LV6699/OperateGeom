#ifndef CORNERMACHINE_H
#define CORNERMACHINE_H
#include"../../../CommonFile/Header.h"
class CornerMachine
{
public:
    CornerMachine(){}
    void OffsetValueBaseCorner(const DefLoop& loop,
                               double R,
                               double &offsetVal);
};
#endif // CORNERMACHINE_H
