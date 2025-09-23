#ifndef POINTONELEMENT_H
#define POINTONELEMENT_H
#include"../../CommonFile/Header.h"
class PointOnElement
{
public:
    PointOnElement(){}
    bool IsPointWithinSegment(const Point& P, const Point& P1,
                              const Point& P2,double err)
    {
        double minx = P1.X(),maxx = P2.X(),miny = P1.Y(),maxy = P2.Y();
        if(minx > maxx){
            minx = P2.X();
            maxx = P1.X();
        }
        if(miny > maxy){
            miny = P2.Y();
            maxy = P1.Y();
        }
        return (P.X() >= minx - err) && (P.X() <= maxx + err) &&
                (P.Y() >= miny - err) && (P.Y() <= maxy + err);
    }
    bool IsPointWithinCircle(const Point& P,const Point& c,
                             double r,double err)
    {
        return (P.X() >= c.X() - r - err) && (P.X() <= c.X() + r + err) &&
                (P.Y() >= c.Y() - r - err) && (P.Y() <= c.Y() + r + err);
    }
    bool IsPointOnElement(const DefElem &ele,const Point& P,
                          double endPre,double pre,
                          bool isMatOnLine,bool isMatOnCir);
    bool IsPointOnLine(const Point& P1, const Point& P2,
                       const Point& P, double pre);
    void IsPointOnSegment(const Point &lSp, const Point &lEp,
                          const Point& P, double endPre, double pre,
                          bool isMatOnLine, bool& isOnSeg);
    void IsPointOnArc(const DefArc& arc,const Point& P,
                      double endPre,double pre,
                      bool isMatOnCir,bool& isOnArc);
    void IsPointOnCircle(const Point& P, const Point &cp,
                         double R, double pre, bool& isOnCircle);
};
#endif // POINTONELEMENT_H