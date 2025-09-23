#include <tbb/tbb.h>
#include "CornerMachine.h"
#if DebugModel
#pragma optimize("", off)
#endif
void CornerMachine::OffsetValueBaseCorner(const DefLoop& loop,
                                          double R,
                                          double &offsetVal)
{
    ///return;
    if(loop.m_vElem.size() < 2)
        return;
    int minId = 0;
    double angle = 180;vector<double>vAngle;
    CornerMachAux coraux;ElemCalculate numcal;
#if 0
    for (int i = 0; i < loop.ElemNum(); ++i) {/**
        if(i == 26){
            int tem = 1 + 1;
        }*/
        double curAngle;
        int nex = numcal.NextIndex(loop.m_vElem.size(),i);
        coraux.GetAdjoinElementAngle(loop.m_vElem[i],
                                     loop.m_vElem[nex],curAngle);
        vAngle.push_back(curAngle);
        if(curAngle < angle){
            angle = curAngle;
            minId = i;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop.ElemNum()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            double curAngle;
            int nex = numcal.NextIndex(loop.m_vElem.size(),i);
            coraux.GetAdjoinElementAngle(loop.m_vElem[i],
                                         loop.m_vElem[nex],curAngle);
            if(curAngle < angle){
                angle = curAngle;
                minId = i;
            }
        }
    });
#endif
    double oftVal = R * (1 + std::sin(angle * M_PI / 360)
                         * OftValCornorMultiplier);
    offsetVal = oftVal < offsetVal ? oftVal : offsetVal;
}