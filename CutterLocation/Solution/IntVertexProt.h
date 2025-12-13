#ifndef INTVERTEXPROT_H
#define INTVERTEXPROT_H

#include "BaseCalc.h"
#include "OperTriaCl.h"


namespace grm {

class IntVertProt {
public:
  IntVertProt() {}

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
      double h = std::sqrt(T.RR() - d2);
      z = v.Z() + h - T._R;
      break;
    }
    case ToolType::RoundNoseEnd: {
      double sub = T._R - T._cr;
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
