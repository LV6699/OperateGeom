#include "CutLocateZ.h"

using std::vector;
using namespace oft;

typedef grm::CutLocateZ clz;
typedef grm::IntVertProt ivp;
typedef grm::IntEdgeProt iep;

#pragma optimize("", off)

namespace grm{
void GetPtAllLocation(const MeshMap& m,const Point& p,
                      vector<ClRelItem>& rels)
{
    OperTriaCl ot;
    const auto& ts = m.Triangles();
    const auto& T = m.Tool();

    for(size_t i = 0;i < ts.size();++i){/***/
        if(/**p.IsSamePoint2D(op,PreErr_4) &&*/ i == 386){
            int tem = 1;
        }
        const auto& t = ts[i];
        if(t.IsInRange(p) && t.N().Z() > PreErr5_10){
            double z = clz::OftTriangleCase(_meshMap.Tool(),t,p);
            rels.push_back(ClRelItem(ClRelType::OftTriangle,t,i,z));
        }
        {
            double z0 = Min_Val,z1 = Min_Val,z2 = Min_Val;
            z0 = ivp::VertexProtectZ(T,t.P0(),p);
            z1 = ivp::VertexProtectZ(T,t.P1(),p);
            z2 = ivp::VertexProtectZ(T,t.P2(),p);
            if(z0 != Min_Val){
                rels.push_back(ClRelItem(ClRelType::PtProtect,t.P0(),i,z0));
            }
            if(z1 != Min_Val){
                rels.push_back(ClRelItem(ClRelType::PtProtect,t.P1(),i,z1));
            }
            if(z2 != Min_Val){
                rels.push_back(ClRelItem(ClRelType::PtProtect,t.P2(),i,z2));
            }
        }
        {
            double z0 = Min_Val,z1 = Min_Val,z2 = Min_Val;
            if(t.IsInE0Range(p,T.R()+PreErr_10)){
                z0 = iep::EdgeProtectCase(T,t.P0(),t.P1(),p);
            }
            if(t.IsInE1Range(p,T.R()+PreErr_10)){
                z1 = iep::EdgeProtectCase(T,t.P1(),t.P2(),p);
            }
            if(t.IsInE2Range(p,T.R()+PreErr_10)){
                z2 = iep::EdgeProtectCase(T,t.P2(),t.P0(),p);
            }
            if(z0 != Min_Val){
                rels.push_back(ClRelItem(ClRelType::EdgeProtecct,t.P0(),t.P1(),i,z0));
            }
            if(z1 != Min_Val){
                rels.push_back(ClRelItem(ClRelType::EdgeProtecct,t.P1(),t.P2(),i,z1));
            }
            if(z2 != Min_Val){
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
