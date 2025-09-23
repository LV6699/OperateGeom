#ifndef PntInPolySolve_H
#define PntInPolySolve_H
#include"../../CommonFile/Header.h"
struct IntInfo
{
    IntInfo() {}
    bool _isComLastEp = false;
    bool _hasComFirSp = false;
    bool _hasComPreEp = false;
    bool _isComNexSp = false;
    bool _hasChaPreStatus = false;
};
class PntInPolySolve
{
public:
    PntInPolySolve(){}
    void IntersectSegmentCase(const DefLoop &loop,
                              int i, const Point& lsp,
                              const Point& lep,
                              const Point& intPnt,
                              vector<int>& indexs,
                              IntInfo& intInfo,
                              int& totalIntNum,
                              bool& isIn);
    void IntersectArcCase(const DefLoop& loop,
                          const Point& lsp,
                          const Point& lep, int i,
                          const DefArc& arc,
                          const GeomIntRes& intRes,
                          vector<int>& indexs,
                          IntInfo& intInfo,
                          int& totalIntNum,
                          bool& isInPoly);
    void IsElemEndpointMaxCoord(const DefElem& ele,
                                bool isSp,
                                bool &isMaxCoord);
private:
    void IsArcEndpointMaxCoord(const DefArc& arc,bool isSp,
                               bool& isMaxCoord);
    void ExicuteMaxOrdinate(bool isLarge, int i, vector<int> &indexs,
                            bool& isInPoly, int& intNum)
    {
        if(isLarge){
            intNum ++;
            isInPoly = !isInPoly;
            indexs.push_back(i);
        }
    }
    bool IsElemNotHorLine(const DefElem& ele);
    void PreElemMaxOrdinateCase(const DefLoop &loop, int i,
                                vector<int> &indexs,
                                int &totalIntNum,
                                bool &isInPoly);
    void SinglePointCase(const DefLoop &loop,                         int i, vector<int> &vIndex,
                         const DefArc& arc,
                         IntInfo& intInfo, bool isEp,
                         int &totalIntNum,
                         bool &isInPoly);
    void TwoIntersectPointCase(const DefLoop& loop,
                               int i,vector<int>& indexs,
                               const DefArc& arc,
                               const Point& p1,
                               const Point& p2,
                               IntInfo& intInfo,
                               int& totalIntNum,
                               bool& isIn);
};
#endif // PntInPolySolve_H