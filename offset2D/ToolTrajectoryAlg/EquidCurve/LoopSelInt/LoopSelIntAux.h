#ifndef LOOPSELINTAUX_H
#define LOOPSELINTAUX_H
#include "../../CommonFile/DataStructure.h"
class LoopSelIntAux 
{
public:
    LoopSelIntAux(){}
    Ofte::DistInOut CurveToLoopDistType(const ofts::DefElem& c,const size_t& id,
                                        bool isOpen,const ofts::DefLoop& loop,double R);

    void DelInvalidCntCurve(const std::vector<ofts::DefElem>& cs);

    void DeleContCurves(std::vector<ofts::DefElem>& cs);
    bool DelInvalidClipCurve(const ofts::DefLoop& oloop,
                             const numst::IntParam& ipa,
                             double R,bool isOpen,
                             std::vector<ofts::DefElem>& cs);

    void DeleDoubCntCurves(std::vector<ofts::DefElem>& cs);

    void BuildCurveToLoops(const ofts::DefLoop& inloop,bool isOpen,double err,
                           std::vector<ofts::DefLoop>& loops);

    void LoopMaxOutContour(const ofts::DefLoop& inloop,ofts::DefLoop& loop);
};
#endif // LOOPSELINTAUX_H
