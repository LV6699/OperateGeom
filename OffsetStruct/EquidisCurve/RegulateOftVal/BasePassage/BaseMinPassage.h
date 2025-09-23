FgVGsmekHj5AlfIcb7izfeZRxPav
Page 1#ifndef BASEMINPASSAGE_H
#define BASEMINPASSAGE_H
#include"../../../CommonFile/Header.h"
class BaseMinPassage
{
public:
    BaseMinPassage(){}
    void ExploreProbableUnprocess(const DefLoop &loop
                                  const double R
                                  const double oftValue
                                  const int index
                                  OffsetStruct::ProgOftValue::IndexDist &idDistance);
};
#endif // BASEMINPASSAGE_H