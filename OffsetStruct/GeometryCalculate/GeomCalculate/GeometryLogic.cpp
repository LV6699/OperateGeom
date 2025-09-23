#include "GeometryLogic.h"
shared_ptr<GeomCalculate>golg_go = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate>golg_num = make_shared<ElemCalculate>();
//shared_ptr<CalCutSearch>golg_calSea = make_shared<CalCutSearch>();
shared_ptr<ElementIntersect>golg_eleInt = make_shared<ElementIntersect>();
shared_ptr<GeomIntersect>golg_goIn = make_shared<GeomIntersect>();
shared_ptr<PointInPolygon>golg_inpol = make_shared<PointInPolygon>();
#if DebugModel
#pragma optimize("", off)
#endif
void GeometryLogic::IsLineCircleTangency(const Point& lSp,
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
    if(ele1._elemType == LINETYPE && ele2._elemType == LINETYPE){
        return false;
    }
    bool isTan = false;
    if(ele1._elemType == LINETYPE && ele2._elemType == ARCTYPE){
        IsLineCircleTangency(ele1._segment._segSp,ele1._segment._segEp,
                             ele2._arc._arcCp,
                             ele2._arc._arcR,
                             pre,isTan);
    }
    else if(ele1._elemType == ARCTYPE && ele2._elemType == ARCTYPE){        IsCircleCircleTangency(ele1._arc._arcCp,
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
}void GeometryLogic::ClosetPointFromTwoPoint(const Point& sp,
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
    double startAgl,endAgl;    gocal.ArcEndpointAngle(arc._arcSp,arc._arcEp,arc._arcCp,
                           arc._arcRot,startAgl,endAgl);
    double centerAgl;
    gocal.TwoAngleIncludedAngle(startAgl,endAgl,arc._arcRot,centerAgl);
    double stepVal_ = stepAgl;
    if(stepVal_ >= centerAgl){
        if(isCloseEp){
            P = arc.ArcStartPt();
        }else{
            P = arc.ArcEndPt();
        }
        return;
    }
#if 1
    if(isCloseEp){
        ElemCalculate().ReverseArcDirection(arc);
    }
    gocal.PointRotate2D(arc._arcSp,arc._arcCp,stepVal_,
                        arc._arcRot == CLOCKWISE,P);
#else
    double angle;
    gocal.CloseArcEndpointAngle(arc,stepVal_,isCloseEp,angle);
    arc.ChoiceSetRadius(PreErr5_2);
    double incx = arc.ArcRadius() * std::cos(angle * _RadAlg);
    double incy = arc.ArcRadius() * std::sin(angle * _RadAlg);
    P.SetX(arc._arcCp.X() + incx);
    P.SetY(arc._arcCp.Y() + incy);
#endif
    P.SetZ(arc.ArcCenterPt().Z());
}
void GeometryLogic::CloseArcEndpointBaseLenth(DefArc arc,
                                              bool isCloseEp,
                                              double stepVal,
                                              Point &P)
{
    arc.ChoiceSetRadius(PreErr5_2);
    double stepAgl = (360 * stepVal) / (2 * M_PI * arc.ArcRadius());
    CloseArcEndpointBaseAngle(arc,isCloseEp,stepAgl,P);
}
void GeometryLogic::ApproachCurveEndpoint(const DefElem& ele,
                                          double stepVal,
                                          bool isAngle,
                                          bool isNearSp,
                                          Point& P)
{
    GeomCalculate go;
    switch (ele.ElemeType()) {
    case LINETYPE:
        go.CloseSegmentEndpoint(ele.ElemSeg().SegStartPt(),
                                ele.ElemSeg().SegEndPt(),
                                stepVal,isNearSp,P);
        break;
    case ARCTYPE:
        if(isAngle){
            CloseArcEndpointBaseAngle(ele.ElemArc(),!isNearSp,stepVal,P);
        }else{
            CloseArcEndpointBaseLenth(ele.ElemArc(),!isNearSp,stepVal,P);
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
                loop.m_vElem[0]._elemType == CIRCLETYPE);
    if(isCircle && !isMatchArc)
        return;
    if(loop.m_vElem.size() != 2 ||
            loop.m_vElem[0]._elemType != ARCTYPE ||
            loop.m_vElem[1]._elemType != ARCTYPE)
        return;
    if(!golg_go->IsIdenticalPoint(loop.m_vElem[0]._arc._arcCp,
                                  loop.m_vElem[1]._arc._arcCp,
                                  PreErr5_10))
        return;
    if(std::abs(loop.m_vElem[0]._arc._arcR -
                loop.m_vElem[1]._arc._arcR) >
            PreErr5_10)
        return;
    if(loop.m_vElem[0]._arc._arcRot !=
            loop.m_vElem[1]._arc._arcRot)
        return;
    isCircle = true;
}
bool GeometryLogic::IsCircleStructureLoop(const DefLoop& loop,
                                          double rErr, double cpErr)
{
    if(loop.m_vElem.size() == 1 &&
            loop.m_vElem[0]._elemType == CIRCLETYPE)
        return true;
    GeomCalculate gocal;
    MathCalculate nummat;
    if(loop.ElemNum() == 2 &&
            loop.m_vElem[0]._elemType == ARCTYPE &&
            loop.m_vElem[1]._elemType == ARCTYPE &&
            gocal.IsIdenticalPoint(loop.m_vElem[0]._arc.ArcCenterPt(),
                                   loop.m_vElem[1]._arc.ArcCenterPt(),
                                   cpErr) &&
            nummat.IsNumericEqual(loop.m_vElem[0]._arc.ArcRadius(),
                                  loop.m_vElem[1]._arc.ArcRadius(),
                                  rErr)){
        return true;
    }
    return false;}
///仅用在loop由圆组成的情况.
void GeometryLogic::CircleStructureCircle(const DefLoop& loop,
                                          DefCircle& circle)
{
    if(loop.IsEmpty())
        return;
    if(loop.ElemNum() == 1){
        circle = loop.IndexElem(0).ElemCircle();
        return;
    }
    DefArc arc1 = loop.IndexElem(0).ElemArc();
    DefArc arc2 = loop.IndexElem(1).ElemArc();
    arc1.ChoiceSetRadius(PreErr5_3);
    arc2.ChoiceSetRadius(PreErr5_3);
    Point cp1 = arc1.ArcCenterPt(),
            cp2 = arc2.ArcCenterPt(),cp;
    GeomCalculate().TwoPointMidpoint(cp1,cp2,cp);
    double R1 = arc1.ArcRadius();
    double R2 = arc2.ArcRadius();
    circle.SetCircleCenter(cp);
    circle.SetCircleRadius((R1 + R2) / 2);
}
void GeometryLogic::CircleArcLoopTransform(bool isTranToArc,
                                           DefLoop &loop)
{
    if(isTranToArc){
        if(loop.m_vElem.size() != 1 ||
                loop.m_vElem[0]._elemType != CIRCLETYPE)
            return;
        Point cp(loop.m_vElem[0]._circle._cirCp.X(),
                loop.m_vElem[0]._circle._cirCp.Y());
        double R = loop.m_vElem[0]._circle._cirR;
        Point sp(cp.X(),cp.Y() + R);
        Point ep(cp.X(),cp.Y() - R);
        DefArc arc1 (sp,ep,cp,R,CLOCKWISE);
        DefArc arc2 (ep,sp,cp,R,CLOCKWISE);
        DefLoop loop_ = loop;
        loop_.m_vElem.clear();
        loop_.m_vElem.push_back(DefElem(ARCTYPE, arc1));
        loop_.m_vElem.push_back(DefElem(ARCTYPE, arc2));
        loop_.m_vElem[0]._elemLabel._isCirDisc = true;
        loop_.m_vElem[1]._elemLabel._isCirDisc = true;
        loop = loop_;
        return;
    }
    GeomCalculate gocal;
    if(loop.m_vElem.size() != 2 ||
            loop.m_vElem[0]._elemType != ARCTYPE ||
            loop.m_vElem[1]._elemType != ARCTYPE){
        return;
    }
    if(!gocal.IsIdenticalPoint(loop.m_vElem[0]._arc.ArcCenterPt(),
                               loop.m_vElem[1]._arc.ArcCenterPt(),
                               PreErr5_10))
        return;
    if(std::abs(loop.m_vElem[0]._arc.ArcRadius() -                loop.m_vElem[1]._arc.ArcRadius()) >
            PreErr5_10)
        return;
    if(loop.m_vElem[0]._arc.ArcRot() !=
            loop.m_vElem[1]._arc.ArcRot())
        return;
    Point cp((loop.m_vElem[0]._arc.ArcCenterPt().X() +
             loop.m_vElem[1]._arc.ArcCenterPt().X()) / 2,
            (loop.m_vElem[0]._arc.ArcCenterPt().Y() +
            loop.m_vElem[1]._arc.ArcCenterPt().Y()) / 2);
    double R = (loop.m_vElem[0]._arc.ArcRadius() +
            loop.m_vElem[1]._arc.ArcRadius()) / 2;
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
    GeomContain gocont;
    int lo = 0;
    while (lo < vLoop.size()) {
        vector<DefLoop> vLoop_;
        for (int i = 0; i < vLoop.size(); ++i) {
            if(i != lo){
                vLoop_.push_back(vLoop[i]);
            }
        }
        bool isInclude_ = false;
        gocont.IsLoopsContainLoop(vLoop_,vLoop[lo],intPre,pre,isInclude_);
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
                                               bool& matchRes){
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
                                        vector<Point> &vPoint)
{
    GeomCalculate gocal;
    switch (ele.ElemeType()) {
    case LINETYPE:
        gocal.DiscreteLineToPoint(ele.ElemSeg().SegStartPt(),
                                  ele.ElemSeg().SegEndPt(),
                                  stepVal,vPoint);
        break;
    case ARCTYPE:{
        double stepVal_ = stepVal;
        if(!isAngle){
            ele._arc.ChoiceSetRadius(PreErr5_2);
            stepVal_ = (360 * stepVal) /
                    (2 * M_PI * ele.ElemArc().ArcRadius());
        }
        gocal.DiscreteArcToPoint(ele.ElemArc(),stepVal_,vPoint);
        break;
    }
    case CIRCLETYPE:
        gocal.DiscreteCircleToPoint(ele.ElemCircle().CircleCenter(),
                                    ele.ElemCircle().CircleRadius(),
                                    CLOCKWISE,stepVal,vPoint);
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
    switch (ele.ElemeType()) {
    case LINETYPE:
        gocal.DiscreteLineToPoint(ele._segment._segSp,
                                  ele._segment._segEp,
                                  lStep,points);
        break;
    case ARCTYPE:{
        double cStep_ = cStep;
        ele._arc.ChoiceSetRadius(PreErr5_3);
        if(!isAngle){
            cStep_ = MathCalculate().AngleBaseBowHeight(
                        ele._arc._arcR,cStep);/**
            stepVal_ = (360 * stepVal) /                    (2 * M_PI * ele.ElemArc().ArcRadius());*/
        }
        gocal.DiscreteArcToPoint(ele._arc,cStep_,points);
        break;
    }
    case CIRCLETYPE:
        gocal.DiscreteCircleToPoint(ele._circle._cirCp,
                                    ele._circle._cirR,
                                    CLOCKWISE,cStep,points);
        break;
    default:
        break;
    }
}
void GeometryLogic::TranslateLoopIntoPoint(DefLoop loop,double lStep,
                                           double cStep,bool isStepAngle,
                                           vector<Point>& vPoint)
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
                                    CLOCKWISE,stepVal,vPoint);
        return;
    }
    for(auto& e : loop.m_vElem){
        if(e.ElemeType() == ARCTYPE){
            e._arc.ChoiceSetRadius(PreErr5_2);
        }
    }
    for (int i = 0; i < loop.ElemNum(); ++i) {
        vector<Point>points;
        if(loop.m_vElem[i]._elemType == LINETYPE){
            const Point& sp = loop.m_vElem[i]._segment._segSp;
            const Point& ep = loop.m_vElem[i]._segment._segEp;
            gocal.DiscreteLineToPoint(sp,ep,lStep,points);
        }
        else if(loop.m_vElem[i]._elemType == ARCTYPE){
            double stepVal = cStep;
            const DefArc& arc = loop.m_vElem[i]._arc;
            if(!isStepAngle){
                stepVal = matcal.AngleBaseBowHeight(arc._arcR,cStep);
            }
            gocal.DiscreteArcToPoint(arc,stepVal,points);
        }
        points.pop_back();
        for(auto& d : points){
            vPoint.emplace_back(d);
        }
    }
}void GeometryLogic::LoopToSegmentLoop(DefLoop loop,
                                      double expStep,
                                      DefLoop& resLoop)
{
    resLoop = loop;
    resLoop.m_vElem.clear();
    vector<Point>vPoint;
    GeomCalculate gocal;ElemCalculate numcal;
    numcal.InitialLoopArcRadius(loop);
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        if(loop.m_vElem[i]._elemType == LINETYPE){
            vPoint.push_back(loop.m_vElem[i]._segment._segSp);
        }
        else if(loop.m_vElem[i]._elemType == ARCTYPE){
            vector<Point> vPnt;
            DefArc arc = loop.m_vElem[i]._arc;
            gocal.DiscreteArcToPoint(arc,expStep,vPnt);
            for (int p = 0; p < vPnt.size(); ++p) {
                vPoint.push_back(vPnt[p]);
            }
        }
        else if(loop.m_vElem[i]._elemType == CIRCLETYPE){
            gocal.DiscreteCircleToPoint(loop.m_vElem[i]._circle._cirCp,
                                        loop.m_vElem[i]._circle._cirR,
                                        CLOCKWISE,expStep,vPoint);
        }
    }
    if(vPoint.size() < 2)
        return;
    numcal.DeduplicationPoint(vPoint,PreErr5_4);
    int len = loop.IsOpenLoop() == true ?
                vPoint.size() - 1 : vPoint.size();
    for (int p = 0; p < len; ++p) {
        int nex = numcal.NextIndex(vPoint.size(),p);
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
    resLoop.ClearElem();
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
            stepVal_ = matcal.AngleBaseBowHeight(circle.CircleRadius(),stepVal);
        }
        gocal.DiscreteCircleToPoint(circle._cirCp,circle._cirR,                                    CLOCKWISE,stepVal_,vPoint);
        if(vPoint.size() < 2)
            return;
        for (int p = 0; p < vPoint.size()-1; ++p) {
            resLoop.PushBackElem(
                        DefElem(LINETYPE,DefSeg(vPoint[p],vPoint[p+1])));
        }
        return;
    }
    for(auto& e : loop.m_vElem){
        if(e.ElemeType() == ARCTYPE){
            e._arc.ChoiceSetRadius(PreErr5_2);
        }
    }
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        if(loop.m_vElem[i]._elemType == LINETYPE){
            resLoop.m_vElem.emplace_back(loop.m_vElem[i]);
        }
        else if(loop.m_vElem[i]._elemType == ARCTYPE){
            vector<Point>vPoint;
            double stepVal_ = stepVal;
            const DefArc& arc = loop.m_vElem[i]._arc;
            if(!isStepAngle){
                stepVal_ = matcal.AngleBaseBowHeight(arc.ArcRadius(),stepVal);
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
        BILoop._bndLoop.PushBack(DefElem(LINETYPE,DefSeg(sp,ep)));
    }
    for(auto& d : BILoop._vIslLoop) {
        if(d.IsEmpty())
            continue;
        DefLoop rloop;
        LoopToSegmentLoop(d,stepVal,isAngle,rloop);
        Point sp_ = rloop.Endpoint(true);
        Point ep_ = rloop.Endpoint(false);
        if(!sp_.IsSamePoint2D(ep_,PreErr5_10)){
            rloop.PushBack(DefElem(LINETYPE,DefSeg(sp_,ep_)));        }
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
            b = mat.BezierCoefficient(n,i) * std::pow(1 - t,n - i) * 
std::pow(t,i);
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
        int nex = numcal.NextIndex(vBezierPt.size(),i);
        loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(vBezierPt[i],
                                                       vBezierPt[nex])));
        i ++;
    }
}
void GeometryLogic::LoopSpecifyLenthPosition(const DefLoop& loop,double lenth,
                                             int &index,Point& P)
{
    if(loop.IsEmpty() || (loop.ElemNum() == 1 &&
                          loop.IndexElem(1).ElemeType() == CIRCLETYPE))
        return;
    ///当lenth大于等于loop整体长度时,计算结果可能不符合预期 .
    double L = 0;
    GeomCalculate gocal;
    ElemCalculate numcal;MathCalculate matnum;
    for (int i = 0; i < loop.ElemNum(); ++i) {
        DefElem ele = loop.IndexElem(i);
        double len = gocal.ElementLenth(ele);
        if(matnum.IsNumericEqual(len + L,lenth,PreErr5_4)){
            index = i;
            numcal.ElementEndpoint(ele,true,P);            return;
        }
        if(len + L > lenth){
            index = i;
            double decrease = lenth - L;
            ApproachCurveEndpoint(ele,decrease,false,true,P);
            return;
        }
        L += len;
    }
    ///处理当lenth大于等于loop整体长度的情况.
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
            int nex = elecal.NextIndex(l,i);
            
loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(points[i],points[nex])));
        }
        return;
    }
    for (int i = 0; i < l - 1; ++i) {
        loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(points[i],points[i+1])));
    }
}