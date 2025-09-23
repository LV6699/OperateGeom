#include <tbb/tbb.h>
#include "GeomContain.h"
#if DebugModel
#pragma optimize("", off)
#endif
void GeomContain::IsLoopOutsideLoopCoord(const DefLoop& outLoop,
                                         const DefLoop& loop,
                                         double pre,
                                         bool& isCoorOut)
{
    LimitCoordinate limcoo;
    double minX,maxX,minY,maxY,incMinX,incMaxX,incMinY,incMaxY;
    limcoo.LoopLimitCoordinate(loop,minX,maxX,minY,maxY);
    limcoo.LoopLimitCoordinate(outLoop,incMinX,
                               incMaxX,incMinY,incMaxY);
    isCoorOut = ((minX - pre) > incMaxX || (maxX + pre) < incMinX ||
                 (minY - pre) > incMaxY || (maxY + pre) < incMinY);
}
void GeomContain::IsLoopContainLoopCoord(const DefLoop& outLoop,
                                         const DefLoop& loop,
                                         double pre,
                                         bool& isCoorInc)
{
    isCoorInc = false;
    LimitCoordinate limcoo;
    double incMinX,minX,incMaxX,maxX,incMinY,minY,incMaxY,maxY;
    limcoo.LoopLimitCoordinate(loop,minX,maxX,minY,maxY);
    limcoo.LoopLimitCoordinate(outLoop,incMinX,
                               incMaxX,incMinY,incMaxY);
    isCoorInc = ((incMinX - pre) < minX && (incMaxX + pre) > maxX &&
                 (incMinY - pre) < minY && (incMaxY + pre) > maxY);
}
void GeomContain::IsLoopsContainLoopCoord(const vector<DefLoop>& loops,
                                          const DefLoop& loop,
                                          double pre,
                                          bool& isInclude)
{
    isInclude = false;
    for(auto& l : loops){
        IsLoopContainLoopCoord(l,loop,pre,isInclude);
        if(isInclude)
            return;
    }
}
void GeomContain::IsLoopContainIntersectLoop(const DefLoop& outLoop,
                                             const DefLoop&loop,
                                             double incErrVal,
                                             double coinErrVal,
                                             double intErrVal,
                                             bool isMatchBord,
                                             bool& resValue)
{
    resValue = false;
    bool isCoorOut = false;
    IsLoopOutsideLoopCoord(outLoop,loop,incErrVal,isCoorOut);
    if(isCoorOut)        return;
    GeoContainSolve consol;
#if 1
    for (int i = 0; i < loop.Size(); ++i) {
        bool resValue_;
        consol.IsLoopContainIntersectElem(outLoop,loop[i],
                                          incErrVal,coinErrVal,
                                          intErrVal,isMatchBord,
                                          resValue_);
        if(resValue_){
            resValue = true;
            return;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop.ElemNum()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            bool resValue_;
            consol.IsLoopContainIntersectElem(outerLoop,loop.IndexElem(i),
                                              incErrVal,coinErrVal,
                                              intErrVal,isMatchBord,
                                              resValue_);
            if(resValue_){
                resValue = true;
                return;
            }
        }
    });
#endif
}
void GeomContain::IsLoopContainLoop(const DefLoop& outLoop,
                                    const DefLoop& loop,
                                    double intPre,
                                    double coinPre,
                                    double incPre,
                                    bool& isContain)
{
    if(outLoop.IsEmpty() || loop.IsEmpty())
        return;
    isContain = false;
    bool isCoorInc = false;
    IsLoopContainLoopCoord(outLoop,loop,incPre,isCoorInc);
    if(!isCoorInc)
        return;
    CheckGeomInt chint;
    bool isSpecInt = false;
    chint.IsLoopSpecifyIntersect(outLoop,loop,coinPre,
                                 coinPre,intPre,isSpecInt);
    if(isSpecInt)
        return;
    GeoContainSolve consol;
    bool isCoincide = true;
#if 1
    for (int i = 0; i < loop.Size(); ++i) {
        bool isCoincide_ = false;
        consol.LoopContainElementSolve(outLoop,loop[i],incPre,
                                       isCoincide_,isContain);
        if(!isCoincide_){
            isCoincide = false;
            break;        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop.ElemNum()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            bool isCoincide_ = false;
            consol.LoopContainElementSolve(outerLoop,loop.IndexElem(i),
                                           incPre,isCoincide_,isContain);
            if(!isCoincide_){
                isCoincide = false;
                break;
            }
        }
    });
#endif
    if(isCoincide && !isContain){
        isContain = true;
    }
}
void GeomContain::IsLoopsContainIntersectLoop(const vector<DefLoop>& loops,
                                              const DefLoop& loop,
                                              double incErrVal,
                                              double coinErrVal,
                                              double intErrVal,
                                              bool isMatchBord,
                                              bool& resValue)
{
    resValue = false;
#if 0
    for (int lo = 0; lo < vLoop.size(); ++lo) {
        IsLoopContainIntersectLoop(vLoop[lo],loop,incErrVal,
                                   coinErrVal,intErrVal,
                                   isMatchBord,resValue);
        if(resValue)
            return;
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,loops.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            IsLoopContainIntersectLoop(loops[i],loop,incErrVal,
                                       coinErrVal,intErrVal,
                                       isMatchBord,resValue);
            if(resValue)
                return;
        }
    });
#endif
}
void GeomContain::IsLoopsContainLoop(const vector<DefLoop>& loops,
                                     const DefLoop& loop,
                                     double intPre,
                                     double pre,
                                     bool& isContain)
{
    isContain = false;/**
    判断一个环是在其他环内 ,如果一个环在其他任一环内 ,则该环被包含在其他环 ,
    判断一个环是否包含在另一环内 ,当环的所有点均在另一环内 (包含边界),则
    该环包含在另一环.*/#if 1
    for (int i = 0; i < loops.size(); ++i) {
        ///IsLoopContainLoop(vLoop[lo],loop,intPre,pre,isContain);
        IsLoopContainLoop(loops[i],loop,intPre,intPre,pre,isContain);
        if(isContain)
            return;
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,vLoop.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t lo = r.begin(); lo < r.end(); ++lo) {
            IsLoopContainLoop(vLoop[lo],loop,intPre,intPre,pre,isContain);
            if(isContain)
                return;
        }
    });
#endif
}