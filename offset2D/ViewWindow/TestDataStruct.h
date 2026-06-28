#ifndef TESTDATASTRUCT_H
#define TESTDATASTRUCT_H
#pragma once
#include<QtCore/QString>
#include<string>
#include<vector>
#include"HearderFile.h"
enum AlgorithmRunStatus{
    OSTRUNING,
    OSTRUNINGFINISHED
};
enum IsOpenNewFile{
    OPENNEWFILE,
    NOTOPENNEWFILE
};
enum IsExistedModelFile{
    EXIST_MODELFILR,
    NOTEXIST_MODELFILR
};
enum IsRemoveAllModel{
    REMOVEALLMODEL,
    NOTREMOVEALLMODEL
};
enum FileIdxChaMode{
    PreviousePartFile = 0,
    NextPartFile,
    DoubleClickPartFile
};
enum CurViewType{
    PartModel,
    CutContourData,
    ToolPathData,
    ToolPathImage
};
struct FileInfo
{
    FileInfo() {}
    FileInfo(QString path,QString name)
    {
        m_filePath = path;
        m_partName = name;
    }
    QString m_filePath;
    QString m_partName;
};
struct FileModel
{
    FileModel() {}
    FileModel(string path,string name)
    {
        m_filePath = path;
        m_fileName = name;
    }
    string m_filePath;
    string m_fileName;
};
struct CutPartView
{
    CutPartView() {}
    CutPartView(QString file,CurViewType type){
        m_partFile =  file;
        m_curViewType = type;
    }
    QString m_partFile;
    CurViewType m_curViewType;
};
struct CutPartPara
{
    CutPartPara() {}
    CutPartPara(vector<QString>vFileList,
                int layTotal, bool isBatchCut,
                bool isAppend,bool isSave,
                string saveDir)
    {
        m_vPartFile = vFileList;
        m_layTotal = layTotal;
        m_isBatchCut = isBatchCut;
        m_isAppendCut = isAppend;
        m_isSaveCut = isSave;
        m_saveDirCut = saveDir;
    }
    CutPartPara(QString partFile,
                int layTotal, bool isBatchCut,
                bool isAppend,bool isSave,
                string saveDir)
    {
        m_partFile = partFile;
        m_layTotal = layTotal;
        m_isBatchCut = isBatchCut;
        m_isAppendCut = isAppend;
        m_isSaveCut = isSave;
        m_saveDirCut = saveDir;
    }
    vector<QString> m_vPartFile;
    QString m_partFile;
    int m_layTotal;
    bool m_isBatchCut;
    bool m_isAppendCut;
    bool m_isSaveCut;
    string m_saveDirCut;
};
class CutModelInfo{
public:
    CutModelInfo(){}
    CutModelInfo(string modelPath,string modelName,
                 string profSaveDir,int layNum,
                 double xExp,double yExp){
        m_modelPath = modelPath;
        m_modelName = modelName;
        m_profSaveDir = profSaveDir;
        m_layNum = layNum;
        m_xExp = xExp;
        m_yExp = yExp;
    }
    string m_modelPath;
    string m_modelName;
    string m_profSaveDir;
    int m_layNum;
    double m_xExp,m_yExp;
};
class CutModelPara{
public:
    CutModelPara(){}
    CutModelPara(vector<CutModelInfo> vInfo,bool isBatchCut)
    {
        m_vModeInfo = vInfo;
        m_isBatchCut = isBatchCut;
    }
    vector<CutModelInfo>m_vModeInfo;
    bool m_isBatchCut;
};
struct BndIslPair
{
    BndIslPair() {}
    BndIslPair(DefLoop bndLoop,
               vector<DefLoop>vIslLoop)
    {
        m_bndLoop = bndLoop;
        m_vIslLoop = vIslLoop;
    }
    DefLoop m_bndLoop;
    vector<DefLoop> m_vIslLoop;
    string m_partName;
    double _toolRadius;
    vector<double>m_vOffsetVal;
    int m_areaIdx = 0;
    int m_layerIdx = 0;
    int m_eleNum = 0;
    double m_lenth = 0;
    double m_weight = 0;
    bool m_isValScale = true;
};
struct PartLayCut
{
    PartLayCut() {}
    PartLayCut(vector<BndIslPair> vPartLayCut)
    {
        m_vPairLoop = vPartLayCut;
    }
    vector<BndIslPair> m_vPairLoop;
};
struct PartAreaCut
{
    PartAreaCut() {}
    PartAreaCut(vector<PartLayCut> vLayData)
    {
        m_vLayData = vLayData;
    }
    vector<PartLayCut> m_vLayData;
};
struct PartCut
{
    PartCut() {}
    PartCut(vector<PartAreaCut> vAreaData)
    {
        m_vAreaData = vAreaData;
    }
    vector<PartAreaCut> m_vAreaData;
};
struct PartCutData
{
    PartCutData() {}
    PartCutData(string cutPartName,
                PartCut cutPartLoop){
        m_CutPartName = cutPartName;
        m_CutPartData = cutPartLoop;
    }
    string m_CutPartName;
    PartCut m_CutPartData;
};
struct CutPartResult
{
    CutPartResult(){}
    CutPartResult(vector<PartCutData> vCutPartResult)
    {
        m_vCutPartResult = vCutPartResult;
    }
    CutPartResult(int cutSucNum,int cutFaiNum,
                  vector<PartCutData> vCutPartResult,
                  vector<string>vCutFailurePart)
    {
        m_cutSucNum = cutSucNum;
        m_cutFaiNum = cutFaiNum;
        m_vCutPartResult = vCutPartResult;
        m_vCutFailurePart = vCutFailurePart;
    }
    int m_cutSucNum;
    int m_cutFaiNum;
    vector<PartCutData> m_vCutPartResult;
    vector<string> m_vCutFailurePart;
};
#endif // TESTDATASTRUCT_H