#ifndef SELINTMACHINE_H
#define SELINTMACHINE_H
#include"../../../CommonFile/Header.h"
class SelIntMachine
{
public:
    SelIntMachine(){}
    void GetOftValueBaseSelInt(const DefLoop& loop,
                               double R,double &offsetVal);
};
#endif // SELINTMACHINE_H
