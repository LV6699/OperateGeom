#ifndef PARALLELLOGIC_H
#define PARALLELLOGIC_H
#include<vector>
///#include <tbb/tbb.h>
#include "../../third_part/cls_util.h"
#include 
"../../../algw/cutter_location_surface/include/cutter_location_surface/
typedef.h"
#include "../../../offset2D/ToolTrajectoryAlg/CommonFile/DataStructure.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
class ParallelLogic
{
    public:
    ParallelLogic(){}
    void SetPointsZ(const std::shared_ptr<cls::model> &clsMod,
                    std::vector<oft::Point> &points);
    void IterateSegmentZ(const std::shared_ptr<cls::model>&clsMod,
                         const oft::Point& lsp,
                         const oft::Point& lep,
                         double err,double limit,vector<Point>& points)
    {
        double z = 0,e = 0;
        bool hasAdjust = false;
        Point sp = lsp,ep = lsp.Midpoint(lep),mp = sp.Midpoint(ep);
        while (true) {
            rmath::vec2 p(mp.X(),mp.Y());
            z = cls::get_cutter_location(p,*clsMod);
            e = std::abs(mp.Z() - z);
#if 0
            double dist = sp.Distance2D(ep);
            double dist1 = sp.Distance3D(ep);
            if(!ElemCalculate().IsValidPoint(mp)){
                std::cout<<dist<<dist1<<endl;
            }
#endif
            mp.SetZ(z);
            if(e <= err){
                if(hasAdjust){
                    break;
                }
                points.emplace_back(mp);
                sp = mp;ep = lep;
                mp = sp.Midpoint(ep);
                hasAdjust = true;
            }else{
                if(sp.IsSamePoint2D(ep,limit)){
                    points.emplace_back(mp);
                    sp = mp;ep = lep;
                    mp = sp.Midpoint(ep);
                    hasAdjust = true;
                }else{
                    ep = mp;
                    mp = sp.Midpoint(mp);
                    hasAdjust = false;
                }
            }
            /// 当且仅当调整sp,mp,ep时,才添加加密点,否则会打乱points中点的顺序.            if(sp.IsSamePoint2D(lep,limit)){/// 防止无限循环.
            break;
        }
    }
}
void PointsPrecision(const std::shared_ptr<cls::model>&clsMod,
                     double err,double limit,vector<Point>& points)
{
    if(points.size() < 2)
        return;
    double z = 0;
    vector<Point>pts;
    for (size_t i = 0; i < points.size()-1; ++i) {
        const Point& p0 = points[i];
        const Point& p1 = points[i+1];
        Point mp = p0.Midpoint(p1);
#if 0
        if (p0.IsSamePoint3D(Point(21.400000000000620, 45.787269592285199,
                                   20.832326889038086),PreErr5_6) &&
                p1.IsSamePoint3D(Point(21.500000000000622,
                                       45.787269592285199,
                                       11),PreErr5_6)) {
            double l = p0.Distance2D(p1);
            double l1 = p0.Distance3D(p1);
            std::cout << l << l1 << endl;
        }
#endif
        rmath::vec2 p(mp.X(),mp.Y());
        z = cls::get_cutter_location(p,*clsMod);
        if(std::abs(mp.Z() - z) <= err){
            pts.emplace_back(p0);
            continue;
        }
        vector<Point>ps{p0};
        IterateSegmentZ(clsMod,p0,p1,err,limit,ps);/**
            if (ps.size() > 13) {
                std::cout << ps.size() << endl;
            }*/
        for(auto& d : ps){/**
                if(!ElemCalculate().IsValidPoint(d)){
                     std::cout << ps.size() << endl;
                }*/
            pts.emplace_back(d);
        }
    }
    pts.emplace_back(points.back());
    points = pts;
}
void LoopSegmentPrecision(const std::shared_ptr<cls::model>&clsMod,
                          double err,double limit,DefLoop& loop)
{
    double z = 0;
    bool isHandle = false;
    DefLoop loop_;
    GeometryLogic golg;
    for (int i = 0; i < loop.ElemNum(); ++i) {
        const Point& lsp = loop.m_vElem[i]._segment._segSp;
        const Point& lep = loop.m_vElem[i]._segment._segEp;
        Point mp = lsp.Midpoint(lep);
        rmath::vec2 p(mp.X(),mp.Y());
        z = cls::get_cutter_location(p,*clsMod);            if(std::abs(mp.Z() - z) <= err){
            loop_.m_vElem.emplace_back(loop.m_vElem[i]);
            continue;
        }
        DefLoop lo;
        vector<Point>ps{lsp};
        IterateSegmentZ(clsMod,lsp,lep,err,limit,ps);
        ps.emplace_back(lep);
        golg.PointToSegmentLoop(ps,false,lo);
        for(auto& e : lo.m_vElem){
            loop_.m_vElem.emplace_back(e);
        }
        isHandle = true;
    }
    if(isHandle){
        loop.SetElem(loop_.Elems());
    }
}
void PointsPrecisionBaseCLS(const std::shared_ptr<cls::model>&clsMod,
                            std::vector<Point> &points);
void SetLoopZCoord(const shared_ptr<cls::model>&clsMod,
                   DefLoop& loop);
void SetLoopsZCoord(const std::shared_ptr<cls::model>&clsMod,
                    std::vector<DefLoop>& loops);
};
#endif // PARALLELLOGIC_H
