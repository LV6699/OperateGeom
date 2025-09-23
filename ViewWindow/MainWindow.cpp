#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <QString>
#include <QToolBar>
#include <QTreeView>
#include <QMessageBox>
#include <QDockWidget>
#include "../OperateView/GeomToShape.h"

///#pragma optimize("", off)

using namespace std;
MainWindow* _mainwind = nullptr;
vector<Quantity_Color>_colors;
TopoDS_Shape _gShape = TopoDS_Shape();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myOccView = new OccView(this);
    _mainwind = this;
    //myOccView->myView->SetProj(V3d_Zpos);
    myOccView->myView->SetProj(V3d_XposYnegZpos);
    ///myOccView->myView
    setCentralWidget(myOccView);
    createActions();
    createToolBars();
    Quantity_Color green(0,0.25,0,Quantity_TOC_RGB);///0
    Quantity_Color blue(0,0,0.7,Quantity_TOC_RGB);///1
    Quantity_Color red(0.7,0,0,Quantity_TOC_RGB);///2
    Quantity_Color purple(0.6,0,0.6,Quantity_TOC_RGB);///3
    Quantity_Color yellow(0.6,0.6,0,Quantity_TOC_RGB);///4
    Quantity_Color cyan(0,0.5,0.5,Quantity_TOC_RGB);///5
    Quantity_Color orange(0.9,0.1,0,Quantity_TOC_RGB);///6
    Quantity_Color brown(0.5,0.2,0.0,Quantity_TOC_RGB);///7
    Quantity_Color black(0,0,0,Quantity_TOC_RGB);///8
    vector<Quantity_Color>colors{green,blue,red,purple,
                yellow,orange,cyan,brown,black};
    _colors = colors;
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
}
MainWindow::~MainWindow(){delete ui;}
void MainWindow::createActions( void )
{
    connect(ui->actPan, SIGNAL(triggered()), myOccView, SLOT(pan()));
    connect(ui->actRotate, SIGNAL(triggered()), myOccView, SLOT(rotate()));
    connect(ui->actReset, SIGNAL(triggered()), myOccView, SLOT(reset()));
    connect(ui->actFitall, SIGNAL(triggered()), myOccView, SLOT(fitAll()));
    connect(ui->actImport, SIGNAL(triggered()), this, SLOT(on_actImportModel_triggered()));
}
void MainWindow::createToolBars( void )
{
    QToolBar* aToolBar = addToolBar(tr("&Navigate"));
    aToolBar->addAction(ui->actPan);
    aToolBar->addAction(ui->actRotate);
    aToolBar->addAction(ui->actReset);
    aToolBar->addAction(ui->actFitall);
    aToolBar->addSeparator();
    aToolBar->addAction(ui->actImport);
    //aToolBar->addAction(ui->actSteepFlat);
    //statusBar() ;
}

TopoDS_Shape MainWindow::PathMode(string path)
{
    STEPControl_Reader reader;
    IFSelect_ReturnStatus status = reader.ReadFile(path.c_str());
    if (status != IFSelect_RetDone) {
        return TopoDS_Shape();
    }
    reader.TransferRoots();
    TopoDS_Shape shape = reader.OneShape();
    return shape;
}


void MainWindow::on_actImportModel_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open STEP File"), "",
                                                    tr("STEP Files (*.stp *.step)"));
    if (fileName.isEmpty()) {
        return;
    }
    try {
        STEPControl_Reader reader;
        IFSelect_ReturnStatus status = reader.ReadFile(fileName.toUtf8().constData());
        if (status != IFSelect_RetDone) {
            QMessageBox::warning(this, "Error", "Failed to read STEP file");
            return;
        }
        // 转移所有根实体
        reader.TransferRoots();
        // 获取形状数量
        int nbShapes = reader.NbShapes();
        if (nbShapes > 0) {
            // 获取第一个形状
            TopoDS_Shape shape = reader.Shape(1);
            if (!shape.IsNull()) {
                myOccView->myRemoveall();
                Handle(AIS_Shape) aisShape = new AIS_Shape(shape);
                myOccView->myContext->Display(aisShape, false);
                statusBar()->showMessage("STEP file imported successfully: " + fileName);
            } else {
                QMessageBox::warning(this, "Error", "No valid shape found in STEP file");
            }
        } else {
            QMessageBox::warning(this, "Error", "No shapes found in STEP file");
        }

    } catch (...) {
        QMessageBox::critical(this, "Error", "An error occurred while importing STEP file");
    }

}
void MainWindow::on_actSteepFlat_triggered()
{

}










