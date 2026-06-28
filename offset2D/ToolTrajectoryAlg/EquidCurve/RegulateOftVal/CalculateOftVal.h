#ifndef CALCULATEOFTVAL_H
#define CALCULATEOFTVAL_H
#include"../../CommonFile/Header.h"
class CalculateOftVal
{
public:
    CalculateOftVal(){}
    void AdjustOffsetValue(const DefLoop& loop,int hasOftNum,
                           bool hasChaOftVal,OffsetParam param,double& offsetVal);
};
#endif // CALCULATEOFTVAL_H
