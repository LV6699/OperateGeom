#ifndef LOOPBOOLINT_H
#define LOOPBOOLINT_H
#include"../../CommonFile/DataStructure.h"
#if 0
#pragma optimize("", off)
#endif
class LoopBoolInt
{
public:
    LoopBoolInt(){}
    void InputPreprocesse(std::vector<OffsetStruct::DefLoop>& inloops,
                          numst::ReLoopErr err = numst::ReLoopErr(
                PreErr5_8,PreErr5_8,PreErr5_8,PreErr5_5));
    void LoopBoolIntersection(std::vector<OffsetStruct::DefLoop>inloops,
                              const NumStruct::IntParam& param,
                              std::vector<OffsetStruct::DefLoop>& rloops,
                              bool isOnce = false);
};
#endif // LOOPBOOLINT_H
