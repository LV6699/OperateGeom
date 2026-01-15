#include "OperateObject.h"
#include "ui_OperateObject.h"
#include<StdSelect_BRepOwner.hxx>
#include "WidgetTool.h"
#include"../Common/ViewTool.h"
#include "../../OperateView/DisplayGeom.h"
#include "../../OperateView/GeomToShape.h"
#include "../../ViewWindow/MainWindow.h"

#pragma optimize("", off)
#pragma GCC optimize ("O0")

OperateObject::OperateObject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OperateObject)
{
    ui->setupUi(this);
}

OperateObject::~OperateObject(){delete ui;}

class myfilter : public SelectMgr_Filter{
public:
    using SelectMgr_Filter::SelectMgr_Filter;
    virtual Standard_Boolean IsOk(const opencascade::handle<SelectMgr_EntityOwner> &anobj)
    const override{
        return !anobj->Selectable()->IsKind(STANDARD_TYPE(AIS_TextLabel));
    }
};

void InitLabel(Handle(AIS_TextLabel)& _label)
{
    _label->SetColor({0, 0, 0,Quantity_TOC_RGB});
    _label->SetZoomable(Standard_False);
    ///_label->SetFontAspect(Font_FontAspect_Bold);
    _label->SetHeight(12);
    _label->SetFont("sans-serif");
}

TCollection_ExtendedString ToOccData(const QString& str){
    QByteArray bytes = str.toUtf8();
    TCollection_AsciiString astr(bytes.constData());
    return TCollection_ExtendedString(astr);
}
QString PntToString(const gp_Pnt& p){
    return QString("%1,%2").arg(p.X()).arg(p.Y());
}
std::string PtToString(const ofts::Point& p)
{
    return "(" + std::to_string(p.X()) + "," +
            std::to_string(p.Y()) + "," + std::to_string(p.Z()) + ")";
}
void OperateObject::ClearAllLabel()
{
    for(auto& d : _labels){
        _mainwind->myOccView->getContext()->Remove(d,true);
    }
    _mainwind->myOccView->update();
}
void OperateObject::CreateInfo()
{

}

void OperateObject::FindSelectObject()
{
    const auto& context = _mainwind->myOccView->getContext();
    ClearAllLabel();
    if(context.IsNull()){return;}

    context->InitSelected();
    if (context->NbSelected() == 0){return;}
    ofts::Point v,sp,ep,cp;
    _hasUiFind = false;
    bool hasFound = false,isVetex = false,isLine = false;

    for(context->InitSelected();context->MoreSelected(); context->NextSelected()){
        Handle(SelectMgr_EntityOwner) sel = context->SelectedOwner();
        if (sel.IsNull()){continue;}

        Handle(StdSelect_BRepOwner) brepSel =
                Handle(StdSelect_BRepOwner)::DownCast(sel);
        if (brepSel.IsNull()){continue;}

        TopoDS_Shape shape = brepSel->Shape();
        if(shape.ShapeType() != TopAbs_VERTEX &&
                shape.ShapeType() != TopAbs_EDGE){continue;}
        hasFound = true;
        if(shape.ShapeType() == TopAbs_VERTEX){
            TopoDS_Vertex myvertex = TopoDS::Vertex(shape);
            v = ToPoint(BRep_Tool::Pnt(myvertex));
            std::cout<<"已拾取到点:"<<v.X()<<","<<v.Y()<<std::endl;
            isVetex = true;
            break;
        }
        Standard_Real first,last;
        TopoDS_Edge edge = TopoDS::Edge(shape);
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        sp = ToPoint(curve->Value(first));
        ep = ToPoint(curve->Value(last));
        ///gp_Pnt mp = curve->Value((first+last)/2);
        Handle(Geom_TrimmedCurve) trimmedCurve = Handle(Geom_TrimmedCurve)::DownCast(curve);
        if (!trimmedCurve.IsNull()) {
            curve = trimmedCurve->BasisCurve();
        }
        break;
    }
    if (!hasFound){
        std::cout<<"未拾取到任何期望目标...\n";
        return;
    }
    if(isVetex){_p = v;
    }else{
        if(isLine){_selObj = ofts::DefElem(LINETYPE,ofts::DefSeg(sp,ep));}
        else{_selObj = ofts::DefElem(ARCTYPE,ofts::DefArc(sp,ep,cp,true));}
    }
    _isPoint = isVetex;
    _hasUiFind = true;
}

void OperateObject::FindTrianges()
{
    if(_isPoint){return;}
    WidgetTool().DisplayOperItem(_opeItem._sel_ts);
    WidgetTool().DisplayOperItem(_opeItem._sel_clts);
    _opeItem._sel_ts = ViewObj::ViewItem();
    _opeItem._sel_clts = ViewObj::ViewItem();
    DisplayGeom dg;GeomToShape gts;Handle(AIS_Shape) a;

    bool hasFind = false;
    bool isorigin = false;
    grm::Triangle clt,tri;
    const auto& sp = _selObj.StarPt();
    const auto& ep = _selObj.EndPt();
    for(size_t i = 0;i < _meshMap._trisCl.size();++i) {
        const auto& t = _meshMap._trisCl[i];
        if(t.IsVertex(sp) && t.IsVertex(ep)){
            clt = t;
            tri = grm::Triangle(t._ot->P0(),t._ot->P1(),t._ot->P2());
            hasFind = true;
            std::cout<<"偏置三角形索引:"<<i<<std::endl;
            break;
        }
    }
    if(!hasFind){
        const auto& tris = _meshMap.Triangles();
        for(size_t i = 0;i < tris.size();++i) {
            const auto& t = _meshMap._tris[i];
            if(t.IsVertex(sp) && t.IsVertex(ep)){
                clt = t;
                hasFind = true;isorigin = true;
                std::cout<<"离散三角形索引:"<<i<<std::endl;
                break;
            }
        }
    }
    if(!hasFind){std::cout<<"No data found!"<<endl;return;}
    TopoDS_Shape shape = ViewTool::TriangleToShape(clt);
    TopoDS_Shape shape1 = ViewTool::TriangleToShape(tri);
    _opeItem._sel_ts = ViewObj::ViewItem(shape,_colors[6],2);
    _opeItem._sel_clts = ViewObj::ViewItem(shape1,_colors[4],1);

    WidgetTool().DisplayOperItem(_opeItem._sel_ts);
    WidgetTool().DisplayOperItem(_opeItem._sel_clts);
}

void OperateObject::FindDiscreteEdge()
{
    if(_isPoint || !_hasUiFind){return;}
    
    size_t id = 0;
    bool hasFind = false;
    const auto& sp = _selObj.StarPt();
    const auto& ep = _selObj.EndPt();
    const auto& edges = _meshMap.GetTEdges();

    for (size_t i = 0; i < edges.size(); i++) {
        const auto& p0 = edges[i]._p0;
        const auto& p1 = edges[i]._p1;
        if ((p0.IsSameCoord3D(sp, PreErr5_6) &&
             p1.IsSameCoord3D(ep, PreErr5_6))) {
            std::cout<<"离散边索引:"<<i<<"->" <<PtToString(p0)<<","<<PtToString(p1)<< std::endl;
            hasFind = true;
            break;
        }
        if ((p0.IsSameCoord3D(ep, PreErr5_6) &&
             p1.IsSameCoord3D(sp, PreErr5_6))) {
            std::cout<<"离散边索引:"<<i<<"->" <<PtToString(p0)<<","<<PtToString(p1)<< std::endl;
            hasFind = true;
            break;
        }
    }
    if(!hasFind){std::cout<<"No data found!"<<endl;return;}
    auto shape = ViewTool::SegmentToShape(edges[id]._p0,edges[id]._p1);
    _opeItem._oriEdge = ViewObj::ViewItem(shape,_colors[1],2);
    WidgetTool().DisplayOperItem(_opeItem._sel_ts);
}

void OperateObject::FindSelItem()
{
    FindSelectObject();
    if(!_hasUiFind){return;}
    if(_isPoint){GetSelPointIndex();
    } else {
        //FindTrianges();
        FindDiscreteEdge();
    }
}

void OperateObject::FindIntBasePoint(const ofts::Point& p)
{
    ///查找偏置面相交信息
    GeomToShape gts;DisplayGeom dg;
    WidgetTool().DisplayOperItem(_opeItem._int_clts);
    _opeItem._int_clts = ViewObj::ViewItem();
    bool hasFind = false;

    vector<grm::Triangle>ts;
    for (size_t i = 0;i < _meshMap._trisCl.size();++i) {
        if(_meshMap._trisCl[i].IsInRange(p)){
            ts.push_back(_meshMap._trisCl[i]);
            hasFind = true;
        }
    }
    if(!hasFind){std::cout<<"No data found!"<<endl;return;}
    Quantity_Color c(0.4,0.4,0,Quantity_TOC_RGB);
    vector<TopoDS_Shape>shapes;
    for(auto& t : ts){
        TopoDS_Shape s = ViewTool::TriangleToShape(t);
        shapes.push_back(s);
    }
    TopoDS_Shape shape;
    gts.ShapesToShape(shapes,shape);
    _opeItem._int_clts = ViewObj::ViewItem(shape,_colors[5],2);
    WidgetTool().DisplayOperItem(_opeItem._int_clts);

}

void OperateObject::GetSelPointIndex()
{
    for (size_t i = 0;i < _meshMap._clPts.size();++i) {
        const auto& ps = _meshMap._clPts[i];
        for (size_t j = 0;j < ps.size();++j) {
            if(_p.IsSameCoord2D(ps[j],PreErr5_4)){
                const auto& p = ps[j];
                std::cout<<"刀位点:("<<p.X()<<","<<p.Y()<<","
                        <<p.Z()<<"),索引:"<<i<<","<<j<<std::endl;
                return;
            }
        }
    }
    std::cout<<"未找到有效刀位点."<<std::endl;
}

void OperateObject::GetTrianglesVert()
{
    if(!_isPoint || !_hasUiFind){return;}
    const auto& vs = _meshMap._tverts;
    for (size_t i = 0; i < vs.size(); i++){
        if(_p.IsSameCoord2D(vs[i],PreErr5_6)){
            std::cout<<"原始三角形顶点索引:"<<i<<","<< ViewTool::PointToString(_p) << std::endl;
            return;
        }
    }
    std::cout<<"No data found!"<<endl;
}





















