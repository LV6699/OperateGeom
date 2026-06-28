#include <utility>
#include <iostream>
#include "LoopSelIntAux.h"
#include "OperOpenLoop.h"
#include "../../GeometryCalculate/RelativeContain/LimitCoord.h"
#include "../../GeometryCalculate/GeomCalculate/CurveLog2D.h"
#include "../../GeometryCalculate/GeomCalculate/CurveDist2D.h"
#include "../../GeometryCalculate/RelativeContain/PointInLoop.h"
#include "../../GeometryClip/ClipIntersect/CurveIntClip.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#ifdef emit
#undef emit
#include <tbb/tbb.h>
#define emit Q_EMIT
#else
#include <tbb/tbb.h>
#endif
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
//删除cs 中无效的连接曲线:
void LoopSelIntAux::DelInvalidCntCurve(const std::vector<ofts::DefElem>& cs)
{

}
void LoopSelIntAux::DeleContCurves(std::vector<ofts::DefElem>& cs)
{
    PointInLoop pil;
    ofts::DefLoop loop;loop.Reserve(cs.size() - 1);
    //DisplayGeom().DisplayLoop(ofts::DefLoop(cs),_colors.back(),1,true,true);
    for (size_t i = 0; i < cs.size(); i++){
        /*if(i == 1){
            int tem = 1;
        }*/
        for (size_t j = 0; j < cs.size(); j++){if(i != j){loop.AddElem(cs[j]);}}
        auto& c = cs[i];
        bool isOnBord = false,isInPoly = false;
        auto mp = BaseGeom2D::CurveMidpoint(c);
        pil.GetPointInsideLoop(loop,mp,PreErr_14,isOnBord,isInPoly);
        if(!isInPoly){continue;}
        pil.GetPointInsideLoop(loop,mp,PreErr_14,isOnBord,isInPoly,false);
        if(!isInPoly){continue;}
        c.SetValid(false);
        loop.Clear();
    }
    cs.erase(std::remove_if(cs.begin(),cs.end(),[](const ofts::DefElem& c){
        return !c.IsValid();}),cs.end());
    //DisplayGeom().DisplayLoop(ofts::DefLoop(cs),_colors[1],1);
    //std::cout<<"";
}
Ofte::DistInOut LoopSelIntAux::CurveToLoopDistType(const ofts::DefElem& c,const size_t& id,
                                                   bool isOpen,const ofts::DefLoop& loop,double R)
{
    if(c.IsAbsValid()){return Ofte::EntiretyOut;}
    double threshold = R - SeIn_Dis_Err;
    bool isOutSide = false,isInside = false;
#if 0
    std::vector<double>dists;
    for (size_t i = 0; i < loop.Size(); ++i) {
        if(/*true || */(id == 14 && i == 8)){
            DisplayGeom().DisplayElement(c,_colors[1],2,true,false);
            DisplayGeom().DisplayElement(loop[i],_colors[2],1,true);
            int tem = 1;
        }
        double d = CurveDist2D::CurveDistance(c,loop[i]);
        dists.emplace_back(d);
        if(d >= threshold){
            isOutSide = true;
        }else{
            isInside = true;
            if(!isOpen || isOutSide){
                break;
            }
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop.Size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i != r.end(); i++) {
            double d = CurveDist2D::CurveDistance(c,loop[i]);
            if(d >= threshold){isOutSide = true;}
            else{isInside = true;if(!isOpen || isOutSide){break;}}
        }});
#endif
    //可通过判断vDistance 中最大值和最小值判断disType 的取值
    if(isOutSide && !isInside){return Ofte::EntiretyOut;}
    else if(isOutSide && isInside){return Ofte::PartOut;}
    return Ofte::EntiretyIn;
}
bool LoopSelIntAux::DelInvalidClipCurve(const ofts::DefLoop& oloop,
                                        const numst::IntParam& ipa,
                                        double R,bool isOpen,
                                        std::vector<ofts::DefElem>& ics)
{
    bool isDelete = false;
    if(oloop.IsEmpty() || ics.empty()){return false;}
    std::vector<size_t>ids;ids.reserve(ics.size());
    struct DistInf{
        DistInf(){}
        DistInf(const ofts::DefElem& c,Ofte::DistInOut d) : curve(c),dt(d){}
        ofts::DefElem curve;
        Ofte::DistInOut dt;
    };
    //DisplayGeom().DisplayLoop(oloop,_colors.back(),1,true);
    //DisplayGeom().DisplayLoop(ofts::DefLoop(ics),_colors[0],1.5,true);
    std::vector<DistInf>infs;
    for(auto& d : ics){infs.emplace_back(d,Ofte::EntiretyOut);}
    ics.clear();
#if 1
    for (size_t i = 0; i < infs.size(); ++i) {
        //DisplayGeom().DisplayElement(infs[i].curve,_colors[1],2,true,false);
        infs[i].dt = CurveToLoopDistType(infs[i].curve,i,isOpen,oloop,R);
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, infs.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            infs[i].dt = CurveToLoopDistType(infs[i].curve,isOpen,i,oloop,R);
        }
    });
#endif
    if(!isOpen){
        for (size_t i = 0; i < infs.size(); i++){
            if(infs[i].dt == Ofte::EntiretyOut){continue;}
            infs[i].curve.SetValid(false);isDelete = true;
        }
        infs.erase(std::remove_if(infs.begin(),infs.end(),[](const DistInf& d){
            return !d.curve.IsValid();}),infs.end());
        for(auto& d : infs){ics.emplace_back(d.curve);}
        //DisplayGeom().DisplayLoop(ofts::DefLoop(ics),_colors[1],2,true);
        return isDelete;
    }
    ofts::DefArc arc1,arc2;
    OperOpenLoop().CreateEndClipArc(oloop,infs[0].curve,infs.back().curve,R,arc1,arc2);
    ofts::DefLoop ecl_l(std::vector<ofts::DefElem>{ofts::DefElem(arc1),ofts::DefElem(arc2)});
    //DisplayGeom().DisplayLoop(ecl_l,_colors.back(),3);
    std::vector<DistInf>infs1;infs1.reserve(infs.size());

    for (size_t i = 0; i < infs.size(); i++){/*
        if(i == 9){
            int tem = 1;
        }*/
        const auto& inf = infs[i];
        if(inf.dt != Ofte::PartOut){
            //DisplayGeom().DisplayElement(inf.curve,_colors[i],1,true);
            continue;
        }
        const auto& c = inf.curve;
        //DisplayGeom().DisplayElement(c,_colors[i],1,true);
        std::vector<ofts::DefElem>cl_cs;
        CurveIntClip().LoopClipCurve(c,ecl_l,ipa,cl_cs);
        for (size_t j = 0; j < cl_cs.size(); j++){
            auto& c1 = cl_cs[j];
            //DisplayGeom().DisplayElement(c1,_colors[i+j],2,true);
            auto t = CurveToLoopDistType(c1,j,isOpen,oloop,R);
            infs.emplace_back(DistInf(c1,t));
        }
        isDelete = true;
    }
    for (size_t i = 0; i < infs.size(); i++){
        if(infs[i].dt != Ofte::EntiretyOut){
            isDelete = true;continue;
        }
        ics.emplace_back(infs[i].curve);
    }
    //DisplayGeom().DisplayLoop(ofts::DefLoop(ics),_colors[1],1,true);
    return isDelete;
}
void LoopSelIntAux::BuildCurveToLoops(const ofts::DefLoop& inloop,
                                      bool isOpen,double err,
                                      std::vector<ofts::DefLoop>& loops)
{
#if 0
    for (int i = 0; i < inloop.Size(); ++i) {
        DisplayGeom().DisplayElement(inloop[i],_colors[i],2);
    }
    DisplayGeom().DisplayLoop(inloop,_colors.back(),1,true,true);
    string s = OperateOftString().LoopElementString(inloop);
    std::cout<<s<<std::endl<<1<<std::endl;
#endif
    if(inloop.Size() < 2){
        if(inloop.IsCircleLoop() || isOpen){loops.emplace_back(inloop);}
        return;
    }
    if(inloop.IsConnect(err,isOpen)){loops.emplace_back(inloop);return;}
    ofts::DefLoop iloop = inloop,loop;
    loop.Reserve(iloop.Size());
    for(size_t i = 0;i < iloop.Size();++i){iloop[i].SetId(i);}
    //组建loop 过程中已判断出的无效曲线索引
    std::vector<size_t>inIds;inIds.reserve(iloop.Size());

    auto DelInCurve = [](const size_t& id,ofts::DefLoop& loop){
        for (size_t i = 0; i < loop.Size(); ++i){
            if(loop[i].Id() == id){loop.Erase(i);break;}
        }
    };
    auto NextCurve = [&](const ofts::Point& p,ofts::DefElem& c){
        std::vector<size_t>ids;ids.reserve(loop.Size());
        for(auto& c : loop){ids.emplace_back(c.Id());}
        for(size_t i = 0; i < iloop.Size(); ++i) {
            if(pnum::IsInArray(iloop[i].Id(),ids)){continue;}
            if(pnum::IsInArray(iloop[i].Id(),inIds)){continue;}
            if(p.IsEqual(iloop[i].GetSp(),err)){
                c = iloop[i];return true;
            }else if(p.IsEqual(iloop[i].GetEp(),err)){
                c = iloop[i];c.Reverse();return true;  //不会修改iloop[i]
            }
        }
        return false;
    };
    auto AddLoop = [&](){
        for(auto& c : loop){
            DelInCurve(c.Id(),iloop);
        }
        if(isOpen && !loop.IsEndToEnd(err)){
            loop.SetEndType(Ofte::EndType::Open);
        }
        loops.emplace_back(loop);
        loop.Clear();inIds.clear();
        if(iloop.IsEmpty()){return;}
        if(!isOpen && iloop.Size() < 2){iloop.Clear();}
        else{loop.AddElem(iloop[0]);}
    };
    /**
    iloop 中有两种曲线导致组建闭环loop 失败:1-两端点不全与iloop 中其他曲线两端点相连曲线c,
    因为在迭代过程中,以c 结束点迭代时,无法找到与之相连的曲线.2-与iloop 中其他曲线重合的无效
    曲线,比如0~i 曲线首尾相连,且0~i 和i+3~n 所有曲线能组建一个有效闭环,i,i+1,i+2 首尾相连,
    且i+2 端点仅与0~i 中的某一个端点相连.如果以i 结束点找到i+3 则迭代成功,如果找到i+1 则会失
    败,因为0~i 已经被添加至loop 中,所以不能找到与i+2 相连的有效曲线.*/
    //iloop 的删除元素仅在loop 是封闭或明确某曲线无效的情况进行
    loop.AddElem(iloop[0]);ofts::DefElem curve;

    while (!iloop.IsEmpty()) {
        const auto& las = loop.Last();
        if(!NextCurve(las.GetEp(),curve)){
            //说明loop 与iloop 中曲线完全一样,直接退出
            if(loop.Size() == iloop.Size()){
                AddLoop();break;
            }
            if(isOpen){
                AddLoop();continue;
            }
            //这种情况可能是las 本身是无效曲线,也可能是loop 中las 前面的曲线是无
            //效曲线引起的,不能直接删除las.
            auto lid = las.Id();
            inIds.emplace_back(lid);
            loop.PopBack();  //这里注意悬空引用
            //说明迭代失效是由于loop 中第一个曲线fc 引起的,即fc 本身就是无效曲线,
            //否则fc 有效的情况下loop 不可能为空.
            if(loop.IsEmpty()){
                DelInCurve(lid,iloop);
                if(!iloop.IsEmpty()){
                    loop.AddElem(iloop[0]);
                    inIds.clear();
                }
            }
            continue;
        }
        loop.AddElem(curve);
        if(!loop.IsEndToEnd(err)){continue;}
        //double d = loop.Endpoint(false).Distance2D(loop.Endpoint(true));
        AddLoop();
    }
#if 0
    DisplayGeom().DisplayLoops(loops,_colors[1],1.5);
#endif
    for (size_t i = 0; i < loops.size(); ++i) {
        double xSpan = 0,ySpan = 0;
        LimitCoord2D::LoopMinimumBox(loops[i],xSpan,ySpan);
        if(xSpan > PreErr5_2 || ySpan > PreErr5_2){continue;}
        loops[i].SetValid(false);
    }
    loops.erase(std::remove_if(loops.begin(),loops.end(),[](const ofts::DefLoop& l){
        return !l.IsValid();}),loops.end());
}
void LoopSelIntAux::DeleDoubCntCurves(std::vector<ofts::DefElem>& cs)
{
    //DisplayGeom().DisplayLoop(ofts::DefLoop(cs),_colors.back(),1,true,true);
    struct CntCurv{
        CntCurv(){}
        CntCurv(size_t i,bool sp) : id(i),issp(sp){}
        size_t id;
        bool issp = false;
        bool isValid = true;
    };
    struct CntInf{
        CntInf(){}
        CntInf(int i) : id(i){}
        int id = -1;
        bool isValid = true;
        std::vector<CntCurv>ss;
        std::vector<CntCurv>es;
    };
    std::vector<CntInf>infs;infs.reserve(cs.size());
    for (size_t i = 0; i < cs.size(); i++){infs.emplace_back(CntInf(i));}

    for (size_t i = 0; i < cs.size(); i++){
        auto& inf = infs[i];
        const auto& sp = cs[i].GetSp();
        const auto& ep = cs[i].GetEp();
        for (size_t j = i+1; j < cs.size(); j++){
            auto& inf1 = infs[j];
            const auto& sp1 = cs[j].GetSp();
            const auto& ep1 = cs[j].GetEp();
            if(sp.IsEqual(sp1,PreErr_14)){
                inf.ss.emplace_back(CntCurv(j,true));
                inf1.ss.emplace_back(CntCurv(i,true));
            }else if(sp.IsEqual(ep1,PreErr_14)){
                inf.ss.emplace_back(CntCurv(j,false));
                inf1.es.emplace_back(CntCurv(i,true));
            }
            if(ep.IsEqual(sp1,PreErr_14)){
                inf.es.emplace_back(CntCurv(j,true));
                inf1.ss.emplace_back(CntCurv(i,false));
            }else if(ep.IsEqual(ep1,PreErr_14)){
                inf.es.emplace_back(CntCurv(j,false));
                inf1.es.emplace_back(CntCurv(i,false));
            }
        }
    }
    auto Invalid = [&](bool& finish){
        for(auto& d : infs){
            if(!d.isValid){continue;}
            if(d.ss.size() < 1 || d.es.size() < 1 || (d.ss.size() > 1 && d.es.size() > 1)){
                cs[d.id].SetValid(false);d.isValid = false;finish = false;
            }
        }
    };
    auto Reset = [&](std::vector<CntCurv>& cns){
        for(auto& d : cns){
            const auto& i = d.id;
            if(infs[i].isValid){continue;}
            d.isValid = false;cs[i].SetValid(false);
        }
        cns.erase(std::remove_if(cns.begin(),cns.end(),[](const CntCurv& d){
            return !d.isValid;}),cns.end());
    };
    while (true){
        bool finish = true;
        Invalid(finish);
        for(auto& d : infs){
            Reset(d.ss);Reset(d.es);
        }
        Invalid(finish);
        if(finish){break;}
    }
    cs.erase(std::remove_if(cs.begin(),cs.end(),[](const ofts::DefElem& c){
        return !c.IsValid();}),cs.end());
    infs.erase(std::remove_if(infs.begin(),infs.end(),[](const CntInf& d){
        return !d.isValid;}),infs.end());

    auto Connect = [](const ofts::DefLoop& loop){

    };
    /*for (size_t i = 0; i < infs.size(); i++){
        const auto& inf = infs[i];
        if(inf.es.size() < 2){continue;}

        int dec = 0;
        auto& es = inf.es;
        ofts::DefLoop loop;loop.Reserve(cs.size() - 1);

        for(auto& e : es){
            if(e.issp){e.issp = false;continue;}
            auto& id = e.id;
            for (size_t j = 0; j < cs.size(); j++){if(j != id){loop.AddElem(cs[j]);}}
        }
    }*/
    //DisplayGeom().DisplayLoop(ofts::DefLoop(cs),_colors[2],1);
}
void LoopSelIntAux::LoopMaxOutContour(const ofts::DefLoop& inloop,ofts::DefLoop& loop)
{

}
