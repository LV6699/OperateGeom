#include "ViewData.h"
#include "ui_ViewData.h"
#include"../MainWindow/mainwindow.h"
shared_ptr<GeomCalculate> veiGo = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate> vie_num = make_shared<ElemCalculate>();
class myfilter : public SelectMgr_Filter{
public:
    using SelectMgr_Filter::SelectMgr_Filter;
    ///过滤掉标签
    virtual Standard_Boolean IsOk(const opencascade::handle<SelectMgr_EntityOwner> &anobj)
    const override{
        return !anobj->Selectable()->IsKind(STANDARD_TYPE(AIS_TextLabel));
    }
};
void InitLabel(Handle(AIS_TextLabel)& label)
{
    label->SetColor({0, 0, 0,Quantity_TOC_RGB});
    label->SetZoomable(Standard_False);
    ///label->SetFontAspect(Font_FontAspect_Bold);
    label->SetHeight(12);
    label->SetFont("sans-serif");
}
TCollection_ExtendedString ToOccData(const QString& str){
    QByteArray bytes = str.toUtf8();
    TCollection_AsciiString astr(bytes.constData());
    return TCollection_ExtendedString(astr);
}
QString PntToString(const gp_Pnt& p){
    return QString("%1,%2").arg(p.X()).arg(p.Y());
}
ViewData::ViewData(QWidget *parent) :
    QWidget(parent),ui(new Ui::ViewData)
{
    ///ui->setupUi(this);
    auto context = _mainwind->get_OCCView()->getContext();
    context->AddFilter(new myfilter());
    context->SelectionStyle()->SetColor({0.2,0.1,0,Quantity_TOC_RGB});
    connect(_mainwind->get_OCCView(),SIGNAL(selectionChanged()),
            this,SLOT(SignalViewSelShapeInfo()));
    connect(_mainwind->get_OCCView(),SIGNAL(SignalPrintSelLoop(AIS_Shape)),
            this,SLOT(PrintSelectLoop(AIS_Shape)));
}
ViewData::~ViewData(){delete ui;}
void ViewData::SignalViewSelShapeInfo()
{
    ClearAllLabel();
    auto context = _mainwind->get_OCCView()->getContext();
    context->InitSelected();
    //context->ShiftSelect(false);
    while(context->MoreSelected()){
        Handle(AIS_InteractiveObject) aio = context->SelectedInteractive();
        if(aio->Type() == AIS_KindOfInteractive_Shape &&
                aio->IsKind((STANDARD_TYPE(AIS_Shape)))){
            Handle(AIS_Shape) ais = Handle(AIS_Shape)::DownCast(aio);
            auto shape = ais->Shape();
            for(TopExp_Explorer edgeEx(shape, TopAbs_EDGE);
                edgeEx.More(); edgeEx.Next()){
                TopoDS_Edge edge = TopoDS::Edge(edgeEx.Current());
                Standard_Real first = 0,last = 0;
                Handle(Geom_Curve) curve = BRep_Tool::Curve(edge,first,last);
                gp_Pnt p0 = curve->Value(first);
                gp_Pnt pMid = curve->Value((first+last)/2.);
                gp_Pnt p1 = curve->Value(last);
                Point startPnt(p0.X(),p0.Y(),p0.Z()),
                        endPnt(p1.X(),p1.Y(),p1.Z());
                bool iscw = true;
                Label label;
                label = GetNewLabel();
                label->SetPosition(pMid);
                int idx,nodIdx;
                bool isTp = false;
                FindSelInfo(ais,idx,nodIdx,isTp);
                label->SetText(TCollection_ExtendedString(std::to_string(idx).c_str()));
                label->SetColor({255/255., 0/255., 0/255., Quantity_TOC_RGB});
                UpdateLabel(label);
                if(curve->IsKind(STANDARD_TYPE(Geom_Line))){
                    label = GetNewLabel();
                    label->SetPosition(pMid);
                    label->SetText(ToOccData(QString("    :  %1").arg(PntToString(p0))));
                    UpdateLabel(label);
                }
                else if(curve->IsKind(STANDARD_TYPE(Geom_Circle))){
                    auto circle = Handle(Geom_Circle)::DownCast(curve);
                    gp_Pnt center = circle->Circ().Location();
                    Point centerPnt(center.X(),center.Y(),center.Z());
                    double radius;
                    veiGo->TwoPointDistance(startPnt,centerPnt,radius);
                    std::stringstream strem;
                    strem << std::setprecision(3) << radius;
                    string str0 = strem.str();
                    QString qstr = PntToString(circle->Circ().Location());
                    string str = qstr.toStdString();
                    str = "    " + str + " r: " + str0;
                    TCollection_ExtendedString aStr(str.c_str());
                    label = GetNewLabel();
                    label->SetPosition(pMid);
                    label->SetText(aStr);
                    //label->SetText(ToOccData(QString("    :%1").arg(PntToString(circle->Circ().Location()))));
                    UpdateLabel(label);
                }
                if(isTp){
                    SameNodeLoopIdx(nodIdx);
                }
            }
        }
        context->NextSelected();
    }
    _mainwind->get_OCCView()->update();
}
void ViewData::FindSelInfo(Handle(AIS_Shape)selShape,int&idx,
                           int& nodIdx,bool& isTp)
{
    for(int i = 0;i < glb_bndIslShape.m_bndShape.m_vShape.size();i++){
        EleShape curShape = glb_bndIslShape.m_bndShape.m_vShape[i];
        if(selShape != curShape.m_occShape){
            continue;
        }
        nodIdx = 0;
        idx = i;
        isTp = false;
        return;
    }
    for(int i = 0;i < glb_bndIslShape.m_vIslShape.size();i++){
        LoopShape loopShape = glb_bndIslShape.m_vIslShape[i];
        for(int j = 0;j < loopShape.m_vShape.size();j++){
            EleShape curShape = loopShape.m_vShape[j];
            if(selShape != curShape.m_occShape){
                continue;
            }
            nodIdx = 0;
            idx = curShape.m_idx;
            isTp = false;
            return;
        }
    }
    isTp = true;
    for(int n = 0;n < glb_vNodeOcc.size();n ++){
        NodeShape curNode = glb_vNodeOcc[n];
        for(int lo = 0 ;lo < curNode.m_vLoopShape.size();lo ++){
            LoopShape loopShape = curNode.m_vLoopShape[lo];
            for(int i = 0;i < loopShape.m_vShape.size();i ++){
                EleShape eleShape = loopShape.m_vShape[i];
                if(eleShape.m_occShape != selShape){
                    continue;
                }
                idx = eleShape.m_idx;
                nodIdx = n;
                return;
            }
        }
    }
}
void ViewData::ClearAllLabel()
{
    for(size_t i = 0; i<m_labelNum; ++i){
        _mainwind->get_OCCView()->getContext()->Erase(m_allLabel[i],
                                                        Standard_False);
    }
    for(int i = 0; i <m_vNodLabel.size(); ++i){
        _mainwind->get_OCCView()->getContext()->Erase(m_vNodLabel[i],
                                                        Standard_False);
    }
    m_labelNum = 0;
    _mainwind->get_OCCView()->update();
}
ViewData::Label ViewData::GetNewLabel()
{
    if(m_allLabel.size() <= m_labelNum){
        Handle(AIS_TextLabel) label = new AIS_TextLabel();
        m_allLabel.push_back(label);
    }
    auto label = m_allLabel[m_labelNum];
    InitLabel(label);
    m_labelNum ++;
    return label;
}
void ViewData::UpdateLabel(ViewData::Label&label)
{
    auto context = _mainwind->get_OCCView()->getContext();
    label->Redisplay();
    context->Display(label, Standard_False);
    context->SetZLayer(label, Graphic3d_ZLayerId_Topmost);
}
void ViewData::SameNodeLoopIdx(int nodIdx)
{
    double r = 0.0,g = 0.0,b = 0.8;
    Quantity_Color defColor(r, g, b, Quantity_TOC_RGB);
    if(glb_vNodeOcc.empty() || nodIdx > glb_vNodeOcc.size() - 1){
        return;
    }
    NodeShape nodeShape = glb_vNodeOcc[nodIdx];
    for(int lo = 0;lo < nodeShape.m_vLoopShape.size();lo++){
        LoopShape loShape = nodeShape.m_vLoopShape[lo];
        if(loShape.m_vShape.empty()){
            continue;
        }
        EleShape curEle = loShape.m_vShape[0];
        TopoDS_Shape shape = curEle.m_occShape->Shape();
        TopExp_Explorer edgeEx(shape, TopAbs_EDGE);
        TopoDS_Edge edge = TopoDS::Edge(edgeEx.Current());
        Standard_Real first = 0,last = 0;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge,first,last);
        gp_Pnt p0 = curve->Value(first);
        Handle(AIS_TextLabel) label = new AIS_TextLabel();
        TCollection_ExtendedString labelContent(std::to_string(nodIdx).c_str());
        label->SetText(labelContent);
        label->SetPosition(p0);
        Handle(Prs3d_TextAspect) textAspect = new Prs3d_TextAspect();
        label->SetColor({200/255, 0/255., 100/255., Quantity_TOC_RGB});
        //label->SetColor(defColor);
        textAspect->SetHeight(14);
        //label->Attributes()->SetTextAspect(textAspect);
        m_vNodLabel.push_back(label);
        UpdateLabel(label);
    }
}
void ViewData::PrintSelectLoop(AIS_Shape shape)
{
    auto topShape = shape.Shape();
    ElemType type;
    Point startPnt,endPnt;
    for(TopExp_Explorer edgeEx(topShape, TopAbs_EDGE);
        edgeEx.More(); edgeEx.Next()){
        TopoDS_Edge edge = TopoDS::Edge(edgeEx.Current());
        Standard_Real first = 0,last = 0;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge,first,last);
        gp_Pnt p0 = curve->Value(first);
        gp_Pnt p1 = curve->Value(last);
        startPnt._x = p0.X();
        startPnt._y = p0.Y();
        endPnt._x = p1.X();
        endPnt._y = p1.Y();
        if(curve->IsKind(STANDARD_TYPE(Geom_Line))){
            type = LINETYPE;
        }
        else if(curve->IsKind(STANDARD_TYPE(Geom_Circle))){
            type = ARCTYPE;
        }
    }
    for(int n = 0;n < glb_vNodeOcc.size();n ++){
        NodeShape curNode = glb_vNodeOcc[n];
        for(int lo = 0 ;lo < curNode.m_vLoopShape.size();lo ++){
            DefLoop loop = curNode.m_vLoopShape[lo].m_loop;
            for(int i = 0;i < loop.m_vElem.size();i ++){
                DefElem ele = loop.m_vElem[i];
                if(type != ele._type){
                    continue;
                }
                if(type == LINETYPE){
                    if(veiGo->IsIdenticalPoint2D(startPnt,ele._segment._segSp,PreErr5_2) &&
                            veiGo->IsIdenticalPoint2D(endPnt,ele._segment._segEp,PreErr5_2)){
                        OperateOftString().PrintLoopData(loop,true,lo);
                        return;
                    }
                }
                else if(type == ARCTYPE){
                    if(veiGo->IsIdenticalPoint2D(startPnt,ele._arc._arcSp,PreErr5_2) &&
                            veiGo->IsIdenticalPoint2D(endPnt,ele._arc._arcEp,PreErr5_2)){
                        OperateOftString().PrintLoopData(loop,true,lo);
                        return;
                    }
                }
            }
        }
    }
}