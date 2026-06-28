#ifndef CURRENTINDEXTYPE_H
#define CURRENTINDEXTYPE_H
#include"../HearderFile.h"
class ManipuWidget;
namespace Ui {
class ManipuWidget;
}
class CurrentIndexType
{
public:
    CurrentIndexType(){}
    void SetCurrentUiStatusInfo(ManipuWidget *pointer,
                                QString str,bool isWarning);
    void GetCurrentIndexFile(ManipuWidget *pointer,
                                    CurViewType viewType);
    void LoadeViewFileList(ManipuWidget*pointer,
                           CurViewType viewType);
    void GetPreNexFileIndex(ManipuWidget *pointer, bool isNex);
    void ViewCurrentIndexFileObject(ManipuWidget*pointer);
    void ViewCurrentIndexModelFile(ManipuWidget*pointer);
    void ViewCurrentIndexContour(ManipuWidget*pointer);
    void ViewCurrentIndexToolPath(ManipuWidget*pointer);
    void ViewCurrentIndexToolPathPicture(ManipuWidget*pointer);
    void GetSpecifyLoopLimitCoorStr(const DefLoop &biLoop, QString &str);
    void ViewSelectContourScale(ManipuWidget*pointer);
};
#endif // CURRENTINDEXTYPE_H