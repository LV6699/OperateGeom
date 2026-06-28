#ifndef EquidistantCurve_H
#define EquidistantCurve_H
#include"../../../CommonFile/Header.h"
class CAMALGORITHM_EXPORT EquidistantCurve
{
public:
    EquidistantCurve(){}
    void EquidistantArc(const DefArc& oriArc,
                        const double& oftD,
                        bool leftward,
                        bool isSetRadius,
                        bool &isExiEquArc,
                        DefArc& equArc);
    void EquidistantSegment(const Point& oriSigSp,
                            const Point& oriSigEp,
                            const double &oftD,
                            bool leftward,
                            Point& lSp,
                            Point& sigEp);
    void EquidistantCircle(const Point& cp,double radius,
                           bool leftward,double oftD,
                           DefCircle& circle);
    bool IsExistEquElement(const DefElem& ele,
                           bool leftward,
                           double value);
    void EquidistantElement(const DefElem& ele,
                            double oftD,
                            bool leftward,
                            bool& isExiEquiEle,
                            DefElem& equEle);
    void SpecifyChangeRadiusArc(const DefArc& arcData,
                                double val,bool isLarger,
                                DefArc &arc);
};
#endif // EquidistantCurve_H
