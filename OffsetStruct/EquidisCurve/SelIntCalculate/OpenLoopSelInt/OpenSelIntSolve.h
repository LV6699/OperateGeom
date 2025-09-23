#ifndef OPENSELINTSOLVE_H
#define OPENSELINTSOLVE_H
#include"../../../CommonFile/Header.h"
class OpenSelIntSolve
{
public:
    OpenSelIntSolve(){}
    void LogicalJudgmentValCutEle(const DefElem& curEle,
                                  const DefElem& nexEle,
                                  double curLenth, double nexLenth,
                                  bool isCurVal, bool isNexVal,
                                  DefElem& valPart);
    void ShortDistLoopElemEnvelope(const DefLoop& oriLoop,
                                   const DefElem& ele,
                                   double offsetVal,
                                   vector<DefLoop>& vEnvLoop);
    void ElemEnvelopeIntPoint(const DefLoop& oriLoop,
                              const DefElem& ele,
                              const vector<DefLoop>& vEnvLoop,
                              double offsetVal,
                              vector<Point>& vPoint);
    void GetCutElementValidPart(const DefLoop& oriLoop,
                                const DefElem& cutEle,
                                bool isCurCutEle,
                                double offsetVal,
                                bool &isCutEleVal,
                                DefElem& valPart);
    void IsConnectArcSameOftDirection(const DefLoop& oriLoop,
                                      const DefArc& arc,
                                      double offsetVal,
                                      bool& isValid);
};
#endif // OPENSELINTSOLVE_H