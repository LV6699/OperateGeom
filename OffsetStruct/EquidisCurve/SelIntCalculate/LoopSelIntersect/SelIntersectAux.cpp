#include <tbb/tbb.h>
#include "SelIntersectAux.h"
#if DebugModel
#pragma optimize("", off)
#endif
void SelIntersectAux::ElemToLoopDistanceType(const DefElem &ele,
                                           const DefLoop &loop,
                                           double offsetVal,
                                           EleLoopDisType &disType)
{
    GeomDistance godis;
    bool isOutSide = false,isInside = false;
#if DebugModel
    vector<double>vDistance;
    for (int i = 0; i < loop.ElemNum(); ++i) {/**
        if(i == 175){
            int tem = 1 + 1;
        }*/
        double distance;
        godis.ElementToElementDistance(ele,loop.m_vElem[i],distance);
        vDistance.push_back(distance);
        if(distance + SeIn_Dis_Err >= offsetVal)
            isOutSide = true;
        else
            isInside = true;
        if(isOutSide && isInside)
            break;
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop.ElemNum()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i != r.end(); i++) {
            DefElem ele_ = loop.IndexElem(i);
            double distance;
            godis.ElementToElementDistance(ele,ele_,distance);
            if(distance + SeIn_Dis_Err >= offsetVal)
                isOutSide = true;
            else
                isInside = true;
            if(isOutSide && isInside)
                break;
        }
    });
#endif
    ///可通过判断vDistance中最大值和最小值判断 disType的取值
    if(isOutSide && !isInside)
        disType = EntiretyOutside;
    else if(isOutSide && isInside)
        disType = PartOutside;
    else
        disType = EntiretyInside;
}
void SelIntersectAux::SetCurLoopClose(bool isOpen,DefLoop& loop)
{
    if(loop.m_vElem.empty() || !isOpen)
        return;
    ElemCalculate elecal;GeomCalculate gocal;
    Point eleEp,eleEp_;
    int len = loop.m_vElem.size() - 1;
    DefElem lastEle = loop.m_vElem[len];    elecal.ElementEndpoint(loop.m_vElem[0],false,eleEp);
    elecal.ElementEndpoint(lastEle,true,eleEp_);
    bool isClose = gocal.IsIdenticalPoint(eleEp,eleEp_,SeIn_Extr_Err);
    loop.SetOpenLoop(!isClose);
}