#include "debuginfodialog.h"
#include "ui_debuginfodialog.h"
#include "MainWindow/mainwindow.h"
#include "MainWindow/OCCView.h"
#include "BRep_Tool.hxx"
#include <SelectMgr_Filter.hxx>
//#include"HearderFile.h"
#include "QDebug"
std::map<Handle(AIS_Shape), RecordData> record_map;
///DefElem m_occCurSelectEle;
class myfilter : public SelectMgr_Filter{
public:
    using SelectMgr_Filter::SelectMgr_Filter;
    // 过滤掉标签
    virtual Standard_Boolean IsOk(const opencascade::handle<SelectMgr_EntityOwner> &anobj)
    const override{
        return !anobj->Selectable()->IsKind(STANDARD_TYPE(AIS_TextLabel));
    }
};
void init_label(Handle(AIS_TextLabel)& label)
{
    label->SetColor({0, 0, 0,Quantity_TOC_RGB});
    label->SetZoomable(Standard_False);
//    label->SetFontAspect(Font_FontAspect_Bold);
    label->SetHeight(12);
    label->SetFont("sans-serif");
}
TCollection_ExtendedString toocc(const QString& str){
    QByteArray bytes = str.toUtf8();
    TCollection_AsciiString astr(bytes.constData());
    return TCollection_ExtendedString(astr);
}
QString ptoa(const gp_Pnt& p){
    return QString("%1,%2,%3").arg(p.X()).arg(p.Y()).arg(p.Z());
}
DebugInfoDialog::DebugInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugInfoDialog)
{
    ui->setupUi(this);
    auto context = _mainwind->get_OCCView()->getContext();
    context->AddFilter(new myfilter());
    context->SelectionStyle()->SetColor({0, 0, 1,Quantity_TOC_RGB});
    /**
    在后面的别的代码中，每次读取文件时，hover 高亮都被设置为绿色了
    context->HighlightStyle(Prs3d_TypeOfHighlight_LocalSelected)->
    SetColor({1, 1, 0,Quantity_TOC_RGB});*/
    connect(_mainwind->get_OCCView(),SIGNAL(selectionChanged()),
            this,SLOT(ReciveSelectChanged()));
}
DebugInfoDialog::~DebugInfoDialog(){delete ui;}
void DebugInfoDialog::ReciveSelectChanged()
{
    clear_all_label();
    auto context = _mainwind->get_OCCView()->getContext();
    context->InitSelected();
    while(context->MoreSelected()){
        Handle(AIS_InteractiveObject) aio = context->SelectedInteractive();
        if(aio->Type() == AIS_KindOfInteractive_Shape &&
                aio->IsKind((STANDARD_TYPE(AIS_Shape)))){
            Handle(AIS_Shape) ais = Handle(AIS_Shape)::DownCast(aio);
            auto it = record_map.find(ais);
            if(it !=record_map.end()) // 从记录表中查找
            {
                auto shape = ais->Shape();
                for(TopExp_Explorer edgeEx(shape, TopAbs_EDGE); edgeEx.More(); edgeEx.Next()){
                    TopoDS_Edge edge = TopoDS::Edge(edgeEx.Current());
                    Standard_Real first = 0;
                    Standard_Real last = 0;
                    Handle(Geom_Curve) curve = BRep_Tool::Curve(edge,first,last);
                    gp_Pnt p0 = curve->Value(first);
                    gp_Pnt pMid = curve->Value((first+last)/2.);
                    gp_Pnt p1 = curve->Value(last);
                    Point startPnt(p0.X(),p0.Y(),p0.Z()),
                            endPnt(p1.X(),p1.Y(),p1.Z());
                    bool iscw = true;
                    Label label;
                    label = get_new_label();
                    label->SetPosition(pMid);
                    label->SetText(toocc(QString("%1").arg(it->second.index)));
                    label->SetColor({255/255., 0/255., 0/255., Quantity_TOC_RGB});
                    update_label(label);
                    if(curve->IsKind(STANDARD_TYPE(Geom_Line))){
                        label = get_new_label();
                        label->SetPosition(pMid);
                        label->SetText(toocc(QString("    :%1").arg(ptoa(p0))));
                        update_label(label);
                    }
                    else if(curve->IsKind(STANDARD_TYPE(Geom_Circle))){
                        auto circle = Handle(Geom_Circle)::DownCast(curve);
                        gp_Pnt centerPnt = circle->Circ().Location();
                        label = get_new_label();
                        label->SetPosition(pMid);
                        label->SetText(toocc(QString("    :%1").arg(ptoa(circle->Circ().Location()))));
                        update_label(label);
                    }
                }
            }
        }
        context->NextSelected();
    }
    _mainwind->get_OCCView()->update();
}
void DebugInfoDialog::clear_all_label()
{
    for(size_t i = 0; i<use_label_count; ++i){
        _mainwind->get_OCCView()->getContext()->Erase(all_label[i], Standard_False);
    }
    use_label_count = 0;
    _mainwind->get_OCCView()->update();
}
DebugInfoDialog::Label DebugInfoDialog::get_new_label()
{
    if(all_label.size() <= use_label_count){
        Handle(AIS_TextLabel) label = new AIS_TextLabel();
        all_label.push_back(label);
    }
    auto label = all_label[use_label_count];
    init_label(label);
    use_label_count++;
    return label;
}
void DebugInfoDialog::update_label(DebugInfoDialog::Label &l)
{
    auto context = _mainwind->get_OCCView()->getContext();
    l->Redisplay();
    context->Display(l, Standard_False);
    context->SetZLayer(l, Graphic3d_ZLayerId_Topmost);
}