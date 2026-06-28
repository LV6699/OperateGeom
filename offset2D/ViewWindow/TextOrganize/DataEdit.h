#ifndef DATAEDIT_H
#define DATAEDIT_H
#include<QtWidgets/QDialog>
#include<QtWidgets/QWidget>
#include<QtWidgets/QMessageBox>
#include"../HearderFile.h"
class DataEdit
{
public:
    DataEdit(){}
    void GetPartViewData(const CutPartResult& cutResult,
                         vector<DefLoop> &vLoopData);
    void GetSinglePartViewData(const PartCutData& cutResult,
                               vector<DefLoop>& vLoopData);
    bool IsSuccessPartFile(string path);
    void SlipeFile(string str,char sep,vector<string>&vStr);
    void SlipeQFile(QString qstr, char sep, vector<QString>&vQstr);
    void SaveCutPartResult(const CutPartPara& cutPar,
                           const CutPartResult& cutResult);
    void SaveCutPairLoopData(string path,
                             const BndIslPair& pairLoop, bool &isSucSave);
    void AnalyticCutPartResult(string path,CutPartResult &cutResult);
    void GetRootNodeLoopNum(const shared_ptr<OffsetNode>&rootNode,
                            int &num);
    void AnalyticlePairLoopData(string path,
                                bool& isAnaSuc,
                                BndIslPair &pairLoop);
    void SaveToolPathNode(string path, string partName,
                          OffsetParam &offsetPar,
                          const BndIslPair &oriPairLoop,
                          const shared_ptr<OffsetNode>&rootNode,
                          bool& isSucSave);
    void AnalyticleToolPathJsonNode(string path, string& partName,
                                    double& firOffsetVal, double& offsetVal,
                                    bool& isAnaSave, BndIslPair &pairLoop,
                                    shared_ptr<OffsetNode>&rootNode);
};
#endif // DATAEDIT_H