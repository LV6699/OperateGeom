#ifndef DISPLAYMODEL_H
#define DISPLAYMODEL_H
#include"../MainWindow/mainwindow.h"
class DisplayModel
{
public:
    DisplayModel(){}
    void initialViewShape(STEPControl_Reader& reader);
    void RotatePartModelAngle(string path,
                              string asSavePath);
    void DisplayReadeModelFile(QString fileName);
    void ReDisplayReadeModelFile(string path);
private:
    string m_asSavePath = "";
};
#endif // DISPLAYMODEL_H