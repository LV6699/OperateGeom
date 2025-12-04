#ifndef CUTTERLOCATIONZ_H
#define CUTTERLOCATIONZ_H

#include "IntEdgeProt.h"
#include "IntVertexProt.h"

#pragma optimize("", off)
#pragma GCC optimize("o0")

namespace grm {
class CutLocateZ {
public:
  CutLocateZ() {}

  static double OftTriangleCase(const DefTool &T, const Triangle &t,
                                const oft::Point &p) {
    double z = Min_Val;
    double t1 = t.N().X() * (t.P0().X() - p.X());
    double t2 = t.N().Y() * (t.P0().Y() - p.Y());
    z = t.P0().Z() + (t1 + t2) / t.N().Z();
    return z;
  }
  static double CutterLocation(const MeshMap &m, oft::Point &p) {
    OperTriaCl ot;
    double z1 = p.Z(), z2 = p.Z(), z3 = p.Z();

    ClRelItem item;
    size_t id1 = 0, id2 = 0, id3 = 0;
    oft::Point op(26, -28);

    const auto &ts = m.Triangles();
    const auto &clts = m.TrianglesCl();
    const auto &T = m.Tool();

    for (size_t i = 0; i < clts.size(); ++i) { /**590*/
      const auto &t = clts[i];                 /***/
      if (p.IsSamePoint2D(op, PreErr_4) /*&& i == 0*/) {
        // ViewTool::DisplayPoint(p);
        // ViewTool::DisplayTriangle(t,_colors[3]);
        int tem = 1;
      }
      if (t.IsInRange(p) && t.N().Z() > PreErr_8) {
        double z = OftTriangleCase(T, t, p);
        if (z > z1) {
          z1 = z;
          id1 = i;
        }
      }
    }
    for (size_t i = 0; i < ts.size(); ++i) { /***/
      const auto &t = ts[i];                 /***/
      if (p.IsSamePoint2D(op, PreErr_4) && i == 1490 /**/) {
        // ViewTool::DisplayPoint(p);
        // ViewTool::DisplayTriangle(t,_colors[3]);
        int tem = 1;
      }

      double z = IntVertProt::TriangleVertProtZ(T, t, p);
      if (z > z2) {
        z2 = z;
        id2 = i;
      }
      z = IntEdgeProt::TriangleEdgeCase(T, t, p);
      if (z > z3) {
        z3 = z;
        id3 = i;
      }
    }
    if (p.IsSamePoint2D(op, PreErr_4)) {
      std::cout << 1 << std::endl;
    }
    return LimVal::ThreeMax(z1, z2, z3);
  }

  static double CurCutterLocation(const MeshMap &m, oft::Point &p) {
    double cur_z = Min_Val;
    oft::Point op(26, -28);

    const auto &ts = m.Triangles();
    const auto &clts = m.TrianglesCl();
    const auto &T = m.Tool();

    for (size_t i = 0; i < clts.size(); ++i) { /**590*/
      const auto &t = clts[i];                 /***/
      if (p.IsSamePoint2D(op, PreErr_4) /*&& i == 0*/) {
        // ViewTool::DisplayPoint(p);
        // ViewTool::DisplayTriangle(t,_colors[3]);
        int tem = 1;
      }
      if (t.IsInRange(p) && t.N().Z() > PreErr_8) {
        double z = OftTriangleCase(T, t, p);
      }
    }

    return 0;
  }

  static void CutterLocation(MeshMap &m) {
    double z = 0;
    auto &pts = m._clPts;
    for (size_t i = 0; i < pts.size(); ++i) {
      for (size_t j = 0; j < pts[i].size(); ++j) {
        z = CutterLocation(m, pts[i][j]);
        pts[i][j].SetZ(z);
      }
    }
  }
};

void GetPtAllLocation(const MeshMap &m, const oft::Point &p,
                      std::vector<ClRelItem> &rels);
void ClRelItemToShape(ClRelItem &item);

} // namespace grm

#endif // CUTTERLOCATIONZ_H
