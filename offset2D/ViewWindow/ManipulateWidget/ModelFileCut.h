#ifndef ModelFileCut_H
#define ModelFileCut_H
#include"../HearderFile.h"
class ManipuWidget;
namespace Ui {
class ManipuWidget;
}
class ModelFileCut
{
public:
    ModelFileCut(){}
    bool CheckCutModel(ManipuWidget*pointer);
    void CutSingleModelToContoure(ManipuWidget*pointer,
                                  vector<vector<BndIslPair>>&modCutData);
    void CutBatchModelToContoure(ManipuWidget*pointer);
public slots:
    void GetUiSelectBndIslLoop(ManipuWidget*pointer,
                               const vector<vector<BndIslPair> > &modCutData,
                               const DefElem &selEle,bool& isFind,
                               BndIslPair &bndIslLoop);
    void InitialUiSelBndIslLoop(ManipuWidget*pointer,
                                const DefElem &selEle);
    void SaveSelectBndIslLoop(ManipuWidget*pointer);
    void RotateCurrentModel(ManipuWidget*pointer);
    void CalculateModelBlank(ManipuWidget*pointer);
    void MoveCurrentModelFile(ManipuWidget*pointer);
};
#endif // ModelFileCut_H