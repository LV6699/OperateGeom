#ifndef OPERATEELEM_H
#define OPERATEELEM_H
#include"../../CommonFile/Header.h"
class OperateElem
{
public:
    OperateElem();
    void AdjoinElemRotDirection(const DefElem &ele1,
                                const DefElem &ele2,
                                RotDir &rotDir);
    void AdjoinSegSegRotDirection(const DefSeg &seg1,
                                  const DefSeg &seg2,
                                  RotDir &rotDir);
    void AdjoinSegArcRotDirection(const DefSeg &seg,
                                  const DefArc &arc,
                                  bool isLineFont,
                                  RotDir &rotDir);
    void AdjoinArcArcRotDirection(const DefArc &arc1,
                                  const DefArc &arc2,
                                  RotDir &rotDir);
};
#endif // OPERATEELEM_H