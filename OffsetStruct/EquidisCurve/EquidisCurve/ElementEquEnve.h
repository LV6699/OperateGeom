#ifndef ELEMENTEQUIDENVE_H
#define ELEMENTEQUIDENVE_H
#include"../../CommonFile/Header.h"
class CAMALGORITHM_EXPORT ElementEquEnve
{
public:
    ElementEquEnve(){}
    void SetArcParameters(const Point& sp,const Point& ep,
                          const Point& cp,double R,
                          RotDir rotDir,DefArc& arc);
    void GetSigmentEnvelopeLoop(const Point& sigSp,
                                const Point& sigEp,
                                double offsetVal,
                                DefLoop& loop);
    void GetSigmentSpecifyEnvelope(const Point& lSp,
                                   const Point& lEp,
                                   ProfType proType,
                                   double offsetVal,
                                   DefLoop& loop);
    void GetNotExistEquArcStructure(const DefArc& arc,
                                    const DefArc &equArc,
                                    double offsetVal,
                                    DefArc& arc1,
                                    DefArc& arc2);
    void GetArcEnvelopeLoop(const DefArc& arc,
                            double offsetVal,
                            DefLoop& loop);
    void GetArcSpecifyEnvelope(const DefArc& arc,
                               ProfType proType,
                               double offsetVal,
                               DefLoop& loop);
    void GetEleEnvelopeLoop(const DefElem& ele,
                            double offsetVal,
                            DefLoop& loop);
    void GetEleSpecifyEnvelope(const DefElem& ele,
                               ProfType proType,
                               double offsetVal,
                               DefLoop& loop);
};
#endif // ELEMENTEQUIDENVE_H