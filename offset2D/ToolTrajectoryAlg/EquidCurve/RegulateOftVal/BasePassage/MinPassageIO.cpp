#include "MinPassageIO.h"
#include<iostream>
using namespace OffsetStruct::ProgOftValue;
// #ifdef emit
// #pragma push_macro("emit")
// #undef emit
// #endif
#ifdef emit
#undef emit
#include <tbb/tbb.h>
#define emit Q_EMIT
#else
#include <tbb/tbb.h>
#endif
#if DebugModel
#pragma optimize("", off)
#endif
void MinPassageIO::OffsetValueBasePassage(const DefLoop& loop,
                                          double R,double &offsetVal)
{
    if(loop.Size() < 3){
        return;
    }
    DefLoop loop_ = loop;
    BaseMinPassage prob;
    bool isGlobal = true;
    vector<double>vDistance;
    vector<IndexDist>vProValue,vProValue_;
    BasePassagePreprocess(loop,R,loop_);
    vProValue.resize(loop_.Size());
#if 1
    for (int i = 0; i < loop_.Size(); ++i) {
        if(i == 15){
            int tem = 1;
        }
        IndexDist idDistance;
        prob.ExploreProbableUnprocess(loop_,R,offsetVal,i,idDistance);
        if(idDistance.IsMeetDistance()){
            vProValue[i] = idDistance;
            ///vProValue.push_back(idDistance);
            if(!isGlobal)
                break;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,loop_.Size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            IndexDist idDistance;
            prob.ExploreProbableUnprocess(loop_,R,offsetVal,i,idDistance);
            if(idDistance.IsMeetDistance()){
                vProValue[i] = idDistance;
                ///vProValue.push_back(idDistance);
                if(!isGlobal)
                    break;
            }
        }
    });
#endif
    for(auto& d : vProValue){
        if(d.IsMeetDistance()){
            vProValue_.push_back(d);
        }
    }
    if(vProValue_.empty())
        return;
    double offsetVal_ = R;
    for(auto &d : vProValue_){
        vDistance.push_back(d.ToDistance());
    }
    if(isGlobal){
        double min = *std::min_element(vDistance.begin(),vDistance.end());
        double v1 = (min / 2) * 0.95,v2 = R * OftValMultiplier;
        ///double v1 = (min / 2) * PreErr_9_,v2 = R * OftValMultiplier;
        offsetVal_ = v1 > v2 ? v1 : v2;
    }
    else{
        offsetVal_ = R * OftValMultiplier;
    }
    if(offsetVal > offsetVal_){
        offsetVal = offsetVal_;
    }
}
void MinPassageIO::BasePassagePreprocess(const DefLoop& loop,
                                         double R,DefLoop& loop_)
{
    loop_.Clear();
    GeomCalculate gocal;
    ElemCalculate elecal;
    for (int i = 0; i < loop.Size(); ++i) {
        Point p;
        DefElem ele = loop.IndexElem(i),pre,nex;
        if(gocal.ElementLenth(ele) < R){
            loop_.AddElem(ele);
            continue;
        }
        elecal.ElementMidpoint(ele,p);
        elecal.PointSplitElement(ele,p,true,pre);
        elecal.PointSplitElement(ele,p,false,nex);
        loop_.AddElem(pre);
        loop_.AddElem(nex);
    }/**
    string str = OperateOftString().LoopElementString(loop_);
    std::cout << str << std::endl;
    int tem = 1;*/
}
