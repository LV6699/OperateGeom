Fo2GCVPT1zuel18pvuchbv4XoLOb
Page 1#ifndef CALCULATEOFTVAL_H
#define CALCULATEOFTVAL_H
#include"../../CommonFile/Header.h"
class CalculateOftVal
{
public:
    CalculateOftVal(){}
    void AdjustCurrentOffsetValue(const DefLoop& loop
                                  int hasOftNum
                                  bool hasChaOftVal
                                  OffsetParam offsetPara
                                  double& offsetVal);
};
#endif // CALCULATEOFTVAL_H