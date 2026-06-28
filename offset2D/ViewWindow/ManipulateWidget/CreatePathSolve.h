#ifndef CREATEPATHSOLVE_H
#define CREATEPATHSOLVE_H
#include <QtCore/QObject>
#include"../HearderFile.h"
#include <QtGui/qkeysequence.h>
//#include <unistd.h>
#include <time.h>
class CreatePathSolve : public QObject
{
    Q_OBJECT
public:
    explicit CreatePathSolve(QObject *parent) : QObject(parent){}
    CreatePathSolve(){}
    void GetEleZValue(const DefElem& eleData,double& zValue);
    bool CheckOffsetVal(vector<double> vOffsetVal);
    void GetToolPathSavePath(string &saveDir,string partName,
                             double offsetVal,string &saveImgName,
                             string& savePath);
    void CreateToolPathSaveDir(bool isCalAllStraTp,
                               string areaClearDir,
                               string areaClearRepNam,
                               string modelCtDir,
                               string modelCtRepNam,
                               string ctFinishingDir,
                               string ctFinishingRepNam,
                               string headStr,
                               MC_Strategy mcStrategy);
    void WriteFailureFileNameReport(bool isCalAllStra,
                                    MC_Strategy mcStrategy,
                                    string areaClearRepNam,
                                    string modelCtRepNam,
                                    string ctFinishingRepNam,
                                    vector<string>vAreCleFaiFil,
                                    vector<string>vModCtFaiFil,
                                    vector<string>vCtFinFaiFil,
                                    int totalNum,
                                    int areCleFaiNum,
                                    int modCtFaiNum,
                                    int ctFinFaiNum);
    void CreateToolPathReport(QString partName,
                              OffsetParam offsetPar,
                              BndIslPair& pairLoop,
                              shared_ptr<OffsetNode>&rootNode,
                              int& failurNum,
                              vector<string> &vFailurFile,
                              string fileName);
    void CalculateStrategyToolPath(QString partName, OffsetParam offsetPar,
                                   BndIslPair pairLoop,
                                   shared_ptr<OffsetNode> &rootNode,
                                   string straSaveDir, string &modelName,
                                   int &failurNum, vector<string> &vFailurFile,
                                   string fileName);
    void GetToolPathDir(string tpPath, bool isSuc, string &mvDir,
                        string &imgSaveDir,
                        string &imgName,
                        bool &isGet);
};
#endif // CREATEPATHSOLVE_H