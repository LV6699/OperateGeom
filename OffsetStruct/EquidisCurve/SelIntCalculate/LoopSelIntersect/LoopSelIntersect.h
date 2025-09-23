#ifndef LOOPSELINTERSECT_H
#define LOOPSELINTERSECT_H
#include "../../../CommonFile/Header.h"
class LoopSelIntersect
{
public:
    LoopSelIntersect(){}
    void LoopClipPreprocesse(DefLoop& loop);
    void ArrayLoopClipPreprocesse(vector<DefLoop>& vLoop);
    void CalculateLoopSelIntersect(const DefLoop &oriLoop,
                                   const DefLoop &loop,
                                   OffsetParam param,
                                   double offsetVal,
                                   bool isDiscernInc,
                                   bool &isExiSelInt,
                                   vector<DefLoop> &rloops);
    void SelfIntersectAftertreatment(const DefLoop &oriLoop,
                                     const vector<DefLoop>&loops,
                                     bool isDiscernInc,
                                     vector<DefLoop>&rloops);
};
#endif // LOOPSELINTERSECT_H