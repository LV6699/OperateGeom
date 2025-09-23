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
        GeomCalculate gocal;
        gocal.TwoPointMidpoint(loop.m_vElem[i]._segment.SegStartPt(),
                               loop.m_vElem[i]._segment.SegEndPt(),
                               midPnt);
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
    if(loop.IsCloseMode()){
        isRightPos = millDir.IsProfClockMill() ==
                millDir.IsToolClock() ? true : false;
    }else{
        isRightPos = millDir.IsAreaClockMill() ==
                millDir.IsToolClock() ? true : false;
    }
}
void HelixCurveSolve::ModelPositionRelativePath(bool isCloseModel,
                                                MillDirction millDir,
                                                bool& isRightPos)
{
    if(isCloseModel){
        isRightPos = millDir.IsProfClockMill() ==
                millDir.IsToolClock() ? true : false;    }else{
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
    if(element._elemType == LINETYPE){
        gocal.SegmentAngle(element._segment.SegStartPt(),
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
void HelixCurveSolve::CurvePointTangent(const DefElem& curve,
                                        const Point& pos,bool isSame,
                                        double l,Point& extpt)
{
    ///在曲线切入点切线上的点 ,isSame:切线方向是否与曲线在点处的切线方向相同 ,l:切线长度.
    double agl;
    OffsetEnum::RotDir loopRot = OffsetEnum::CLOCKWISE;
    if(curve.ElemeType() == OffsetEnum::CIRCLETYPE &&
            !curve._circle.IsCW()){
        loopRot = OffsetEnum::ANTICLOCKWISE;
    }
    PointOnElementAngle(curve,pos,loopRot,agl);
    double x_ = l * std::cos(agl * _RadAlg);
    double y_ = l * std::sin(agl * _RadAlg);
    if(isSame){
        extpt.SetX(pos.X() + x_);
        extpt.SetY(pos.Y() + y_);
    }else{
        extpt.SetX(pos.X() - x_);
        extpt.SetY(pos.Y() - y_);
    }
    extpt.SetZ(pos.Z());
}
void HelixCurveSolve::HelixCircleCenter(const DefElem& element,
                                        const Point& posPnt,
                                        double helixRdius,
                                        bool isCpRightPos,
                                        RotDir loopRot,
                                        Point& centerPt)
{
    Point extpt;#if 1
    CurvePointTangent(element,posPnt,true,helixRdius,extpt);
#else
    double slopeDir;
    PointOnElementAngle(element,posPnt,loopRot,slopeDir);
    double varX = helixRdius * std::cos(slopeDir * _RadAlg);
    double varY = helixRdius * std::sin(slopeDir * _RadAlg);
    double x = posPnt.X() + varX;
    double y = posPnt.Y() + varY;
    Point extpt(x,y);
#endif
    RotDir rotDir = isCpRightPos == true ? CLOCKWISE : ANTICLOCKWISE;
    GeomCalculate().PointAroundFixedRotate(extpt,posPnt,
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
    for (int i = 0; i < oriLoop.IslLoop().size(); ++i) {
        double distance_;
        godis.PointToLoopDistance(oriLoop.IslLoop()[i],
                                  helix.HelixCenter(),
                                  distance_);
        if(distance_ < distance){
            distance = distance_;
        }
    }
    if((distance - PreErr5_6) >= (helix.HelixRadius() + toolRdius)){
        return;
    }
    double helixRadius = distance - toolRdius;
    if(helixRadius >= helix.HelixRadius()){
        return;
    }
    helix.SetHelixRadius(helixRadius);
    Point helCenter;
    HelixCircleCenter(element,posPnt,helixRadius,
                      isCpRightPos,loopRot,helCenter);
    helix.SetHelixCenter(helCenter);
}
void HelixCurveSolve::GenerateSpecifyHelixPoint(const Point& helCenter,
                                                double helRadius,
                                                double startAngle,
                                                double perAngHeight,
                                                double totalAngle,
                                                bool isRightHelix,
                                                vector<Point>&vPoint)
{
    auto HexPt = [](double perHei,double hisAng,double R,double curAngle,
            const Point& cp,vector<Point>&vPoint)->void {
        double _x = R * std::cos(curAngle * M_PI / 180);        double _y = R * std::sin(curAngle * M_PI / 180);
        double x = cp.X() + _x;
        double y = cp.Y() + _y;
        //double z = cp.Z() + perHei * (curAngle - startAngle);
        double z = cp.Z() + perHei * hisAng;
        Point P(x,y,z);
        vPoint.push_back(P);
    };
    double hisAngle = 0,curAngle = startAngle,stepVal = 10;
    if(isRightHelix){
        while (hisAngle < totalAngle){
            HexPt(perAngHeight,hisAngle,helRadius,
                  curAngle,helCenter,vPoint);
            curAngle += stepVal;
            hisAngle += stepVal;
        }
        return;
    }
    while (hisAngle < totalAngle) {
        HexPt(perAngHeight,hisAngle,helRadius,
              curAngle,helCenter,vPoint);
        curAngle -= stepVal;
        hisAngle += stepVal;
    }
}