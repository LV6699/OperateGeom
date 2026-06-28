#ifndef CURVEENVE_H
#define CURVEENVE_H
#include"../../../CommonFile/Header.h"
class CAMALGORITHM_EXPORT CurveEnve
{
public:
    CurveEnve(){}
    void SetArcParameters(const Point& sp,const Point& ep,
                          const Point& cp,double R,
                          bool iscw,DefArc& arc);
    void GetSigmentEnvelopeLoop(const Point& sigSp,
                                const Point& sigEp,
                                double oftD,
                                DefLoop& loop);
    void GetSigmentSpecifyEnvelope(const Point& lSp,
                                   const Point& lEp,
                                   bool leftward,
                                   double oftD,
                                   DefLoop& loop);
    void GetNotExistEquArcStructure(const DefArc& arc,
                                    const DefArc &equArc,
                                    double oftD,
                                    DefArc& arc1,
                                    DefArc& arc2);
    void GetArcEnvelopeLoop(const DefArc& arc,
                            double oftD,
                            DefLoop& loop);
    void GetArcSpecifyEnvelope(const DefArc& arc,
                               bool leftward,
                               double oftD,
                               DefLoop& loop);
    void GetEleEnvelopeLoop(const DefElem& ele,
                            double oftD,
                            DefLoop& loop);
    void GetEleSpecifyEnvelope(const DefElem& ele,
                               bool leftward,
                               double oftD,
                               DefLoop& loop);
};
#endif // CURVEENVE_H
