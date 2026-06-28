#include "CutLocateZ.h"

using std::vector;
using namespace ofts;

typedef grm::CutLocateZ clz;
typedef grm::IntVertProt ivp;
typedef grm::IntEdgeProt iep;

#pragma optimize("", off)

namespace grm{

double CutLocateZ::CurrentLocation(const MeshMap& m, ofts::Point& op)
{
    IntVertProt verp;IntEdgeProt edgp;
    double cur_z = m.ModelData().MinZPt().Z();
    const auto& clts = m.TrianglesCl();
    const auto& T = m.Tool();
    double RR = T.RR();
#if 0
    ofts::Point op1(20,-36,25.3137/*22,26,27.6066*/);///31,-24,22.6569 28,-24,25.0757
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
            if (t._limVal._maxz <= cur_z) {
                continue;
            }
            if (t.N().Z() < PreErr_8 || !t.IsInRange(op)) {
                continue;
            }
            double z = ProtectFaceZ(t, op);
            ResetResult(z, cur_z);
            if(std::isinf(cur_z)){
                int tem = 1;
            }
        }
    }
    {
        /// 在点保护面上求解
        const auto& verts = m.GetTVerts();
        for (size_t i = 0; i < verts.size(); ++i) {
            //continue;
            const auto& v = verts[i];
            if(v.Z() <= cur_z){
                continue;
            }
            double z = verp.VertexProtectZ(T, v, op);
            ResetResult(z, cur_z);
            if(std::isinf(z)){
                int tem = 1;
            }
        }
        if(std::isinf(cur_z)){
            int tem = 1;
        }
    }
    {
        /// 在边保护面上求解
        const auto& edges = m.GetTEdges();
        for (size_t i = 0; i < edges.size(); ++i) {
            //continue;
            /*if (i == 1641) {
                std::cout << "i="<<i<<std::endl;
            }*/
            const auto& e = edges[i];
            const auto& p0 = e.P0();
            const auto& p1 = e.P1();
            if(p1.Z() <= cur_z){
                continue;
            }
            ///处理竖直边
            if (p0.IsSameCoord2D(p1, PreErr5_6)) {
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
                //v = pro._p;
                z = verp.VertexProtectZ(T, v, op);
                ResetResult(z, cur_z);
                if(std::isinf(z)){
                    int tem = 1;
                }
                continue;
            }
            z = edgp.EdgeProtectCase(T,pro,p0,p1,op,cur_z,i);
            //if(z > 15){
            //int tem = 1;
            //}
            ResetResult(z, cur_z);
            if(std::isinf(z)){
                int tem = 1;
            }
        }
    }

    return cur_z;
}

void CutLocateZ::CutterLocation(MeshMap& m) {
    double z = 0;
    auto& pts = m._clPts;
    for (size_t i = 0; i < pts.size(); ++i) {
        for (size_t j = 0; j < pts[i].size(); ++j) {
            // z = CutterLocation(m, pts[i][j]);
            if(i == 0 && j == 38){
                std::cout<<1<<std::endl;
            }
            z = CurrentLocation(m, pts[i][j]);
            if(std::isinf(z) || m._xEdges[0][37]._ep->Z() < -100 || m._xEdges[0][37]._ep->Z() > 100){
                std::cout<<1<<std::endl;
            }
            pts[i][j].SetZ(z);
        }
    }
}


void GetPtAllLocation(const MeshMap& m,const Point& p,
                      vector<ClRelItem>& rels)
{
    OperTriaCl ot;IntEdgeProt edgp;
    const auto& ts = m.Triangles();
    const auto& T = m.Tool();

    for(size_t i = 0;i < ts.size();++i){/***/
        if(/**p.IsSamePoint2D(op,PreErr_4) &&*/ i == 386){
            int tem = 1;
        }
        const auto& t = ts[i];
        if(t.IsInRange(p) && t.N().Z() > PreErr5_10){
            double z = clz::ProtectFaceZ(t,p);
            rels.push_back(ClRelItem(ClRelType::OftTriangle,t,i,z));
        }
        {
            double z0 = Min_Value,z1 = Min_Value,z2 = Min_Value;
            z0 = ivp::VertexProtectZ(T,t.P0(),p);
            z1 = ivp::VertexProtectZ(T,t.P1(),p);
            z2 = ivp::VertexProtectZ(T,t.P2(),p);
            if(z0 != Min_Value){
                rels.push_back(ClRelItem(ClRelType::PtProtect,t.P0(),i,z0));
            }
            if(z1 != Min_Value){
                rels.push_back(ClRelItem(ClRelType::PtProtect,t.P1(),i,z1));
            }
            if(z2 != Min_Value){
                rels.push_back(ClRelItem(ClRelType::PtProtect,t.P2(),i,z2));
            }
        }
        {
            double z0 = Min_Value,z1 = Min_Value,z2 = Min_Value;
            if(t.IsInE0Range(p,T.R()+PreErr_10)){
                auto pro = BaseCalc::GetProjPoint(p, t.P0(), t.P1());
                z0 = edgp.EdgeProtectCase(T, pro, t.P0(), t.P1(), p,z0,i);
            }
            if(t.IsInE1Range(p,T.R()+PreErr_10)){
                auto pro = BaseCalc::GetProjPoint(p, t.P1(), t.P2());
                z1 = edgp.EdgeProtectCase(T, pro, t.P1(), t.P2(), p,z1,i);
            }
            if(t.IsInE2Range(p,T.R()+PreErr_10)){
                auto pro = BaseCalc::GetProjPoint(p, t.P2(), t.P0());
                z2 = edgp.EdgeProtectCase(T, pro,t.P2(), t.P0(), p,z2,i);
            }
            if(z0 != Min_Value){
                rels.push_back(ClRelItem(ClRelType::EdgeProtecct,t.P0(),t.P1(),i,z0));
            }
            if(z1 != Min_Value){
                rels.push_back(ClRelItem(ClRelType::EdgeProtecct,t.P1(),t.P2(),i,z1));
            }
            if(z2 != Min_Value){
                rels.push_back(ClRelItem(ClRelType::EdgeProtecct,t.P2(),t.P0(),i,z2));
            }

        }

    }
}

void ClRelItemToShape(ClRelItem& item)
{
    TopoDS_Shape shape;gp_Pnt pos;
    Handle(AIS_TextLabel) textLabel = new AIS_TextLabel();
    switch (item.Type()) {
    case ClRelType::OftTriangle:{
        shape = ViewTool::TriangleToShape(item.Tria());
        auto cen = item.Tria().Centroid();
        pos = gp_Pnt(cen.X(),cen.Y(),cen.Z());
        break;
    }
    case ClRelType::PtProtect:{
        const auto& p = item.P();
        BRepBuilderAPI_MakeVertex v(gp_Pnt(p.X(),p.Y(),p.Z()));
        shape = v;
        pos = gp_Pnt(p.X(),p.Y(),p.Z());
        break;
    }
    case ClRelType::EdgeProtecct:{
        shape = ViewTool::SegmentToShape(item.SP(),item.EP());
        auto mp = item.SP().Midpoint(item.EP());
        pos = gp_Pnt(mp.X(),mp.Y(),mp.Z());
        break;
    }
    default:{
        std::cout<<"未知来源对象类型"<<std::endl;
        return;
    }
    }
    if(shape.IsNull()){
        return;
    }
    item._vieItem._aShape = new AIS_Shape(shape);
    item._vieItem._aShape->SetColor(_colors[5]);
    item._vieItem._aShape->SetWidth(2);

    std::stringstream ss;
    ss << item.Id();
    ss << ",";
    ss << item.Z();
    std::string strValue = ss.str();
    textLabel->SetText(strValue.c_str());
    textLabel->SetPosition(pos);
    textLabel->SetColor(Quantity_NOC_BLACK);
    textLabel->SetHeight(0.5);

    item._vieItem._texAspe = textLabel;
    item._vieItem._hasInitial = true;
}



}
