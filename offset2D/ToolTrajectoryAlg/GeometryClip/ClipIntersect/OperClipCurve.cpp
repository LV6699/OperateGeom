#include <iostream>
#include "OperClipCurve.h"
#include "CurveIntClip.h"
#include "../../OperCurve2D/OperCurve2D/OperateLoop.h"
#include "../../GeometryCalculate/RelativeContain/PointInLoop.h"
#include "../../GeometryCalculate/GeomCalculate/BaseGeom2D.h"
#ifdef emit
#undef emit
#include <tbb/tbb.h>
#define emit Q_EMIT
#else
#include <tbb/tbb.h>
#endif
using std::vector;
using namespace NumStruct;
using namespace OffsetStruct ;
#if 0
#pragma optimize("", off)
#endif
//P 仅满足在loops 一个环中即返回
bool OperClipCurve::IsPointInLoops(const ofts::Point& P,
                                   std::vector<ofts::DefLoop>& loops,
                                   bool isTemp,double bErr)
{
#if 0
    string s = OperateOftString().ArrayLoopString(vLoop);
    std::cout<<s<<endl;
#endif
    for(auto& l : loops){LimitCoord2D::SetLoopLimCoord(l);}
    for (size_t i = 0; i < loops.size(); ++i) {
        bool isOnBorder = false,isInPoly = false;  //PreErr5_10
        PointInLoop().GetPointInsideLoop(loops[i],P,bErr,
                                         isOnBorder,isInPoly);
        if(isOnBorder){
            if(isTemp){
                return false;
            }else{
                return true;
            }
        }
        if(isInPoly){
            return true;
        }
    }
    return false;
}
bool OperClipCurve::IsValidPointBaseInLoops(const ofts::Point& P,
                                            vector<ofts::DefLoop>& loops,
                                            double bordPre,bool isIn){
    for(auto& l : loops){LimitCoord2D::SetLoopLimCoord(l);}
    for(const auto& l : loops){
        bool isOnBorder = false,isInPoly = false;  //PreErr5_10
        PointInLoop().GetPointInsideLoop(l,P,bordPre,isOnBorder,isInPoly);
        if(isIn){
            if(!isInPoly && !isOnBorder){
                return false;
            }
        }else{
            if(isInPoly){
                return false;
            }
        }
    }
    return true;
}
bool OperClipCurve::IsClipCurveValid(ofts::DefElem& c,
                                     std::vector<ofts::DefLoop>& loops,
                                     bool isTemp,double bordPre,
                                     bool isIn,bool isOnce){
    auto mp = BaseGeom2D::CurveMidpoint(c);
    if(isOnce){
        auto isIn_ = IsPointInLoops(mp,loops,isTemp,bordPre);
        return isIn_ == isIn;
    }
    return IsValidPointBaseInLoops(mp,loops,bordPre,isIn);
}
//参考loops 删除loop 中无效的元素.
void OperClipCurve::LoopInvalidClipCurve(ofts::DefLoop& loop,
                                         std::vector<ofts::DefLoop>& loops,
                                         bool isTemp,double bordPre,
                                         bool isIn,bool isOnce)
{
    std::vector<size_t>ids;
#if 0
    for (size_t i = 0; i < loop.Size(); ++i) {
        if(!IsClipCurveValid(loop[i],loops,isTemp,
                             bordPre,isIn,isOnce)){
            ids.push_back(static_cast<int>(i));
        }
    }
#else
    // 使用ETS 存储每个线程的局部结果
    tbb::enumerable_thread_specific<std::vector<std::size_t>> local_res;
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,loop.Size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        auto& local_vec = local_res.local();
        local_vec.reserve(r.end() - r.begin());
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            if(!IsClipCurveValid(loop[i],loops,isTemp,
                                 bordPre,isIn,isOnce)){
                local_vec.push_back(i);
            }
        }
        //自动选择分块策略,兼顾负载均衡与开销
        tbb::auto_partitioner{};
    });
    //合并所有线程结果
    ids.reserve(loop.Size());
    for(const auto& d : local_res){
        ids.insert(ids.end(),d.begin(),d.end());
    }
    std::sort(ids.begin(),ids.end(),[](const size_t& a,
              const size_t& b){return a < b;});
#endif
    pnum::DeleteIndexElems(ids,loop.m_vElem);
}
void OperClipCurve::ResetCurvesOrder(ofts::DefLoop& loop,double e)
{
    //重置loop 顺序,使loop 中起点不与其他元素相连的元素放至前面
    std::vector<size_t>ids,ids1;
    for(size_t i = 0;i < loop.Size();++i){
        //对于已经与其他相连的元素直接跳过
        if(pnum::IsIn2DArray(i,ids1)){continue;}
        bool isFind = false;
        const auto& sp = loop[i].GetSp();
        for(size_t j = 0;j < loop.Size();++j){
            if(j == i){continue;}
            if(sp.IsSamePoint3D(loop[j].GetSp(),e)){
                isFind = true;ids1.push_back(j);break;
            }else if(sp.IsSamePoint3D(loop[j].GetEp(),e)){
                isFind = true;break;
            }
        }
        if(isFind){continue;}
        ids.push_back(i);
    }
    if(ids.empty()){return;}
    ofts::DefLoop loop1;
    loop1.Reserve(loop.Size());
    for(size_t i = 0;i < ids.size();++i){
        loop1.AddElem(loop[ids[i]]);
    }
    for(size_t i = 0;i < loop.Size();++i){
        if(pnum::IsIn2DArray(i,ids)){continue;}
        loop1.AddElem(loop[i]);
    }
    loop.SetCurves(loop1.Curves());
}
//oloops 是loops 裁减前的环,loops 中索引对应的环需要与oloops 中相同索引的环一致.
void OperClipCurve::DeleteInvalidClipCurve(std::vector<ofts::DefLoop>& oloops,
                                           std::vector<ofts::DefLoop>& loops,
                                           double bordPre,bool isIn,
                                           bool isOnce)
{
    if(oloops.size() != loops.size() || loops.size() < 2){return;}
    for (size_t i = 0; i < loops.size(); ++i) {
        std::vector<ofts::DefLoop>tloops;
        for (size_t j = 0; j < oloops.size(); ++j) {
            if(i != j){tloops.emplace_back(oloops[j]);}
        }
        LoopInvalidClipCurve(loops[i],tloops,loops[i].IsBoundary(),
                             bordPre,isIn,isOnce);
    }
#if 0
    for (size_t i = 0; i < loops.size(); ++i) {
        DisplayGeom().DisplayLoop(loops[i],_colors[i],2);
    }
#endif
}
void OperClipCurve::ArrangeCurveToLoop(const ofts::DefLoop &inloop,
                                       double err, bool isClose,
                                       std::vector<ofts::DefLoop>& loops)
{
    if(inloop.Size() < 2){
        if(isClose){
            if(inloop.IsConnect(err)){
                loops.push_back(inloop);
            }
        }else{loops.push_back(inloop);}
        return;
    }
#if 0
    for (size_t i = 0; i < inloop.size(); ++i) {
        DisplayGeom().DisplayLoop(inloop[i],_colors[i],2);
    }
#endif
    size_t nex = 0;
    auto iloop = inloop;
    auto NextCurve = [&](const ofts::Point& p,ofts::DefElem& c)->bool{
        for (size_t i = 0; i < iloop.Size(); ++i) {
            if(p.IsSamePoint3D(iloop[i].GetSp(),err)){
                nex = i;
                c = iloop[i];
                return true;
            }else if(p.IsSamePoint3D(iloop[i].GetEp(),err)){
                nex = i;
                c = iloop[i];
                c.Reverse();
                return true;
            }
        }
        return false;
    };
    auto AddLoop = [](ofts::DefLoop &iloop,ofts::DefLoop &loop,
            bool isClose,std::vector<ofts::DefLoop>& loops)->void{
        loops.push_back(loop);
        loop.Clear();
        if(!iloop.IsEmpty()){
            loop.AddElem(iloop[0]);
            iloop.Erase(0);
        }
        if(!isClose && iloop.IsEmpty() && !loop.IsEmpty()){
            loops.push_back(loop);
        }
    };
    ofts::DefLoop loop;
    loop.AddElem(iloop[0]);
    iloop.Erase(0);
    ofts::DefElem curve;
    while (!iloop.IsEmpty()) {
        if(!NextCurve(loop.Endpoint(true),curve)){
            if(isClose){
                return;
            }
            AddLoop(iloop,loop,isClose,loops);
            continue;
        }
        loop.AddElem(curve);
        iloop.Erase(nex);
        if(loop.IsEndToEnd(err)){
            AddLoop(iloop,loop,isClose,loops);continue;
        }
        if(isClose){continue;}
        else{
            if(iloop.IsEmpty()){AddLoop(iloop,loop,isClose,loops);}
        }
    }
    for(auto& l : loops){
        /*DisplayGeom().DisplayLoop(l,_colors[1],2,true);
        string s = OperateOftString().LoopElementString(l);
        std::cout<<s<<std::endl;*/
        bool iscw = LoopDirection().IsClockwiseLoop(l);
        if(!iscw){l.Reverse();}
    }
    for(auto it = loops.begin();it != loops.end();){
        if(it->IsEmpty()){it = loops.erase(it);}
        else{++it;}
    }
#if 0
    for (size_t i = 0; i < loops.size(); ++i) {
        DisplayGeom().DisplayLoop(loops[i],_colors[i],2);
    }
#endif
}
//目前仅支持相邻索引曲线相连的情况
void OperClipCurve::CloseLoopToEndtoend(const ofts::DefLoop& inloop,
                                        double err,ofts::DefLoop& loop)
{
    if(inloop.Size() < 2){
        if(inloop.IsCircleLoop()){loop = inloop;}
        return;
    }
    loop.Clear();
    loop.Reserve(inloop.Size());{
        auto fc = inloop[0];
        const auto& sc = inloop[1];
        if(fc.GetEp().IsSamePoint2D(sc.GetSp(), err) ||
                fc.GetEp().IsSamePoint2D(sc.GetEp(), err)) {
            loop.AddElem(fc);
        } else if(fc.GetSp().IsSamePoint2D(sc.GetSp(), err) ||
                  fc.GetSp().IsSamePoint2D(sc.GetEp(), err)) {
            fc.Reverse();
            loop.AddElem(fc);
        }else{
            return;
        }
    }
    for (size_t i = 1; i < inloop.Size(); i++){
        const auto& curve = inloop[i];
        const auto& ep = loop.Endpoint(true);
        if(ep.IsSamePoint2D(curve.GetSp(),err)){
            loop.AddElem(curve);
        }else if(ep.IsSamePoint2D(curve.GetEp(),err)){
            auto c = curve;
            c.Reverse();
            loop.AddElem(c);
        }else{
            loop.Clear();
            return;
        }
    }
    if(!loop.IsConnect(err)) {
        loop.Clear();
    }
}
