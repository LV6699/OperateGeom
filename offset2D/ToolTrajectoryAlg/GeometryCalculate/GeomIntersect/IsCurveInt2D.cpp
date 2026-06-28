#include "IsCurveInt2D.h"
#ifdef emit
#undef emit
#include <tbb/tbb.h>
#define emit Q_EMIT
#else
#include <tbb/tbb.h>
#endif
bool IsCurveInt2D::IsCurveLoopInt(const ofts::DefElem& curve,
                                  const ofts::DefLoop& loop,
                                  const numst::IntParam& param)
{
#if 0
    for(size_t i = 0;i < loop.Size();++i){
        if(IsCurveIntersect(curve,loop[i],param)){
            return true;
        }
    }
    return false;
#else
    std::atomic<bool>IsInt(false);
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,loop.Size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        if (IsInt.load(std::memory_order_relaxed)) {return;}
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            if (IsInt.load(std::memory_order_relaxed)) {return;}
            if(IsCurveIntersect(curve,loop[i],param)){
                IsInt.store(true,std::memory_order_relaxed);
                return;
            }
        }
    });
    return IsInt.load();
#endif 
}
bool IsCurveInt2D::IsLoopIntersect(const ofts::DefLoop& loop1,
                                   const ofts::DefLoop& loop2,
                                   const numst::IntParam& param) {
    for(size_t i = 0; i < loop1.Size(); ++i) {
        if(IsCurveLoopInt(loop1[i], loop2, param)) {
            return true;
        }
    }
    return false;
}
