#include "CalculateValidity.h"
#include "GeometryCalculate/RelativeContain/LoopContain.h"
#include "GeometryCalculate/GeomCalculate/CurveDist2D.h"
#include "../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
void CalculateValidity::IsCurrentValidOffset(ofts::OffsetParam param,
                                             std::vector<ofts::DefLoop>& oftloops,
                                             const ofts::DefLoop& oloop,
                                             const ofts::DefLoop& loop,
                                             double offsetVal,
                                             bool& isValCal)
{
    isValCal = false;
    if(param._offsetUnit.IsPureEqud() || loop.IsCircleLoop()){
        isValCal = true;return;
    }
#if 0
    DisplayGeom().DisplayLoop(oloop,_colors.back(),1,true,true);
    DisplayGeom().DisplayLoop(loop,_colors.back(),1.5,true);
#endif
    double dist = Max_Value;
#if 0
    for (size_t i = 0; i < loop.Size(); ++i) {
        if(loop[i].IsAbsValid()){continue;}
        double d = CurveDist2D().CurveToLoopDist(oloop,loop1[i]);
        if(d < dist){
            dist = d;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop.Size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for(std::size_t i = r.begin(); i < r.end(); ++i) {
            if(loop[i].IsAbsValid()){continue;}
            double d = CurveDist2D().CurveToLoopDist(oloop,loop[i]);
            if(d < dist) {dist = d;}
        }});
#endif
    if(std::abs(dist - offsetVal) > PreErr5_3){
        if(!oftloops.empty()){oftloops.pop_back();}
        return;
    }
    if(!IsMeetMinBoxMeasure(loop)){
        if(!oftloops.empty()){oftloops.pop_back();}
        return;
    }
    if(loop.IsOpen()){isValCal = true;return;}
    if(!IsVallidLocationRelation(oloop,loop)){
        if(!oftloops.empty()){oftloops.pop_back();}
        return;
    }
    if(!LoopDirection().IsClockwiseLoop(loop)){
        if(!oftloops.empty()){oftloops.pop_back();}
        return;
    }
    isValCal = true;
}
bool CalculateValidity::IsVallidLocationRelation(const ofts::DefLoop& oloop,
                                                 const ofts::DefLoop& loop)
{
    if(loop.IsCircleLoop()){return true;}
    auto olimc = LimitCoord2D::LoopLimitCoord(oloop);
    auto limc = LimitCoord2D::LoopLimitCoord(loop);
    bool include = LimitCoord2D::IsLimitCoordCont(olimc,limc,PreErr5_8);
    if(!oloop.Leftward()){if(!include){return false;}}
    else{if(include){return false;}}
    return true;
}
bool CalculateValidity::LoopMinBoundingBox(const ofts::DefLoop& loop)
{
    double w,h,l;
    auto c = LimitCoord2D::LoopLimitCoord(loop);
    w = std::abs((c.MaxX() + c.MinX()) / 2);
    h = std::abs((c.MaxY() + c.MinY()) / 2);
    l = w > h ? w : h;
    return l >= PreErr5_2;
}
bool CalculateValidity::IsMeetMinBoxMeasure(const ofts::DefLoop& loop)
{
    double girth = 0;
    for (int i = 0; i < loop.Size(); ++i) {
        girth += CurveLog2D::CurveLenth(loop[i]);
        if(girth >= PreErr_1){return true;}
    }
    return false;
}
