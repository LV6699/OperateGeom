#ifndef MINPASSAGESOLVE_H
#define MINPASSAGESOLVE_H
#include"../../../CommonFile/Header.h"
class MinPassageSolve
{
public:
    MinPassageSolve(){}
    void IndexDistanceSort(vector<OffsetStruct::ProgOftValue::ValidDist> 
&vIdDis);
    bool IsValidExploreUnprocess(const DefLoop &loop,
                                 const int index,
                                 const double segStep,
                                 const double arcStep,
                                 const int id,
                                 const double nodeDis,
                                 const double Distance);
};
#endif // MINPASSAGESOLVE_H