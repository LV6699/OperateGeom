#include <tbb/tbb.h>
#include "BIReguOftVal.h"
#if DebugModel
#pragma optimize("", off)
#endif
void BIReguOftVal::GetLoopReferenceDistance(const DefLoop& loop1,
                                            const DefLoop& loop2,
                                            double refVal,
                                            bool& isExiLess)
{
    GeomDistance godis;
    for (int i = 0; i < loop1.m_vElem.size(); ++i) {
        #if 0
        for (int e = 0; e < loop2.m_vElem.size(); ++e) {
            double distance;
            godis.ElementToElementDistance(loop1.m_vElem[i],
                                           loop2.m_vElem[e],distance);
            if(distance < refVal){
                isExiLess = true;
                return;
            }
        }
        #else
        tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop2.ElemNum()),
                          [&](tbb::blocked_range<std::size_t>& r) {
            for (std::size_t e = r.begin(); e < r.end(); ++e) {
                double distance;
                godis.ElementToElementDistance(loop1.m_vElem[i],
                                               loop2.m_vElem[e],distance);
                if(distance < refVal){
                    isExiLess = true;
                    return;
                }
            }
        });
        #endif
    }
}
void BIReguOftVal::IsBndIslProbableInt(const DefLoop& bndLop,
                                       const vector<DefLoop>& vIslLoop,
                                       double offsetVal,
                                       bool& isProbInt)
{
    ///CheckGeomInt chint;
    double valLimit = offsetVal * 2;
#if 0
    for (int lo = 0; lo < vIslLoop.size(); ++lo) {
        GetLoopReferenceDistance(bndLop,vIslLoop[lo],
                                 valLimit,isProbInt);
        /*chint.IsLoopLoopIntersect(bndLop,vIslLoop[lo],
                                  PreErr5_10,isProbInt);*/
        if(isProbInt)
            return;
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, vIslLoop.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t lo = r.begin(); lo < r.end(); ++lo) {
            GetLoopReferenceDistance(bndLop,vIslLoop[lo],                                     valLimit,isProbInt);/**
            chint.IsLoopLoopIntersect(bndLop,vIslLoop[lo],
                                      PreErr5_10,isProbInt);*/
            if(isProbInt)
                return;
        }
    });
#endif
    if(vIslLoop.size() < 2)
        return;
    ElemCalculate elecal;
#if 1
    for (int i = 0; i < vIslLoop.size(); ++i) {
        vector<DefLoop>vLoop_;
        DefLoop loop = vIslLoop[i];
        elecal.ExceptIndexLoop(vIslLoop,i,vLoop_);/**
        chint.IsLoopArrayLoopIntersect(vLoop_,loop,PreErr5_10,
                                       isProbInt);
        if(isProbInt)
            return;*/
        for (int lo = 0; lo < vLoop_.size(); ++lo) {
            GetLoopReferenceDistance(loop,vLoop_[lo],
                                     valLimit,isProbInt);
            if(isProbInt)
                return;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, vIslLoop.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            vector<DefLoop>vLoop_;
            DefLoop loop = vIslLoop[i];
            elecal.ExceptIndexLoop(vIslLoop,i,vLoop_);/**
            chint.IsLoopArrayLoopIntersect(vLoop_,loop,PreErr5_10,
                                           isProbInt);
            if(isProbInt)
                return;*/
            for (int lo = 0; lo < vLoop_.size(); ++lo) {
                GetLoopReferenceDistance(loop,vLoop_[lo],
                                         valLimit,isProbInt);
                if(isProbInt)
                    return;
            }
        }
    });
#endif
}
void BIReguOftVal::BndIslRegulateOffsetVal(const DefLoop& bndLop,
                                           const vector<OffsetLoop>&vIslNode,
                                           bool isFirOffset,
                                           double firOftVal,
                                           double oriOftVal,
                                           OffsetParam& oftPara)
{
    double offsetVal = isFirOffset == true ? firOftVal : oriOftVal;
    oftPara.SetIsFirstOffset(isFirOffset);
    oftPara._offsetUnit.SetOffsetValue(offsetVal);
    if(!oftPara._pathTech.IsAddOriginLoop() && isFirOffset)
        return;
    if(oftPara._offsetUnit.OffsetValue() <=            oftPara._offsetUnit.ToolRadius())
        return;
    ///return;
    vector<DefLoop>vIslLoop;
    for (int i = 0; i < vIslNode.size(); ++i) {
        vIslLoop.push_back(vIslNode[i].m_loop);
    }
    bool isBIProbInt = false;
    IsBndIslProbableInt(bndLop,vIslLoop,offsetVal,isBIProbInt);
    if(isBIProbInt){
        offsetVal = oftPara._offsetUnit.ToolRadius() * OftValMultiplier;
        double offsetValue = offsetVal < oftPara._offsetUnit.OffsetValue() ?
                    offsetVal : oftPara._offsetUnit.OffsetValue();
        oftPara._offsetUnit.SetOffsetValue(offsetValue);
    }
}