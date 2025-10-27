#include "OperateObject.h"
#include "ui_OperateObject.h"
#include<StdSelect_BRepOwner.hxx>
#include "WidgetTool.h"
#include"../Common/ViewData.h"

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
    oft::Point v,sp,ep,cp;
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
    if(isVetex){
        _p = v;
    }else{
        if(isLine){_selObj = oft::DefElem(LINETYPE,oft::DefSeg(sp,ep));}
        else{_selObj = oft::DefElem(ARCTYPE,oft::DefArc(sp,ep,cp,true));}
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
    grm::Triangle clt,tri;
    const auto& sp = _selObj.StarPt();
    const auto& ep = _selObj.EndPt();
    for(size_t i = 0;i < _meshMap._trisCl.size();++i) {
        const auto& t = _meshMap._trisCl[i];
        if(t.IsVertex(sp) && t.IsVertex(ep)){
            clt = t;
            tri = grm::Triangle(*t._op0,*t._op1,*t._op2);
            hasFind = true;
            std::cout<<"偏置三角形索引:"<<i<<std::endl;
            break;
        }
    }
    if(!hasFind){std::cout<<"No data found!"<<endl;return;}
    TopoDS_Shape shape = grm::ToOcc::TriangleToShape(clt);
    TopoDS_Shape shape1 = grm::ToOcc::TriangleToShape(tri);
    _opeItem._sel_ts = ViewObj::ViewItem(shape,_colors[6],2);
    _opeItem._sel_clts = ViewObj::ViewItem(shape1,_colors[4],1);

    WidgetTool().DisplayOperItem(_opeItem._sel_ts);
    WidgetTool().DisplayOperItem(_opeItem._sel_clts);
}

void OperateObject::FindSelItem()
{
    FindSelectObject();
    FindTrianges();
}

void OperateObject::FindIntBasePoint(const oft::Point& p)
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
        TopoDS_Shape s = grm::ToOcc::TriangleToShape(t);
        shapes.push_back(s);
    }
    TopoDS_Shape shape;
    gts.ShapesToShape(shapes,shape);
    _opeItem._int_clts = ViewObj::ViewItem(shape,_colors[5],2);
    WidgetTool().DisplayOperItem(_opeItem._int_clts);

}























