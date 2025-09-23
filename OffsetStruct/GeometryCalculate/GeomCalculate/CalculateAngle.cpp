#include "CalculateAngle.h"
#if DebugModel
#pragma optimize("", off)
#endif
void CalculateAngle::CalculateSigSigAngle(const Point& l1Sp,
                                          const Point& l1Ep,
                                          const Point& l2Sp,
                                          const Point& l2Ep,
                                          double& angle)
{
    ///需要保证两线段是首尾相连的
    GeomCalculate gocal;
    bool isParall = false;
    gocal.IsSigmentsParallel(l1Sp,l1Ep,l2Sp,l2Ep,isParall);
    if(isParall){
        angle = 0;
        return;
    }
    double angle1,angle2;
    angle1 = std::atan2((l1Sp._y - l1Ep._y),
                        (l1Sp._x - l1Ep._x));
    angle2 = std::atan2((l2Ep._y - l2Sp._y),
                        (l2Ep._x - l2Sp._x));
    angle1 = angle1 * 180.0 / M_PI;
    angle2 = angle2 * 180.0 / M_PI;
    if(angle1 < 0){
        angle1 += 360;
    }
    if(angle2 < 0){
        angle2 += 360;
    }
    RotDir sigRot;
    gocal.TriangleRotDirection(l1Ep,l1Sp,l2Ep,sigRot);
    gocal.TwoAngleIncludedAngle(angle1,angle2,sigRot,angle);
}
void CalculateAngle::SegmentSegmentAngle(const Point& l1Sp,
                                         const Point& l1Ep,
                                         const Point& l2Sp,
                                         const Point& l2Ep,
                                         bool isCW,
                                         double& angle)
{
    GeomCalculate gocal;
    double angle1,angle2;
    angle1 = std::atan2((l1Ep.Y() - l1Sp.Y()),(l1Ep.X() - l1Sp.X()));
    angle2 = std::atan2((l2Ep.Y() - l2Sp.Y()),(l2Ep.X() - l2Sp.X()));
    angle1 = angle1 * 180.0 / M_PI;
    angle2 = angle2 * 180.0 / M_PI;
    if(angle1 < 0)
        angle1 += 360;
    if(angle2 < 0)
        angle2 += 360;
    RotDir rot = isCW == true ? CLOCKWISE : ANTICLOCKWISE;
    gocal.TwoAngleIncludedAngle(angle1,angle2,rot,angle);
}
void CalculateAngle::AdjoinSegmentSegmentAngle(const Point& l1Sp,
                                               const Point& l1Ep,                                               const Point& l2Sp,
                                               const Point& l2Ep,
                                               double& angle)
{
    ///需要保证两线段是首尾相连的
    GeomCalculate gocal;
    bool isParall = false;
    gocal.IsSigmentsParallel(l1Sp,l1Ep,l2Sp,l2Ep,isParall);
    if(isParall){
        angle = 0;
        return;
    }
    double angle1,angle2;
    angle1 = std::atan2((l1Sp._y - l1Ep._y),
                        (l1Sp._x - l1Ep._x));
    angle2 = std::atan2((l2Ep._y - l2Sp._y),
                        (l2Ep._x - l2Sp._x));
    angle1 = angle1 * 180.0 / M_PI;
    angle2 = angle2 * 180.0 / M_PI;
    if(angle1 < 0){
        angle1 += 360;
    }
    if(angle2 < 0){
        angle2 += 360;
    }
    RotDir sigRot;
    gocal.TriangleRotDirection(l1Ep,l1Sp,l2Ep,sigRot);
    gocal.TwoAngleIncludedAngle(angle1,angle2,sigRot,angle);
}
void CalculateAngle::CalculateSigArcAngle(const Point& lSp,
                                          const Point& lEp,
                                          const DefArc& arc,
                                          bool isLineFont,
                                          double& angle)
{
    ///需要保证线段圆弧首尾相连 ,这种情况angle不可能大于180度.
    ///平移线段,使得线段上与圆弧连接的点与圆心重合 (只需要平移线段一次),
    ///或把连接圆心和连接点线段的旋转线段平移至切点 (只需要平移旋转线段一次 ).
    GeomCalculate gocal;
    double lAgl,aAgl;
    gocal.SegmentAngle(lSp,lEp,lAgl);
    Point arcCp = arc._arcCp,cntPt = lEp,rotResPt;
    if(!isLineFont){
        cntPt = lSp;
    }
    double A = 90;
    ///以连接点为中心旋转圆心 ,旋转方向是圆弧旋向的反方向 .
    RotDir cpRotDir = arc._arcRot == CLOCKWISE ? ANTICLOCKWISE : CLOCKWISE;
    gocal.PointAroundFixedRotate(arcCp,cntPt,A,cpRotDir,rotResPt);
    gocal.SegmentAngle(cntPt,rotResPt,aAgl);
    double R;
    gocal.TwoPointDistance(arcCp,cntPt,R);
    RotDir rotDir;
    if(isLineFont){
        gocal.TriangleRotDirection(cntPt,lEp,rotResPt,rotDir);
        gocal.TwoAngleIncludedAngle(lAgl,aAgl,cpRotDir,angle);
    }    else {
        gocal.TriangleRotDirection(cntPt,rotResPt,lSp,rotDir);
        gocal.TwoAngleIncludedAngle(aAgl,lAgl,cpRotDir,angle);
    }
}
void CalculateAngle::AdjoinSegmentArcAngle(const Point& lSp,
                                           const Point& lEp,
                                           const DefArc& arc,
                                           bool isLineFont,
                                           double& angle)
{
    ///这里计算规则是前一线段或圆弧角度改变 ,后一线段或圆弧角度不改变
    double lAgl,aAgl;
    Point arcCp = arc._arcCp,cntPt = lEp,rotResPt;
    if(!isLineFont){
        cntPt = lSp;
    }
    double A = 90;
    RotDir rotDir;
    GeomCalculate gocal;
    ///以连接点为中心旋转圆心 ,旋转方向是圆弧旋向的反方向 .
    if(isLineFont){
        gocal.SegmentAngle(lEp,lSp,lAgl);
        RotDir cpRotDir = arc._arcRot == CLOCKWISE ? ANTICLOCKWISE : CLOCKWISE;
        gocal.PointAroundFixedRotate(arcCp,cntPt,A,cpRotDir,rotResPt);
        gocal.SegmentAngle(cntPt,rotResPt,aAgl);
        gocal.TriangleRotDirection(cntPt,lSp,rotResPt,rotDir);
        gocal.TwoAngleIncludedAngle(lAgl,aAgl,rotDir,angle);
    }
    else{
        gocal.PointAroundFixedRotate(arcCp,cntPt,A,arc._arcRot,rotResPt);
        gocal.SegmentAngle(cntPt,rotResPt,aAgl);
        gocal.SegmentAngle(lSp,lEp,lAgl);
        gocal.TriangleRotDirection(cntPt,rotResPt,lEp,rotDir);
        gocal.TwoAngleIncludedAngle(aAgl,lAgl,rotDir,angle);
    }
}
void CalculateAngle::AdjoinArcArcAngle(const DefArc& arc1,
                                       const DefArc& arc2,
                                       double& angle)
{
    ///需要保证线段圆弧首尾相连 ,这种情况angle不可能大于180度.
    ///这里计算规则是前一圆弧角度改变 ,后一圆弧角度不改变.
    double A = 90;
    GeomCalculate gocal;
    Point rotPt1,rotPt2;
    RotDir rotDir2 = arc2._arcRot == CLOCKWISE ? ANTICLOCKWISE : CLOCKWISE;
    gocal.PointAroundFixedRotate(arc1._arcCp,arc1._arcEp,
                                 A,arc1._arcRot,rotPt1);
    gocal.PointAroundFixedRotate(arc2._arcCp,arc1._arcEp,
                                 A,rotDir2,rotPt2);
    double angle1,angle2;
    gocal.SegmentAngle(arc1._arcEp,rotPt1,angle1);
    gocal.SegmentAngle(arc1._arcEp,rotPt2,angle2);    RotDir rotDir;
    gocal.TriangleRotDirection(arc1._arcEp,rotPt1,rotPt2,rotDir);
    gocal.TwoAngleIncludedAngle(angle1,angle2,rotDir,angle);
}
void CalculateAngle::AdjoinElementAngle(const DefElem& ele1,
                                        const DefElem& ele2,
                                        double& angle)
{
    if(ele1._elemType == LINETYPE){
        Point l1Sp = ele1._segment._segSp;
        Point l1Ep = ele1._segment._segEp;
        if(ele2._elemType == LINETYPE){
            AdjoinSegmentSegmentAngle(l1Sp,l1Ep,ele2._segment._segSp,
                                      ele2._segment._segEp,angle);
        }
        else if(ele2._elemType == ARCTYPE){
            AdjoinSegmentArcAngle(l1Sp,l1Ep,ele2._arc,
                                  true,angle);
        }
    }
    else if(ele1._elemType == ARCTYPE){
        if(ele2._elemType == LINETYPE){
            AdjoinSegmentArcAngle(ele2._segment._segSp,
                                  ele2._segment._segEp,
                                  ele1._arc,false,angle);
        }
        else if(ele2._elemType == ARCTYPE){
            AdjoinArcArcAngle(ele1._arc,ele2._arc,angle);
        }
    }
}
void CalculateAngle::AdjoinSegSegAngleBaseRot(const Point& l1Sp,
                                              const Point& l1Ep,
                                              const Point& l2Sp,
                                              const Point& l2Ep,
                                              RotDir rotDir,
                                              double& angle)
{
    GeomCalculate gocal;
    double angle1,angle2;
    gocal.SegmentAngle(l1Ep,l1Sp,angle1);
    gocal.SegmentAngle(l1Ep,l2Ep,angle2);
    ///约定以l1Sp,l1Ep作为起始角.
    gocal.TwoAngleIncludedAngle(angle2,angle1,rotDir,angle);
}
void CalculateAngle::AdjoinSegArcAngleBaseRot(const Point& lSp,
                                              const Point& lEp,
                                              const DefArc& arc,
                                              bool isLineFont,
                                              RotDir rotDir,
                                              double& angle)
{
    Point rotPt;
    GeomCalculate gocal;
    double angle1,angle2;
    if(isLineFont){
        RotDir cpRotDir = arc.ArcRot() == CLOCKWISE ?
                    ANTICLOCKWISE : CLOCKWISE;        gocal.SegmentAngle(lEp,lSp,angle2);
        gocal.PointAroundFixedRotate(arc.ArcCenterPt(),arc.ArcStartPt(),
                                     90,cpRotDir,rotPt);
        gocal.SegmentAngle(lEp,rotPt,angle1);
    }
    else{
        gocal.PointAroundFixedRotate(arc.ArcCenterPt(),arc.ArcEndPt(),
                                     90,arc.ArcRot(),rotPt);
        gocal.SegmentAngle(lSp,rotPt,angle1);
        gocal.SegmentAngle(lSp,lEp,angle2);
    }
    gocal.TwoAngleIncludedAngle(angle1,angle2,rotDir,angle);
}
void CalculateAngle::AdjoinArcArcAngleBaseRot(const DefArc& arc1,
                                              const DefArc& arc2,
                                              RotDir rotDir,
                                              double& angle)
{
    GeomCalculate gocal;
    Point rotPt1,rotPt2;
    gocal.PointAroundFixedRotate(arc1.ArcCenterPt(),
                                 arc1.ArcEndPt(),
                                 90,arc1.ArcRot(),rotPt1);
    RotDir cpRotDir = arc2.ArcRot() == CLOCKWISE ?
                ANTICLOCKWISE : CLOCKWISE;
    gocal.PointAroundFixedRotate(arc2.ArcCenterPt(),
                                 arc2.ArcStartPt(),
                                 90,cpRotDir,rotPt2);
    AdjoinSegSegAngleBaseRot(rotPt1,arc1.ArcEndPt(),
                             arc1.ArcEndPt(),rotPt2,
                             rotDir,angle);
}
void CalculateAngle::AdjoinElementAngleBaseRot(const DefElem& ele1,
                                               const DefElem& ele2,
                                               RotDir rotDir,
                                               double& angle)
{
    switch (ele1.ElemeType()) {
    case LINETYPE:
        switch (ele2.ElemeType()) {
        case LINETYPE:
            AdjoinSegSegAngleBaseRot(ele1.ElemSeg().SegStartPt(),
                                     ele1.ElemSeg().SegEndPt(),
                                     ele2.ElemSeg().SegStartPt(),
                                     ele2.ElemSeg().SegEndPt(),
                                     rotDir,angle);
            break;
        case ARCTYPE:
            AdjoinSegArcAngleBaseRot(ele1.ElemSeg().SegStartPt(),
                                     ele1.ElemSeg().SegEndPt(),
                                     ele2.ElemArc(),true,
                                     rotDir,angle);
            break;
        default:
            break;
        }
        break;
    case ARCTYPE:
        switch (ele2.ElemeType()) {
        case LINETYPE:
            AdjoinSegArcAngleBaseRot(ele2.ElemSeg().SegStartPt(),
                                     ele2.ElemSeg().SegEndPt(),                                     ele1.ElemArc(),false,
                                     rotDir,angle);
            break;
        case ARCTYPE:
            AdjoinArcArcAngleBaseRot(ele2.ElemArc(),
                                     ele2.ElemArc(),
                                     rotDir,angle);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}