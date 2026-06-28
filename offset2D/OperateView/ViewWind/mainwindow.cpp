#include "mainwindow.h"
#include "ui_mainwindow.h"
#pragma optimize("", off)
#include <iostream>
#include <fstream>
#include <vector>
#include <QToolBar>
#include <QTreeView>
#include <QMessageBox>
#include <QDockWidget>
using namespace std;
MainWindow* _mainwind = nullptr;
vector<Quantity_Color>___colors;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myOccView = new OccView(this);
    _mainwind = this;
    myOccView->myView->SetProj(V3d_Zpos);
    //myOccView->myView->SetProj(V3d_XposYnegZpos);
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
    __colors = colors;
    for(auto& d : __colors){__colors.emplace_back(d);}
    for(auto& d : __colors){__colors.emplace_back(d);}
    for(auto& d : __colors){__colors.emplace_back(d);}
    for(auto& d : __colors){__colors.emplace_back(d);}
    for(auto& d : __colors){__colors.emplace_back(d);}
}
MainWindow::~MainWindow(){delete ui;}
void MainWindow::createActions( void )
{
    connect(ui->actionPan, SIGNAL(triggered()), myOccView, SLOT(pan()));
    connect(ui->actionRotate, SIGNAL(triggered()), myOccView, SLOT(rotate()));
    connect(ui->actionReset, SIGNAL(triggered()), myOccView, SLOT(reset()));
    connect(ui->actionFitAll, SIGNAL(triggered()), myOccView, SLOT(fitAll()));
    connect(ui->actImportModel, SIGNAL(triggered()), myOccView, SLOT(fitAll()));
}
void MainWindow::createToolBars( void )
{
    QToolBar* aToolBar = addToolBar(tr("&Navigate"));
    aToolBar->addAction(ui->actionPan);
    aToolBar->addAction(ui->actionRotate);
    aToolBar = addToolBar(tr("&View"));
    aToolBar->addAction(ui->actionReset);
    aToolBar->addAction(ui->actionFitAll);
    aToolBar->addAction(ui->actImportModel);
    aToolBar->addAction(ui->actSteepFlat);
}
