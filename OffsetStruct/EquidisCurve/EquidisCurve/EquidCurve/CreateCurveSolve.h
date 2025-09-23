#ifndef CREATECURVESOLVE_H
#define CREATECURVESOLVE_H
#include"../../../CommonFile/Header.h"
class CreateCurveSolve
{
public:
    CreateCurveSolve(){}
    void SetArcParameters(const Point& sp,
                          const Point& ep,
                          const Point& cp,
                          RotDir rotDir,
                          double R,DefArc& arc);
    void NotExistEquidCurve(const DefArc& oriArc, double R,
                            DefArc& arc1, DefArc& arc2);
    void CreateElemConnectArc(const DefElem &ele1_, const DefElem& ele1,
                              const DefElem& ele2, const Point &cp,
                              double R, ProfType proType,
                              DefArc& arc);
    void SetEquedElemEndpoint(const Point& p,
                              const Point &sp1,
                              DefElem& ele1,
                              DefElem& ele2,
                              vector<DefElem> &elems)
    {
        ele1.ChangeEndpoint(p,true);
        ele2.ChangeEndpoint(p,false);
        if(!sp1.IsSamePoint2D(p,PreErr5_10)){
            elems.emplace_back(ele1);
        }
    }
    void ResetInvalidElem(vector<DefElem> &vElem, double pre);
    bool IsCreateDoubleArc(const vector<DefElem> &vOriElem,
                           const vector<DefElem> &vElem,
                           ProfType proType, double R, int i);
    void ChangeEndpointCase(DefElem& ele1,DefElem& ele2,
                            const Point& ep1,const Point& sp2,
                            vector<DefElem> &elems);
};
#endif // CREATECURVESOLVE_H