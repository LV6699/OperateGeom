#ifndef EQUIDLOOPAUX_H
#define EQUIDLOOPAUX_H
#include"../../CommonFile/DataStructure.h"
class EquidLoopAux
{
public:
    EquidLoopAux(){}
    void SetArcParameters(const ofts::Point& sp,const ofts::Point& ep,
                          const ofts::Point& cp,bool iscw,double R,ofts::DefArc& arc)
    {
        arc.SetSp(sp);arc.SetEp(ep);
        arc.SetCp(cp);arc.SetR(R);arc.SetCW(iscw);
    }
    bool AdjoinEndConnect(const ofts::DefLoop& oloop,double R,
                          const numst::IntParam& ipa,
                          ofts::DefElem& c1,ofts::DefElem& c2,
                          std::vector<ofts::DefElem>& rcs);
    void NotExistEquidCurve(const ofts::DefArc& oArc, double R,
                            ofts::DefArc& arc1, ofts::DefArc& arc2);
    void GetConnectArc(const ofts::DefElem& prec, const ofts::DefElem& c1,
                       const ofts::DefElem& c2, const ofts::Point &cp,
                       double R,bool leftward,ofts::DefArc& arc);

    void SetCurveEndptCase(const ofts::Point& p,const ofts::Point& sp1,
                           ofts::DefElem& c1,ofts::DefElem& c2,
                           std::vector<ofts::DefElem> &cs)
    {
        c1.ChangeEndpt(p,true);
        c2.ChangeEndpt(p,false);
        if(!sp1.IsEqual(p,PreErr5_10)){
            cs.emplace_back(c1);
        }
    }
    void ResetInvalidCurves(std::vector<ofts::DefElem>& cs,Ofte::EndType etype);
    void ChangeEndpointCase(ofts::DefElem& c1,ofts::DefElem& c2,
                            const ofts::Point& ep1,const ofts::Point& sp2,
                            std::vector<ofts::DefElem>& cs);
    void ResetCoinAgjCurve(std::vector<ofts::DefElem>& curves);

    void OpenLoopEndConnect(ofts::DefLoop& oloop,ofts::DefLoop& loop1,
                            ofts::DefLoop& loop2,double R);
};
#endif // EQUIDLOOPAUX_H
