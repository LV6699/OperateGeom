#ifndef ORGANIZELOOP_H
#define ORGANIZELOOPE_H
#include"../../CommonFile/DataStructure.h"
class LayoutRegion
{
public:
    LayoutRegion(){}
    void OrganizeLoopToArea(const std::vector<ofts::DefLoop> &inloops,
                            double intPre, double coinPre, double incPre,
                            std::vector<ofts::GeomArea>& regions);
    std::vector<std::shared_ptr<ofts::LoopTree>> LoopsToLoopTrees(numst::ContParam param,
                                                                  std::vector<ofts::DefLoop>& loops);
    void LoopIncludeLoops(ofts::DefLoop& loop,
                          std::vector<ofts::DefLoop>& vLoop,
                          double intPre,double coinPre,
                          double incPre, bool isDel,
                          std::vector<ofts::DefLoop>& vIncLoop);
    void LoopsToRegions(const std::vector<ofts::DefLoop>& loops,
                        std::vector<ofts::GeomArea>& regions);
};
#endif // ORGANIZELOOP_H
