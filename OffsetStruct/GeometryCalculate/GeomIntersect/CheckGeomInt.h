#ifndef CHECKGEOMINT_H
#define CHECKGEOMINT_H
#include"../../CommonFile/Header.h"
class CheckGeomInt
{
public:
    CheckGeomInt(){}
    void JudgeArcRadius(DefArc& arc);
    void IsCircleCircleIntersect(const Point &cp1,
                                 double R1,
                                 const Point &cp2,
                                 double R2,
                                 bool &isExiCCInt);
    void IsCircleCircleIntersect(const Point &cp1,
                                 double R1,
                                 const Point &cp2,
                                 double R2,
                                 double pre,
                                 bool &isExiCCInt);
    void IsLineCircleIntersect(const Point& lSp,
                               const Point& lEp,
                               const Point& cp,
                               double R, double pre,
                               bool &isExiInt);
    void IsLineArcIntersect(const Point& lSp,
                            const Point& lEp,
                            DefArc arc, double pre,
                            bool& isExiInt);
    void IsSegmentSegmentIntersect(const Point& lSp1,
                                   const Point& lEp1,
                                   const Point& lSp2,
                                   const Point& lEp2, double pre,
                                   bool& isExiInt);
    void IsSegmentCircleIntersect(const Point& lSp,
                                  const Point& lEp,
                                  const Point& cp,
                                  double R, double pre,
                                  bool& isExiInt);
    void IsArcCircleIntersect(DefArc arc,
                              const Point& cp,
                              double R,
                              double pre,
                              bool& isExiInt);
    void IsSegmentArcIntersect(const Point& lSp,
                               const Point& lEp,
                               DefArc arc,double pre,
                               bool& isExiInt);
    void IsArcArcIntersect(DefArc arc1,
                           DefArc arc2,
                           double pre,
                           bool& isExiInt);    void IsElementElementIntersect(const DefElem& ele1,
                                   const DefElem& ele2,
                                   double pre,bool& isExiInt);
    bool IsElementElementIntersect(const DefElem& ele1,
                                   const DefElem& ele2,
                                   double pre);
    void IsElementSpecifyIntersect(const DefElem& ele1,
                                   const DefElem& ele2,
                                   double coinPre,
                                   double tanPre,
                                   double intPre,
                                   bool& isIntersect);
    void IsLoopElemSpecifyIntersect(const DefLoop& loop,
                                    const DefElem& ele,
                                    double coinPre,
                                    double tanPre,
                                    double intPre,
                                    bool& isIntersect);
    void IsLoopLoopIntersect(const DefLoop& loop1,
                             const DefLoop& loop2,
                             double pre,
                             bool& isExiInt);
    void IsLoopSpecifyIntersect(const DefLoop& loop1,
                                const DefLoop& loop2,
                                double coinPre,
                                double tanPre,
                                double intPre,
                                bool& isSpecInt);
    void IsLoopArrayLoopIntersect(const vector<DefLoop>&vLoop,
                                  const DefLoop& loop,
                                  double pre,
                                  bool& isExiInt);
};
#endif // CHECKGEOMINT_H