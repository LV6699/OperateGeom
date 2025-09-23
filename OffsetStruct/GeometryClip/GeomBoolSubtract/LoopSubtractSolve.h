#ifndef LOOPSUBTRACTSOLVE_H
#define LOOPSUBTRACTSOLVE_H
#include"../../CommonFile/Header.h"
class LoopSubtractSolve
{
public:
    LoopSubtractSolve(){}
    bool IsPointInArrayLoop(const vector<DefLoop>& vLoop,
                            const Point& P, bool isTemplate, double bErr);
    void IntersectPointSplitLoop(const DefLoop& tempLoop,
                                 const vector<DefLoop>& vLoop,
                                 bool &isExisClip,
                                 DefLoop &resLoop,
                                 vector<DefLoop> &vResLoop);
    void DeleteInvalidClipElement(const DefLoop& tempLoop,
                                  const vector<DefLoop>& vLoop,
                                  bool &isExiDelete,
                                  DefLoop& clipLoop,
                                  vector<DefLoop>& vClipLoop);
    void LoopInvalidElement(DefLoop& loop, const vector<DefLoop> &vLoop,
                            bool isTemplate,
                            bool &isExiDelete,
                            vector<int>& vIndex);
    void ClassfyClipArrayLoop(const vector<DefLoop>& vLoop,
                              vector<DefLoop>& vNotCliLoop,
                              vector<DefLoop>& vClipLoop);
    bool IsTerminateSearch(const Point& endpoint,
                           const Point& curPnt,
                           vector<DefElem>& vElement);
    void CloseLoopCase(vector<DefElem>&vElement);
    void TempateConnectElement(const DefLoop& tempLoop,
                               const Point& refPnt,
                               bool& isFound,int& index);
    void ArrayLoopConnectElement(const vector<DefLoop> &vLoop,
                                 const Point& refPnt,
                                 bool& isFound,int& loIndex,
                                 int& index);
    void AddReverseElement(DefElem ele, vector<DefElem> &vElement);
};
#endif // LOOPSUBTRACTSOLVE_H