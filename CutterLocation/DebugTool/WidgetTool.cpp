#include "WidgetTool.h"
#include "ui_WidgetTool.h"
#include "../../OperateView/DisplayGeom.h"

#include<iostream>
using namespace std;

WidgetTool::WidgetTool(QWidget *parent) :
    QDialog(parent),
    sub_ui(new Ui::WidgetTool)
{

    //connect(sub_ui->douPtXCoord, SIGNAL(valueChanged(double)),
            //this, SLOT(ReDrawPosLine(double)));
    sub_ui->setupUi(this);
}

WidgetTool::~WidgetTool()
{
    delete sub_ui;
}
void WidgetTool::ConnectConnect()
{
    //connect(sub_ui->douPtXCoord, SIGNAL(valueChanged(double)),
            //this, SLOT(ReDrawPosLine(double)));
}
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
    if(!_posLine.IsNull()){_mainwind->myOccView->getContext()->Remove(_posLine,true);}
    TopoDS_Edge e = BRepBuilderAPI_MakeEdge(gp_Pnt(x,y,-100),gp_Pnt(x,y,100));
    _posLine = new AIS_Shape(e);
    _posLine->SetColor(Quantity_Color(1,0.3,0.3,Quantity_TOC_RGB));
    _mainwind->myOccView->getContext()->Display(_posLine,true);
    _mainwind->myOccView->fitAll();
}

void WidgetTool::on_btLineInt_clicked()
{

}

