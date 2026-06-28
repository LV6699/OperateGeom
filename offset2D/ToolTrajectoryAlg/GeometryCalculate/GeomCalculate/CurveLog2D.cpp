#include "CurveDist2D.h"
#ifdef emit
#undef emit
#include <tbb/tbb.h>
#define emit Q_EMIT
#else
#include <tbb/tbb.h>
#endif
double CurveDist2D::PointToLoopDist(const ofts::DefLoop& loop,
                                    const ofts::Point& p)
{
    if(loop.IsEmpty()) {
        return 0;
    }
    double dist = 10e8;
#if 0
    ElementToElementDistance(ele,loop.m_vElem[0],dist);
    for (int i = 1; i < loop.m_vElem.size(); ++i) {
        double cdist;
        ElementToElementDistance(ele,loop.m_vElem[i],cdist);
        if(curDistance < distance){
            dist = cdist;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop.Size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for(std::size_t i = r.begin(); i < r.end(); ++i) {
            double d_ = PointToCurveDist(p, loop[i]);
            if(d_ < dist) {
                dist = d_;
            }
        }
    });
#endif
    return dist;
}
double CurveDist2D::CurveToLoopDist(const ofts::DefLoop& loop,
                                    const ofts::DefElem& curve)
{
    if(loop.IsEmpty()){return 0;}
    double dist = 10e8;
#if 0
    for (size_t i = 0; i < loop.Size(); ++i) {
#if 0
        Point p1(127262.34998542604,115290.60016296266),
                p2(127264.34998542604,115288.6001629525);
        if(curve.GetSp().IsSamePoint2D(p1,PreErr5_6) &&
                loop[i].GetSp().IsSamePoint2D(p2,PreErr5_6)){
            DisplayGeom().DisplayElement(curve,_colors[1],1,true);
            DisplayGeom().DisplayElement(loop[i],_colors.back(),1);
            DisplayGeom().DisplayPoint(curve.GetSp(),_colors[4]);
            DisplayGeom().DisplayPoint(loop[i].GetSp(),_colors[4]);
            std::cout<<1<<std::endl;
        }
#endif
        double d = CurveDistance(curve,loop[i]);
        if(d < dist){
            dist = d;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,loop.Size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            double d = CurveDistance(curve,loop[i]);
            if(d < dist){
                dist = d;
            }
        }
    });
#endif
    return dist;
}
double CurveDist2D::LoopDistance(const ofts::DefLoop& loop1, 
                                 const ofts::DefLoop& loop2) {
    if(loop1.IsEmpty() || loop2.IsEmpty()) {
        return 0;
    }
    double dist = 1e10;
#if 0
    dist = CurveDistance(loop1[0],loop2[0]);
    for (size_t i = 1; i < loop1.Size(); ++i) {
        double d = CurveToLoopDist(loop2,loop1[i]);
        if(d < dist){
            dist = d;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop1.Size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for(std::size_t i = r.begin(); i < r.end(); ++i) {
            double d = CurveToLoopDist(loop2, loop1[i]);
            if(d < dist) {
                dist = d;
            }}});
#endif
    return dist;
}
