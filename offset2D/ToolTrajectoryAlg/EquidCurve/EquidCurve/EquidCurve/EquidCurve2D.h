#ifndef EQUIDCURVE2D_H
#define EQUIDCURVE2D_H
#include"../../../CommonFile/DataStructure.h"
#include"../../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class EquidCurve2D
{
public:
    EquidCurve2D(){}
    static void GetOffsetPoint(const ofts::Point& p, const ofts::Point& c,
                               const double& R,const double& d,ofts::Point& r) {
#if 1
        ofts::Point rad(p.X() - c.X(),p.Y() - c.Y());
        rad.Normalize2D();
        double x = p.X() + rad.X() * d;
        double y = p.Y() + rad.Y() * d;
        r.SetX(x);r.SetY(y);r.SetZ(p.Z());
#else
        auto v = p - c;
        auto univ = v.Normalize2D();
        r = c + univ * R;
#endif
    }
    static void EquidistantSeg(const ofts::Point& sp,const ofts::Point& ep,
                               double d,bool leftward,
                               ofts::Point& rsp,ofts::Point& rep)
    {
        double A = ep.Y() - sp.Y();
        double B = sp.X() - ep.X();
        if(std::abs(A) < PreErr_15 && B != 0) {
            double y;
            double D = leftward ? -d : d;
            double C1 = ep.X() * sp.Y() - sp.X() * ep.Y();
            if (-B > 0){
                y = -C1 / B - D;
            }else{
                y = -C1 / B + D;
            }
            rsp.SetX(sp.X());rsp.SetY(y);
            rep.SetX(ep.X());rep.SetY(y);
        }else if (A != 0 && std::abs(B) < PreErr_15){
            double x;
            double D = leftward ? -d : d;
            if (A > 0){
                x = ep.X() + D;
            }else{
                x = ep.X() - D;
            }
            rsp.SetX(x);rsp.SetY(sp.Y());
            rep.SetX(x);rep.SetY(ep.Y());
        }else{
            double a,b;
            double dir[2] = {ep.X()-sp.X(),ep.Y()-sp.Y()};
            double len = std::sqrt(dir[0]*dir[0] + dir[1]*dir[1]);
            double nor[2] = {dir[0]/len,dir[1]/len};
            if(leftward){
                a = -nor[1];b = nor[0];
            }else{
                a = nor[1];b = -nor[0];
            }
            rsp.SetX(sp.X() + d * a);
            rsp.SetY(sp.Y() + d * b);
            rep.SetX(ep.X() + d * a);
            rep.SetY(ep.Y() + d * b);
        }
#if 0
        double d1 = sp.Distance2D(rsp);
        double d2 = ep.Distance2D(rep);
        double e1 = d1 - d;
        double e2 = d2 - d;
        DisplayGeom().DisplaySegment(sp,ep,_colors.back(),1);
        DisplayGeom().DisplaySegment(rsp,rep,_colors[0],1);
        DisplayGeom().DisplayPoint(sp,_colors[4]);
        DisplayGeom().DisplayPoint(rsp,_colors[4]);
        _mainwind->myOccView->fitAll();
        int tem = 1;
#endif
    }
    static ofts::DefSeg EquidistantSeg(const ofts::DefSeg& s,double d,bool leftward)
    {
        ofts::Point rsp,rep;
        EquidistantSeg(s.GetSp(),s.GetEp(),d,leftward,rsp,rep);
        return ofts::DefSeg(rsp,rep);
    }
    static bool EquidistantArc(const ofts::DefArc& arc,double d,
                               bool leftward,ofts::DefArc& rarc)
    {
        double D = leftward ? d : -d;
        double R = arc.GetCalR();
        double R1 = arc.IsCW() ? R + D : R - D;
        if(R1 < Oft_CircleVal_Err){return false;}
        rarc.SetCp(arc.GetCp());
        rarc.SetR(R1);
        rarc.SetCW(arc.IsCW());
#define Way 2
#if Way == 1
        double sAgl = std::atan2((arc.GetSp().Y() - arc.GetCp().Y()),(arc.GetSp().X() - arc.GetCp().X()));
        double eAgl = std::atan2((arc.GetEp().Y() - arc.GetCp().Y()),(arc.GetEp().X() - arc.GetCp().X()));
        rarc._arcSp.SetX(arc.GetCp().X() + R1 * std::cos(sAgl));
        rarc._arcSp.SetY(arc.GetCp().Y() + R1 * std::sin(sAgl));
        rarc._arcEp.SetX(arc.GetCp().X() + R1 * std::cos(eAgl));
        arc._arcEp.SetY(arc.GetCp().Y() + R1 * std::sin(eAgl));
#elif Way == 2
        if(R1 < R){d = -d;}
        ofts::Point sp,ep;
        GetOffsetPoint(arc.GetSp(),arc.GetCp(),R1,d,sp);
        GetOffsetPoint(arc.GetEp(),arc.GetCp(),R1,d,ep);
        rarc.SetSp(sp);
        rarc.SetEp(ep);
#else
        double vsx = arc.GetSp().X() - arc.GetCp().X();
        double vsy = arc.GetSp().Y() - arc.GetCp().Y();
        double vex = arc.GetEp().X() - arc.GetCp().X();
        double vey = arc.GetEp().Y() - arc.GetCp().Y();
        double k = R1 / R;
        rarc.SetSp(ofts::Point(arc.GetCp().X()+k*vsx,arc.GetCp().Y()+k*vsy));
        rarc.SetEp(ofts::Point(arc.GetCp().X()+k*vex,arc.GetCp().Y()+k*vey));
#endif
#if 0
        double d1 = rarc.GetSp().Distance2D(arc.GetCp());
        double d2 = rarc.GetEp().Distance2D(arc.GetCp());
        double e1 = d1 - R1;
        double e2 = d2 - R1;
        DisplayGeom().DisplayArc(arc,_colors.back(),1);
        DisplayGeom().DisplayArc(rarc,_colors[1],1);
        DisplayGeom().DisplayPoint(arc.GetSp(),_colors[4]);
        DisplayGeom().DisplayPoint(rarc.GetSp(),_colors[4]);
        _mainwind->myOccView->fitAll();
        int tem = 1;
#endif
        return true;
    }
    static ofts::DefArc EquidistantArc(const ofts::DefArc& arc,double d,bool leftward)
    {
        ofts::DefArc rarc;
        EquidistantArc(arc,d,leftward,rarc);return rarc;
    }
    static bool EquidistantCircle(const ofts::DefCircle& cir,double d,
                                  bool leftward,ofts::DefCircle& rcir)
    {
        double R1 = leftward ? (cir.R() + d) : (cir.R() - d);
        if(R1 < Oft_CircleVal_Err){return false;}
        rcir.SetCp(cir.GetCp());
        rcir.SetR(R1);
        return true;
    }
    static ofts::DefCircle EquidistantCircle(const ofts::DefCircle& cir,double d,bool leftward)
    {
        ofts::DefCircle rcir;
        EquidistantCircle(cir,d,leftward,rcir);return rcir;
    }
    static bool EquidistantCurve(const ofts::DefElem& c,double d,
                                 bool leftward,ofts::DefElem& rc){
        rc.SetType(c.Type());
        switch (c._type) {
        case Ofte::LINETYPE:
            EquidistantSeg(c._segment.GetSp(),c._segment.GetEp(),
                           d,leftward,rc._segment._segSp,
                           rc._segment._segEp);
            break;
        case Ofte::ARCTYPE:
            return EquidistantArc(c._arc,d,leftward,rc._arc);
        case Ofte::CIRCLETYPE:
            return EquidistantCircle(c._circle,d,leftward,rc._circle);
        default:return false;
        }
        return true;
    }
    static ofts::DefElem EquidistantCurve(const ofts::DefElem& c,double d,bool leftward)
    {
        ofts::DefElem rc;
        EquidistantCurve(c,d,leftward,rc);return rc;
    }
    static bool IsCurveExistEquid(const ofts::DefElem& c,double dist,bool leftward){
        switch (c._type) {
        case Ofte::LINETYPE:
            return true;
        case Ofte::ARCTYPE:
            return leftward || c._arc.GetOptR() >= Oft_CircleVal_Err;
        case Ofte::CIRCLETYPE:
            return leftward || c._circle.R() >= Oft_CircleVal_Err;
        default:return false;
        }
    }
};
#endif // EQUIDCURVE2D_H
