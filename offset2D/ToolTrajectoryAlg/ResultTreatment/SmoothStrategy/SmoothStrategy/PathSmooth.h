#ifndef PATHSMOOTH_H
#define PATHSMOOTH_H
#include"../../../CommonFile/Header.h"
class PathSmooth
{
public:
    PathSmooth(){}
    void SetSmoothOffsetParam(OffsetParam& offsetPara);
    void ArrayLoopSmooth(OffsetParam offsetPara, bool isAddOriLoop,
                         vector<DefLoop>& vLoop);
    void ExicuteLoopSmooth(OffsetParam offsetPara,
                           const DefLoop &loop,
                           vector<DefLoop> &loops);
    void CalculateLoopSmooth(const DefLoop& loop, double R,
                             vector<DefLoop>& loops, bool isOne = false);
};
#endif // PATHSMOOTH_H
