#ifndef CREATTEOOLPATH_H
#define CREATTEOOLPATH_H
#include"../HearderFile.h"
class ManipuWidget;
namespace Ui {
class ManipuWidget;
}
class CreateToolPath
{
public:
    CreateToolPath(){}
    void GetCurrentStrategy(int index, MC_Strategy &mcStrategy,
                            bool &isFixOftNum,
                            int &offsetNum, bool &isSmooth);
    void CalculateToolPath(ManipuWidget*pointer);
    void CalculateImportFileToolPath(ManipuWidget*pointer,
                                     bool isDiscret, double discPre,
                                     OffsetParam& offsetPar);
    void BatchCalculateToolPath(ManipuWidget*pointer,
                                bool isDiscret,double discPre,
                                OffsetParam offsetPar);
    void GetAreaClearToolPath(QString partName,
                              OffsetParam offsetPar,
                              BndIslPair pairLoop,
                              shared_ptr<OffsetNode>&rootNode,
                              string areaClearDir,
                              string areaClearRepNam,
                              vector<double>vOffsetVal,
                              int& areCleFaiNum,
                              vector<string> &vOffsetValErr,
                              vector<string>&vAreCleFaiFil);
    void GetModelContourToolPath(QString partName, OffsetParam offsetPar,
                                 BndIslPair pairLoop,
                                 vector<double>vOffsetVal,
                                 shared_ptr<OffsetNode>&rootNode,
                                 string modelCtDir,
                                 string modelCtRepNam,
                                 int& modCtFaiNum,
                                 vector<string>& vOffsetValErr,
                                 vector<string>&vModCtFaiFil);
    void GetModelProfFiniToolPath(QString partName, OffsetParam offsetPar,
                                 BndIslPair pairLoop,
                                  vector<double>vOffsetVal,
                                 shared_ptr<OffsetNode>&rootNode,
                                 string ctFinishingDir,
                                 string ctFinishingRepNam,
                                 int& ctFinFaiNum,
                                 vector<string>& vOffsetValErr,
                                 vector<string>&vCtFinFaiFil);
    void SetOffsetVal(MC_Strategy msStratagy, double offsetVal,
                      OffsetParam& offsetPar);
};
#endif // CREATETOOLPATH_H