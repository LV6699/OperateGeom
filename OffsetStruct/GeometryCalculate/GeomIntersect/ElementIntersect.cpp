#include "ElementIntersect.h"
#if DebugModel
#pragma optimize("", off)
#endif
shared_ptr<GeomCalculate>calInt_go = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate>calInt_num = make_shared<ElemCalculate>();
shared_ptr<GeometryLogic>calInt_goLog = make_shared<GeometryLogic>();
shared_ptr<GeomIntersect>calInt_goIn = make_shared<GeomIntersect>();
shared_ptr<GeomDistance>calInt_godis = make_shared<GeomDistance>();
void ElementIntersect::GetCloseArcExtremePnt(DefArc arc,
                                             const Point &P1,
                                             const Point &P2,
                                             bool isCloseSp,
                                             Point &P)
{
    double dis1,dis2;
    calInt_godis->PointToArcEndpointDistance(arc,P1,isCloseSp,dis1);
    calInt_godis->PointToArcEndpointDistance(arc,P2,isCloseSp,dis2);
    P = dis1 <= dis2 ? P1 : P2;
}
void ElementIntersect::GetCloseEleExtremePnt(const DefElem &ele,
                                             const Point &P1,
                                             const Point &P2,
                                             bool isCloseSp,
                                             Point& P)
{
    double dis1,dis2;
    calInt_godis->PointToEleEndpointDistance(ele,P1,isCloseSp,dis1);
    calInt_godis->PointToEleEndpointDistance(ele,P2,isCloseSp,dis2);
    P = dis1 <= dis2 ? P1 : P2;
}
void ElementIntersect::GetLoopEleIntersectPoint(const DefLoop& loop,
                                                const DefElem& ele,
                                                double tanPre,
                                                double endptPre,
                                                double intPre,
                                                vector<Point>& vIntPnt)
{
    vIntPnt.clear();
    ElemCalculate num;
    GeomIntersect goInt;
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        int intNum = 0;
        GeomIntRes intRes;
        goInt.CalculateCurveIntersect(ele,loop.m_vElem[i],true,
                                        tanPre,endptPre,intPre,intRes);
        if(!num.IsEleExistValInt(ele,loop.m_vElem[i],intRes)){
            continue;
        }
        vIntPnt.push_back(intRes._intPnt1);
        if(intNum == 2){
            vIntPnt.push_back(intRes._intPnt2);
        }
    }
}void ElementIntersect::GetTwoPntCloseEleEndPnt(const DefElem&ele,
                                               const Point& intPnt1,
                                               const Point& intPnt2,
                                               bool isCloseSp,
                                               Point& objPnt)
{
    Point eleSp,eleEp;
    if(ele._elemType == LINETYPE){
        eleSp = ele._segment._segSp;
        eleEp = ele._segment._segEp;
        if(isCloseSp){
            calInt_goLog->ClosetPointFromTwoPoint(eleSp,intPnt1,
                                                  intPnt2,objPnt);
        }
        else{
            calInt_goLog->ClosetPointFromTwoPoint(eleEp,intPnt1,
                                                  intPnt2,objPnt);
        }
    }
    else if(ele._elemType == ARCTYPE){
        Point arcSp = ele._arc._arcSp,
                arcEp = ele._arc._arcEp,
                arcCp = ele._arc._arcCp;
        RotDir rot = ele._arc._arcRot;
        double startAgl1,endAgl1,aglAbs1,startAgl2,endAgl2,aglAbs2;
        if(isCloseSp){
            calInt_go->ArcEndpointAngle(arcSp,intPnt1,
                                        arcCp,rot,
                                        startAgl1,endAgl1);
            calInt_go->TwoAngleIncludedAngle(startAgl1,endAgl1,
                                             rot,aglAbs1);
            calInt_go->ArcEndpointAngle(arcSp,intPnt2,
                                        arcCp,rot,
                                        startAgl2,endAgl2);
            calInt_go->TwoAngleIncludedAngle(startAgl2,endAgl2,
                                             rot,aglAbs2);
        }
        else{
            calInt_go->ArcEndpointAngle(intPnt1,arcEp,arcCp,
                                        rot,startAgl1,endAgl1);
            calInt_go->TwoAngleIncludedAngle(startAgl1,endAgl1,
                                             rot,aglAbs1);
            calInt_go->ArcEndpointAngle(intPnt2,arcEp,arcCp,
                                        rot,startAgl2,endAgl2);
            calInt_go->TwoAngleIncludedAngle(startAgl2,endAgl2,
                                             rot,aglAbs2);
        }
        objPnt = aglAbs1 <= aglAbs2 ? intPnt1 : intPnt2;
    }
}
void ElementIntersect::GetSigArcIntUnderDelEle(const DefArc &arcData,
                                               bool isLineFont,
                                               const Point& intPnt1,
                                               const Point& intPnt2,
                                               Point &interPnt)
{
    DefArc temArc1(arcData._arcSp,arcData._arcEp,
                   arcData._arcCp,arcData._arcR,                   arcData._arcRot);
    DefArc temArc2(arcData._arcSp,arcData._arcEp,
                   arcData._arcCp,arcData._arcR,
                   arcData._arcRot);
    if(isLineFont){
        temArc1._arcSp = intPnt1;
        temArc2._arcSp = intPnt2;
    }
    else{
        temArc1._arcEp = intPnt1;
        temArc2._arcEp = intPnt2;
    }
    double startAgl1,endAgl1,aglAbs1,startAgl2,endAgl2,aglAbs2;
    calInt_go->ArcEndpointAngle(temArc1._arcSp,
                                temArc1._arcEp,
                                temArc1._arcCp,
                                temArc1._arcRot,
                                startAgl1,endAgl1);
    calInt_go->TwoAngleIncludedAngle(startAgl1,endAgl1,
                                     temArc1._arcRot,aglAbs1);
    calInt_go->ArcEndpointAngle(temArc2._arcSp,
                                temArc2._arcEp,
                                temArc2._arcCp,
                                temArc2._arcRot,
                                startAgl2,endAgl2);
    calInt_go->TwoAngleIncludedAngle(startAgl2,endAgl2,
                                     temArc2._arcRot,aglAbs2);
    if(aglAbs1 <= aglAbs2){
        interPnt = intPnt1;
    }
    else{
        interPnt = intPnt2;
    }
}
void ElementIntersect::GetSigArcIntPntBaseFixedPnt(const DefArc &arc,
                                                   bool isLineFont,
                                                   const Point &intPnt1,
                                                   const Point &intPnt2,
                                                   Point &interPnt)
{
    DefArc temArc1(arc._arcSp,arc._arcEp,
                   arc._arcCp,arc._arcR,
                   arc._arcRot);
    DefArc temArc2(arc._arcSp,arc._arcEp,
                   arc._arcCp,arc._arcR,
                   arc._arcRot);
    if(isLineFont){
        temArc1._arcSp = intPnt1;
        temArc2._arcSp = intPnt2;
    }
    else{
        temArc1._arcEp = intPnt1;
        temArc2._arcEp = intPnt2;
    }
    double startAgl1,endAgl1,aglAbs1,startAgl2,endAgl2,aglAbs2;
    calInt_go->ArcEndpointAngle(temArc1._arcSp,
                                temArc1._arcEp,
                                temArc1._arcCp,
                                temArc1._arcRot,
                                startAgl1,endAgl1);    calInt_go->TwoAngleIncludedAngle(startAgl1,endAgl1,
                                     temArc1._arcRot,aglAbs1);
    calInt_go->ArcEndpointAngle(temArc2._arcSp,
                                temArc2._arcEp,
                                temArc2._arcCp,
                                temArc2._arcRot,
                                startAgl2,endAgl2);
    calInt_go->TwoAngleIncludedAngle(startAgl2,endAgl2,
                                     temArc2._arcRot,aglAbs2);
    if(aglAbs1 <= aglAbs2){
        interPnt = intPnt1;
    }
    else{
        interPnt = intPnt2;
    }
}
void ElementIntersect::GetToSigMinDis(Point& pnt1,Point& pnt2,
                                      Point& startPnt,
                                      Point& endPnt,Point& valPnt)
{
    double pnt1ToSigDis,pnt1_StartDis,pnt1_EndtDis;
    calInt_go->TwoPointDistance(pnt1,startPnt,pnt1_StartDis);
    calInt_go->TwoPointDistance(pnt1,endPnt,pnt1_EndtDis);
    pnt1ToSigDis = pnt1_StartDis <=  pnt1_EndtDis ?
                pnt1_StartDis : pnt1_EndtDis;
    double pnt2ToSigDis,pnt2_StartDis,pnt2_EndtDis;
    calInt_go->TwoPointDistance(pnt2,startPnt,pnt2_StartDis);
    calInt_go->TwoPointDistance(pnt2,endPnt,pnt2_EndtDis);
    pnt2ToSigDis = pnt2_StartDis <=  pnt2_EndtDis ?
                pnt2_StartDis : pnt2_EndtDis;
    if(pnt1ToSigDis <= pnt2ToSigDis){
        valPnt = pnt1;
    }
    else{
        valPnt = pnt2;
    }
}