#include "RegionSubt.h"
#include "../AuxCalculate/HandleSelInt.h"
#include "../AuxCalculate/RegiClipAux.h"
#include "../../../GeometryCalculate/RelativeContain/PointInLoop.h"
#include "../../../GeometryCalculate/GeomCalculate/LoopDirection.h"
void RegionSubt::OperateInput()
{
    /*    std::vector<ofts::DefLoop> _scl_ls;
    std::vector<ofts::DefLoop> _ccl_ls; */
    HandleSelInt hst;RegiClipAux rcaux;
    auto& sloops = _sloops;auto& scl_ls = _scl_ls;
    auto& cloops = _sloops;auto& ccl_ls = _ccl_ls;



    auto ToLoops = [&](const std::vector<ofts::DefLoop>& loops,
            std::vector<ofts::DefLoop>& rloops){
        for(size_t i = 0;i < loops.size();++i){
            const auto& l = loops[i];
            auto cl_ls = hst.HandleSelIntLoop(l);
            for (size_t j = 0; j < cl_ls.size(); j++){
                auto& cl_l = cl_ls[j];
                bool iscw = LoopDirection().IsClockwiseLoop(cl_l);
                cl_l.SetCW(iscw);
                auto op = LimitCoord2D::LoopLimitPoint(cl_l,true,true);
                int wn = rcaux.PointWindNum(cl_l,op,PreErr5_10);
                wn = iscw ? wn ++ : wn --;
                cl_l.SetWindNum(wn);
            }
            for(auto& d : cl_ls){
                rloops.emplace_back(d);
            }
        }
    };
    ToLoops(sloops,scl_ls);
    ToLoops(cloops,ccl_ls);
    const auto& rule = _rule;


}
void RegionSubt::Exicute()
{
    HandleSelInt hst;
    auto& sloops = _sloops,cloops = _sloops;
    std::vector<ofts::DefLoop>scl_ls,ccl_ls;
    std::vector<ofts::GeomArea>sRegions,cRegions;

    auto HandleSelInt = [&](std::vector<ofts::DefLoop>& loops,
            std::vector<ofts::DefLoop>& rloops){
        for(size_t i = 0;i < loops.size();++i){
            auto l = loops[i];
            auto ls = hst.HandleSelIntLoop(l);
            for (size_t j = 0; j < ls.size(); j++){
                auto op = LimitCoord2D::LoopLimitPoint(ls[i],true,true);
                auto iData = PointInLoop().InLoopIntData(ls[i],op,PreErr5_10);

            }
        }
    };
    for(size_t i = 0;i < sloops.size();++i){
        auto loop = sloops[i];
        scl_ls = hst.HandleSelIntLoop(sloops[i]);
    }
    for(size_t i = 0;i < cloops.size();++i){
        auto loop = cloops[i];
        ccl_ls = hst.HandleSelIntLoop(loop);
    }

}
