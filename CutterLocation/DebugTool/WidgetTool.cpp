#include "WidgetTool.h"
#include "ui_WidgetTool.h"
#include "../../OperateView/DisplayGeom.h"
#include"../../ViewWindow/MainWindow.h"
#include "OperateObject.h"
#include<iostream>

using namespace std;

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
    OperateObject().FindIntBasePoint(oft::Point(x,y));
}

void WidgetTool::on_btLineInt_clicked()
{

}
void WidgetTool::DisplayOperItem(ViewObj::ViewItem& item)
{
    if(!item._hasInitial){return;}
    if(!item._hasDisplay){
        _mainwind->myOccView->getContext()->Display(item._ashape,true);
    }else{
        _mainwind->myOccView->getContext()->Erase(item._ashape,true);
    }
    item.SetHasDisplay();
    _mainwind->myOccView->getContext()->Activate(
                AIS_Shape::SelectionMode(TopAbs_EDGE));
}
void WidgetTool::on_cheOriModel_clicked()
{
    DisplayOperItem(_opeItem._model);
}
void WidgetTool::on_cheDiscTria_clicked()
{
    DisplayOperItem(_opeItem._modTris);
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

























