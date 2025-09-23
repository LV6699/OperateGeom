#include <tbb/tbb.h>
#include "SelIntMachine.h"
#if DebugModel
#pragma optimize("", off)
#endif
void SelIntMachine::GetOftValueBaseSelInt(const DefLoop& loop,
                                          double R,double &offsetVal)
{
    ///return;
    if(loop.m_vElem.size() < 3)
        return;
    SelIntMachAux selaux;
    double oriOftVal = offsetVal;
#if 0
    for (int i = 0; i < loop.m_vElem.size(); ++i) {/**
        if(i == 6){
            int tem = 1;
        }*/
        bool isReset = false;
        selaux.IsResetOftValBaseSelInt(loop,loop.IndexElem(i),
                                       i,oriOftVal,isReset);
        if(isReset){
            double offsetVal_ = R * OftValMultiplier;
            offsetVal = offsetVal_ < offsetVal ? offsetVal_ : offsetVal;
            return;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,loop.ElemNum()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            bool isReset = false;
            selaux.IsResetOftValBaseSelInt(loop,loop.IndexElem(i),
                                           i,oriOftVal,isReset);
            if(isReset){
                double offsetVal_ = R * OftValMultiplier;
                offsetVal = offsetVal_ < offsetVal ? offsetVal_ : offsetVal;
                return;
            }
        }
    });
#endif
}