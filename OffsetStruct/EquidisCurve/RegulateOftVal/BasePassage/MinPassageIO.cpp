#include <tbb/tbb.h>
#include "MinPassageIO.h"
#include<iostream>
using namespace oft::ProgOftValue;
#if DebugModel
#pragma optimize("", off)
#endif
void MinPassageIO::OffsetValueBasePassage(const DefLoop& loop,
                                          double R,double &offsetVal)
{
    if(loop.ElemNum() < 3){
        return;
    }
    DefLoop loop_ = loop;
    BaseMinPassage prob;
    bool isGlobal = true;
    vector<double>vDistance;
    vector<IndexDist>vProValue,vProValue_;
    BasePassagePreprocess(loop,R,loop_);
    vProValue.resize(loop_.ElemNum());
#if 1
    for (int i = 0; i < loop_.ElemNum(); ++i) {
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
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,loop_.ElemNum()),
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
        double min = *std::min_element(vDistance.begin(),vDistance.end());        double v1 = (min / 2) * 0.95,v2 = R * OftValMultiplier;
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
    loop_.ClearElem();
    GeomCalculate gocal;
    ElemCalculate elecal;
    for (int i = 0; i < loop.ElemNum(); ++i) {
        Point p;
        DefElem ele = loop.IndexElem(i),pre,nex;
        if(gocal.ElementLenth(ele) < R){
            loop_.PushBackElem(ele);
            continue;
        }
        elecal.ElementMidpoint(ele,p);
        elecal.PointSplitElement(ele,p,true,pre);
        elecal.PointSplitElement(ele,p,false,nex);
        loop_.PushBackElem(pre);
        loop_.PushBackElem(nex);
    }/**
    string str = OperateOftString().LoopElementString(loop_);
    std::cout << str << std::endl;
    int tem = 1;*/
}