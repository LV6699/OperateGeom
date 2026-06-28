#include "LoopUnionSolve.h"
#include "../../GeometryCalculate/RelativeContain/PointInLoop.h"
#include "../../GeometryCalculate/GeomCalculate/CurveLog2D.h"
#ifdef emit
#undef emit
#include <tbb/tbb.h>
#define emit Q_EMIT
#else
#include <tbb/tbb.h>
#endif
bool LoopUnionSolve::IsIdenticalEndpoint(const DefElem& ele,
                                         double errVal)
{
    GeomCalculate gocal;
    ElemCalculate numcal;
    return gocal.IsIdenticalPoint(numcal.ElementEndpoint(ele,false),
                                  numcal.ElementEndpoint(ele,true),
                                  errVal);
}
///vResLoop 中索引对应的元素需要匹配vLoop 中索引对应的元素.
void LoopUnionSolve::IntersectPointSplitLoop(const vector<DefLoop>& vLoop,
                                             bool& isExisClip,
                                             vector<DefLoop>& vResLoop)
{
    ElemCalculate numcal;
#if 0
    for (int i = 0; i < vLoop.size(); ++i) {/**
        if(i == 14){
            int tem = 1;
        }*/
        vector<DefLoop>vLoop_;
        DefLoop loop,resLoop = vLoop[i];
        resLoop.Clear();
        bool isExisClip_ = false;
        numcal.ExceptIndexLoop(vLoop,i,loop,vLoop_);
        LoopLoopsIntersectPoint(loop,vLoop_,isExisClip_,resLoop);
        vResLoop.push_back(resLoop);
        if(isExisClip_){
            isExisClip = true;
        }
    }
#else
    vResLoop.resize(vLoop.size());
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, vLoop.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            vector<DefLoop>vLoop_;
            DefLoop loop,resLoop = vLoop[i];
            resLoop.Clear();
            bool isExisClip_ = false;
            numcal.ExceptIndexLoop(vLoop,i,loop,vLoop_);
            LoopLoopsIntersectPoint(loop,vLoop_,isExisClip_,resLoop);
            vResLoop[i] = resLoop;
            if(isExisClip_){
                isExisClip = true;
            }
        }
    });
#endif
}
///loop 与vLoop 的相交点拆分loop.
void LoopUnionSolve::LoopLoopsIntersectPoint(const DefLoop& loop,
                                             const vector<DefLoop>&vLoop,
                                             bool& isExisClip,
                                             DefLoop& resLoop)
{
    resLoop = loop;
    resLoop.Clear();
    for (int i = 0; i < loop.Size(); ++i) {/**
        if(i == 1){
            int tem = 1;
        }*/
        vector<Point>vPoint;
        DefElem ele = loop.IndexElem(i);
        bool isExistInt = false;
        ElementLoopsIntersect(ele,vLoop,isExistInt,vPoint);
        if (vPoint.empty()){
            resLoop.AddElem(ele);
            continue;
        }
        isExisClip = true;
        vector<DefElem> vElem;
        CurveLog2D::PointsClipCurve(ele,vPoint,vElem);
        for (int e = 0; e < vElem.size(); ++e) {
            vElem[e]._label._isClip = true;
            if(!IsIdenticalEndpoint(vElem[e],PreErr5_9)){
                resLoop.AddElem(vElem[e]);
            }
        }
        resLoop._label._isIntersect = true;
    }
}
///计算ele 与vLoop 的所有相交点,针对bool 运算,如果相交点是元素端点,也存在需要裁剪的情况.
void LoopUnionSolve::ElementLoopsIntersect(const DefElem& ele,
                                           const vector<DefLoop>&vLoop,
                                           bool& isExistInt,
                                           vector<Point>& vPoint)
{
    GeomIntersect geoint;
    ElemCalculate numcal;
    for (int lo = 0; lo < vLoop.size(); ++lo) {
        for (int i = 0; i < vLoop[lo].Size(); ++i) {/**
            if(lo == 0 && i == 1){
                int tem = 1;
            }*/
            GeomIntRes intRes;
            geoint.CalculateCurveIntersect(ele,vLoop[lo].IndexElem(i),
                                           true,PreErr_10,PreErr_10,
                                           PreErr_10,intRes);
            if(!numcal.IsEleExistValInt(ele,vLoop[lo].IndexElem(i),intRes)){
                continue;
            }
            isExistInt = true;/**
            bool isEndpt1 = false,isEndpt2 = false;
            isEndpt1 = numcal.IsElementEndpoint(ele,intRes._intPnt1,
                                                SeIn_IntExtr_Err);
            if(!isEndpt1){
                vPoint.push_back(intRes._intPnt1);
            }*/
            vPoint.push_back(intRes._intPnt1);
            if(intRes._intNum <= 1){
                continue;
            }/**
            isEndpt2 = numcal.IsElementEndpoint(ele,intRes._intPnt2,
                                                SeIn_IntExtr_Err);
            if(!isEndpt2){
                vPoint.push_back(intRes._intPnt2);
            }*/
            vPoint.push_back(intRes._intPnt2);
        }
    }
    numcal.DeduplicationPoint(vPoint,PreErr5_10);
}
///vLoop 中索引对应的元素需要匹配vOriLoop 中索引对应的元素.
void LoopUnionSolve::DeleteInvalidClipElement(vector<DefLoop>& vBarLoop,
                                              bool& isExiDelete,
                                              vector<DefLoop>&vLoop)
{
    ElemCalculate numcal;
    vector<vector<size_t>>vArrIndex;
    for (size_t lo = 0; lo < vLoop.size(); ++lo) {
        vector<size_t>vIndex;
        vector<DefLoop>vLoop_;
        numcal.ExceptIndexLoop(vLoop,lo,vLoop_);
#if 0
        for (int i = 0; i < vLoop[lo].Size(); ++i) {/**
            if(lo == 14 && i == 1){
                int tem = 1;
            }*/
            Point midPt;
            DefElem ele = vLoop[lo].IndexElem(i);
            numcal.ElementMidpoint(ele,midPt);
            if(!IsPointInArrayLoop(vLoop_,midPt) &&
                    !IsPointInArrayLoop(vBarLoop,midPt)){
                continue;
            }
            vIndex.push_back(i);
            isExiDelete = true;
        }
        vArrIndex.push_back(vIndex);
#else
        DefLoop loop_ = vLoop[lo];
        tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop_.Size()),
                          [&](tbb::blocked_range<std::size_t>& r) {
            for (std::size_t i = r.begin(); i < r.end(); ++i) {
                Point midPt;
                DefElem ele = loop_.IndexElem(i);
                numcal.ElementMidpoint(ele,midPt);
                if(!IsPointInArrayLoop(vLoop_,midPt) &&
                        !IsPointInArrayLoop(vBarLoop,midPt)){
                    continue;
                }
                loop_.m_vElem[i]._label._isValid = false;
                isExiDelete = true;
            }
        });
        for (size_t i = 0; i < loop_.Size(); ++i) {
            if(!loop_.IndexElem(i)._label._isValid)
                vIndex.push_back(i);
        }
        vArrIndex.push_back(vIndex);
#endif
    }
    for (size_t i = 0; i < vArrIndex.size(); ++i) {
        numcal.DeleteIndexElement(vArrIndex[i],vLoop[i].m_vElem);
    }
}
bool LoopUnionSolve::IsPointInArrayLoop(vector<DefLoop>&vLoop,
                                        const Point&P)
{
    for(auto& l : vLoop){LimitCoord2D::SetLoopLimCoord(l);}
#if 1
    for (size_t i = 0; i < vLoop.size(); ++i) {
        bool isOnBorder = false,isInPoly = false;
        PointInLoop().GetPointInsideLoop(vLoop[i],P,PreErr5_10,
                                         isOnBorder,isInPoly);
        if(isOnBorder)
            return false;
        if(isInPoly)
            return true;
    }
    return false;
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, vLoop.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            bool isOnBorder = false,isInPoly = false;
            inpoly.IsPointInsideLoop(vLoop[i],P,PreErr5_10,
                                     isOnBorder,isInPoly);
            if(isOnBorder)
                return false;
            if(isInPoly)
                return true;
        }
        return false;
    });
#endif
}
