#ifndef CLOSESTSELINT_H
#define CLOSESTSELINT_H
#include"../../../CommonFile/Header.h"
class ClosestSelInt
{
public:
    ClosestSelInt(){}
    void GetEleClosestSelInt(const DefLoop &oriLoop,
                             const DefLoop &loop,
                             double offsetVal,
                             const DefElem &element,
                             bool isJudSelInPt,
                             bool isOriIndex,
                             const vector<Point> &vSelInPt,
                             int index, bool &isExiSelIn,
                             int &intIndex, Point &intPnt);
    void GetEleClosetIntPoint(const DefLoop& loop,
                              const DefElem& ele,
                              int idx,
                              bool &isExiCloInt,
                              int &intIdx,
                              Point& intPnt);
    bool IsValidSelIntPoint(int i1,int i2,const DefLoop &loop,
                            const GeomIntRes &intRes);
    void ElementSelIntersectPoint(const DefLoop &loop, int id,
                                  vector<Point> &points);
    void ElemSelIntersectPoint(DefLoop &loop, int index,
                               vector<Point> &vPoint);
};
#endif // CLOSESTSELINT_H