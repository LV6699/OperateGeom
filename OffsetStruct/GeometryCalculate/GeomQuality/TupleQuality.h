#ifndef TUPLEQUALITY_H
#define TUPLEQUALITY_H
#include"../../CommonFile/Header.h"
class TupleQuality
{
public:
    TupleQuality(){}
    bool IsLineLineParallel(const Point& l1Sp, const Point& l1Ep,
                            const Point& l2Sp, const Point& l2Ep,
                            double pre);
    bool IsLineExistCoincide(const Point& l1Sp, const Point& l1Ep,
                             const Point& l2Sp, const Point& l2Ep,
                             double pre);
    bool IsLineCompleteCoincide(const Point& l1Sp,
                                const Point& l1Ep,
                                const Point& l2Sp,
                                const Point& l2Ep,
                                double pre);
    bool IsLineLocatedOnLine(const Point& refSp,
                             const Point& refEp,
                             const Point& lSp,
                             const Point& lEp,
                             double pre);
    bool IsArcExistCoincide(const DefArc& arc1,
                            const DefArc& arc2,
                            double pre);
    bool IsArcCompleteCoincide(const DefArc& arc1,
                               const DefArc& arc2,
                               double pre);
    bool IsArcLocatedOnArc(const DefArc &refArc,
                           const DefArc &arc,
                           double pre);
    bool IsCirclesCointcoincide(const Point& cp1,double r1,
                                const Point& cp2,double r2,
                                double pre);
    bool IsElementsCoincide(const DefElem& ele1,
                            const DefElem& ele2,
                            bool isComplete,
                            double pre);
    bool IsEleExistCoincide(const DefElem& ele1,
                            const DefElem& ele2,
                            double pre);
    bool IsEleLocatedOnEle(const DefElem& refEle,
                           const DefElem& ele,
                           double pre);
    bool IsHorizontalLine(const Point& lSp, const Point& lEp,
                          double pre);
    bool IsVerticalLine(const Point& lSp,const Point& lEp,
                        double pre);    void GetEleSpecifyTangentLine(const DefElem&ele,
                                  bool isSp, bool isReverse,
                                  Point& lSp,Point& lEp);
};
#endif // TUPLEQUALITY_H