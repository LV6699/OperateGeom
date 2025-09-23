#include "CloseModelSmooth.h"
void CloseModelSmooth::ObjectSmoothLoop(const DefLoop& oriLoop,
                                        OffsetParam offsetPara,
                                        DefLoop& loop)
{
    vector<DefLoop> vLoop;
    PathSmooth smooth;
    smooth.ExicuteLoopSmooth(offsetPara,oriLoop,vLoop);
    if(vLoop.empty())
        return;
    else if(vLoop.size() == 1)
        loop = vLoop[0];
    else
        ElemCalculate().ArrayLoopMaxLenthLoop(vLoop,true,loop);
    loop.SetCloseMode(true);
}
///需要保证biLoop原始环已经初始化.
void CloseModelSmooth::CloseModelPathSmooth(OffsetParam offsetPara,
                                            GeomArea& biLoop)
{
    PathSmooth smooth;
    smooth.SetSmoothOffsetParam(offsetPara);
    offsetPara._offsetUnit.SetIsPureEqud(true);
    DefLoop bLoop;
    ObjectSmoothLoop(biLoop._bndLoop,offsetPara,bLoop);
    biLoop.SetBndLoop(bLoop);
    vector<DefLoop>vIslLoop;
    for (int i = 0; i < biLoop.IslLoop().size(); ++i) {
        DefLoop loop;
        ObjectSmoothLoop(biLoop.IslLoop()[i],offsetPara,loop);
        if(loop.IsEmpty())
            continue;
        vIslLoop.push_back(loop);
    }
    biLoop.SetIslLoop(vIslLoop);
}