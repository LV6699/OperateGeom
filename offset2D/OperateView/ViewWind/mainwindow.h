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
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    //! create all the actions.
    void createActions(void);
    void createToolBars(void);
private slots:
public:
    Ui::MainWindow *ui;
    OccView* myOccView;
public slots:
};
extern MainWindow* _mainwind;
extern std::vector<Quantity_Color>_colors;
#endif // MAINWINDOW_H
