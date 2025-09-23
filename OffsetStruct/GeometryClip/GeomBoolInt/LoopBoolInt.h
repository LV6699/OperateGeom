FjdNkzuN-MnB7bD7GovWsYt3vLO3
Page 1#ifndef LOOPBOOLINT_H
#define LOOPBOOLINT_H
#include"../../CommonFile/DataStructure.h"
#include"../../CommonFile/NumStruct.h"
class LoopBoolInt
{
public:
    LoopBoolInt(){}
    void InputPreprocesse(std::vector<OffsetStruct::DefLoop>& inloops);
    void LoopGeomBoolIntersection(std::vector<OffsetStruct::DefLoop>inloops
                                  std::vector<OffsetStruct::DefLoop>& rloops);
};
#endif // LOOPBOOLINT_H