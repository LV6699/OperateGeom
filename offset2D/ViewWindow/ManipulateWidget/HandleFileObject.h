#ifndef HANDLEFILEOBJECT_H
#define HANDLEFILEOBJECT_H
#include"../HearderFile.h"
class ManipuWidget;
namespace Ui {
class ManipuWidget;
}
class HandleFileObject
{
public:
    HandleFileObject(){}
    void SaveSelectProfObject(ManipuWidget*pointer);
    void SetOffsetValFromUi(ManipuWidget*pointer);
    void MoveCannotCutModelFile(ManipuWidget*pointer);
    void MoveCurrentModelFile(ManipuWidget*pointer);
    void LoadToolPathSaveImg(ManipuWidget*pointer,bool isSuc,
                             string &imgSaveDir,
                             string &imgName);
    void SaveToolPathDataInfo(ManipuWidget *pointer,
                              bool isNex, bool isSuc);
};
#endif // HANDLEFILEOBJECT_H