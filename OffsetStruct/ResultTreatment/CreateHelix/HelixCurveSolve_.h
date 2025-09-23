#include "HelixCurveSolve.h"
void HelixCurveSolve::LoopCutEntryPoint(DefLoop& loop,double helRadius,
                                        bool& isReorderEle)
{
    GeomCalculate gocal;
    ElemCalculate numcal;
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        if(loop.m_vElem[i]._elemType != LINETYPE){
            continue;
        }
        double lenth;
        gocal.TwoPointDistance(loop.m_vElem[i]._segment.SegStartPt(),
                               loop.m_vElem[i]._segment.SegStartPt(),
                               lenth);
        if(lenth < helRadius * 2){
            continue;
        }
        Point midPnt;
        gocal.TwoPointMidpoint(loop.m_vElem[i]._segment.SegStartPt(),
                               loop.m_vElem[i]._segment.SegEndPt(),midPnt);
        DefElem 
ele1(LINETYPE,DefSeg(midPnt,loop.m_vElem[i]._segment.SegEndPt()));
        DefElem 
ele2(LINETYPE,DefSeg(loop.m_vElem[i]._segment.SegStartPt(),midPnt));
        if(isReorderEle){
            numcal.ReorederLoopElement(i,true,false,loop);
            loop.m_vElem.insert(loop.m_vElem.begin(),ele1);
            loop.m_vElem.push_back(ele2);
        }
        return;
    }
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        if(loop.m_vElem[i]._elemType != ARCTYPE){
            continue;
        }
        return;
    }
}
void HelixCurveSolve::ModelPositionRelativePath(const DefLoop& loop,
                                                MillDirction millDir,
                                                bool& isRightPos)
{
    if(loop.PastOffsetNum() <= 1){
        isRightPos = millDir.IsProfClockMill() ==
                millDir.IsToolClock() ? true : false;
    }
    else{
        isRightPos = millDir.IsAreaClockMill() ==
                millDir.IsToolClock() ? true : false;
    }
}
void HelixCurveSolve::PointOnElementAngle(const DefElem& element,
                                          const Point& P,
                                          RotDir rotDir,
                                          double &slope)
{
    GeomCalculate gocal;
    if(element._elemType == LINETYPE){        gocal.SegmentAngle(element._segment.SegStartPt(),
                           element._segment.SegEndPt(),slope);
        return;
    }
    Point rotPt;
    if(element._elemType == ARCTYPE){
        gocal.PointAroundFixedRotate(P,element._arc.ArcCenterPt(),
                                     90,element._arc.ArcRot(),rotPt);
        gocal.SegmentAngle(element._arc.ArcCenterPt(),
                           rotPt,slope);
    }
    else if(element._elemType == CIRCLETYPE){
        gocal.PointAroundFixedRotate(P,element._circle.CircleCenter(),
                                     90,rotDir,rotPt);
        gocal.SegmentAngle(element._circle.CircleCenter(),
                           rotPt,slope);
    }
}
void HelixCurveSolve::HelixCircleCenter(const DefElem& element,
                                        const Point& posPnt,
                                        double helixRdius,
                                        bool isCpRightPos,
                                        RotDir loopRot,
                                        Point& centerPt)
{
    double slopeDir;
    PointOnElementAngle(element,posPnt,loopRot,slopeDir);
    double varX = helixRdius * std::cos(slopeDir * M_PI / 180);
    double varY = helixRdius * std::sin(slopeDir * M_PI / 180);
    double x = posPnt.X() + varX;
    double y = posPnt.Y() + varY;
    Point extendPt(x,y);
    RotDir rotDir = isCpRightPos == true ? CLOCKWISE : ANTICLOCKWISE;
    GeomCalculate().PointAroundFixedRotate(extendPt,posPnt,
                                          90,rotDir,centerPt);
    centerPt.SetZ(posPnt.Z());
}
void HelixCurveSolve::HelixRadiusInterfere(const GeomArea& oriLoop,
                                           const DefElem& element,
                                           const Point& posPnt,
                                           double toolRdius,
                                           bool isCpRightPos,
                                           RotDir loopRot,
                                           CylinderHelix &helix)
{
    double distance;
    GeomDistance godis;
    godis.PointToLoopDistance(oriLoop.BndLoop(),
                              helix.HelixCenter(),distance);
    if(distance - helix.HelixRadius() - toolRdius - PreErr5_2 >= 0){
        return;
    }
    double helixRadius = distance - toolRdius - PreErr5_2;
    if(helixRadius >= helix.HelixRadius()){
        return;
    }
    helix.SetHelixRadius(helixRadius);
    Point helCenter;
    HelixCircleCenter(element,posPnt,helixRadius,
                      isCpRightPos,loopRot,helCenter);    helix.SetHelixCenter(helCenter);
}
void HelixCurveSolve::GenerateSpecifyHelixPoint(const Point& helCenter,
                                                double helRadius,
                                                double startAngle,
                                                double perAngHeight,
                                                double totalAngle,
                                                bool isRightHelix,
                                                vector<Point>&vPoint)
{
    double historyAngle = 0,curAngle = startAngle,stepVal = 10;
    if(isRightHelix){
        while (historyAngle < totalAngle){
            double _x = helRadius * std::cos(curAngle * M_PI / 180);
            double _y = helRadius * std::sin(curAngle * M_PI / 180);
            double x = helCenter.X() + _x;
            double y = helCenter.Y() + _y;
            ///zValue = perAngHeight * (angle - startAngle);
            Point P(x,y,helCenter.Z() + perAngHeight * (curAngle - startAngle));
            vPoint.push_back(P);
            curAngle += stepVal;
            historyAngle += stepVal;
        }
        return;
    }
    while (historyAngle < totalAngle) {
        double _x = helRadius * std::cos(curAngle * M_PI / 180);
        double _y = helRadius * std::sin(curAngle * M_PI / 180);
        double x = helCenter.X() + _x;
        double y = helCenter.Y() + _y;
        Point P(x,y,perAngHeight * historyAngle);
        vPoint.push_back(P);
        curAngle -= stepVal;
        historyAngle += stepVal;
    }
}