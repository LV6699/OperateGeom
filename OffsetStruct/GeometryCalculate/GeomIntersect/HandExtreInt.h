#ifndef HANDEXTREINT_H
#define HANDEXTREINT_H
#include"../../CommonFile/Header.h"
class HandExtreInt
{
public:
    HandExtreInt(){}
    void SegEndpointOnArc(const Point& extrPnt,
                          const DefArc& arc,
                          bool isArcSpOn,
                          bool isArcEpOn,
                          GeomIntRes &intRes);
    void ArcEndpointOnSeg(const Point& extrPnt,
                          const Point& lSp,
                          const Point& lEp,
                          bool isSigSpOn,
                          bool isSigEpOn,
                          GeomIntRes &intRes);
    void SegArcIntersectEndpoint(const Point& lSp,const Point& lEp,
                                 DefArc arc, double endptPre,
                                 GeomIntRes &intRes);
};
#endif // HANDEXTREINT_H