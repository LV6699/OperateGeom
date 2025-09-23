#ifndef CALCULATEELEINT_H
#define CALCULATEELEINT_H
#include"../../CommonFile/Header.h"
class CAMALGORITHM_EXPORT ElementIntersect
{
public:
    ElementIntersect(){}
    void GetCloseArcExtremePnt(DefArc arc,
                               const Point &P1,
                               const Point &P2,
                               bool isCloseSp,
                               Point &P);
    void GetCloseEleExtremePnt(const DefElem &ele,
                               const Point &P1,
                               const Point &P2,
                               bool isCloseSp,
                               Point& P);
    void GetLoopEleIntersectPoint(const DefLoop& loop,
                                  const DefElem& ele,
                                  double tanPre,
                                  double endptPre,
                                  double intPre,
                                  vector<Point>& vIntPnt);
    void GetTwoPntCloseEleEndPnt(const DefElem &ele,
                                 const Point& intPnt1,
                                 const Point& intPnt2,
                                 bool isCloseSp,
                                 Point& objPnt);
    void GetSigArcIntPntBaseFixedPnt(const DefArc &arc,
                                     bool isLineFont,
                                     const Point &_intPnt1,
                                     const Point &intPnt2,
                                     Point &interPnt);
    void GetSigArcIntUnderDelEle(const DefArc &arcData,
                                 bool isLineFont,
                                 const Point &intPnt1,
                                 const Point &ntPnt2,
                                 Point &interPnt);
    void GetToSigMinDis(Point& pnt1, Point& pnt2,
                        Point& startPnt,
                        Point& endPnt, Point& valPnt);
};
#endif // CALCULATEELEINT_H