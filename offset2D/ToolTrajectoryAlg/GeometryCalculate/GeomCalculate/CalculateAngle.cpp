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
    bool iscw;
    gocal.TriangleRotDirection(l1Ep,l1Sp,l2Ep,iscw);
    gocal.TwoAngleIncludedAngle(angle1,angle2,iscw,angle);
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
    gocal.TwoAngleIncludedAngle(angle1,angle2,isCW,angle);
}
void CalculateAngle::AdjoinSegmentSegmentAngle(const Point& l1Sp,
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
    bool iscw;
    gocal.TriangleRotDirection(l1Ep,l1Sp,l2Ep,iscw);
    gocal.TwoAngleIncludedAngle(angle1,angle2,iscw,angle);
}
void CalculateAngle::CalculateSigArcAngle(const Point& lSp,
                                          const Point& lEp,
                                          const DefArc& arc,
                                          bool isLineFont,
                                          double& angle)
{
    ///需要保证线段圆弧首尾相连,这种情况angle 不可能大于180 度.
    ///平移线段,使得线段上与圆弧连接的点与圆心重合(只需要平移线段一次),
    ///或把连接圆心和连接点线段的旋转线段平移至切点(只需要平移旋转线段一次).
    GeomCalculate gocal;
    double lAgl,aAgl;
    gocal.SegmentAngle(lSp,lEp,lAgl);
    Point arcCp = arc._arcCp,cntPt = lEp,rotResPt;
    if(!isLineFont){
        cntPt = lSp;
    }
    double A = 90;
    ///以连接点为中心旋转圆心,旋转方向是圆弧旋向的反方向.
    bool iscw = !arc.IsCW();
    gocal.PointAroundFixedRotate(arcCp,cntPt,A,iscw,rotResPt);
    gocal.SegmentAngle(cntPt,rotResPt,aAgl);
    double R;
    gocal.TwoPointDistance(arcCp,cntPt,R);
    bool iscw1;
    if(isLineFont){
        gocal.TriangleRotDirection(cntPt,lEp,rotResPt,iscw1);
        gocal.TwoAngleIncludedAngle(lAgl,aAgl,iscw,angle);
    }
    else {
        gocal.TriangleRotDirection(cntPt,rotResPt,lSp,iscw1);
        gocal.TwoAngleIncludedAngle(aAgl,lAgl,iscw,angle);
    }
}
void CalculateAngle::AdjoinSegmentArcAngle(const Point& lSp,
                                           const Point& lEp,
                                           const DefArc& arc,
                                           bool isLineFont,
                                           double& angle)
{
    ///这里计算规则是前一线段或圆弧角度改变,后一线段或圆弧角度不改变
    double lAgl,aAgl;
    Point arcCp = arc._arcCp,cntPt = lEp,rotResPt;
    if(!isLineFont){
        cntPt = lSp;
    }
    double A = 90;
    bool iscw;
    GeomCalculate gocal;
    ///以连接点为中心旋转圆心,旋转方向是圆弧旋向的反方向.
    if(isLineFont){
        gocal.SegmentAngle(lEp,lSp,lAgl);
        bool iscw1 = !arc._isCW;
        gocal.PointAroundFixedRotate(arcCp,cntPt,A,iscw1,rotResPt);
        gocal.SegmentAngle(cntPt,rotResPt,aAgl);
        gocal.TriangleRotDirection(cntPt,lSp,rotResPt,iscw);
        gocal.TwoAngleIncludedAngle(lAgl,aAgl,iscw,angle);
    }
    else{
        gocal.PointAroundFixedRotate(arcCp,cntPt,A,arc._isCW,rotResPt);
        gocal.SegmentAngle(cntPt,rotResPt,aAgl);
        gocal.SegmentAngle(lSp,lEp,lAgl);
        gocal.TriangleRotDirection(cntPt,rotResPt,lEp,iscw);
        gocal.TwoAngleIncludedAngle(aAgl,lAgl,iscw,angle);
    }
}
void CalculateAngle::AdjoinArcArcAngle(const DefArc& arc1,
                                       const DefArc& arc2,
                                       double& angle)
{
    ///需要保证线段圆弧首尾相连,这种情况angle 不可能大于180 度.
    ///这里计算规则是前一圆弧角度改变,后一圆弧角度不改变.
    double A = 90;
    GeomCalculate gocal;
    Point rotPt1,rotPt2;
    bool iscw2 = !arc2._isCW;
    gocal.PointAroundFixedRotate(arc1._arcCp,arc1._arcEp,
                                 A,arc1._isCW,rotPt1);
    gocal.PointAroundFixedRotate(arc2._arcCp,arc1._arcEp,
                                 A,iscw2,rotPt2);
    double angle1,angle2;
    gocal.SegmentAngle(arc1._arcEp,rotPt1,angle1);
    gocal.SegmentAngle(arc1._arcEp,rotPt2,angle2);
    bool iscw;
    gocal.TriangleRotDirection(arc1._arcEp,rotPt1,rotPt2,iscw);
    gocal.TwoAngleIncludedAngle(angle1,angle2,iscw,angle);
}
void CalculateAngle::AdjoinElementAngle(const DefElem& ele1,
                                        const DefElem& ele2,
                                        double& angle)
{
    if(ele1._type == LINETYPE){
        Point l1Sp = ele1._segment._segSp;
        Point l1Ep = ele1._segment._segEp;
        if(ele2._type == LINETYPE){
            AdjoinSegmentSegmentAngle(l1Sp,l1Ep,ele2._segment._segSp,
                                      ele2._segment._segEp,angle);
        }
        else if(ele2._type == ARCTYPE){
            AdjoinSegmentArcAngle(l1Sp,l1Ep,ele2._arc,
                                  true,angle);
        }
    }
    else if(ele1._type == ARCTYPE){
        if(ele2._type == LINETYPE){
            AdjoinSegmentArcAngle(ele2._segment._segSp,
                                  ele2._segment._segEp,
                                  ele1._arc,false,angle);
        }
        else if(ele2._type == ARCTYPE){
            AdjoinArcArcAngle(ele1._arc,ele2._arc,angle);
        }
    }
}
void CalculateAngle::AdjoinSegSegAngleBaseRot(const Point& l1Sp,
                                              const Point& l1Ep,
                                              const Point& l2Sp,
                                              const Point& l2Ep,
                                              bool iscw,
                                              double& angle)
{
    GeomCalculate gocal;
    double angle1,angle2;
    gocal.SegmentAngle(l1Ep,l1Sp,angle1);
    gocal.SegmentAngle(l1Ep,l2Ep,angle2);
    ///约定以l1Sp,l1Ep 作为起始角.
    gocal.TwoAngleIncludedAngle(angle2,angle1,iscw,angle);
}
void CalculateAngle::AdjoinSegArcAngleBaseRot(const Point& lSp,
                                              const Point& lEp,
                                              const DefArc& arc,
                                              bool isLineFont,
                                              bool iscw,
                                              double& angle)
{
    Point rotPt;
    GeomCalculate gocal;
    double angle1,angle2;
    if(isLineFont){
        gocal.SegmentAngle(lEp,lSp,angle2);
        gocal.PointAroundFixedRotate(arc.GetCp(),arc.GetSp(),
                                     90,!arc.IsCW(),rotPt);
        gocal.SegmentAngle(lEp,rotPt,angle1);
    }
    else{
        gocal.PointAroundFixedRotate(arc.GetCp(),arc.GetEp(),
                                     90,arc.IsCW(),rotPt);
        gocal.SegmentAngle(lSp,rotPt,angle1);
        gocal.SegmentAngle(lSp,lEp,angle2);
    }
    gocal.TwoAngleIncludedAngle(angle1,angle2,iscw,angle);
}
void CalculateAngle::AdjoinArcArcAngleBaseRot(const DefArc& arc1,
                                              const DefArc& arc2,
                                              bool iscw,
                                              double& angle)
{
    GeomCalculate gocal;
    Point rotPt1,rotPt2;
    gocal.PointAroundFixedRotate(arc1.GetCp(),arc1.GetEp(),
                                 90,arc1.IsCW(),rotPt1);
    bool iscw1 = !arc2.IsCW();
    gocal.PointAroundFixedRotate(arc2.GetCp(),
                                 arc2.GetSp(),
                                 90,iscw1,rotPt2);
    AdjoinSegSegAngleBaseRot(rotPt1,arc1.GetEp(),
                             arc1.GetEp(),rotPt2,
                             iscw,angle);
}
void CalculateAngle::AdjoinElementAngleBaseRot(const DefElem& ele1,
                                               const DefElem& ele2,
                                               bool iscw,
                                               double& angle)
{
    switch (ele1.Type()) {
    case LINETYPE:
        switch (ele2.Type()) {
        case LINETYPE:
            AdjoinSegSegAngleBaseRot(ele1.GetSeg().GetSp(),
                                     ele1.GetSeg().GetEp(),
                                     ele2.GetSeg().GetSp(),
                                     ele2.GetSeg().GetEp(),
                                     iscw,angle);
            break;
        case ARCTYPE:
            AdjoinSegArcAngleBaseRot(ele1.GetSeg().GetSp(),
                                     ele1.GetSeg().GetEp(),
                                     ele2.GetArc(),true,
                                     iscw,angle);
            break;
        default:
            break;
        }
        break;
    case ARCTYPE:
        switch (ele2.Type()) {
        case LINETYPE:
            AdjoinSegArcAngleBaseRot(ele2.GetSeg().GetSp(),
                                     ele2.GetSeg().GetEp(),
                                     ele1.GetArc(),false,
                                     iscw,angle);
            break;
        case ARCTYPE:
            AdjoinArcArcAngleBaseRot(ele2.GetArc(),
                                     ele2.GetArc(),
                                     iscw,angle);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
