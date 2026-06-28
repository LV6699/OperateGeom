#ifndef CUTMODELSOLVE_H
#define CUTMODELSOLVE_H
#include"../HearderFile.h"
struct BIMatchWeight
{
    BIMatchWeight() {}
    BIMatchWeight(DefLoop bndLoop,vector<DefLoop> vIslLoop,
                  int eleNum,double lenth,double weitht)
    {
        m_bndLoop = bndLoop;
        m_vIslLoop = vIslLoop;
        m_eleNum = eleNum;
        m_lenth = lenth;
        m_weitht = weitht;
    }
    DefLoop m_bndLoop;
    vector<DefLoop> m_vIslLoop;
    int m_eleNum = 0;
    double m_lenth = 0;
    double m_weitht = 0;
};
class CutModelSolve
{
public:
    CutModelSolve(){}
    void SaveBatchCutContour(string saveDir, string modelName,
                             vector<BndIslPair>&vBILoop,
                             bool &isSuccess);
};
#endif // CUTMODELSOLVE_H