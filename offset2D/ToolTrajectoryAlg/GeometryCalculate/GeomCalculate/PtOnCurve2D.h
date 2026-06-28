#ifndef PTONCURVE2D_H
#define PTONCURVE2D_H
#include"BaseGeom2D.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class PtOnCurve2D
{
public:
    PtOnCurve2D(){}
    static bool IsPointWithinSeg(const ofts::Point& P, const ofts::Point& P1,
                                 const ofts::Point& P2,double err)
    {
        double minx = P1.X(),maxx = P2.X(),miny = P1.Y(),maxy = P2.Y();
        if(minx > maxx){
            minx = P2.X();maxx = P1.X();
        }
        if(miny > maxy){
            miny = P2.Y();maxy = P1.Y();
        }
        bool is1 = P.X() >= minx - err;
        bool is2 = P.X() <= maxx + err;
        bool is3 = P.Y() >= miny - err;
        bool is4 = P.Y() <= maxy + err;
        return is1 && is2 && is3 && is4;
    }
    static bool IsPointWithinCircle(const ofts::Point& P,const ofts::Point& c,
                                    double r,double err)
    {
        return (P.X() >= c.X() - r - err) && (P.X() <= c.X() + r + err) &&
                (P.Y() >= c.Y() - r - err) && (P.Y() <= c.Y() + r + err);
    }
    static bool IsPointOnLine(const ofts::Point& p1,const ofts::Point& p2,
                              const ofts::Point& p,double pre)
    {
        ofts::Point op;
        BaseGeom2D().PointOnLineProj(p,p1,p2,op);
        return p.IsEqual(op,pre);
    }
    static bool IsPtOnCircle(const ofts::Point& p,const ofts::Point& cp,double R, double e)
    {
        double d = p.Distance2D(cp);
        return std::abs(d - R) <= e;
    }
    static bool IsPointOnSegment(const ofts::Point& sp,const ofts::Point& ep,
                                 const ofts::Point& P,double endPre,
                                 double pre,bool isMatOnL = true)
    {
        if(!IsPointWithinSeg(P,sp,ep,endPre)){return false;}
        if(isMatOnL && !IsPointOnLine(sp,ep,P,pre)){return false;}
        return true;
    }
    static bool IsPointOnSegment(const ofts::Point& p,const ofts::DefSeg& s,
                                 double endPre,double pre,bool isMatOnL = true)
    {
        return IsPointOnSegment(s.GetSp(),s.GetEp(),p,endPre,pre,isMatOnL);
    }
    static bool IsPointOnArc(const ofts::DefArc& arc,const ofts::Point& P,
                             double endPre,double pre,bool isMatOnCir = true)
    {
        bool isOnArc = false;
        if(!IsPointWithinCircle(P,arc.GetCp(),arc.R(),pre)){return false;}
        if(isMatOnCir){
            double d = P.DistSquare2D(arc.GetCp());
            if(std::abs(d - (arc.R() * arc.R())) > pre){return false;}
        }
        if(P.IsSamePoint2D(arc.GetSp(),endPre) ||
                P.IsSamePoint2D(arc.GetEp(),endPre)){
            return true;
        }
        double sAgl,eAgl;
        BaseGeom2D().ArcEndpointAngle(arc,sAgl,eAgl);
        double theta = std::atan2((P.Y() - arc.GetCp().Y()),
                                  (P.X() - arc.GetCp().X())) * _AlgRad;
        if(theta < 0){
            theta += 360;
        }
        isOnArc = pnum::AglIsBetweenAgls(sAgl,eAgl,arc.IsCW(),theta,pre);
        return isOnArc;
    }
    static bool IsPointOnCurve(const ofts::DefElem& curve,
                               const ofts::Point& P,
                               double endPre,double pre,
                               bool isMatOnLine,bool isMatOnCir)
    {
        switch (curve._type) {
        case LINETYPE:
            return IsPointOnSegment(curve._segment.GetSp(),
                                    curve._segment.GetEp(),P,
                                    endPre,pre,isMatOnLine);
        case ARCTYPE:
            return IsPointOnArc(curve._arc,P,endPre,pre,isMatOnCir);
        case CIRCLETYPE:
            return IsPtOnCircle(P,curve._circle._cirCp,
                                curve._circle._cirR,pre);
        default : return false;
        }
        return false;
    }
};
#endif // PTONCURVE2D_H
