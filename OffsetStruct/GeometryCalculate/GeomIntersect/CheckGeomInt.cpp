#include <tbb/tbb.h>
#include "CheckGeomInt.h"
#if DebugModel
#pragma optimize("", off)
#endif
void CheckGeomInt::JudgeArcRadius(DefArc& arc)
{
    if(!(MathCalculate().IsLegalValue(arc._arcR)) ||
            (arc._arcR <= PreErr5_3))
        arc.IniArcRadius();
}
void CheckGeomInt::IsCircleCircleIntersect(const Point &cp1,
                                           double R1,
                                           const Point &cp2,
                                           double R2,
                                           bool &isExiCCInt)
{
    isExiCCInt = false;
    double maxRadius = (R1 >= R2) ? R1 : R2;
    double minRadius = (R1 < R2) ? R1 : R2;
    double cpDis;
    GeomCalculate gocal;
    gocal.TwoPointDistance(cp1,cp2,cpDis);
    if(cpDis > (R1 + R2 + CC_ExiIn_Err))
        return;
    else if(maxRadius > (minRadius + cpDis + CC_ExiIn_Err))
        return;
    if(gocal.IsIdenticalPoint(cp1,cp2,PreErr5_10) &&
            std::abs(R1 - R2) < PreErr5_10)
        return;
    isExiCCInt = true;
}
void CheckGeomInt::IsCircleCircleIntersect(const Point &cp1,double R1,
                                           const Point &cp2,double R2,
                                           double pre,
                                           bool &isExiCCInt)
{
    isExiCCInt = false;
    double maxRadius = (R1 >= R2) ? R1 : R2;
    double minRadius = (R1 < R2) ? R1 : R2;
    double cpDis;
    GeomCalculate gocal;
    gocal.TwoPointDistance(cp1,cp2,cpDis);
    if(cpDis > (R1 + R2 + pre))
        return;
    else if(maxRadius > (minRadius + cpDis + pre))
        return;
    if(gocal.IsIdenticalPoint(cp1,cp2,pre) &&
            std::abs(R1 - R2) < pre)
        return;
    isExiCCInt = true;
}
void CheckGeomInt::IsLineCircleIntersect(const Point& lSp,
                                         const Point& lEp,
                                         const Point& cp,
                                         double R,double pre,                                         bool &isExiInt)
{
    isExiInt = false;
    double dis,limitVal = R + pre;
    GeomCalculate().PointToLineDistance(lSp,lEp,cp,dis);
    if(limitVal < dis)
        return;
    isExiInt = true;
}
void CheckGeomInt::IsLineArcIntersect(const Point& lSp,
                                      const Point& lEp,
                                      DefArc arc,
                                      double pre,
                                      bool& isExiInt)
{
    isExiInt = false;
    JudgeArcRadius(arc);
    GeomIntRes intRes;
    GeomIntersect goInt;
    goInt.CalculateLineCircleInt(lSp,lEp,arc._arcCp,
                                 arc._arcR,pre,pre,intRes);
    if(!intRes._isExistInt)
        return;
    PointOnElement pton;
    if(pton.IsPointOnLine(lSp,lEp,arc._arcSp,pre) ||
            pton.IsPointOnLine(lSp,lEp,arc._arcEp,pre)){
        isExiInt = true;
        return;
    }
    bool isPnt1OnArc = false,isPnt2OnArc = false;
    pton.IsPointOnArc(arc,intRes._intPnt1,pre,pre,
                      false,isPnt1OnArc);
    if(isPnt1OnArc){
        isExiInt = true;
        return;
    }
    if(intRes._intNum == 2){
        pton.IsPointOnArc(arc,intRes._intPnt2,
                          pre,pre,false,isPnt2OnArc);
        if(isPnt2OnArc)
            isExiInt = true;
    }
}
void CheckGeomInt::IsSegmentSegmentIntersect(const Point& lSp1,
                                             const Point& lEp1,
                                             const Point& lSp2,
                                             const Point& lEp2,
                                             double pre,
                                             bool& isExiInt)
{
    isExiInt = false;
    ElemBoxIntersect boxint;
    if(!boxint.IsRectangleExistOverlap(lSp1,lEp1,lSp2,lEp2,
                                       pre * ErrTimes_10))
        return;
    PointOnElement pton;
    pton.IsPointOnSegment(lSp2,lEp2,lSp1,pre,pre,true,isExiInt);
    if(isExiInt)
        return;    pton.IsPointOnSegment(lSp2,lEp2,lEp1,pre,pre,true,isExiInt);
    if(isExiInt)
        return;
    pton.IsPointOnSegment(lSp1,lEp1,lSp2,pre,pre,true,isExiInt);
    if(isExiInt)
        return;
    pton.IsPointOnSegment(lSp1,lEp1,lEp2,pre,pre,true,isExiInt);
    if(isExiInt)
        return;
    Point intPnt;
    bool isExiLLInt = false;
    GeomIntersect goInt;
    goInt.CalculateLineLineInt(lSp1,lEp1,lSp2,lEp2,isExiLLInt,intPnt);
    if(!isExiLLInt)
        return;
    bool isOnSeg1 = false,isOnSeg2 = false;
    pton.IsPointOnSegment(lSp1,lEp1,intPnt,pre,pre,false,isOnSeg1);
    pton.IsPointOnSegment(lSp2,lEp2,intPnt,pre,pre,false,isOnSeg2);
    if(isOnSeg1 && isOnSeg2)
        isExiInt = true;
}
void CheckGeomInt::IsSegmentCircleIntersect(const Point& lSp,
                                            const Point& lEp,
                                            const Point& cp,
                                            double R,
                                            double pre,
                                            bool& isExiInt)
{
    isExiInt = false;
    PointOnElement pton;
    pton.IsPointOnCircle(lSp,cp,R,pre,isExiInt);
    if(isExiInt)
        return;
    pton.IsPointOnCircle(lEp,cp,R,pre,isExiInt);
    if(isExiInt)
        return;
    GeomIntRes intRes;
    GeomIntersect goInt;
    goInt.CalculateLineCircleInt(lSp,lEp,cp,R,pre,pre,intRes);
    if(!intRes._isExistInt)
        return;
    pton.IsPointOnSegment(lSp,lEp,intRes._intPnt1,
                          pre,pre,false,isExiInt);
    if(isExiInt)
        return;
    if(intRes._intNum == 2)
        pton.IsPointOnSegment(lSp,lEp,intRes._intPnt2,
                              pre,pre,false,isExiInt);
}
void CheckGeomInt::IsArcCircleIntersect(DefArc arc,
                                        const Point& cp,
                                        double R,
                                        double pre,
                                        bool& isExiInt)
{
    isExiInt = false;
    PointOnElement pton;
    pton.IsPointOnCircle(arc._arcSp,cp,R,pre,isExiInt);
    if(isExiInt)        return;
    pton.IsPointOnCircle(arc._arcEp,cp,R,pre,isExiInt);
    if(isExiInt)
        return;
    JudgeArcRadius(arc);
    GeomIntRes intRes;
    GeomIntersect goInt;
    goInt.CalculateCircleCircleInt(arc._arcCp,arc._arcR,
                                   cp,R,pre,pre,intRes);
    if(!intRes._isExistInt)
        return;
    pton.IsPointOnArc(arc,intRes._intPnt1,pre,pre,false,isExiInt);
    if(isExiInt)
        return;
    if(intRes._intNum == 2){
        pton.IsPointOnArc(arc,intRes._intPnt2,pre,pre,false,isExiInt);
    }
}
void CheckGeomInt::IsSegmentArcIntersect(const Point& lSp,
                                         const Point& lEp,
                                         DefArc arc,double pre,
                                         bool& isExiInt)
{
    isExiInt = false;
    JudgeArcRadius(arc);
    PointOnElement pton;
    pton.IsPointOnSegment(lSp,lEp,arc._arcSp,pre,
                          pre,true,isExiInt);
    if(isExiInt)
        return;
    pton.IsPointOnSegment(lSp,lEp,arc._arcEp,pre,
                          pre,true,isExiInt);
    if(isExiInt)
        return;
    pton.IsPointOnArc(arc,lSp,pre,pre,true,isExiInt);
    if(isExiInt)
        return;
    pton.IsPointOnArc(arc,lEp,pre,pre,true,isExiInt);
    if(isExiInt)
        return;
    GeomIntersect goInt;
    GeomIntRes intRes;
    goInt.CalculateLineCircleInt(lSp,lEp,arc._arcCp,
                                 arc._arcR,pre,pre,intRes);
    if(!intRes._isExistInt)
        return;
    bool isPnt1OnSig = false,isPnt2OnSig = false,
            isPnt1OnArc = false,isPnt2OnArc = false;
    pton.IsPointOnSegment(lSp,lEp,intRes._intPnt1,
                          pre,pre,false,isPnt1OnSig);
    if(isPnt1OnSig){
        pton.IsPointOnArc(arc,intRes._intPnt1,pre,
                          pre,false,isPnt1OnArc);
        if(isPnt1OnArc){
            isExiInt = true;
            return;
        }
    }
    if(intRes._intNum == 2){
        pton.IsPointOnSegment(lSp,lEp,intRes._intPnt2,
                              pre,pre,false,isPnt2OnSig);
        if(isPnt2OnSig){
            pton.IsPointOnArc(arc,intRes._intPnt2,pre,                              pre,false,isPnt2OnArc);
            if(isPnt2OnArc)
                isExiInt = true;
        }
    }
}
void CheckGeomInt::IsArcArcIntersect(DefArc arc1,DefArc arc2,
                                     double pre,bool& isExiInt)
{
    isExiInt = false;
    JudgeArcRadius(arc1);
    JudgeArcRadius(arc2);
    GeomIntersect goInt;
    PointOnElement pton;
    pton.IsPointOnArc(arc1,arc2._arcSp,pre,pre,true,isExiInt);
    if(isExiInt)
        return;
    pton.IsPointOnArc(arc1,arc2._arcEp,pre,pre,true,isExiInt);
    if(isExiInt)
        return;
    pton.IsPointOnArc(arc2,arc1._arcSp,pre,pre,true,isExiInt);
    if(isExiInt)
        return;
    pton.IsPointOnArc(arc2,arc1._arcEp,pre,pre,true,isExiInt);
    if(isExiInt)
        return;
    GeomIntRes intRes;
    goInt.CalculateCircleCircleInt(arc1._arcCp,arc1._arcR,
                                   arc2._arcCp,arc2._arcR,
                                   pre,pre,intRes);
    if(!intRes._isExistInt)
        return;
    bool isPnt1OnArc1 = false,isPnt1OnArc2 = false,
            isPnt2OnArc1 = false,isPnt2OnArc2 = false;
    pton.IsPointOnArc(arc1,intRes._intPnt1,
                      pre,pre,false,isPnt1OnArc1);
    if(isPnt1OnArc1){
        pton.IsPointOnArc(arc2,intRes._intPnt1,
                          pre,pre,false,isPnt1OnArc2);
        if(isPnt1OnArc2){
            isExiInt = true;
            return;
        }
    }
    if(intRes._intNum == 2){
        pton.IsPointOnArc(arc1,intRes._intPnt2,
                          pre,pre,false,isPnt2OnArc1);
        if(isPnt2OnArc1){
            pton.IsPointOnArc(arc2,intRes._intPnt2,
                              pre,pre,false,isPnt2OnArc2);
            if(isPnt2OnArc2)
                isExiInt = true;
        }
    }
}
void CheckGeomInt::IsElementElementIntersect(const DefElem& ele1,
                                             const DefElem& ele2,
                                             double pre,                                             bool& isExiInt)
{
    if(ele1._elemType == LINETYPE){
        if(ele2._elemType == LINETYPE){
            IsSegmentSegmentIntersect(ele1._segment._segSp,
                                      ele1._segment._segEp,
                                      ele2._segment._segSp,
                                      ele2._segment._segEp,
                                      pre,isExiInt);
        }
        if(ele2._elemType == ARCTYPE){
            IsSegmentArcIntersect(ele1._segment._segSp,
                                  ele1._segment._segEp,
                                  ele2._arc,pre,isExiInt);
        }
        else if(ele2._elemType == CIRCLETYPE){
            IsSegmentCircleIntersect(ele1._segment._segSp,
                                     ele1._segment._segEp,
                                     ele2._circle._cirCp,
                                     ele2._circle._cirR,
                                     pre,isExiInt);
        }
    }
    else if(ele1._elemType == ARCTYPE){
        if(ele2._elemType == LINETYPE){
            IsSegmentArcIntersect(ele2._segment._segSp,
                                  ele2._segment._segEp,
                                  ele1._arc,pre,isExiInt);
        }
        if(ele2._elemType == ARCTYPE){
            IsArcArcIntersect(ele1._arc,ele2._arc,
                              pre,isExiInt);
        }
        else if(ele2._elemType == CIRCLETYPE){
            IsArcCircleIntersect(ele1._arc,ele2._circle._cirCp,
                                 ele2._circle._cirR,pre,isExiInt);
        }
    }
    else if(ele1._elemType == CIRCLETYPE){
        if(ele2._elemType == LINETYPE){
            IsSegmentCircleIntersect(ele2._segment._segSp,
                                     ele2._segment._segEp,
                                     ele1._circle._cirCp,
                                     ele1._circle._cirR,
                                     pre,isExiInt);
        }
        else if(ele2._elemType == ARCTYPE){
            IsArcCircleIntersect(ele2._arc,ele1._circle._cirCp,
                                 ele1._circle._cirR,pre,isExiInt);
        }
        else if(ele2._elemType == CIRCLETYPE){
            IsCircleCircleIntersect(ele1._circle._cirCp,
                                    ele1._circle._cirR,
                                    ele2._circle._cirCp,
                                    ele2._circle._cirR,
                                    pre,isExiInt);
        }
    }
}bool CheckGeomInt::IsElementElementIntersect(const DefElem& ele1,
                                             const DefElem& ele2,
                                             double pre)
{
    bool isExiInt = false;
    IsElementElementIntersect(ele1,ele2,pre,isExiInt);
    return isExiInt;
}
void CheckGeomInt::IsElementSpecifyIntersect(const DefElem& ele1,
                                             const DefElem& ele2,
                                             double coinPre,
                                             double tanPre,
                                             double intPre,
                                             bool& isIntersect)
{
    isIntersect = false;
    if(TupleQuality().IsElementsCoincide(ele1,ele2,false,coinPre))
        return;
    GeomTangency tan;ElemCalculate num;GeomIntersect goInt;
    ///判断当前元素是否与环相交
    GeomIntRes intRes;
    goInt.CalculateCurveIntersect(ele1,ele2,false,tanPre,
                                    intPre,intPre,intRes);
    ///这里需要判断是否是有效相交 ,即相交元素不能是两平行线段 ,也不能是相切相交,
    if(!num.IsEleExistValInt(ele1,ele2,intRes))
        return;
    if(intRes._intNum == 1 &&
            tan.IsEleEleTangency(ele1,ele2,false,tanPre))
        return;
    ///相交点也不能是两相交元素的端点 .
    bool isEleExtr1 = false,isEleExtr2 = false,isSp;
    num.PointIsElemEndpoint(ele1,intRes._intPnt1,intPre,isEleExtr1,isSp);
    num.PointIsElemEndpoint(ele2,intRes._intPnt1,intPre,isEleExtr2,isSp);
    if(!isEleExtr1 && !isEleExtr2){
        isIntersect = true;
        return;
    }
    if(intRes._intNum == 2){
        bool isEleExtr1_ = false,isEleExtr2_ = false;
        num.PointIsElemEndpoint(ele1,intRes._intPnt2,intPre,
                                isEleExtr1_,isSp);
        num.PointIsElemEndpoint(ele2,intRes._intPnt2,intPre,
                                isEleExtr2_,isSp);
        if(!isEleExtr1 && !isEleExtr2){
            isIntersect = true;
            return;
        }
    }
}
void CheckGeomInt::IsLoopElemSpecifyIntersect(const DefLoop& loop,
                                              const DefElem& ele,
                                              double coinPre,
                                              double tanPre,
                                              double intPre,
                                              bool& isIntersect)
{
#if 1
    for (int i = 0; i < loop.ElemNum(); ++i) {
        IsElementSpecifyIntersect(ele,loop.IndexElem(i),coinPre,
                                  tanPre,intPre,isIntersect);        if(isIntersect)
            return;
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop.ElemNum()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            IsElementSpecifyIntersect(ele,loop.IndexElem(i),coinPre,
                                      tanPre,intPre,isIntersect);
            if(isIntersect)
                return;
        }
    });
#endif
}
void CheckGeomInt::IsLoopLoopIntersect(const DefLoop& loop1,
                                       const DefLoop& loop2,
                                       double pre,
                                       bool& isExiInt)
{
    isExiInt = false;
    for (int i = 0; i < loop1.ElemNum(); ++i) {
#if 1
        for (int j = 0; j < loop2.ElemNum(); ++j) {
            isExiInt = IsElementElementIntersect(loop1.IndexElem(i),
                                                 loop2.IndexElem(j),pre);
            if(isExiInt)
                return;
        }
#else
        tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop2.ElemNum()),
                          [&](tbb::blocked_range<std::size_t>& r) {
            for (std::size_t j = r.begin(); j < r.end(); ++j) {
                isExiInt = IsElementElementIntersect(loop1.IndexElem(i),
                                                     loop2.IndexElem(j),pre);
                if(isExiInt)
                    return;
            }
        });
#endif
    }
}
void CheckGeomInt::IsLoopSpecifyIntersect(const DefLoop& loop1,
                                          const DefLoop& loop2,
                                          double coinPre,
                                          double tanPre,
                                          double intPre,
                                          bool& isSpecInt)
{
    isSpecInt = false;
    for (int i = 0; i < loop1.ElemNum(); ++i) {
#if 1
        for (int j = 0; j < loop2.ElemNum(); ++j) {
            IsElementSpecifyIntersect(loop1.IndexElem(i),
                                      loop2.IndexElem(j),
                                      coinPre,tanPre,
                                      intPre,isSpecInt);
            if(isSpecInt)
                return;
        }
#else        tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop2.ElemNum()),
                          [&](tbb::blocked_range<std::size_t>& r) {
            for (std::size_t j = r.begin(); j < r.end(); ++j) {
                IsElementSpecifyIntersect(loop1.IndexElem(i),
                                          loop2.IndexElem(j),
                                          coinPre,tanPre,
                                          intPre,isSpecInt);
                if(isSpecInt)
                    return;
            }
        });
#endif
    }
}
void CheckGeomInt::IsLoopArrayLoopIntersect(const vector<DefLoop>& vLoop,
                                            const DefLoop& loop,
                                            double pre,
                                            bool& isExiInt)
{
#if 1
    for (int i = 0; i < vLoop.size(); ++i) {
        IsLoopLoopIntersect(loop,vLoop[i],pre,isExiInt);
        if(isExiInt)
            return;
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,vLoop.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (int i = 0; i < vLoop.size(); ++i) {
            IsLoopLoopIntersect(loop,vLoop[i],pre,isExiInt);
            if(isExiInt)
                return;
        }
    });
#endif
}