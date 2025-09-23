Fudj9byOurm-CBry3VDYE4PxT7Pl
Page 1#ifndef HANDLELOOP_H
#define HANDLELOOP_H
#include"../../CommonFile/Header.h"
class HandleLoop
{
public:
    HandleLoop(){}
    void ResetOpenLoopEleOrder(DefLoop& loop);
    void LoopRangeIndexElement(const DefLoop& oriLoop
                               int sIndex int eIndex
                               DefLoop& loop);
    void SetLoopCurveSourceType(ToolPathType type DefLoop& loop);
    void SetLoopsCurveSource(ToolPathType type vector<DefLoop>& loops);
    void SetMatchEleSourceType(GeomArea& region);
    void SetRegionsCurveSourceType(vector<GeomArea>&regions);
};
#endif // HANDLELOOP_H