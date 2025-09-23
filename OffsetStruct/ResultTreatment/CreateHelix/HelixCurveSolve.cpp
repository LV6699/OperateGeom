#ifndef HELIXCURVESOLVE_H
#define HELIXCURVESOLVE_H
#include"../../CommonFile/Header.h"
class HelixCurveSolve
{
public:
    HelixCurveSolve(){}
    void LoopCutEntryPoint(DefLoop& loop, double helRadius,
                           bool& isReorderEle);
    void ModelPositionRelativePath(const DefLoop& loop,
                                   MillDirction millDir,
                                   bool& isRightPos);
    ///需要保证useDefPt位于element上.
    void PointOnElementAngle(const DefElem& element,
                             const Point& P,
                             RotDir rotDir,
                             double &slope);
    void HelixCircleCenter(const DefElem& element,
                           const Point& posPnt,
                           double helixRdius, bool isCpRightPos,
                           RotDir loopRot, Point& centerPt);
    void HelixRadiusInterfere(const GeomArea& oriLoop,
                              const DefElem& element,
                              const Point& posPnt,
                              double toolRdius,
                              bool isCpRightPos,
                              RotDir loopRot,
                              CylinderHelix &helix);
    void GenerateSpecifyHelixPoint(const Point& helCenter,
                                   double helRadius,
                                   double startAngle,
                                   double perAngHeight,
                                   double totalAngle,
                                   bool isRightHelix,
                                   vector<Point>&vPoint);
};
#endif // HELIXCURVESOLVE_H