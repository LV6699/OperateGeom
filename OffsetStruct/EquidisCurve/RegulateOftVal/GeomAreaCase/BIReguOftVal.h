#ifndef BIREGUOFTVAL_H
#define BIREGUOFTVAL_H
#include"../../../CommonFile/Header.h"
class BIReguOftVal
{
public:
    BIReguOftVal(){}
    void GetLoopReferenceDistance(const DefLoop& loop1,
                                  const DefLoop& loop2,
                                  double refVal,
                                  bool &isExiLess);
    void IsBndIslProbableInt(const DefLoop& bndLop,
                             const vector<DefLoop>& vIslLoop,
                             double offsetVal,
                             bool& isProbInt);
    void RegulateRegionOftValue(const DefLoop& bndLop,
                                const vector<LoopNode> &vIslNode,
                                bool isFirOffset,
                                double firOftVal,
                                double oriOftVal,
                                OffsetParam& oftPara);
};
#endif // BIREGUOFTVAL_H