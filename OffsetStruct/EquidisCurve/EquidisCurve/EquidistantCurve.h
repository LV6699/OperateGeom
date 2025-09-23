#ifndef EquidistantCurve_H
#define EquidistantCurve_H
#include"../../CommonFile/Header.h"
class CAMALGORITHM_EXPORT EquidistantCurve
{
public:
    EquidistantCurve(){}
    void EquidistantArc(const DefArc& oriArc,
                        const double& offsetVal,
                        const ProfType loopRot,
                        bool isSetRadius,
                        bool &isExiEquArc,
                        DefArc& equArc);
    void EquidistantSegment(const Point& oriSigSp,
                            const Point& oriSigEp,
                            const double &offsetVal,
                            const ProfType prof,
                            Point& lSp,
                            Point& sigEp);
    void EquidistantCircle(const Point& cp,double radius,
                           ProfType proType,double offsetVal,
                           DefCircle& circle);
    bool IsExistEquElement(const DefElem& ele,
                           ProfType proType,
                           double value);
    void EquidistantElement(const DefElem& ele,
                            double offsetVal,
                            const ProfType prof,
                            bool& isExiEquiEle,
                            DefElem& equEle);
    void SpecifyChangeRadiusArc(const DefArc& arcData,
                                double val,bool isLarger,
                                DefArc &arc);
};
#endif // EquidistantCurve_H