#ifndef GEOMETRICCAL_H
#define GEOMETRICCAL_H
#include"../../CommonFile/Header.h"
typedef Point Vector;
class CAMALGORITHM_EXPORT GeomCalculate
{
public:
    GeomCalculate(){}
    void SegmentAngle(const Point& lSp, const Point& lEp,
                      double& angle);
    double SegmentAngle3D(const Point sp,const Point& ep);
    void SegmentAngleWithXY(const Point& sp, const Point& ep,
                            double& angle);
    double SegmentSlope(const Point& lSp, const Point& lEp);
    void IsSigmentsParallel(const Point& l1Sp,const Point& l1Ep,
                            const Point& l2Sp,const Point& l2Ep,
                            bool& isPara);
    void TwoPointDistance(const Point& P1, const Point& P2, double &dis);
    double TwoPointDistance(const Point& P1, const Point& P2);
    double TwoPointDistance2D(const Point& P1, const Point& P2);
    double TwoPointDistance3D(const Point& P1,const Point& P2);
    double CalculateArcRadius(const DefArc& arc);
    double SegmentLenth(const DefSeg& seg);
    bool IsPntDisGreaterVal(const Point& P1, const Point& P2,
                            double val);
    void CloseArcEndpointAngle(const DefArc& arcData, double step,
                               bool isCloseEndPnt,
                               double& closeAgl);
    void InitialArcRadius(DefArc& arc);
    void DiscreteLineToPoint(const Point& lSp,
                             const Point& lEp,
                             double stepVal,
                             vector<Point>& vPoint);
    void DiscreteLineToPoint3D(const Point& lsp,
                               const Point& lep,
                               double stepVal,
                               vector<Point>& points);
    void DiscreteArcToPoint(const DefArc &arc,
                            double stepVal,
                            vector<Point>& points);
    void DiscreteCircleToPoint(const Point &cp,
                               const double &R,                               RotDir rot, double stepVal,
                               vector<Point>& vPoint);
    void TranslateElemToPoint(const DefElem& ele,
                              bool isStepAngle,
                              double segStep, double arcStep,
                              vector<Point>&vPoint);
    void IsArcOstBefAftSameDir(const Point& origArcSp,
                               const Point& origArcEp,
                               const Point& arcSp,
                               const Point& arcEp,
                               bool &isSameDir);
    bool IsLegalSigment(const Point& lSp,const Point& lEp);
    bool IsLegalArc(const DefArc& arc);
    void IsValidElem(const DefElem& ele,bool& isVal);
    void PointOnLineProjection(const Point &pnt,const Point &lSp,
                               const Point &lEp,Point& objPnt);
    void CalLineParameters(const Point& lSp, const Point& lEp,
                           double linePar[3]);
    void LineParameters(const Point& lSp, const Point& lEp,
                        double& A,double& B,double& C);
    double AngleMidAngle(double sAng, double eAng, RotDir rotDir);
    void ArcMidpoint(const DefArc& arc,Point& midPnt);
    void CalAngleBetweenTwoVectors(const Point& l1Sp,
                                   const Point& l1Ep,
                                   const Point& l2Sp,
                                   const Point& l2Ep,
                                   double& sAgl,
                                   double& eTheta,
                                   double& angle);
    void TwoPointMidpoint(const Point& P1,
                          const Point& P2,Point& P);
    void TwoPointMidpoint2D(const Point& P1,
                            const Point& P2,Point& P);
    void GetClosedFixedPnt(const Point&fixedPnt,
                           const Point &varPnt,
                           const double dis,
                           Point&endPnt);
    double dot(const Vector& a,const Vector& b);
    double OuterProduct(Vector& a,const Vector& b);
    void CloseSegmentEndpoint(const Point& lSp,
                              const Point& lEp,
                              double stepVal,
                              bool isCloseSp,
                              Point& P);
    void ArcEndpointAngle(const Point &sP,
                          const Point &eP,                          const Point &cP,
                          const RotDir &rot,
                          double& startAgl,
                          double& endAgl);
    void ArcEndpointAngle(const DefArc& arc,
                          double& startAgl,
                          double& endAgl);
    void ArcCenterAngle(const DefArc& arc,
                        double& angle);
    void PointToLineDistance(const Point &lSp,
                             const Point &lEp,
                             const Point &P,
                             double& dis);
    void CircleTangentEquation(const Point &cirCdt1,
                               const double &radius1,
                               const Point &P,
                               double(&lines)[3]);
    void IsIdenticalPoint(const Point& p1,const Point& p2,double pre,
                          bool& isSame);
    bool IsIdenticalPoint(const Point& p1,const Point& p2,
                          double pre);
    bool IsIdenticalPoint2D(const Point& p1,const Point& p2,
                            double pre);
    void IsIdenticalPoint2D(const Point& p1, const Point& p2,
                            double pre, bool &isSame);
    void TwoPointAnotherPoint(const Point& p1,
                              const Point& p2,
                              const Point &cmpP, Point& p);
    void AngleIsBetweenAngles(double startAgl,
                              double endAgl,
                              RotDir rot,
                              double theta,
                              double pre,
                              bool& isIn);
    void IsPointInFan(const DefArc& arcData,
                      const Point& objPnt,
                      bool& isInFan);
    void AngleInWhichQuadrant(const double& angle,
                              AngleWhichQuadrant whichQuadrant);
    void TwoAngleIncludedAngle(const double& startAgl,
                               const double& endAgl,
                               const RotDir& rot,
                               double& aglAbs);
    void CalculateArcLength(DefArc arc,
                            double& len);
    double ElementLenth(const DefElem& ele);
    void GetSameDirCloseRayExtremePnt(const Point& raySp,
                                      const Point& rayEp,                                      const Point& pnt1,
                                      const Point& pnt2,
                                      bool isCloseSp,
                                      Point& objPnt);
    void TriangleRotDirection(const Point& P1,const Point& P2,
                              const Point& P3,RotDir& rot);
    void ElementCloseEndpoint(const DefElem& ele,
                              bool isCloseEp,
                              double stepVal,
                              Point &P);
    void PointAroundFixedRotate(const Point& p,
                                const Point& fix,
                                double A, RotDir isCW,
                                Point& pt);
    void PointRotate2D(const Point& p,
                       const Point& fix,
                       double A, bool isCW,
                       Point& pt);
    void SegmentBisectorVertical(const Point&lSp, const Point&lEp,
                                 double& A, double& B, double& C);
    void SegmentVerticalBisector(const Point& sp, const Point& ep,
                                 double& A, double& B, double& C);
    double ArcBowHeight(const DefArc &arc);
    double ArcPointTangentAngle(const DefArc &arc,const Point &P);
    bool ThreePointCollinear(const Point& p0,const Point& p1,
                             const Point& p2, double err);
    void SimplePointsBaseLine(double err,std::vector<Point>& points);
};
#endif // GEOMETRICCAL_H