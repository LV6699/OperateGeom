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

  static double ProtectFaceZ(const DefTool& T, const Triangle& t,
                             const ofts::Point& p) {
    double z = Min_Val;
    double t1 = t.N().X() * (t.P0().X() - p.X());
    double t2 = t.N().Y() * (t.P0().Y() - p.Y());
    z = t.P0().Z() + (t1 + t2) / t.N().Z();
    return z;
  }
  static double CutterLocation(const MeshMap& m, ofts::Point& p) {
    OperTriaCl ot;
    double z1 = p.Z(), z2 = p.Z(), z3 = p.Z();

    ClRelItem item;
    size_t id1 = 0, id2 = 0, id3 = 0;
    ofts::Point op1(48, 16);

    const auto& ts = m.Triangles();
    const auto& clts = m.TrianglesCl();
    const auto& T = m.Tool();

    for (size_t i = 0; i < clts.size(); ++i) { /**590*/
      const auto& t = clts[i];                 /***/
      if (p.IsSamePoint2D(p, PreErr_4) /*&& i == 0*/) {
        // ViewTool::DisplayPoint(p);
        // ViewTool::DisplayTriangle(t,_colors[3]);
        int tem = 1;
      }
      if (t.IsInRange(p) && t.N().Z() > PreErr_8) {
        double z = ProtectFaceZ(T, t, p);
        if (z > z1) {
          z1 = z;
          id1 = i;
        }
      }
    }
    for (size_t i = 0; i < ts.size(); ++i) { /***/
      const auto& t = ts[i];                 /***/
      if (p.IsSamePoint2D(p, PreErr_4) && i == 1490 /**/) {
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
    } /**
     if (p.IsSamePoint2D(p, PreErr_4)) {
       std::cout << 1 << std::endl;
     }*/
    return LimVal::ThreeMax(z1, z2, z3);
  }
  
  static double CurrentLocation(const MeshMap& m, ofts::Point& op) 
  {
    double cur_z = m.ModelData().MinZPt().Z();
    const auto& clts = m.TrianglesCl();
    const auto& T = m.Tool();
    double RR = T.RR();
#if 0
    ofts::Point op1(30,-36);
    if(op.IsSameCoord2D(op1,PreErr_8)){
        int tem = 1;
    }
#endif
    #define ResetResult(z, cur_z){if (z > cur_z) {cur_z = z;}}

    {
      /// 在保护面上求解
      for (size_t i = 0; i < clts.size(); ++i) {
        const auto& t = clts[i]; /**
         if (op.IsSamePoint2D(op, PreErr_4)) {
           int tem = 1;
         }*/
        /// 处理无效三角形
        if (t.N().Z() < PreErr_8 || !t.IsInRange(op)) {
          continue;
        }
        double z = ProtectFaceZ(T, t, op);
        ResetResult(z, cur_z);
      }
    }
    {
      /// 在点保护面上求解
      const auto& verts = m.GetTVerts();
      for (size_t i = 0; i < verts.size(); ++i) {
        //continue;
        const auto& v = verts[i];
        double z = IntVertProt::VertexProtectZ(T, v, op);
        ResetResult(z, cur_z);
      }
    }
    {
      /// 在边保护面上求解
      const auto& edges = m.GetTEdges();
      for (size_t i = 0; i < edges.size(); i++) {
        //continue;
        /*if(i == 1591){
            std::cout<<"";
        }*/
        const auto& e = edges[i];
        const auto& p0 = e.P0();
        const auto& p1 = e.P1();
        ///处理竖直边
        if (p0.IsSameCoord2D(p1,PreErr5_6)) {  
          continue;
        }
        double z = 0;
        auto pro = BaseCalc::GetProjPoint(op, p0, p1);
        double ym2 = op.DistSquare2D(pro._p);
        if (ym2 > RR) {
          continue;
        }
        ///处理水平边
        if (p1.Z() - p0.Z() < PreErr5_6) {
          ofts::Point v;
          if (pro._val < 0) {v = p0;} 
          else if (pro._val > 1) {v = p1;} 
          else {v = pro._p;}
          z = IntVertProt::VertexProtectZ(T, v, op);
          ResetResult(z, cur_z);
          continue;
        } 
        z = IntEdgeProt::EdgeProtectCase(T,pro,p0,p1,op,cur_z);
        //if(z > 15){
            //int tem = 1;
        //}
        ResetResult(z, cur_z);
      }
    }

    return cur_z;
  }

  static void CutterLocation(MeshMap& m) {
    double z = 0;
    auto& pts = m._clPts;
    for (size_t i = 0; i < pts.size(); ++i) {
      for (size_t j = 0; j < pts[i].size(); ++j) {
        // z = CutterLocation(m, pts[i][j]);
        //if(i == 14 && j == 90){
            //std::cout<<1<<std::endl;
        //}
        z = CurrentLocation(m, pts[i][j]);
        pts[i][j].SetZ(z);
      }
    }
  }
};

void GetPtAllLocation(const MeshMap& m, const ofts::Point& p,
                      std::vector<ClRelItem>& rels);
void ClRelItemToShape(ClRelItem& item);

}  // namespace grm

#endif  // CUTTERLOCATIONZ_H




















