#include "OperateObject.h"
#include "ui_OperateObject.h"
#include<StdSelect_BRepOwner.hxx>

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
    auto context = _mainwind->myOccView->getContext();
    ClearAllLabel();
    if(context.IsNull()){return;}

    context->InitSelected();
    if (context->NbSelected() == 0){return;}
    oft::Point v,sp,ep,cp;
    _hasUiFind = false;
    bool hasFound = false,isVetex = false,isLine = false;

    for (context->InitSelected();context->MoreSelected(); context->NextSelected()){
        Handle(SelectMgr_EntityOwner) sel = context->SelectedOwner();
        if (sel.IsNull()){continue;}

        Handle(StdSelect_BRepOwner) brepSel =
                Handle(StdSelect_BRepOwner)::DownCast(sel);
        if (brepSel.IsNull()){continue;}

        TopoDS_Shape shape = brepSel->Shape();
        if(shape.ShapeType() != TopAbs_VERTEX &&
                shape.ShapeType() != TopAbs_EDGE){continue;}

        Standard_Real first,last;
        TopoDS_Edge edge = TopoDS::Edge(shape);
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if(shape.ShapeType() == TopAbs_VERTEX){
            TopoDS_Vertex myvertex = TopoDS::Vertex(shape);
            v = ToPoint(BRep_Tool::Pnt(myvertex));
            std::cout<<"已拾取到点...\n";
            isVetex = true;
            hasFound = true;
            break;
        }
        sp = ToPoint(curve->Value(first));
        ep = ToPoint(curve->Value(last));
        ///gp_Pnt mp = curve->Value((first+last)/2);
        Handle(Geom_TrimmedCurve) trimmedCurve = Handle(Geom_TrimmedCurve)::DownCast(curve);
        if (!trimmedCurve.IsNull()) {
            curve = trimmedCurve->BasisCurve();
        }
        if (curve->DynamicType() == STANDARD_TYPE(Geom_Circle)) {
            Handle(Geom_Circle) circle = Handle(Geom_Circle)::DownCast(curve);
            const gp_Circ& circ = circle->Circ();
            cp = ToPoint(circ.Location());
            isLine = true;
            std::cout<<"已拾取到圆弧...\n";
        }else{
            std::cout<<"已拾取到线段...\n";
        }
        hasFound = true;
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

void OperateObject::FindTriageCutterLocate()
{
    if(!_hasUiFind || _isPoint){std::cout<<"No data found!"<<endl;return;}
    DisplayGeom dg;GeomToShape gts;Handle(AIS_Shape) a;
    if(!_trisCl_as.IsNull()){dg.RemoveAShape(_trisCl_as);}
    if(!_tris_as.IsNull()){dg.RemoveAShape(_tris_as);}
    _hasFind = false;
    const auto& sp = _selObj.StarPt();
    grm::Triangle tri,tri1;
    const auto& ep = _selObj.EndPt();
    for(auto& t : _meshMap._trisCl){
        if(t.IsVertex(sp) || t.IsVertex(ep)){
            tri = t;
            tri1 = grm::Triangle(*t._op0,*t._op1,*t._op2);
            _hasFind = true;
            break;
        }
    }
    if(!_hasFind){std::cout<<"No data found!"<<endl;return;}
    Quantity_Color c(0.3,0.35,0.35,Quantity_TOC_RGB);

    TopoDS_Shape shape = gts.TriangleToShape(tri);
    TopoDS_Shape shape1 = gts.TriangleToShape(tri1);
    _trisCl_as = dg.ShapeToAis(shape,_colors[1],1);
    _tris_as = dg.ShapeToAis(shape1,c,2);

    dg.DisplayAShape(_trisCl_as,false);
    dg.DisplayAShape(_tris_as,false);
}

























