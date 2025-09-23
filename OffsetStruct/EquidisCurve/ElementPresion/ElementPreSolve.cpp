#include "ElementPreSolve.h"
#if DebugModel
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
void ElementPreSolve::CheckLoopArcRadius(DefLoop& loop)
{
    for(auto& it : loop.m_vElem){
        if(it._elemType == ARCTYPE)
            it._arc.ChoiceSetRadius(PreErr5_3);
    }
}
void ElementPreSolve::HandleAdjacentParallSeg(DefLoop& loop)
{
    GeomCalculate gocal;ElemCalculate elecal;
    int i = 0;
    while (i < int(loop.ElemNum())) {
        int nex = elecal.NextIndex(loop.ElemNum(),i);
        if(loop.m_vElem[i]._elemType != LINETYPE ||
                loop.m_vElem[nex]._elemType != LINETYPE){
            i ++;
            continue;
        }
        double slop1,slop2;
        slop1 = gocal.SegmentSlope(loop.m_vElem[i]._segment._segSp,
                                   loop.m_vElem[i]._segment._segEp);
        slop2 = gocal.SegmentSlope(loop.m_vElem[nex]._segment._segSp,
                                   loop.m_vElem[nex]._segment._segEp);
        if(std::abs(slop1 - slop2) < Pre_ParallSeg_Err){
            loop.m_vElem[i]._segment._segEp =
                    loop.m_vElem[nex]._segment._segEp;
            loop.m_vElem.erase(loop.m_vElem.begin() + nex);
            continue;
        }
        i ++;
    }
}
void ElementPreSolve::HandleAdjacentParallArc(DefLoop& loop)
{
    if(loop.ElemNum() < 2)
        return;
    int i = 0;
    GeomIntSolve intsol;GeomCalculate gocal;ElemCalculate elecal;
    while (i < int(loop.ElemNum())) {
        if(!elecal.IsLoopExistNexEle(loop,i)){
            i ++;
            continue;
        }
        int nex = elecal.NextIndex(loop.ElemNum(),i);
        if(loop.m_vElem[i]._elemType != ARCTYPE ||
                loop.m_vElem[nex]._elemType != ARCTYPE){
            i ++;
            continue;
        }
        intsol.JudgeArcRadius(loop.m_vElem[i]._arc);
        intsol.JudgeArcRadius(loop.m_vElem[nex]._arc);
        if(std::abs(loop.m_vElem[i]._arc._arcR -
                    loop.m_vElem[nex]._arc._arcR) >=
                Pre_SameCirR_Err){            i ++;
            continue;
        }
        bool isSamePnt = false;
        Point cp1 = loop.m_vElem[i]._arc._arcCp;
        Point cp2 = loop.m_vElem[nex]._arc._arcCp;
        gocal.IsIdenticalPoint(cp1,cp2,Pre_SameCirPt_Err,isSamePnt);
        if(isSamePnt){
            Point arcSp = loop.m_vElem[i]._arc._arcSp;
            Point arcEp = loop.m_vElem[nex]._arc._arcEp;
            double radius = loop.m_vElem[i]._arc._arcR;
            RotDir rot = loop.m_vElem[i]._arc._arcRot;
            DefArc arc (arcSp,arcEp,cp1,radius,rot);
            loop.m_vElem[i]._arc = arc;
            loop.m_vElem.erase(loop.m_vElem.begin() + nex);
            continue;
        }
        i ++;
    }
}
void ElementPreSolve::CircleStructureArcLoop(DefLoop& loop)
{
    GeomCalculate gocal;ElemCalculate elecal;
    for (int i = 0; i < loop.Size(); ++i) {
        int nex = elecal.NextIndex(loop.ElemNum(),i);
        if(loop.IndexElem(i).ElemeType() != ARCTYPE ||
                loop.IndexElem(nex).ElemeType() != ARCTYPE)
            return;
        if(loop.m_vElem[i]._arc._arcRot !=
                loop.m_vElem[nex]._arc._arcRot)
            return;
        Point cp1 = loop.m_vElem[i]._arc._arcCp;
        Point cp2 = loop.m_vElem[nex]._arc._arcCp;
        bool isSamePnt = false;
        gocal.IsIdenticalPoint(cp2,cp1,Pre_SameCirPt_Err,isSamePnt);
        if(!isSamePnt)
            return;
    }
    ///当所有圆弧圆心坐标均相同时 ,所有圆弧转换为一个圆 .
    int last = loop.ElemNum() - 1;
    Point sp_ = loop.m_vElem[0]._arc._arcSp;
    Point ep_ = loop.m_vElem[last]._arc._arcEp;
    bool isSame = false;
    gocal.IsIdenticalPoint(sp_,ep_,PreErr_1,isSame);
    if(!isSame)
        return;
    Point sp = loop.m_vElem[0]._arc._arcSp;
    Point ep = loop.m_vElem[0]._arc._arcEp;
    Point cp = loop.m_vElem[0]._arc._arcCp;
    double R1 = sqrt(pow(sp._x - cp._x, 2) + pow(sp._y - cp._y, 2));
    double R2 = sqrt(pow(ep._x - cp._x, 2) + pow(ep._y - cp._y, 2));
    double R = (R1 + R2) / 2;
    DefCircle circle(cp,R);
    loop.ClearElem();
    loop.PushBackElem(DefElem(CIRCLETYPE, circle));
}
void ElementPreSolve::PreviousElemEndpoint(const DefLoop &loop,
                                           int i, Point &P)
{    ElemCalculate elecal;
    if(loop.ElemNum() < 2)
        return;
    if(!elecal.IsLoopExistPreEle(loop,i)){
        elecal.ElementEndpoint(loop.m_vElem[0],false,P);
        return;
    }
    const auto ele = loop.PreviousElem(i);
    P = ele.EndPt();
}
void ElementPreSolve::ResetArcCenter(DefArc &arc)
{
    GeomIntersect goInt;
    arc.ChoiceSetRadius(PreErr5_3);
    double R = arc._arcR;
    const auto& sp = arc._arcSp,ep = arc._arcEp,cp_ = arc._arcCp;
    double A1,B1,C1,A2,B2,C2;
    GeomCalculate().SegmentVerticalBisector(sp,ep,A1,B1,C1);
    if(sp.DistSquare2D(cp_) > ep.DistSquare2D(cp_)){
        GeomCalculate().LineParameters(sp,cp_,A2,B2,C2);
    }else{
        GeomCalculate().LineParameters(ep,cp_,A2,B2,C2);
    }
    bool isExiInt = false;
    goInt.CalculateLineParameterInt(A1,B1,C1,A2,B2,C2,isExiInt,arc._arcCp);
    if(isExiInt){
        arc.IniArcRadius();
        return;
    }
    GeomIntRes intRes;
    goInt.CalculateCircleCircleInt(sp,R,ep,R,PreErr_14,
                                   PreErr_14,intRes);
    if(!intRes._isExistInt){
        double dis = sp.Distance2D(ep);
        double dis_ = dis / 2;
        goInt.CalculateCircleCircleInt(sp,dis_,ep,dis_,PreErr_14,
                                       PreErr_14,intRes);
    }
    if(!intRes.IsInt()){
        arc.IniArcRadius();
        return;
    }
    if(intRes.Num() == 1){
        arc.SetArcCenterPt(intRes.P1());
        arc.IniArcRadius();
        return;
    }
    double dis1= intRes.P1().DistSquare2D(cp_),
            dis2 = intRes.P2().DistSquare2D(cp_);
    if(dis1 <= dis2){
        arc.SetArcCenterPt(intRes.P1());
    }else{
        arc.SetArcCenterPt(intRes.P2());
    }
    arc.IniArcRadius();
}
void ElementPreSolve::ResetArcEndpoint(bool isLast,DefArc& arc)
{
    if(isLast){
        ResetArcCenter(arc);
        return;    }
    const auto& sp = arc.ArcStartPt();
    const auto& ep = arc.ArcEndPt();
    const auto& cp = arc.ArcCenterPt();
    double R1 = sp.Distance2D(cp),R2 = ep.Distance2D(cp);
    if(std::abs(R1 - R2) < PreErr_16){
        arc._arcR = (R1 + R2) / 2;
        return;
    }
    ///GeomCalculate().ArcEndpointAngle(sp,ep,cp,rot,startAgl,endAgl);
    double endAgl = std::atan2((ep.Y() - cp.Y()),(ep.X() - cp.X()));/**
    endAgl *= _AlgRad;
    if (endAgl < 0){
        endAgl += 360;
    }*/
    double incx = R1 * std::cos(endAgl/** _RadAlg*/);
    double incy = R1 * std::sin(endAgl/** _RadAlg*/);
    Point newEp(cp._x + incx,cp._y + incy);
    R2 = newEp.Distance2D(cp);
    arc._arcEp = newEp;
    arc._arcR = (R1 + R2) / 2;
}
void ElementPreSolve::SetLineHorVerQuality(Point& lSp,Point& lEp,
                                           bool isChEp,double pre)
{
    TupleQuality tupqua;
    if(tupqua.IsVerticalLine(lSp,lEp,pre)){
        if(isChEp)
            lEp._x = lSp.X();
        else
            lSp._x = lEp.X();
    }
    if(tupqua.IsHorizontalLine(lSp,lEp,pre)){
        if(isChEp)
            lEp._y = lSp.Y();
        else
            lSp._y = lEp.Y();
    }
}