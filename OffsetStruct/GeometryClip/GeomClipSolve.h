#ifndef GEOMCLIPSOLVE_H
#define GEOMCLIPSOLVE_H
#include"../CommonFile/Header.h"
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
    void HandleCoincideLoops(std::vector<OffsetStruct::DefLoop>& loops);
};
#endif // GEOMCLIPSOLVE_H