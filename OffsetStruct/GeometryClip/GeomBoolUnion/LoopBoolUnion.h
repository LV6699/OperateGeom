#ifndef LOOPBOOLUNION_H
#define LOOPBOOLUNION_H
#include"../../CommonFile/Header.h"
class ClipIndex
{
public:
    ClipIndex() = default;
    ClipIndex(int loId,int eleId) : loopIndex(loId),eleIndex(eleId){}
    int eleIndex;
    int loopIndex;
};
class LoopBoolUnion
{
public:
    LoopBoolUnion(){}
    void LoopGeomBooleanUnion(vector<DefLoop> vInputLoop,
                                  vector<DefLoop> vBarLoop,
                                  vector<DefLoop> &vResLoop);
    void TraverseArrayLoopConnect(vector<DefLoop> vLoop,
                                  vector<vector<ClipIndex>>& vArrIndex);
    void CurrentLoopConnectLoop(vector<vector<ClipIndex>>& vArrIndex,
                                vector<ClipIndex> &vIndex,
                                Point endpoint,
                                vector<DefLoop>& vLoop, const bool isFirstInto,
                                const int loIndex,
                                const int eleIndex);
};
#endif // LOOPBOOLUNION_H