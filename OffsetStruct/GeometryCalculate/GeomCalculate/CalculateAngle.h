#ifndef CALCULATEANGLE_H
#define CALCULATEANGLE_H
#include"../../CommonFile/Header.h"
class CalculateAngle
{
public:
    CalculateAngle(){}
    void CalculateSigSigAngle(const Point& l1Sp,
                              const Point& l1Ep,
                              const Point& l2Sp,
                              const Point& l2Ep,
                              double& angle);
    void SegmentSegmentAngle(const Point& l1Sp,
                             const Point& l1Ep,
                             const Point& l2Sp,
                             const Point& l2Ep,
                             bool isCW,
                             double& angle);
    void AdjoinSegmentSegmentAngle(const Point& l1Sp,
                                   const Point& l1Ep,
                                   const Point& l2Sp,
                                   const Point& l2Ep,
                                   double& angle);
    void CalculateSigArcAngle(const Point& lSp,
                              const Point& lEp,
                              const DefArc& arc,
                              bool isLineFont,
                              double& angle);
    void AdjoinSegmentArcAngle(const Point& lSp,
                               const Point& lEp,
                               const DefArc& arc,
                               bool isLineFont,
                               double& angle);
    void AdjoinArcArcAngle(const DefArc& arc1,
                           const DefArc& arc2,
                           double& angle);
    void AdjoinElementAngle(const DefElem& ele1,
                            const DefElem& ele2,
                            double& angle);
    void AdjoinSegSegAngleBaseRot(const Point& l1Sp,
                                  const Point& l1Ep,
                                  const Point& l2Sp,
                                  const Point& l2Ep,
                                  RotDir rotDir,
                                  double& angle);
    void AdjoinSegArcAngleBaseRot(const Point& lSp,
                                  const Point& lEp,
                                  const DefArc& arc,
                                  bool isLineFont,
                                  RotDir rotDir,
                                  double& angle);
    void AdjoinArcArcAngleBaseRot(const DefArc& arc1,                                  const DefArc& arc2,
                                  RotDir rotDir,
                                  double& angle);
    void AdjoinElementAngleBaseRot(const DefElem &ele1,
                                   const DefElem &ele2,
                                   RotDir rotDir,
                                   double& angle);
};
#endif // CALCULATEANGLE_H