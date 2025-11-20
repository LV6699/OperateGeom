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
#include"../CutterLocation/DebugTool/WidgetTool.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    TopoDS_Shape PathMode(std::string path);

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
extern MainWindow* _mainwind;
extern TopoDS_Shape _gShape;


#endif // MAINWINDOW_H

