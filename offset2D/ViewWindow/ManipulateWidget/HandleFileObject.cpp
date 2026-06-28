#include "HandleFileObject.h"
#include"../../AlgorithmDeve/AutomaticCheck/AutoCheckPath.h"
#include"../../AlgorithmDeve/CheckToolPath/AreaClearCheck/ToolPathCheck.h"
#include"../DisplayShape/DisplayToolPath.h"
#include"../DisplayShape/DisplayModel.h"
#include "ui_ManipuWidget.h"
shared_ptr<DataEdit>widfil_dat = make_shared<DataEdit>();
shared_ptr<ModelFileCut>widfil_modcut = make_shared<ModelFileCut>();
shared_ptr<CurrentIndexType>widfil_idtyp = make_shared<CurrentIndexType>();
shared_ptr<CreatePathSolve>widfil_crttpso = make_shared<CreatePathSolve>();
#if DebugModel
#include<iostream>
#pragma optimize("", off)
#endif
void HandleFileObject::SetOffsetValFromUi(ManipuWidget*pointer)
{
    if(pointer->ui->linEdtOffsetValue_3->text() == ""){
        pointer->ui->linEdtOffsetValue_3->setText("3");
    }
    double val3 = pointer->ui->linEdtOffsetValue_3->text().toDouble();
    double val2,val1;
    if(val3 == 7){
        val2 = 5;
        val1 = 3;
    }
    else if(val3 == 6){
        val2 = 4;
        val1 = 2;
    }
    else if(val3 == 4){
        val2 = 3;
        val1 = 1.5;
    }
    else if(val3 == 3){
        val2 = 1.5;
        val1 = 0.8;
    }
    else{
        val3 == 4;
        val2 = 3;
        val1 = 2;
    }
    pointer->ui->linEdtOffsetValue_2->setText(QString::number(val2));
    pointer->ui->linEdtOffsetValue_1->setText(QString::number(val1));
}
void HandleFileObject::SaveSelectProfObject(ManipuWidget*pointer)
{
    if(pointer->ui->cheBoxIsBatchCut->isChecked() ||
            !widfil_modcut->CheckCutModel(pointer) || !m_isCurSelFind){
        widfil_idtyp->SetCurrentUiStatusInfo(pointer,"当前状态存储轮廓数据发生错误!",
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
    widfil_dat->SaveCutPairLoopData(saveDir,m_curSelPairLoop,isSuccess);
    QString labStr = "选择的轮廓数据存储成功!";
    if(!isSuccess){
        labStr = "选择的轮廓数存储存失败!";
    }
    widfil_idtyp->SetCurrentUiStatusInfo(pointer,labStr,!isSuccess);
}
void HandleFileObject::MoveCannotCutModelFile(ManipuWidget*pointer)
{
    if(m_curViewType != PartModel || m_vModelFile.empty()){
        return;
    }
    string savePath = pointer->ui->linEdiCanNotCutDir
            ->text().toStdString();
    if(savePath == ""){
        savePath = "./CutErrModelDir";
    }
    string filePath = m_vModelFile[m_curFileIdx].
            m_filePath.toStdString();
    /*if (access(filePath.c_str(), F_OK) != 0){
        return;
    }
    if (access(savePath.c_str(), F_OK) != 0){
        system(("mkdir " + savePath).c_str());
    }*/
    system(("mv " + filePath + " " + savePath).c_str());
    string labStr = filePath + "已移动至" + savePath;
    widfil_idtyp->SetCurrentUiStatusInfo(pointer,QString::fromStdString(labStr),false);
}
void HandleFileObject::MoveCurrentModelFile(ManipuWidget*pointer)
{
    if(m_curViewType != PartModel){
        return;
    }
    string filePath = m_vModelFile[m_curFileIdx].
            m_filePath.toStdString();
    /*if (access(filePath.c_str(), F_OK) != 0){
        widfil_idtyp->SetCurrentUiStatusInfo(pointer,"当前文件不存在，无法删除!",true);
        return;
    }*/
    string removeFileDir = "removeFileDir";
    /*if (access(removeFileDir.c_str(), F_OK) != 0){
        system(("mkdir " + removeFileDir).c_str());
    }*/
    system(("mv " + filePath + " " + removeFileDir).c_str());
    string labStr = filePath + "已删除!";
    widfil_idtyp->SetCurrentUiStatusInfo(pointer,QString::fromStdString(labStr),false);
}
void HandleFileObject::LoadToolPathSaveImg(ManipuWidget*pointer,
                                           bool isSuc,
                                           string &imgSaveDir,
                                           string &imgName)
{
    QString labStr;
    string tpPath = m_vToolPathFile[m_curFileIdx].
            m_filePath.toStdString();
    /*if (access(tpPath.c_str(), F_OK) != 0){
        labStr = "当前刀路文件不存在！";
        widfil_idtyp->SetCurrentUiStatusInfo(pointer,labStr,true);
        return;
    }*/
    bool isGet = false;
    string mvDir;
    widfil_crttpso->GetToolPathDir(tpPath,isSuc,mvDir,imgSaveDir,
                                   imgName,isGet);
    if(!isGet){
        /*if (access(tpPath.c_str(), F_OK) != 0){
            labStr = "当前刀路操作失败,需要进行相应操作！";
            widfil_idtyp->SetCurrentUiStatusInfo(pointer,labStr,true);
            return;
        }*/
    }
    labStr = m_vToolPathFile[m_curFileIdx].m_partName;
    labStr = labStr + "已移至:" +QString::fromStdString(mvDir);
    widfil_idtyp->SetCurrentUiStatusInfo(pointer,labStr,false);
}
void HandleFileObject::SaveToolPathDataInfo(ManipuWidget*pointer,
                                            bool isNex,bool isSuc)
{
    QString labStr;
    if(m_curViewType != ToolPathData || m_vToolPathFile.empty()){
        labStr = "刀路数据读取发生错误！";
        widfil_idtyp->SetCurrentUiStatusInfo(pointer,labStr,true);
        return;
    }
    string tpPath = m_vToolPathFile[m_curFileIdx].
            m_filePath.toStdString();
    /*if (access(tpPath.c_str(), F_OK) != 0){
        labStr = "当前刀路文件不存在！";
        widfil_idtyp->SetCurrentUiStatusInfo(pointer,labStr,true);
        return;
    }*/
    string imgSaveDir,imgName;
    LoadToolPathSaveImg(pointer,isSuc,imgSaveDir,imgName);
    //usleep(2000);
    //emit SignalScreen(imgSaveDir,imgName);
    widfil_idtyp->GetPreNexFileIndex(pointer,isNex);
}