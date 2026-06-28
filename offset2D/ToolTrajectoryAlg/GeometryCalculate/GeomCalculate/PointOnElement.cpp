#include "PointOnElement.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if DebugModel
#pragma optimize("", off)
#endif
bool PointOnElement::IsPointOnElement(const DefElem &ele,
                                      const Point& P,
                                      double endPre,
                                      double pre,
                                      bool isMatOnLine,
                                      bool isMatOnCir)
{
    bool result = false;
    switch (ele._type) {
    case LINETYPE:
        IsPointOnSegment(ele._segment.GetSp(),
                         ele._segment.GetEp(),P,endPre,
                         pre,isMatOnLine,result);
        break;
    case ARCTYPE:
        IsPointOnArc(ele._arc,P,endPre,pre,isMatOnCir,result);
        break;
    case CIRCLETYPE:
        IsPtOnCircle(P,ele._circle.GetCp(),
                     ele._circle.R(),pre,
                     result);
        break;
    default:
        break;
    }
    return result;
}
bool PointOnElement::IsPointOnLine(const Point& P1, const Point& P2,
                                   const Point& P, double pre)
{
    Point P_;
    GeomCalculate().PointOnLineProjection(P,P1,P2,P_);
    return P.IsSamePoint2D(P_,pre);
}
void PointOnElement::IsPointOnSegment(const Point& lSp,
                                      const Point& lEp,
                                      const Point& P,
                                      double endPre,
                                      double pre,
                                      bool isMatOnLine,
                                      bool& isOnSeg)
{
    isOnSeg = false;
    if(!IsPointWithinSegment(P,lSp,lEp,endPre))
        return;
    if(isMatOnLine && !IsPointOnLine(lSp,lEp,P,pre))
        return;
    isOnSeg = true;
}
void PointOnElement::IsPointOnArc(const DefArc& arc, const Point& P,
                                  double endPre, double pre,
                                  bool isMatOnCir, bool& isOnArc)
{
    isOnArc = false;
    if(!IsPointWithinCircle(P,arc._arcCp,arc._arcR,pre))
        return;
    if(isMatOnCir){
        double dist = P.DistSquare2D(arc._arcCp);
        if(std::abs(dist - (arc._arcR * arc._arcR)) > pre)
            return;
    }
    if(P.IsSamePoint2D(arc._arcSp,endPre) ||
            P.IsSamePoint2D(arc._arcEp,endPre)){
        isOnArc = true;
        return;
    }
    GeomCalculate gocal;
    double startAgl,endAgl;
    gocal.ArcEndpointAngle(arc._arcSp,arc._arcEp,
                           arc._arcCp,arc._isCW,
                           startAgl,endAgl);
    double theta = std::atan2((P._y - arc._arcCp._y),
                              (P._x - arc._arcCp._x)) * _AlgRad;
    if(theta < 0){
        theta += 360;
    }
    gocal.AngleIsBetweenAngles(startAgl,endAgl,arc._isCW,
                               theta,pre,isOnArc);
}
void PointOnElement::IsPtOnCircle(const Point& P,
                                  const Point& cp,
                                  double R,double pre,
                                  bool& isOnCircle)
{
    double dist = P.DistSquare2D(cp);
    isOnCircle = std::abs(dist - (R * R)) <= pre;
}
