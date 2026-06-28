#ifndef GEOMCLIPSOLVE_H
#define GEOMCLIPSOLVE_H
#include"../CommonFile/Header.h"
#include"../CommonFile/NumStruct.h"
class GeomClipSolve
{
public:
    GeomClipSolve(){}
    void IslCircleLoopCoincide(const OffsetStruct::DefLoop& loop1,
                               const OffsetStruct::DefLoop& loop2,
                               bool &isCirCoin);
    void SubtractCircleCoincide(const OffsetStruct::DefLoop &tempLoop,
                                std::vector<OffsetStruct::DefLoop> &vLoop,
                                bool &isCoincide);
    void HandleSubtractCoincide(const OffsetStruct::DefLoop& temLoop,
                                std::vector<OffsetStruct::DefLoop>& vLoop);
    bool IsCoincideWithOthers(const OffsetStruct::DefLoop& loop,
                              const std::vector<OffsetStruct::DefLoop>& loops);
    void HandleCoincideLoops(std::vector<OffsetStruct::DefLoop>& loops,
                             NumStruct::ReLoopErr err = NumStruct::ReLoopErr(
                PreErr5_8,PreErr5_8,PreErr5_8,PreErr5_5),bool isRestruct = true);
    void DeleteCoincideLoops(ofts::DefLoop loop,
                             std::vector<ofts::DefLoop>& loops,
                             NumStruct::ReLoopErr err = NumStruct::ReLoopErr(
                PreErr5_8,PreErr5_8,PreErr5_8,PreErr5_5));
};
#endif // GEOMCLIPSOLVE_H
