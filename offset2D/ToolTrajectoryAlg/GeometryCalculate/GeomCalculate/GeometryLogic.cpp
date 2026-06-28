#include "GeometryLogic.h"
#include"../../GeometryCalculate/RelativeContain/LoopContain.h"
shared_ptr<GeomCalculate>golg_go = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate>golg_num = make_shared<ElemCalculate>();
shared_ptr<ElementIntersect>golg_eleInt = make_shared<ElementIntersect>();
shared_ptr<GeomIntersect>golg_goIn = make_shared<GeomIntersect>();
#if DebugModel
#pragma optimize("", off)
#endif
void GeometryLogic::IsLineCircleTan(const Point& lSp,
                                    const Point& lEp,
                                    const Point& CP,
                                    double R,
                                    double pre,
                                    bool& isTan)
{
    isTan = false;
    double dis;
    golg_go->PointToLineDistance(lSp,lEp,CP,dis);
    if(std::abs(dis - R) <= pre){
        isTan = true;
    }
}
void GeometryLogic::IsCircleCircleTangency(const Point& cp1,
                                           double R1,
                                           const Point& cp2,
                                           double R2,
                                           double pre,
                                           bool& isTan)
{
    isTan = false;
    double maxR = (R1 >= R2) ? R1 : R2;
    double minR = (R1 < R2) ? R1 : R2;
    double cirDis ;
    golg_go->TwoPointDistance(cp1,cp2,cirDis);
    if(std::abs(cirDis - (R1 + R2)) <= pre ||
            std::abs(maxR - minR - cirDis) <= pre){
        isTan = true;
    }
}
bool GeometryLogic::IsElementTangency(const DefElem& ele1,
                                      const DefElem& ele2,
                                      double pre)
{
    if(ele1._type == LINETYPE && ele2._type == LINETYPE){
        return false;
    }
    bool isTan = false;
    if(ele1._type == LINETYPE && ele2._type == ARCTYPE){
        IsLineCircleTan(ele1._segment._segSp,ele1._segment._segEp,
                        ele2._arc._arcCp,
                        ele2._arc._arcR,
                        pre,isTan);
    }
    else if(ele1._type == ARCTYPE && ele2._type == ARCTYPE){
        IsCircleCircleTangency(ele1._arc._arcCp,
                               ele1._arc._arcR,
                               ele2._arc._arcCp,
                               ele2._arc._arcR,
                               pre,isTan);
    }
    return isTan;
}
void GeometryLogic::IsTwoArcEndPntSame(const DefArc& arc1,
                                       const DefArc& arc2,
                                       double endptPre,
                                       GeomIntRes& intRes)
{
    Point sPt1 = arc1._arcSp,ePt1 = arc1._arcEp,
            sPt2 = arc2._arcSp,ePt2 = arc2._arcEp,
            samePt1,samePt2;
    bool isSamePt1 = false,isSamePt2 = false;
    golg_go->IsIdenticalPoint(sPt1,sPt2,endptPre,isSamePt1);
    if(isSamePt1){
        samePt1 = sPt1;
    }
    else{
        golg_go->IsIdenticalPoint(sPt1,ePt2,endptPre,
                                  isSamePt1);
        if(isSamePt1){
            samePt1 = sPt1;
        }
    }
    golg_go->IsIdenticalPoint(ePt1,sPt2,endptPre,isSamePt2);
    if(isSamePt2){
        samePt2 = ePt1;
    }
    else{
        golg_go->IsIdenticalPoint(ePt1,ePt2,endptPre,
                                  isSamePt2);
        if(isSamePt2){
            samePt2 = ePt1;
        }
    }
    if(!isSamePt1 && !isSamePt2){
        return;
    }
    if(isSamePt1 && !isSamePt2){
        intRes._intNum = 1;
        intRes._intPnt1 = samePt1;
        intRes._intPnt2 = samePt1;
    }
    else if(!isSamePt1 && isSamePt2){
        intRes._intNum = 1;
        intRes._intPnt1 = samePt2;
        intRes._intPnt2 = samePt2;
    }
    else {
        intRes._intNum = 2;
        intRes._intPnt1 = samePt1;
        intRes._intPnt2 = samePt2;
    }
    intRes._isExistInt = true;
}
void GeometryLogic::ClosetPointFromTwoPoint(const Point& sp,
                                            const Point& p1,
                                            const Point& p2,
                                            Point& P)
{
    double p1Dis,p2Dis;
    golg_go->TwoPointDistance(sp,p1,p1Dis);
    golg_go->TwoPointDistance(sp,p2,p2Dis);
    P = p1Dis <= p2Dis ? p1 : p2;
}
void GeometryLogic::DistancePointFromPoint(const Point& sp,
                                           const Point& p1,
                                           const Point& p2,
                                           bool isClose,
                                           Point& P)
{
    GeomCalculate gocal;
    double dis1,dis2;
    gocal.TwoPointDistance(sp,p1,dis1);
    gocal.TwoPointDistance(sp,p2,dis2);
    if(isClose){
        P = dis1 <= dis2 ? p1 : p2;
    }
    else{
        P = dis1 >= dis2 ? p1 : p2;
    }
}
#pragma optimize("", off)
void GeometryLogic::SameSegDircetionPoint(const Point& lSp,
                                          const Point& lEp,
                                          const Point& P1,
                                          const Point& P2,
                                          Point& P)
{
    GeomCalculate gocal;
    double angle,angle1,angle2;
    gocal.SegmentAngle(lSp,lEp,angle);
    gocal.SegmentAngle(P1,lSp,angle1);
    gocal.SegmentAngle(P2,lSp,angle2);
    P = std::abs(angle1 - angle) <=
            std::abs(angle2 - angle) ? P1 : P2;
}
void GeometryLogic::AnotherFromTwoPoint(const Point& P1,
                                        const Point& P2,
                                        const Point& comPnt,
                                        Point& P)
{
    double dis1,dis2;
    golg_go->TwoPointDistance(P1,comPnt,dis1);
    golg_go->TwoPointDistance(P2,comPnt,dis2);
    P = dis1 < dis2 ? P2 : P1;
}
void GeometryLogic::CloseArcEndpointBaseAngle(DefArc arc,
                                              bool isCloseEp,
                                              double stepAgl,
                                              Point& P)
{
    GeomCalculate gocal;
    double startAgl,endAgl;
    gocal.ArcEndpointAngle(arc._arcSp,arc._arcEp,arc._arcCp,
                           arc._isCW,startAgl,endAgl);
    double centerAgl;
    gocal.TwoAngleIncludedAngle(startAgl,endAgl,arc._isCW,centerAgl);
    double stepVal_ = stepAgl;
    if(stepVal_ >= centerAgl){
        if(isCloseEp){P = arc.GetSp();
        }else{P = arc.GetEp();}
        return;
    }
#if 1
    if(isCloseEp){
        arc.Reverse();
    }
    gocal.PointRotate2D(arc._arcSp,arc._arcCp,stepVal_,
                        arc._isCW,P);
#else
    double angle;
    gocal.CloseArcEndpointAngle(arc,stepVal_,isCloseEp,angle);
    arc.OptInitR(PreErr5_2);
    double incx = arc.R() * std::cos(angle * _RadAlg);
    double incy = arc.R() * std::sin(angle * _RadAlg);
    P.SetX(arc._arcCp.X() + incx);
    P.SetY(arc._arcCp.Y() + incy);
#endif
    P.SetZ(arc.GetCp().Z());
}
void GeometryLogic::CloseArcEndpointBaseLenth(DefArc arc,
                                              bool isCloseEp,
                                              double stepVal,
                                              Point &P)
{
    arc.OptInitR(PreErr5_2);
    double stepAgl = (360 * stepVal) / (2 * M_PI * arc.R());
    CloseArcEndpointBaseAngle(arc,isCloseEp,stepAgl,P);
}
void GeometryLogic::ApproachCurveEndpoint(const DefElem& ele,
                                          double stepVal,
                                          bool isAngle,
                                          bool isNearSp,
                                          Point& P)
{
    GeomCalculate go;
    switch (ele.Type()) {
    case LINETYPE:
        go.CloseSegmentEndpoint(ele.GetSeg().GetSp(),
                                ele.GetSeg().GetEp(),
                                stepVal,isNearSp,P);
        break;
    case ARCTYPE:
        if(isAngle){
            CloseArcEndpointBaseAngle(ele.GetArc(),!isNearSp,stepVal,P);
        }else{
            CloseArcEndpointBaseLenth(ele.GetArc(),!isNearSp,stepVal,P);
        }
        break;
    default:
        break;
    }

}
void GeometryLogic::GetObjFromTwoPnt(const DefElem &ele,
                                     int pntNum,
                                     const Point& P1,
                                     const Point& P2,
                                     bool isCloseSp,
                                     Point& P)
{
    if(pntNum == 1)
        P = P1;
    else
        golg_eleInt->GetCloseEleExtremePnt(ele,P1,P2,
                                           isCloseSp,P);
}
void GeometryLogic::IsCircleStructureLoop(const DefLoop& loop,
                                          bool isMatchArc,
                                          bool& isCircle)
{
    isCircle = (loop.m_vElem.size() == 1 &&
                loop.m_vElem[0]._type == CIRCLETYPE);
    if(isCircle && !isMatchArc)
        return;
    if(loop.m_vElem.size() != 2 ||
            loop.m_vElem[0]._type != ARCTYPE ||
            loop.m_vElem[1]._type != ARCTYPE)
        return;
    if(!golg_go->IsIdenticalPoint(loop.m_vElem[0]._arc._arcCp,
                                  loop.m_vElem[1]._arc._arcCp,
                                  PreErr5_10))
        return;
    if(std::abs(loop.m_vElem[0]._arc._arcR -
                loop.m_vElem[1]._arc._arcR) >
            PreErr5_10)
        return;
    if(loop.m_vElem[0]._arc._isCW != loop.m_vElem[1]._arc._isCW)
        return;
    isCircle = true;
}
bool GeometryLogic::IsCircleStructureLoop(const DefLoop& loop,
                                          double rErr, double cpErr)
{
    if(loop.m_vElem.size() == 1 &&
            loop.m_vElem[0]._type == CIRCLETYPE)
        return true;
    GeomCalculate gocal;
    MathCalculate nummat;
    if(loop.Size() == 2 &&
            loop.m_vElem[0]._type == ARCTYPE &&
            loop.m_vElem[1]._type == ARCTYPE &&
            gocal.IsIdenticalPoint(loop.m_vElem[0]._arc.GetCp(),
                                   loop.m_vElem[1]._arc.GetCp(),
                                   cpErr) &&
            nummat.IsNumericEqual(loop.m_vElem[0]._arc.R(),
                                  loop.m_vElem[1]._arc.R(),
                                  rErr)){
        return true;
    }
    return false;
}
///仅用在loop 由圆组成的情况.
void GeometryLogic::CircleStructureCircle(const DefLoop& loop,
                                          DefCircle& circle)
{
    if(loop.IsEmpty())
        return;
    if(loop.Size() == 1){
        circle = loop.IndexElem(0).GetCircle();
        return;
    }
    DefArc arc1 = loop.IndexElem(0).GetArc();
    DefArc arc2 = loop.IndexElem(1).GetArc();
    arc1.OptInitR(PreErr5_3);
    arc2.OptInitR(PreErr5_3);
    Point cp1 = arc1.GetCp(),
            cp2 = arc2.GetCp(),cp;
    GeomCalculate().TwoPointMidpoint(cp1,cp2,cp);
    double R1 = arc1.R();
    double R2 = arc2.R();
    circle.SetCp(cp);
    circle.SetR((R1 + R2) / 2);
}
void GeometryLogic::CircleArcLoopTransform(bool isTranToArc,
                                           DefLoop &loop)
{
    if(isTranToArc){
        if(loop.m_vElem.size() != 1 ||
                loop.m_vElem[0]._type != CIRCLETYPE)
            return;
        Point cp(loop.m_vElem[0]._circle._cirCp.X(),
                loop.m_vElem[0]._circle._cirCp.Y());
        double R = loop.m_vElem[0]._circle._cirR;
        Point sp(cp.X(),cp.Y() + R);
        Point ep(cp.X(),cp.Y() - R);
        DefArc arc1 (sp,ep,cp,R,true);
        DefArc arc2 (ep,sp,cp,R,true);
        DefLoop loop_ = loop;
        loop_.m_vElem.clear();
        loop_.m_vElem.push_back(DefElem(ARCTYPE, arc1));
        loop_.m_vElem.push_back(DefElem(ARCTYPE, arc2));
        loop_.m_vElem[0]._label._isCirDisc = true;
        loop_.m_vElem[1]._label._isCirDisc = true;
        loop = loop_;
        return;
    }
    GeomCalculate gocal;
    if(loop.m_vElem.size() != 2 ||
            loop.m_vElem[0]._type != ARCTYPE ||
            loop.m_vElem[1]._type != ARCTYPE){
        return;
    }
    if(!gocal.IsIdenticalPoint(loop.m_vElem[0]._arc.GetCp(),
                               loop.m_vElem[1]._arc.GetCp(),
                               PreErr5_10))
        return;
    if(std::abs(loop.m_vElem[0]._arc.R() -
                loop.m_vElem[1]._arc.R()) >
            PreErr5_10)
        return;
    if(loop.m_vElem[0]._arc.IsCW() != loop.m_vElem[1]._arc.IsCW())
        return;
    Point cp((loop.m_vElem[0]._arc.GetCp().X() +
             loop.m_vElem[1]._arc.GetCp().X()) / 2,
            (loop.m_vElem[0]._arc.GetCp().Y() +
            loop.m_vElem[1]._arc.GetCp().Y()) / 2);
    double R = (loop.m_vElem[0]._arc.R() +
            loop.m_vElem[1]._arc.R()) / 2;
    DefLoop loop_ = loop;
    loop_.m_vElem.clear();
    DefCircle circle(cp,R);
    loop_.m_vElem.push_back(DefElem(CIRCLETYPE, circle));
    loop = loop_;
}
void GeometryLogic::DeleteInsideLoop(vector<DefLoop>& vLoop,
                                     double intPre,
                                     double pre)
{
    int lo = 0;
    while (lo < vLoop.size()) {
        vector<DefLoop> vLoop_;
        for (int i = 0; i < vLoop.size(); ++i) {
            if(i != lo){
                vLoop_.push_back(vLoop[i]);
            }
        }
        bool isInclude_ = false;
        ///gocont.IsLoopsContainLoop(vLoop_,vLoop[lo],intPre,pre,isInclude_);
        numst::ContParam cpa(intPre,pre,pre);
        isInclude_ = LoopContain::IsLoopsContLoop(vLoop_,vLoop[lo],cpa);
        if(isInclude_){
            vLoop.erase(vLoop.begin() + lo);
            continue;
        }
        lo ++;
    }
}
void GeometryLogic::IsElementLocatedOnLoop(const DefElem& ele,
                                           const DefLoop& loop,
                                           double pre,
                                           bool& matchRes)
{
    matchRes = false;
    TupleQuality tup;
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        if(tup.IsEleLocatedOnEle(loop.m_vElem[i],ele,
                                 pre)){
            matchRes = true;
            return;
        }
    }
}
void GeometryLogic::IsLoopLoopExistCoincideEle(const DefLoop& loop1,
                                               const DefLoop& loop2,
                                               double pre,
                                               bool& matchRes)
{
    for (int i = 0; i < loop1.m_vElem.size(); ++i) {
        IsElementLocatedOnLoop(loop1.m_vElem[i],loop2,
                               pre,matchRes);
        if(matchRes){
            return;
        }
    }
}
void GeometryLogic::DiscreteElemToPoint(DefElem ele,
                                        bool isAngle,
                                        double stepVal,
                                        vector<Point> &points)
{
    GeomCalculate gocal;
    switch (ele.Type()) {
    case LINETYPE:
        gocal.DiscreteLineToPoint(ele.GetSeg().GetSp(),
                                  ele.GetSeg().GetEp(),
                                  stepVal,points);
        break;
    case ARCTYPE:{
        double stepVal_ = stepVal;
        if(!isAngle){
            ele._arc.OptInitR(PreErr5_2);
            stepVal_ = (360 * stepVal) /
                    (2 * M_PI * ele.GetArc().R());
        }
        gocal.DiscreteArcToPoint(ele.GetArc(),stepVal_,points);
        break;
    }
    case CIRCLETYPE:
        gocal.DiscreteCircleToPoint(ele.GetCircle().GetCp(),
                                    ele.GetCircle().R(),
                                    true,stepVal,points);
        break;
    default:
        break;
    }
}
void GeometryLogic::DiscreteCurveToPoint(DefElem ele,
                                         double lStep,double cStep,
                                         bool isAngle,
                                         vector<Point>& points)
{
    if(lStep < PreErr5_5){lStep = PreErr5_3;}
    if(cStep < PreErr5_5){cStep = PreErr5_3;}
    GeomCalculate gocal;
    switch (ele.Type()) {
    case LINETYPE:
        gocal.DiscreteLineToPoint(ele._segment._segSp,
                                  ele._segment._segEp,
                                  lStep,points);
        break;
    case ARCTYPE:{
        double cStep_ = cStep;
        ele._arc.OptInitR(PreErr5_3);
        if(!isAngle){
            cStep_ = MathCalculate().AngleBaseBowHeight(
                        ele._arc._arcR,cStep);/**
            stepVal_ = (360 * stepVal) /
                    (2 * M_PI * ele.GetArc().R());*/
        }
        gocal.DiscreteArcToPoint(ele._arc,cStep_,points);
        break;
    }
    case CIRCLETYPE:
        gocal.DiscreteCircleToPoint(ele._circle._cirCp,
                                    ele._circle._cirR,
                                    true,cStep,points);
        break;
    default:
        break;
    }
}
void GeometryLogic::TranslateLoopIntoPoint(DefLoop loop, double lStep,
                                           double cStep, bool isStepAngle,
                                           vector<Point>& points)
{
    if(lStep < PreErr5_5){
        lStep = PreErr5_3;
    }
    GeomCalculate gocal;MathCalculate matcal;
    if(IsCircleStructureLoop(loop)){
        CircleArcLoopTransform(false,loop);
        if(loop.IsEmpty())
            return;
        double stepVal = cStep;
        DefCircle circle = loop.IndexElem(0)._circle;
        if(!isStepAngle){
            stepVal = matcal.AngleBaseBowHeight(circle._cirR,cStep);
        }
        gocal.DiscreteCircleToPoint(circle._cirCp,circle._cirR,
                                    true,stepVal,points);
        return;
    }
    for(auto& e : loop.m_vElem){
        if(e.Type() == ARCTYPE){
            e._arc.OptInitR(PreErr5_2);
        }
    }
    for (int i = 0; i < loop.Size(); ++i) {
        vector<Point>points;
        if(loop.m_vElem[i]._type == LINETYPE){
            const Point& sp = loop.m_vElem[i]._segment._segSp;
            const Point& ep = loop.m_vElem[i]._segment._segEp;
            gocal.DiscreteLineToPoint(sp,ep,lStep,points);
        }
        else if(loop.m_vElem[i]._type == ARCTYPE){
            double stepVal = cStep;
            const DefArc& arc = loop.m_vElem[i]._arc;
            if(!isStepAngle){
                stepVal = matcal.AngleBaseBowHeight(arc._arcR,cStep);
            }
            gocal.DiscreteArcToPoint(arc,stepVal,points);
        }
        points.pop_back();
        for(auto& d : points){
            points.emplace_back(d);
        }
    }
}
void GeometryLogic::LoopToSegmentLoop(DefLoop loop,
                                      double expStep,
                                      DefLoop& resLoop)
{
    resLoop = loop;
    resLoop.m_vElem.clear();
    vector<Point>vPoint;
    GeomCalculate gocal;ElemCalculate numcal;
    numcal.InitialLoopArcRadius(loop);
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        if(loop.m_vElem[i]._type == LINETYPE){
            vPoint.push_back(loop.m_vElem[i]._segment._segSp);
        }
        else if(loop.m_vElem[i]._type == ARCTYPE){
            vector<Point> vPnt;
            DefArc arc = loop.m_vElem[i]._arc;
            gocal.DiscreteArcToPoint(arc,expStep,vPnt);
            for (int p = 0; p < vPnt.size(); ++p) {
                vPoint.push_back(vPnt[p]);
            }
        }
        else if(loop.m_vElem[i]._type == CIRCLETYPE){
            gocal.DiscreteCircleToPoint(loop.m_vElem[i]._circle._cirCp,
                                        loop.m_vElem[i]._circle._cirR,
                                        true,expStep,vPoint);
        }
    }
    if(vPoint.size() < 2)
        return;
    numcal.DeduplicationPoint(vPoint,PreErr5_4);
    int len = loop.IsOpen() == true ?
                vPoint.size() - 1 : vPoint.size();
    for (int p = 0; p < len; ++p) {
        int nex = numcal.NexId(vPoint.size(),p);
        resLoop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(vPoint[p],
                                                          vPoint[nex])));
    }
}
void GeometryLogic::LoopToSegmentLoop(DefLoop loop,
                                      double stepVal,
                                      bool isStepAngle,
                                      DefLoop& resLoop)
{
    if(stepVal < PreErr5_5){
        stepVal = PreErr5_2;}
    resLoop = loop;
    resLoop.Clear();
    ElemCalculate numcal;
    GeomCalculate gocal;MathCalculate matcal;
    if(IsCircleStructureLoop(loop)){
        CircleArcLoopTransform(false,loop);
        if(loop.IsEmpty())
            return;
        double stepVal_ = stepVal;
        vector<Point>vPoint;
        const DefCircle& circle = loop.IndexElem(0)._circle;
        if(!isStepAngle){
            stepVal_ = matcal.AngleBaseBowHeight(circle.R(),stepVal);
        }
        gocal.DiscreteCircleToPoint(circle._cirCp,circle._cirR,
                                    true,stepVal_,vPoint);
        if(vPoint.size() < 2)
            return;
        for (int p = 0; p < vPoint.size()-1; ++p) {
            resLoop.AddElem(
                        DefElem(LINETYPE,DefSeg(vPoint[p],vPoint[p+1])));
        }
        return;
    }
    for(auto& e : loop.m_vElem){
        if(e.Type() == ARCTYPE){
            e._arc.OptInitR(PreErr5_2);
        }
    }
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        if(loop.m_vElem[i]._type == LINETYPE){
            resLoop.m_vElem.emplace_back(loop.m_vElem[i]);
        }
        else if(loop.m_vElem[i]._type == ARCTYPE){
            vector<Point>vPoint;
            double stepVal_ = stepVal;
            const DefArc& arc = loop.m_vElem[i]._arc;
            if(!isStepAngle){
                stepVal_ = matcal.AngleBaseBowHeight(arc.R(),stepVal);
            }
            gocal.DiscreteArcToPoint(arc,stepVal_,vPoint);
            if(vPoint.size() < 2){
                if(arc._arcSp.IsSamePoint2D(arc._arcEp,PreErr5_12)){
                    resLoop.m_vElem.emplace_back(
                                DefElem(LINETYPE,DefSeg(arc._arcSp,arc._arcEp)));
                }
                continue;
            }
            for (int p = 0; p < vPoint.size()-1; ++p) {
                resLoop.m_vElem.emplace_back(
                            DefElem(LINETYPE,DefSeg(vPoint[p],vPoint[p+1])));
            }
        }
    }
}
void GeometryLogic::TranslateBndIslToSegment(double stepVal,
                                             bool isAngle,
                                             GeomArea& BILoop)
{
    LoopToSegmentLoop(BILoop.BndLoop(),stepVal,
                      isAngle,BILoop._bndLoop);
    Point sp = BILoop._bndLoop.Endpoint(true);
    Point ep = BILoop._bndLoop.Endpoint(false);
    if(!sp.IsSamePoint2D(ep,PreErr5_10)){
        BILoop._bndLoop.AddElem(DefElem(LINETYPE,DefSeg(sp,ep)));
    }
    for(auto& d : BILoop._vIslLoop) {
        if(d.IsEmpty())
            continue;
        DefLoop rloop;
        LoopToSegmentLoop(d,stepVal,isAngle,rloop);
        Point sp_ = rloop.Endpoint(true);
        Point ep_ = rloop.Endpoint(false);
        if(!sp_.IsSamePoint2D(ep_,PreErr5_10)){
            rloop.AddElem(DefElem(LINETYPE,DefSeg(sp_,ep_)));
        }
        d = rloop;
    }
}
void GeometryLogic::ThirdOrderBezierCurve(vector<Point> &vControlPt,
                                          DefLoop& loop)
{
    if(vControlPt.empty())
        return;
    loop.m_vElem.clear();
    double zCoord = vControlPt[0].Z();
    MathCalculate mat;
    auto bezierPt = [&mat,zCoord](const vector<Point>& vPoint,
            double t)->Point{
        Point P;
        double x = 0, y = 0,b;
        int n = vPoint.size() - 1;
        for (int i = 0; i <= n; ++i) {
            b = mat.BezierCoefficient(n,i) * std::pow(1 - t,n - i) * std::pow(t,i);
            x += vPoint[i].X() * b;
            y += vPoint[i].Y() * b;
        }
        P.SetX(x);
        P.SetY(y);
        P.SetZ(zCoord);
        return P;
    };
    vector<Point>vBezierPt;
    for (double t = 0.0; t <= 1.0; t += 0.05) {
        Point P = bezierPt(vControlPt,t);
        vBezierPt.push_back(P);
    }
    int i = 0;
    ElemCalculate numcal;
    while (i < vBezierPt.size() - 1) {
        int nex = numcal.NexId(vBezierPt.size(),i);
        loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(vBezierPt[i],
                                                       vBezierPt[nex])));
        i ++;
    }
}
void GeometryLogic::LoopSpecifyLenthPosition(const DefLoop& loop,double lenth,
                                             int &index,Point& P)
{
    if(loop.IsEmpty() || (loop.Size() == 1 &&
                          loop.IndexElem(1).Type() == CIRCLETYPE))
        return;
    ///当lenth 大于等于loop 整体长度时,计算结果可能不符合预期.
    double L = 0;
    GeomCalculate gocal;
    ElemCalculate numcal;MathCalculate matnum;
    for (int i = 0; i < loop.Size(); ++i) {
        DefElem ele = loop.IndexElem(i);
        double len = gocal.ElementLenth(ele);
        if(matnum.IsNumericEqual(len + L,lenth,PreErr5_4)){
            index = i;
            numcal.ElementEndpoint(ele,true,P);
            return;
        }
        if(len + L > lenth){
            index = i;
            double decrease = lenth - L;
            ApproachCurveEndpoint(ele,decrease,false,true,P);
            return;
        }
        L += len;
    }
    ///处理当lenth 大于等于loop 整体长度的情况.
    index = 0;
    numcal.ElementMidpoint(loop.IndexElem(0),P);
}
void GeometryLogic::PointToSegmentLoop(const vector<Point>& points,
                                       bool isClose,
                                       DefLoop& loop){
    if(points.size() < 2)
        return;
    ElemCalculate elecal;
    int l = points.size();
    if(isClose){
        for (int i = 0; i < l; ++i) {
            int nex = elecal.NexId(l,i);
            loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(points[i],points[nex])));
        }
        return;
    }
    for (int i = 0; i < l - 1; ++i) {
        loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(points[i],points[i+1])));
    }
}
