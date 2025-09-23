#ifndef LOOPUNIONSOLVE_H
#define LOOPUNIONSOLVE_H
#include"../../CommonFile/Header.h"
class LoopUnionSolve
{
public:
    LoopUnionSolve(){}
    bool IsIdenticalEndpoint(const DefElem& ele, double errVal);
    void IntersectPointSplitLoop(const vector<DefLoop> &vLoop,
                                 bool &isExisClip,
                                 vector<DefLoop> &vResLoop);
    void LoopLoopsIntersectPoint(const DefLoop &loop,
                                 const vector<DefLoop>&vLoop,
                                 bool &isExisClip,
                                 DefLoop &resLoop);
    void ElementLoopsIntersect(const DefElem& ele,
                               const vector<DefLoop>&vLoop,
                               bool& isExistInt,
                               vector<Point>&vPoint);
    void DeleteInvalidClipElement(const vector<DefLoop> &vBarLoop,
                                  bool &isExiDelete,
                                  vector<DefLoop>&vLoop);
    bool IsPointInArrayLoop(const vector<DefLoop>&vLoop,
                            const Point&P);
};
#endif // LOOPUNIONSOLVE_H