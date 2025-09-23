FoiiMZz6iajbDBPrGmS7FVf_rEtH
Page 1#ifndef FITTINGSEGTOARC_H
#define FITTINGSEGTOARC_H
#include"../../../CommonFile/Header.h"
class FittingSegToArc
{
public:
    FittingSegToArc(){}
    void ExicuteFittingSegmentToArc(const DefLoop& loop
                                    double lenLim
                                    double disVar
                                    double angleVar
                                    vector<OffsetStruct::GeomFitting::FittArc> &vFitArc);
};
#endif // FITTINGSEGTOARC_H