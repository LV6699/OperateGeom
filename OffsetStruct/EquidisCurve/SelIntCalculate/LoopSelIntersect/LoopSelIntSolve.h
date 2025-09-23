#ifndef LOOPSELINTSOLVE_H
#define LOOPSELINTSOLVE_H
#include "../../../CommonFile/Header.h"
class LoopSelIntSolve
{
public:
    LoopSelIntSolve(){}
    ///需要保证vPoint里面的点在ele上.
    void ArrayPointSliptElement(const DefElem& ele,
                                const vector<Point>&vPoint,
                                vector<DefElem>&vElem);
    void LocalSelIntLoopIndex(DefElem curEle,
                              vector<DefElem>&vElem,
                              vector<vector<int> > vArrIndex,
                              bool isOpenLoop, int index,
                              vector<int>& vIndex);
    void LoopEnvelopClipElement(const DefLoop& oriLoop,
                                const DefElem& ele,
                                double offsetVal,
                                vector<DefElem>&vElem);
    void HandleInterfereElement(const DefLoop &oriLoop,
                                vector<DefElem>&vElem,
                                double offsetVal,
                                double errVal);
    void SpecifyInvalidClipElem(const DefLoop& oriLoop,
                                double offsetVal,
                                vector<DefElem>& vElem);
    void CreateLocalIntersectLoop(int index,
                                  vector<DefElem>& vElem,
                                  bool isOpenLoop,
                                  vector<int>& vIndex);
    void FindLoopConnectPoint(vector<DefElem>& vElem, const Point& P,
                              bool &isFound, int &idRes);
    bool IsCloseIndexLoop(const vector<DefElem>& vElem,
                          const vector<int>& vIndex);
};
#endif // LOOPSELINTSOLVE_H