#ifndef REGICLIPAUX_H
#define REGICLIPAUX_H
#include "../AuxCalculate/ClipStruct.h"
class RegiClipAux
{
public:
    RegiClipAux(){}

    bool IsValidWindNum(const clst::FillRule& rule,int wn);
    void OperateInput(const clst::FillRule& rule,
                      const std::vector<ofts::DefLoop>& sloops,
                      const std::vector<ofts::DefLoop>& cloops,
                      std::vector<ofts::DefLoop>& scl_ls,
                      std::vector<ofts::DefLoop>& ccl_ls);

    ofts::Point LoopInternalPt(const ofts::DefLoop& loop);
    int PointWindNum(const ofts::DefLoop& loop,const ofts::Point& P,double bPre);


};
#endif // REGICLIPAUX_H
