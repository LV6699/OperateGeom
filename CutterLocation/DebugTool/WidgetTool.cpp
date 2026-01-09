#include "WidgetTool.h"
#include "ui_WidgetTool.h"
#include<iostream>

#include "OperateObject.h"
#include"../../ViewWindow/MainWindow.h"
#include"../Common/ViewTool.h"
#include "../Solution/CutLocateZ.h"


using std::vector;
using std::string;

#pragma optimize("", off)
#pragma GCC optimize ("o0")

WidgetTool::WidgetTool(QWidget *parent) :
    QDialog(parent),
    sub_ui(new Ui::WidgetTool)
{
    sub_ui->setupUi(this);
}
WidgetTool::~WidgetTool(){delete sub_ui;}

void WidgetTool::on_douPtXCoord_valueChanged()
{
    ReDrawPosLine();
}
void WidgetTool::on_douPtYCoord_valueChanged()
{
    ReDrawPosLine();
}
void WidgetTool::ReDrawPosLine()
{
    double x = sub_ui->douPtXCoord->text().toDouble();
    double y = sub_ui->douPtYCoord->text().toDouble();
    if(!_posLine.IsNull()){_mainwind->myOccView->getContext()->Erase(_posLine,true);}
    TopoDS_Edge e = BRepBuilderAPI_MakeEdge(gp_Pnt(x,y,-100),gp_Pnt(x,y,100));
    _posLine = new AIS_Shape(e);
    _posLine->SetColor(Quantity_Color(1,0.3,0.3,Quantity_TOC_RGB));
    _posLine->SetWidth(1.5);
    _mainwind->myOccView->getContext()->Display(_posLine,true);
    ///_mainwind->myOccView->fitAll();
    OperateObject().FindIntBasePoint(ofts::Point(x,y));
}

void WidgetTool::on_btLineInt_clicked()
{
}
void WidgetTool::DisplayOperItem(ViewObj::ViewItem& item)
{
    if(!item._hasInitial){return;}
    if(!item._hasDisplay){
        if(!item._texAspe.IsNull()){
            _mainwind->myOccView->getContext()->Display(item._texAspe,true);
        }
        _mainwind->myOccView->getContext()->Display(item._ashape,true);
    }else{
        _mainwind->myOccView->getContext()->Erase(item._ashape,true);
        if(!item._texAspe.IsNull()){
            _mainwind->myOccView->getContext()->Erase(item._texAspe,true);
        }
    }
    item.SetHasDisplay();
    _mainwind->myOccView->getContext()->Activate(
        AIS_Shape::SelectionMode(TopAbs_EDGE));
    _mainwind->myOccView->getContext()->Activate(
        AIS_Shape::SelectionMode(TopAbs_VERTEX));
    // 确保启用了动态高亮
    _mainwind->myOccView->getContext()->SetAutoActivateSelection(Standard_True);
    _mainwind->myOccView->CreateViewDir();
    


}
void WidgetTool::on_cheOriModel_clicked()
{
    DisplayOperItem(_opeItem._model);
}
void WidgetTool::on_cheDiscTria_clicked()
{
    //DisplayOperItem(_opeItem._modTris);
    DisplayOperItem(_opeItem._oriEdge);
}
void WidgetTool::on_chePtProtect_clicked()
{
    DisplayOperItem(_opeItem._ptProt);
}
void WidgetTool::on_cheEdgProtect_clicked()
{
    DisplayOperItem(_opeItem._edgeProt);
}
void WidgetTool::on_cheTrisCl_clicked()
{
    DisplayOperItem(_opeItem._trisCl);
}
void WidgetTool::on_che_xEdge_clicked()
{
    DisplayOperItem(_opeItem._xEdges);
}
void WidgetTool::on_che_yEdge_clicked()
{
    DisplayOperItem(_opeItem._yEdges);
}
void WidgetTool::on_cheClPt_clicked()
{
    DisplayOperItem(_opeItem._clPts);
}
void WidgetTool::FindSelItem()
{
    OperateObject().FindSelItem();
}
void WidgetTool::on_btSelPtAllZ_clicked()
{
    for(auto& d : _clRelItems){
        DisplayOperItem(d._vieItem);
    }
    OperateObject opo;
    opo.FindSelectObject();
    if(!opo._hasUiFind || !opo._isPoint){return;}
    const auto& p = opo._p;
    grm::GetPtAllLocation(_meshMap,p,_clRelItems);
    for(auto& d : _clRelItems){
        grm::ClRelItemToShape(d);
    }
    for(auto& d : _clRelItems){
        DisplayOperItem(d._vieItem);
    }

}
void WidgetTool::on_btVieIdTria_clicked()
{
    DisplayOperItem(_idTria);
    int id = sub_ui->spinTriaId->text().toInt();
    if(id > _meshMap._trisCl.size() - 1){
        std::cout<<"无效三角形索引"<<std::endl;return;
    }
    const auto& t = _meshMap._trisCl[id];
    auto shape = ViewTool::TriangleToShape(t);
    _idTria = ViewObj::ViewItem(shape,_colors[5],2);
    _idTria._hasInitial = true;
    DisplayOperItem(_idTria);
    std::cout<<"已显示索引三角形"<<std::endl;
}

