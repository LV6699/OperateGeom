#ifndef INTVERTEXPROT_H
#define INTVERTEXPROT_H

#include "OperTriaCl.h"
#include "BaseCalc.h"

namespace grm
{

    class IntVertProt
    {
    public:
        IntVertProt() {}

        static double VertexProtectZ(const DefTool &T, const oft::Point &v,
                                     const oft::Point &p)
        {
            double RR = T._R * T._R;
            double dd = p.DistSquare2D(v);
            if (dd > RR)
            {
                return Min_Val;
            }

            double z = Min_Val;
            double h = std::sqrt(RR - dd);
            switch (T._type)
            {
            case ToolType::PlaneEnd:
            {
                z = v.Z();
                break;
            }
            case ToolType::BallNoseEnd:
            {
                z = v.Z() + h - T._R;
                break;
            }
            case ToolType::RoundNoseEnd:
            {
                double d = std::sqrt(dd);
                double sub = T._R - T._cr;
                if (dd <= sub * sub)
                {
                    z = v.Z();
                }
                else if (pnum::RightInc2(sub, d, T._R))
                {
                    double rr = T._cr * T._cr;
                    double t = d - T._R + T._cr;
                    double tt = t * t;
                    double h = std::sqrt(rr - tt);
                    z = v.Z() + h;
                }
                break;
            }
            default:
                break;
            }
            return z;
        }

        static double TriangleVertProtZ(const DefTool &T, const Triangle &t,
                                        const oft::Point &p)
        {
            double z = Min_Val;
            double z0 = VertexProtectZ(T, t.P0(), p);
            double z1 = VertexProtectZ(T, t.P1(), p);
            double z2 = VertexProtectZ(T, t.P2(), p);
            z = LimVal::ThreeMax(z0, z1, z2);
            return z;
        }
    };

}

#endif // INTVERTEXPROT_H
