#ifndef ACQUIREELELOOP_H
#define ACQUIREELELOOP_H
#include"../HearderFile.h"
class AcquireEleLoop
{
public:
    AcquireEleLoop(){}
    void GetEleZValue(const DefElem& ele,double& zValue);
    void IsEleInLoop(const DefLoop& loop,
                     const DefElem& ele,
                     double pre,bool& isIn);
    void GetLoopFromePartCutData(const vector<vector<BndIslPair> > &modCutData,
                                 const DefElem& eleData, bool &isGetLoop,
                                 BndIslPair &pairData);
};
#endif // ACQUIREELELOOP_H