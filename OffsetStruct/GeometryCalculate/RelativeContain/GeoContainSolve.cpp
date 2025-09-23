#include "GeoContainSolve.h"
#if DebugModel
#pragma optimize("", off)
#endif
void GeoContainSolve::IsPointOnLoopBorder(const DefLoop& loop,
                                          const Point& P,
                                          double pre,
                                          bool &isOnBorder)
{
    isOnBorder = false;/**
    if(!LimitCoordinate().IsPointInLoopCoord(loop,P,pre * 10))
        return;*/
    PointOnElement pton;
    for (int i = 0; i < loop.Size(); ++i) {
        if(pton.IsPointOnElement(loop[i],P,pre,pre,true,true)){
            isOnBorder = true;
            return;
        }
    }
}
void GeoContainSolve::IsPointInsideCircle(const Point& cp,double R,
                                          const Point& P,double pre,
                                          bool& inInside)
{
    double distance;
    GeomCalculate().TwoPointDistance(cp,P,distance);
    inInside = distance <= (R + pre);
}
void GeoContainSolve::IsCircleContainCircle(const Point& outCp,double outRadius,
                                            const Point& cp,double R,double pre,
                                            bool& isContain)
{
    double distance;
    GeomCalculate().TwoPointDistance(outCp,cp,distance);
    isContain = R + distance - pre <= outRadius;
}
void GeoContainSolve::IsCircleContainSeg(const Point& cp, double R,
                                         const Point& lSp,const Point& lEp,
                                         double pre,bool& isContain)
{
    isContain = false;
    IsPointInsideCircle(cp,R,lSp,pre,isContain);
    if(!isContain)
        return;
    IsPointInsideCircle(cp,R,lEp,pre,isContain);
    if(!isContain)
        return;
    isContain = true;
}
void GeoContainSolve::IsCircleContainArc(const Point& cp,double R,
                                         const DefArc& arc,double pre,
                                         bool& isContain)
{
    isContain = false;
    IsPointInsideCircle(cp,R,arc._arcSp,pre,isContain);
    if(!isContain)
        return;    IsPointInsideCircle(cp,R,arc._arcEp,pre,isContain);
    if(!isContain)
        return;
    Point midPnt;
    GeomCalculate().ArcMidpoint(arc,midPnt);
    IsPointInsideCircle(cp,R,midPnt,pre,isContain);
    if(!isContain)
        return;
    bool isExiInt = false;
    CheckGeomInt chInt;
    chInt.IsArcCircleIntersect(arc,cp,R,PreErr_10,isExiInt);
    if(isExiInt)
        return;
    isContain = true;
}
void GeoContainSolve::IsCircleContainElem(const Point& cp,double R,
                                          const DefElem& ele,
                                          double pre,
                                          bool& isContain)
{
    if(ele._elemType == LINETYPE){
        IsCircleContainSeg(cp,R,ele._segment._segSp,
                           ele._segment._segEp,pre,isContain);
    }
    if(ele._elemType == ARCTYPE){
        IsCircleContainArc(cp,R,ele._arc,
                           pre,isContain);
    }
    if(ele._elemType == CIRCLETYPE){
        IsCircleContainCircle(cp,R,ele._circle._cirCp,
                              ele._circle._cirR,pre,isContain);
    }
}
void GeoContainSolve::IsCircleContainLoop(const Point& cp,double R,
                                          const DefLoop& loop,
                                          double pre,
                                          bool& isContain)
{
    isContain = false;
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        bool isContain_ = false;
        IsCircleContainElem(cp,R,loop.m_vElem[i],
                            pre,isContain_);
        if(!isContain_)
            return;
    }
    isContain = true;
}
void GeoContainSolve::IsLoopContainCircle(const DefLoop& loop,
                                          const Point& cp,double R,
                                          double pre,
                                          bool& isContain)
{
    isContain = false;
    GeomDistance godis;
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        double distance;
        godis.PointToElementDistance(cp,loop.m_vElem[i],
                                     distance);        if((R - pre) > distance)
            return;
    }
    isContain = true;
}
void GeoContainSolve::IsLoopContainElemEndpoint(const DefLoop& loop,
                                                const DefElem& ele,
                                                double pre,
                                                bool isMatchBord,
                                                bool& matchResult)
{
    matchResult = false;
    ElemCalculate numcal;
    LimitCoordinate limcoo;
    Point eleSp,eleEp;
    numcal.ElementEndpoint(ele,false,eleSp);
    numcal.ElementEndpoint(ele,true,eleEp);
    bool isSpInc = false,isEpInc = false;
    limcoo.IsPointInsideLoopCoord(loop,eleSp,pre,isSpInc);
    limcoo.IsPointInsideLoopCoord(loop,eleEp,pre,isEpInc);
    PointInPolygon inpoly;
    bool isSpOnBorder = false,isEpOnBorder = false;
    if(isSpInc){
        inpoly.IsPointInsideLoop(loop,eleSp,pre,
                                isSpOnBorder,matchResult);
        if(matchResult)
            return;
        if(isMatchBord && isSpOnBorder){
            matchResult = true;
            return;
        }
    }
    if(isEpInc){
        inpoly.IsPointInsideLoop(loop,eleEp,pre,
                                isEpOnBorder,matchResult);
        if(matchResult)
            return;
        if(isMatchBord && isEpOnBorder){
            matchResult = true;
            return;
        }
    }
}
///isMatchBorder 表示是否匹配ele端点在loop边界上.
void GeoContainSolve::IsLoopContainIntersectElem(const DefLoop& loop,
                                                 const DefElem& ele,
                                                 double incErrVal,
                                                 double coinErrVal,
                                                 double intPre,
                                                 bool isMatchBord,
                                                 bool& resValue)
{
    resValue = false;
    CheckGeomInt chint;
    GeometryLogic golg;
    bool isCirLoop1 = false;
    golg.IsCircleStructureLoop(loop,false,isCirLoop1);    if(isCirLoop1 || ele._elemType == CIRCLETYPE){
        IsLoopContainCurve(loop,ele,intPre,incErrVal,resValue);
        if(resValue)
            return;
        chint.IsLoopElemSpecifyIntersect(loop,ele,coinErrVal,
                                         coinErrVal,intPre,resValue);
        return;
    }
    LimitCoordinate limcoo;
    bool isOutCoord = false;
    limcoo.IsElemOutsideLoopCoord(loop,ele,incErrVal,isOutCoord);
    if(isOutCoord)
        return;
    bool isCoincide = false;
    LoopContainElementSolve(loop,ele,incErrVal,isCoincide,resValue);
    if(isCoincide || resValue){
        if(resValue)
            return;
        if(isMatchBord){
            resValue = true;
            return;
        }
    }
    chint.IsLoopElemSpecifyIntersect(loop,ele,coinErrVal,
                                     coinErrVal,intPre,resValue);
    if(resValue)
        return;
    if(isMatchBord){
        IsLoopContainElemEndpoint(loop,ele,coinErrVal,
                                  isMatchBord,resValue);
    }
}
void GeoContainSolve::IsLoopContainCurve(const DefLoop& loop,
                                           const DefElem& ele,
                                           double intPre,
                                           double incPre,
                                           bool& isInclude)
{
    isInclude = false;
    LimitCoordinate limcoo;
    bool isIncCoor = false,isExiSpecInt = false;
    limcoo.IsEleInsideLoopCoord(loop,ele,incPre,isIncCoor);
    if(!isIncCoor)
        return;
    CheckGeomInt chInt;
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        chInt.IsElementSpecifyIntersect(ele,loop.m_vElem[i],intPre,
                                        intPre,intPre,isExiSpecInt);
        if(isExiSpecInt)
            return;
    }
    bool isCoincide = false;
    LoopContainElementSolve(loop,ele,intPre,isCoincide,isInclude);
    if(!isInclude && isCoincide){
        isInclude = true;
    }
}
void GeoContainSolve::LoopContainElementSolve(const DefLoop& loop,
                                              const DefElem& ele,
                                              double errVal,                                              bool& isCoincide,
                                              bool& isInclude)
{
    isCoincide = false;
    Point midPnt;
    ElemCalculate elecal;/**
    if(ele.ElemeType() == ARCTYPE &&
            ele.ElemArc().ArcStartPt().
            IsSamePoint2D(Point(0,32.5),0.0001)){
        int tem = 1;
    }*/
    elecal.ElementMidpoint(ele,midPnt);
    PointInPolygon inpoly;
    bool isOnBorder = false;
    inpoly.IsPointInsideLoop(loop,midPnt,errVal,
                            isOnBorder,isInclude);
    if(isInclude || !isOnBorder)
        return;
    Point sp,ep;
    bool isOnBorder_ = false;
    elecal.ElementEndpoint(ele,false,sp);
    inpoly.IsPointInsideLoop(loop,sp,errVal,
                            isOnBorder_,isInclude);
    if(isInclude || !isOnBorder_)
        return;
    bool isOnBorder__ = false;
    elecal.ElementEndpoint(ele,false,ep);
    inpoly.IsPointInsideLoop(loop,sp,errVal,
                            isOnBorder__,isInclude);
    if(isInclude || !isOnBorder__)
        return;
    if(isOnBorder && isOnBorder_ && isOnBorder__){
        Point mp_;
        DefElem ele_ = ele;
        elecal.SetElementEndpoint(midPnt,true,ele_);
        elecal.ElementMidpoint(ele_,mp_);
        bool isOnBorder___ = false;
        inpoly.IsPointInsideLoop(loop,mp_,errVal,
                                isOnBorder___,isInclude);
        if(isInclude || !isOnBorder___)
            return;
        isCoincide = true;
    }
}