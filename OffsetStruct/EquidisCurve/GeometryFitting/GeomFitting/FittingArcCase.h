/**
  @author: lvdong
  @date: 2025-05
**/
#ifndef FITTINGARCCASE_H
#define FITTINGARCCASE_H
#include"./GeomFittingSolve.h"
#include"../../../CommonFile/Header.h"
using namespace OffsetStruct::GeomFitting;
class FittingArcCase
{
public:
    inline FittingArcCase(){}
    void FittingArrayPoint(const std::vector<Point>& vPoint,
                           vector<DefArc>& vArc);
    inline void FittingPointToArc(const std::vector<Point>& vPoint,
                                  vector<DefArc>& vArc,
                                  double hErr = PreErr_2,
                                  double aErr = 10)
    {
        FittArc fitData;
        GeomFittingSolve sol;
        for (int i = 0; i < vPoint.size(); ++i) {
            if(!fitData._indexs.empty()){
            }
        }
    }
};
#endif // FITTINGARCCASE_H