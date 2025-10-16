#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<vector>
#include<QtWidgets/QMainWindow>
#include<QtCore/QSettings>
#include<QtWidgets/QLabel>
#include<QtGui/QScreen>
#include<string>
#include<cstring>
#include"OccView.h"
#include "../OperateView/DisplayGeom.h"
#include"../CutterLocation/DebugTool/WidgetTool.h"
#include"../CutterLocation/Common/ClStruct.h"
///#include"../CutterLocation/DebugTool/OperateItem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    TopoDS_Shape PathMode(string path);

protected:
    //! create all the actions.
    void createActions(void);
    void createToolBars(void);

private slots:
    void on_actImportModel_triggered();
    void on_actSteepFlat_triggered();

public:
    Ui::MainWindow *ui;
    OccView* myOccView;
    WidgetTool *_subWind;

public slots:

};
//extern ViewObj::OpeViewItem _vieItem;
extern grm::MeshMap _meshMap;
extern MainWindow* _mainwind;
extern TopoDS_Shape _gShape;
extern std::vector<Quantity_Color>_colors;

#endif // MAINWINDOW_H

