Fqy_-9Px-nvvuPo6n_HMyoHfEU9_
Page 1#ifndef PATHSMOOTH_H
#define PATHSMOOTH_H
#include"../../../CommonFile/Header.h"
class PathSmooth
{
public:
    PathSmooth(){}
    void SetSmoothOffsetParam(OffsetParam& offsetPara);
    void ArrayLoopSmooth(OffsetParam offsetPara  bool isAddOriLoop
                         vector<DefLoop>& vLoop);
    void ExicuteLoopSmooth(OffsetParam offsetPara
                           const DefLoop &loop
                           vector<DefLoop> &vLoop);
};
#endif // PATHSMOOTH_H