#ifndef INTVERTEXPROT_H
#define INTVERTEXPROT_H

#include "BaseCalc.h"
#include "OperTriaCl.h"


namespace grm {

class IntVertProt {
public:
    IntVertProt() {}

    static double TriangleFaceZ(const ofts::Point& p0,const ofts::Point& p1,
                                const ofts::Point& p2,const ofts::Point& p)
    {
        Triangle t(p0,p1,p2);
        t.CalNormal();
        double z = Min_Val;
        double t1 = t.N().X() * (t.P0().X() - p.X());
        double t2 = t.N().Y() * (t.P0().Y() - p.Y());
        z = t.P0().Z() + (t1 + t2) / t.N().Z();
        return z;
    }

    static double PlaneEndVertProtZ(const DefTool& T, const ofts::Point& v,
                                    const ofts::Point& p)
    {
        return v.Z();
    }
    static double BallNoseVertProtZ(const DefTool& T, const ofts::Point& v,
                                    const ofts::Point& p,double d2)
    {
        double h = std::sqrt(T.RR() - d2);
        double z = v.Z() + h - T._R;
        return z;
    }

    static double RoundNoseVertProtZ(const DefTool& T, const ofts::Point& v,
                                     const ofts::Point& p,double d2)
    {
        double z = Min_Val;
        double sub = T._R_r;
        if (d2 <= sub * sub) {
            z = v.Z();
        } else {
            double d = std::sqrt(d2);
#if 0
            if (pnum::RightInc2(sub, d, T._R)) {
                double rr = T._cr * T._cr;
                double t = d - T._R + T._cr;
                double tt = t * t;
                double h = std::sqrt(rr - tt);
                z = v.Z() + h;
            }
#else
            double q = d - T._R + T._cr;
            auto h2 = T._cr * T._cr - q * q;
            if (h2 <= 0) {
                z = v.Z() - T._cr;
            } else {
                z = v.Z() - T._cr + std::sqrt(h2);
            }
#endif
        }
        return z;
    }

    static double TaperEndVertProtZ(const DefTool& T, double tz,double d2)
    {
        double d = std::sqrt(d2);
        double full_l = T._taper_l;
        double l = T._h;
        double curr_l = full_l / T._R * (T._R - d);
        return tz - (l - std::min(curr_l, l));
    }

    static double VertexProtectZ(const DefTool& T, const ofts::Point& v,
                                 const ofts::Point& p) {
        double z = Min_Val;
        double d2 = p.DistSquare2D(v);
        if(d2 > T.RR()){
            return z;
        }
        switch (T._type) {
        case ToolType::PlaneEnd: {
            z = v.Z();
            break;
        }
        case ToolType::BallNoseEnd: {
            z = BallNoseVertProtZ(T, v, p, d2);
            break;
        }
        case ToolType::RoundNoseEnd: {
            z = RoundNoseVertProtZ(T, v, p, d2);
            break;
        }
        case ToolType::TaperEnd: {
            z = TaperEndVertProtZ(T,v.Z(),d2);
            break;
        }
        default:
            break;
        }
        return z;
    }

    static double TriangleVertProtZ(const DefTool &T, const Triangle &t,
                                    const ofts::Point &p) {
        double z = Min_Val;
        double z0 = VertexProtectZ(T, t.P0(), p);
        double z1 = VertexProtectZ(T, t.P1(), p);
        double z2 = VertexProtectZ(T, t.P2(), p);
        z = LimVal::ThreeMax(z0, z1, z2);
        return z;
    }
};

} // namespace grm

#endif // INTVERTEXPROT_H
