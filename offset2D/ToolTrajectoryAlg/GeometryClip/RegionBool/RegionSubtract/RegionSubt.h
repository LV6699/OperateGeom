#ifndef REGIONSUBT_H
#define REGIONSUBT_H
#include "../AuxCalculate/ClipStruct.h"
class RegionSubt
{
public:
    RegionSubt(){}
    RegionSubt(const std::vector<ofts::DefLoop>& s,
               const std::vector<ofts::DefLoop>& c,clst::FillRule rule) :
        _sloops(s),_cloops(c),_rule(rule){}
    void OperateInput();
    void Exicute();
  
public: 
    clst::FillRule _rule;
    std::vector<ofts::DefLoop> _sloops;
    std::vector<ofts::DefLoop> _cloops;
    std::vector<ofts::DefLoop> _scl_ls;
    std::vector<ofts::DefLoop> _ccl_ls;
};
#endif // REGIONSUBT_H