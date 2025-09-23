#ifndef GEOMLOGICCAL_H
#define GEOMLOGICCAL_H
#include"../../CommonFile/Header.h"
class CAMALGORITHM_EXPORT GeometryLogic
{
public:
    GeometryLogic(){}
    void IsLineCircleTangency(const Point& lSp,
                              const Point& lEp,
                              const Point& CP,
                              double R, double pre,
                              bool& isTan);
    void IsCircleCircleTangency(const Point& cp1,
                                double R1,
                                const Point& cp2,
                                double R2,
                                double pre,
                                bool& isTan);
    bool IsElementTangency(const DefElem& ele1,
                           const DefElem& ele2, double pre);
    void IsTwoArcEndPntSame(const DefArc &arc1,
                            const DefArc &arc2,
                            double endptPre,
                            GeomIntRes &intRes);
    void ClosetPointFromTwoPoint(const Point& sp,
                                 const Point& p1,
                                 const Point& p2,
                                 Point& P);
    void DistancePointFromPoint(const Point& sp,
                                const Point& p1,
                                const Point& p2,
                                bool isClose,
                                Point& P);
    void SameSegDircetionPoint(const Point& lSp,
                               const Point& lEp,
                               const Point& P1,
                               const Point& P2,
                               Point& P);
    void AnotherFromTwoPoint(const Point& P1,
                             const Point& P2,
                             const Point& comP,
                             Point& P);
    void CloseArcEndpointBaseAngle(DefArc arc,
                                   bool isCloseEp,
                                   double stepAgl,
                                   Point &P);
    void CloseArcEndpointBaseLenth(DefArc arc,
                                   bool isCloseEp,
                                   double stepVal,
                                   Point &P);    void ApproachCurveEndpoint(const DefElem& ele,double stepVal,
                               bool isAngle,bool isNearSp,
                               Point& P);
    void GetObjFromTwoPnt(const DefElem&ele,
                          int pntNum,
                          const Point& P1,
                          const Point& P2,
                          bool isCloseSp,
                          Point& P);
    void IsCircleStructureLoop(const DefLoop& loop,
                               bool isMatchArc,
                               bool& isCircle);
    bool IsCircleStructureLoop(const DefLoop& loop,
                               double rErr = PreErr5_10,
                               double cpErr = PreErr5_10);
    void CircleStructureCircle(const DefLoop& loop,DefCircle& circle);
    void CircleArcLoopTransform(bool isTranToArc,
                                DefLoop &loop);
    void DeleteInsideLoop(vector<DefLoop>& vLoop,
                          double intPre, double pre);
    void IsElementLocatedOnLoop(const DefElem& ele,
                                const DefLoop& loop,
                                double pre,
                                bool& matchRes);
    void IsLoopLoopExistCoincideEle(const DefLoop& loop1,
                                    const DefLoop& loop2,
                                    double pre,
                                    bool& matchRes);
    void DiscreteElemToPoint(DefElem ele, bool isAngle, double stepVal,
                             vector<Point> &vPoint);
    void DiscreteCurveToPoint(DefElem ele,
                              double lStep, double cStep,
                              bool isAngle,
                              vector<Point>& points);
    void TranslateLoopIntoPoint(DefLoop loop,
                                double lStep,double cStep,
                                bool isStepAngle,
                                vector<Point>& vPoint);
    void LoopToSegmentLoop(DefLoop loop,
                           double expStep,
                           DefLoop& resLoop);
    void LoopToSegmentLoop(DefLoop loop,
                           double stepVal,
                           bool isStepAngle,
                           DefLoop& resLoop);
    void TranslateBndIslToSegment(double stepVal,
                                  bool isStepAngle,
                                  GeomArea& BILoop);
    void ThirdOrderBezierCurve(vector<Point>& vControlPt,                               DefLoop& loop);
    ///从loop首元素开始正向(起点指向终点方向)计算.
    void LoopSpecifyLenthPosition(const DefLoop& loop,
                                  double lenth, int &index, Point& P);
    void PointToSegmentLoop(const vector<Point>&points, bool isClose, DefLoop& 
loop);
};
#endif // GEOMLOGICCAL_H