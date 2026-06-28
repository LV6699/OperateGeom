#include "ModelFileCut.h"
#include"../../AlgorithmDeve/AutomaticCheck/AutoCheckPath.h"
#include"../../AlgorithmDeve/CheckToolPath/AreaClearCheck/ToolPathCheck.h"
#include"../ProgramCall/CreatePathCall.h"
#include"../DisplayShape/DisplayToolPath.h"
#include"../DisplayShape/DisplayModel.h"
#include"../BatchCutModel/BatchCutSolve.h"
#include "ui_ManipuWidget.h"
shared_ptr<DataEdit>cutmod_dat = make_shared<DataEdit>();
shared_ptr<CutParts>cutmod_getpro = make_shared<CutParts>();
shared_ptr<ExicuteProgram>cutmod_pro = make_shared<ExicuteProgram>();
shared_ptr<ElemCalculate>cutmod_num = make_shared<ElemCalculate>();
shared_ptr<DisplayModel>cutmod_viemod = make_shared<DisplayModel>();
shared_ptr<CutModelSolve>cutmod_cutsol = make_shared<CutModelSolve>();
shared_ptr<ViewCurObject>cutmod_vieobj = make_shared<ViewCurObject>();
shared_ptr<CurrentIndexType>cutmod_idtyp = make_shared<CurrentIndexType>();
shared_ptr<AcquireEleLoop>cutmod_eleInlo = make_shared<AcquireEleLoop>();
shared_ptr<BatchCutSolve>cutmod_batsol = make_shared<BatchCutSolve>();
shared_ptr<BatchCutModel>cutmod_batcut = make_shared<BatchCutModel>();
bool ModelFileCut::CheckCutModel(ManipuWidget*pointer)
{
   /* if(m_curViewType != PartModel || m_vModelFile.empty() ||
            (pointer->ui->cheBoxIsBatchCut->isChecked() &&
             access(m_vModelFile[m_curFileIdx].m_filePath.
                    toStdString().c_str(),F_OK) != 0)){
        QString dlgTitle="警告！";
        QString strInfo="导入文件为空,请先导入零件模型！";
        QMessageBox::warning(pointer, dlgTitle, strInfo);
        return false;
    }*/
    return true;
}
void ModelFileCut::CutSingleModelToContoure(ManipuWidget*pointer,
                                            vector<vector<BndIslPair>>&modCutData)
{
    if(!CheckCutModel(pointer)){
        return;
    }
    modCutData.clear();
    int layNum = pointer->ui->inputLayerTotal->value();
    double xExp = pointer->ui->linEdiBlankXExp->text().toDouble();
    double yExp = pointer->ui->linEdiBlankYExp->text().toDouble();
    string saveDir = "./ContourData",hasCutModDir = "./hasCutModelDir";
    string modPath = m_vModelFile[m_curFileIdx].m_filePath.toStdString(),
            modName = m_vModelFile[m_curFileIdx].m_partName.toStdString();
    CutModelInfo modelInf(modPath,modName,saveDir,layNum,xExp,yExp);
    bool isCutSuccess = false;
    cutmod_getpro->CutSingleModelFile(modelInf,isCutSuccess,modCutData);
    if(isCutSuccess){
        ///system(("mv " + modPath + " " + hasCutModDir).c_str());
    }
    cutmod_vieobj->DisplayCurrentContoure(modCutData);
}
void ModelFileCut::CutBatchModelToContoure(ManipuWidget*pointer)
{
    if(!CheckCutModel(pointer)){
        return;
    }
    double xExp = pointer->ui->linEdiBlankXExp->text().toDouble();
    double yExp = pointer->ui->linEdiBlankYExp->text().toDouble();
    string saveDir = "./batchContour",cutFailure = "./cutFailure",
            hasCutModel = "./hasCutModel";
    ///saveDir = pointer->ui->linEdiSaveSelPath->text().toStdString();
    cutmod_batsol->CreateSpecifyDirectory(saveDir,false);
    cutmod_batsol->CreateSpecifyDirectory(hasCutModel,false);
    cutmod_batsol->CreateSpecifyDirectory(cutFailure,false);
    vector<CutModelInfo>vModelFile;
    for (int f = 0; f < m_vModelFile.size(); ++f) {
        int layNum = 6;
        CutModelInfo modelInf(m_vModelFile[f].m_filePath.toStdString(),
                              m_vModelFile[f].m_partName.toStdString(),
                              saveDir,layNum,xExp,yExp);
        vModelFile.push_back(modelInf);
    }
    int successNum = 0,failureNum = 0;
    vector<vector<BndIslPair>>vAreLayLoop;
    vector<string>vCutFailure;
    for (int m = 0; m < vModelFile.size(); ++m) {
        vAreLayLoop.clear();
        bool isCutSuccess = false;
        cutmod_getpro->CutSingleModelFile(vModelFile[m],isCutSuccess,
                                          vAreLayLoop);
        if(!isCutSuccess || vAreLayLoop.empty()){
            failureNum ++;
            vCutFailure.push_back(vModelFile[m].m_modelName);
            system(("mv " + vModelFile[m].m_modelPath + " " +
                    cutFailure).c_str());
            continue;
        }
        bool isSuccess = false;
        vector<BndIslPair>vBndIslLoop;
        cutmod_batcut->GetSpecifyBndIslLoop(vAreLayLoop,vBndIslLoop);
        cutmod_cutsol->SaveBatchCutContour(saveDir,vModelFile[m].m_modelName,
                                           vBndIslLoop,isSuccess);
        if(!isSuccess){
            failureNum ++;
            continue;
        }
        /*system(("mv " + vModelFile[m].m_modelPath + " " +
                hasCutModel).c_str());*/
        successNum ++;
        std::cout <<"执行文件总数:"<<vModelFile.size()<<",当前执行第"<< m + 1
            <<",剩余:"<<vModelFile.size() - m - 1 << std::endl;
    }
    QString partNum = QString::number(m_vModelFile.size());
    QString strSucNum = QString::number(successNum);
    QString strFaiNum = QString::number(failureNum);
    QString labStr = "零件剖分已完成,总数:" + partNum +
            "成功:" + strSucNum + ",失败:" + strFaiNum;
    bool isWarning = false;
    cutmod_idtyp->SetCurrentUiStatusInfo(pointer,labStr,isWarning);
    std::cout<<labStr.toStdString()<<std::endl;
    std::cout<<"失败零件:\n";
    for (int i = 0; i < vCutFailure.size(); ++i) {
        std::cout<<vCutFailure[i]<<std::endl;
    }
    std::cout<<"";
}
void ModelFileCut::InitialUiSelBndIslLoop(ManipuWidget*pointer,
                                          const DefElem &selEle)
{
    m_isCurSelFind = false;
    QString labStr = "当前状态无法找到所选择的轮廓!";
    bool isBatchCut = pointer->ui->cheBoxIsBatchCut->isChecked();
    if(!CheckCutModel(pointer) || m_curModCutData.empty()
            || isBatchCut){
        cutmod_idtyp->SetCurrentUiStatusInfo(pointer,labStr,true);
        return;
    }
    bool isSuccess = false;
    BndIslPair temPair;
    m_curSelPairLoop = temPair;
    GetUiSelectBndIslLoop(pointer,m_curModCutData,selEle,
                          isSuccess,m_curSelPairLoop);
    m_curSelPairLoop.m_partName = m_vModelFile[m_curFileIdx].
            m_partName.toStdString();
    m_isCurSelFind = isSuccess;
    labStr = "成功找到所选择的轮廓!";
    if(!isSuccess){
        labStr = "选择的轮廓无法找到!";
    }
    cutmod_idtyp->SetCurrentUiStatusInfo(pointer,labStr,!isSuccess);
}
void ModelFileCut::GetUiSelectBndIslLoop(ManipuWidget*pointer,
                                         const vector<vector<BndIslPair> > &modCutData,
                                         const DefElem &selEle,bool& isFind,
                                         BndIslPair &bndIslLoop)
{
    m_isCurSelFind = false;
    if(modCutData.empty()){
        return;
    }
    cutmod_eleInlo->GetLoopFromePartCutData(modCutData,selEle,
                                            isFind,bndIslLoop);
    QString labStr;
    if(!isFind){
        labStr = "没有找到鼠标选择的轮廓数据,请选择其他容易判别的轮廓图元!";
        cutmod_idtyp->SetCurrentUiStatusInfo(pointer,labStr,true);
        return;
    }
    double zVal;
    zVal = ElemCalculate().LoopZCoordinate(bndIslLoop.m_bndLoop);/**
    cutmod_elepre->InputPretreatment(bndIslLoop.m_bndLoop,
                                     bndIslLoop.m_vIslLoop,
                                     true,true,true,true,true);*/
    cutmod_num->SetLoopElementZValue(zVal,bndIslLoop.m_bndLoop);
    cutmod_num->SetArrayLoopElementZValue(zVal,bndIslLoop.m_vIslLoop);
    int areIdx = bndIslLoop.m_areaIdx;
    int layIdx = bndIslLoop.m_layerIdx;
    pointer->ui->spiBoxAreaIdx->setValue(areIdx);
    pointer->ui->spiBoxLayIdx->setValue(layIdx);
    labStr = "成功找到鼠标选择的轮廓数据!";
    cutmod_idtyp->SetCurrentUiStatusInfo(pointer,labStr,false);
}
void ModelFileCut::SaveSelectBndIslLoop(ManipuWidget*pointer)
{
    if(pointer->ui->cheBoxIsBatchCut->isChecked() ||
            !CheckCutModel(pointer) || !m_isCurSelFind){
        cutmod_idtyp->SetCurrentUiStatusInfo(pointer,"当前状态存储轮廓数据发生错误!",
                                             true);
        return;
    }
    string saveDir = "./ContourData";
    if(!pointer->ui->linEdiSaveSelPath->text().isEmpty()){
        saveDir = pointer->ui->linEdiSaveSelPath->text().toStdString();
    }
    bool isSuccess = false;
    ///BndIslPair bndIslLoop;
    vector<double>vOffsetVal;
    vOffsetVal.push_back(pointer->ui->linEdtOffsetValue_1->text().toDouble());
    vOffsetVal.push_back(pointer->ui->linEdtOffsetValue_2->text().toDouble());
    vOffsetVal.push_back(pointer->ui->linEdtOffsetValue_3->text().toDouble());
    m_curSelPairLoop.m_vOffsetVal = vOffsetVal;
    m_curSelPairLoop._toolRadius =
            pointer->ui->linEdtToolRadius->text().toDouble();
    cutmod_dat->SaveCutPairLoopData(saveDir,m_curSelPairLoop,isSuccess);
    QString labStr = "选择的轮廓数据存储成功!";
    if(!isSuccess){
        labStr = "选择的轮廓数存储存失败!";
    }
    cutmod_idtyp->SetCurrentUiStatusInfo(pointer,labStr,!isSuccess);
}
void ModelFileCut::RotateCurrentModel(ManipuWidget*pointer)
{
    if(m_curViewType != PartModel || m_vModelFile.empty()){
        return;
    }
    string filePath = m_vModelFile[m_curFileIdx].m_filePath.toStdString();
   /* if (access(filePath.c_str(), F_OK) != 0){
        return;
    }*////cutmod_dat->SlipeQFile(it,'/',vFile);
    vector<QString>vFile;
    cutmod_dat->SlipeQFile(m_vModelFile[m_curFileIdx].m_filePath,'/',vFile);
    if(vFile.size() < 2){
        std::cout<<"旋转模型失败!\n";
        return;
    }
    vFile.pop_back();
    string backPath;
    for(auto& it : vFile){
        if(it == ""){continue;}
        backPath = backPath + "/" + it.toStdString();
    }
    string asSaveFile = "transCoord_" + m_vModelFile[m_curFileIdx].
            m_partName.toStdString();
    backPath = backPath + "/" + asSaveFile;
    cutmod_viemod->RotatePartModelAngle(filePath,backPath);
}
void ModelFileCut::CalculateModelBlank(ManipuWidget*pointer)
{
    double xExp = pointer->ui->linEdiBlankXExp->text().toDouble();
    double yExp = pointer->ui->linEdiBlankYExp->text().toDouble();
    QString labStr;
    if(m_vModelFile.empty() || m_curFileIdx >
            m_vModelFile.size() - 1){
        labStr = "创建毛坯前需要先选择导入的零件！";
        cutmod_idtyp->SetCurrentUiStatusInfo(pointer,labStr,false);
        return;
    }
    else{
        labStr = "毛坯创建成功！";
        cutmod_idtyp->SetCurrentUiStatusInfo(pointer,labStr,true);
    }
    string path = m_vModelFile[m_curFileIdx].m_filePath.toStdString();
}
void ModelFileCut::MoveCurrentModelFile(ManipuWidget*pointer)
{
    if(m_curViewType != PartModel){
        return;
    }
    string filePath = m_vModelFile[m_curFileIdx].
            m_filePath.toStdString();
    /*if (access(filePath.c_str(), F_OK) != 0){
        cutmod_idtyp->SetCurrentUiStatusInfo(pointer,"当前文件不存在，无法删除!",true);
        return;
    }*/
    string removeFileDir = "removeFileDir";
    /*if (access(removeFileDir.c_str(), F_OK) != 0){
        system(("mkdir " + removeFileDir).c_str());
    }*/
    system(("mv " + filePath + " " + removeFileDir).c_str());
    string labStr = filePath + "已删除!";
    cutmod_idtyp->SetCurrentUiStatusInfo(pointer,QString::fromStdString(labStr),false);
}