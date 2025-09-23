#ifndef GEOMETRICINT_H
#define GEOMETRICINT_H
#include"../../CommonFile/Header.h"
class GeomIntersect
{
public:
    GeomIntersect(){}
    void CalCircleCircleExiIntPnt(const Point& C1,
                                  const double& R1,
                                  const Point& C2,
                                  const double& R2,
                                  Point& interP1,
                                  Point& interP2);
    void CalculateLineLineInt(const Point &l1Sp,
                              const Point &l1Ep,
                              const Point &l2Sp,
                              const Point &l2Ep,
                              bool &isExiIntPnt,
                              Point &intPnt);
    void CalculateLineParameterInt(double A1, double B1, double C1,
                                   double A2, double B2, double C2,
                                   bool &isExiInt,Point &P);
    void LineParameterCircleInt(double linePara[3], const Point& cp, double R,
    double tanPre, double intPre, GeomIntRes& intRes);
    void CalculateLineCircleInt(const Point& lSp,
                                const Point& lEp,
                                const Point& cp,
                                double radius,
                                double tanPre,
                                double intPre,
                                GeomIntRes& intRes);
    void CalculateLineArcInt(const Point& lSp,
                             const Point& lEp,
                             DefArc arc,
                             double tanPre,
                             double endptPre,
                             double intPre,
                             GeomIntRes& intRes);
    void CalculateCircleCircleInt(const Point &cp1,
                                  double R1,
                                  const Point &cp2,
                                  double R2,
                                  double tanPre,
                                  double intPre,
                                  GeomIntRes &intRes);
    void CalculateSegSegIntersect(const Point& lSp1,
                                  const Point& lEp1,
                                  const Point& lSp2,
                                  const Point& lEp2,
                                  bool isCalParall,
                                  double endptPre,
                                  double intPre,
                                  GeomIntRes& intRes);    void CalculateSegCircleIntersect(const Point& lSp,
                                     const Point& lEp,
                                     const Point& cp,
                                     double R,
                                     double tanPre,
                                     double endptPre,
                                     double intPre,
                                     GeomIntRes &intRes);
    void CalculateArcCircleInt(DefArc arc,
                               const Point &cp,
                               double R,double tanPre,
                               double endptPre,
                               double intPre,
                               GeomIntRes& intRes);
    void CalculateSegmentArcInt(const Point& lSp,
                                const Point& lEp,
                                DefArc arc,
                                double tanPre,
                                double endptPre,
                                double intPre,
                                GeomIntRes& intRes);
    void CalculateArcArcInt(DefArc arc1,
                            DefArc arc2,
                            double tanPre,
                            double endptPre,
                            double intPre,
                            GeomIntRes& intRes);
    void CalculateCurveIntersect(const DefElem& ele1,
                                 const DefElem& ele2,
                                 bool isCalParallel,
                                 double tanPre,
                                 double endptPre,
                                 double intPre,
                                 GeomIntRes& intRes);
    void CalculateEleGeomIntersect(const DefElem &ele1,
                                   const DefElem &ele2,
                                   double tanPre,
                                   double intPre,
                                   GeomIntRes& intRes);
};
#endif // GEOMETRICINT_H