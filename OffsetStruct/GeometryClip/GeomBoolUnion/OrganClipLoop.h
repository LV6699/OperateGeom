#ifndef ORGANCLIPLOOP_H
#define ORGANCLIPLOOP_H
#include "LoopBoolUnion.h"
class OrganClipLoop
{
public:
    OrganClipLoop() {}
    bool IsTerminateSearch(const Point&firstPt, const Point&curSp,
                           vector<vector<ClipIndex>>&vArrIndex,
                           vector<ClipIndex>&vIndex);
    void SpecifyConnectElement(const vector<DefLoop>& vLoop,
                               const Point& preEp, const int curLoIndex,
                               bool& isFound, int& loopIndex,
                               int & eleIndex);
    void LoopClipIndexElement(const vector<DefLoop>& vLoop,
                              const vector<vector<ClipIndex>>&vArrIndex,
                              vector<DefLoop>& vResLoop);
};
#endif // ORGANCLIPLOOP_H