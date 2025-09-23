#ifndef GEOMETRYDISTANCE_H
#define GEOMETRYDISTANCE_H
#include"../../CommonFile/Header.h"
class GeomDistance
{
public:
    GeomDistance(){}
    ///需要保证点在圆弧上 .
    void PointToArcEndpointDistance(DefArc arc,
                                    const Point& P,
                                    bool isCloseSp,
                                    double& dis);
    void PointToEleEndpointDistance(const DefElem &ele,
                                    const Point& P,
                                    bool isCloseSp,
                                    double& dis);
    double PointToSegMaxDistance(const Point &P,
                                 const Point &lSp,
                                 const Point &lEp);
    double PointToArcMaxDistance(const Point &P,
                                 const DefArc &arc);
    double PointToCircleMaxDistance(const Point &P,
                                    const DefCircle &circle);
    void PointToSegmentDistance(const Point &P,
                                const Point &lSp,
                                const Point &lEp,
                                double &distance);
    void PointToArcDistance(const Point &P,
                            const DefArc &arc,
                            double &distance);
    void PointToCircleDistance(const Point &P,
                               const Point &cp,
                               double radius,
                               double &distance);
    void ElementEndpointDistance(const Point &ele1Sp,
                                 const Point &ele1Ep,
                                 const Point &ele2Sp,
                                 const Point &ele2Ep,
                                 double &distance);
    void SegmentSegmentDistance(const Point &l1Sp,
                                const Point &l1Ep,
                                const Point &l2Sp,
                                const Point &l2Ep,
                                double &distance);
    void SegmentArcDistance(const Point &lSp,
                            const Point &lEp,
                            const DefArc &arc,
                            double &distance);
    void SegmentCircleDistance(const Point &lSp,                               const Point &lEp,
                               const Point &cp,
                               double R,
                               double &distance);
    void ArcExtrToArcDistance(const DefArc &arc1,
                              const DefArc &arc2,
                              double &distance);
    void ArcCircleDistance(const DefArc &arc,
                           const Point &cp,
                           double R,
                           double &distance);
    void ArcArcDistance(const DefArc &arc1,
                        const DefArc &arc2,
                        double &distance);
    void PointToElementDistance(const Point& P,
                                const DefElem &ele,
                                double& distance);
    void ElementToElementDistance(const DefElem &ele1,
                                  const DefElem &ele2,
                                  double &distance);
    void PointToElemMaxDistance(const Point &P,
                               const DefElem &ele,
                               double &distance);
    void PointToLoopMaxDistance(const DefLoop &loop,
                                const Point& P,
                                double& distance);
    void PointToLoopDistance(const DefLoop &loop,
                             const Point& P,
                             double& distance);
    void ElementToLoopDistance(const DefLoop& loop,
                               const DefElem& ele,
                               double& distance);
    void LoopToLoopDistance(const DefLoop& loop1,
                            const DefLoop& loop2,
                            double& distance);
};
#endif // GEOMETRYDISTANCE_H