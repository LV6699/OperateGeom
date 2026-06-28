#include <iostream>
#include <QApplication>
#include"./ViewGeom/DisplayGeom.h"
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMaximumSize(1100,950);
    w.setMinimumSize(1100,950);
    w.show();
    return a.exec();
}
