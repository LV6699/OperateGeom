#include "CreateToolPath.h"
#include "CreatePathSolve.h"
#include "ui_ManipuWidget.h"
#include"../../AlgorithmDeve/AutomaticCheck/AutoCheckPath.h"
#include"../../AlgorithmDeve/CheckToolPath/AreaClearCheck/ToolPathCheck.h"
#include"../ProgramCall/CreatePathCall.h"
#include"../DisplayShape/DisplayToolPath.h"
#include"../DisplayShape/DisplayModel.h"
#include"../ProgramCall/CreatePathCall.h"
#include"../../offset2D/ToolTrajectoryAlg/EquidCurve/PreprocLoop/PreprocLoop.h"
shared_ptr<DataEdit>wicrttp_dat = make_shared<DataEdit>();
shared_ptr<CreatePathCall>wicrttp_exiAlg = make_shared<CreatePathCall>();
shared_ptr<ViewCurObject>wicrttp_vieobj = make_shared<ViewCurObject>();
shared_ptr<CurrentIndexType>wicrttp_idtyp = make_shared<CurrentIndexType>();
shared_ptr<HandleFileObject>wicrttp_wifil = make_shared<HandleFileObject>();
shared_ptr<CreatePathSolve>wicrttp_tpsol = make_shared<CreatePathSolve>();
#if DebugModel
#include<iostream>
#pragma optimize("", off)
#endif
void CreateToolPath::GetCurrentStrategy(int index,MC_Strategy& mcStrategy,
                                        bool& isFixOftNum,int& offsetNum,
                                        bool& isSmooth)
{
    isSmooth = false;
    isFixOftNum = false;
    switch (index) {
    case 0:
        isSmooth = true;
        mcStrategy = ModelAreaClearing;
        break;
    case 1:
        isFixOftNum = true;
        offsetNum = 3;
        mcStrategy = ModelContour;
        break;
    case 2:
        isFixOftNum = true;
        offsetNum = 1;
        mcStrategy = ContourFinishing;
        break;
    default:
        mcStrategy = ModelAreaClearing;
        break;
    }
}
void CreateToolPath::CalculateToolPath(ManipuWidget*pointer)
{
    QString str = "There is currently no running status.";
    pointer->ui->labRunStatus->setText(str);
    int index = pointer->ui->comBoxMCStrategy->currentIndex();
    int offsetNum;
    double offsetVal = pointer->ui->linEdtOffsetValue_1->text().toDouble(),
            radius = (offsetVal / OftValDiamMultiplier) / 2,
            firOftVal = radius,smoothVal = offsetVal * 0.05;
    bool isFixOftNum = false,isSmooth = false,borIsBlank = true,isAcceBorOut = false;
    MC_Strategy mcStrategy;
    GetCurrentStrategy(index,mcStrategy,isFixOftNum,offsetNum,isSmooth);
    shared_ptr<OffsetNode>rootNode;
    OffsetParam offsetPar(isFixOftNum,offsetNum,firOftVal,
                              offsetVal,isSmooth,smoothVal,radius,
                              borIsBlank,isAcceBorOut,
                              Simultaneously_Offset,mcStrategy);
    offsetPar._offsetUnit.SetIsSmooth(false);
    if(pointer->ui->cheBoxIsTestSel->isChecked()){
        if(m_curViewType == CutContourData ||
                m_curSelPairLoop.m_bndLoop.m_vElem.empty() ||
                !m_isCurSelFind){
            str = "当选择以当前选择的轮廓生成刀路时,当前所选择的轮廓数据无效!";
            wicrttp_idtyp->SetCurrentUiStatusInfo(pointer,str,true);
            return;
        }
        str = "刀路计算中...";
        wicrttp_idtyp->SetCurrentUiStatusInfo(pointer,str,false);
        wicrttp_exiAlg->ExicuteToolPathCalculate(m_curSelPairLoop,
                                                 offsetPar,rootNode);
        str = "刀路计算完成!";
        wicrttp_idtyp->SetCurrentUiStatusInfo(pointer,str,false);
        pointer->ui->labRunStatus->setText(str);
        GeomArea bndIslLoop(m_curSelPairLoop.m_bndLoop,
                              m_curSelPairLoop.m_vIslLoop);
        wicrttp_vieobj->DisplayCurrentToolPath(bndIslLoop,rootNode);
        return;
    }
    bool isDiscret = false;
    double discPre = 0.005;
    CalculateImportFileToolPath(pointer,isDiscret,discPre,offsetPar);
}
void CreateToolPath::CalculateImportFileToolPath(ManipuWidget*pointer,
                                                 bool isDiscret,double discPre,
                                                 OffsetParam& offsetPar)
{
    QString str = "There is currently no running status.";
    if(m_curViewType != CutContourData || m_vCutFile.empty()){
        str = "当计算存储轮廓数据的刀路时,需要先导入要计算刀路的轮廓数据!";
        wicrttp_idtyp->SetCurrentUiStatusInfo(pointer,str,true);
        return;
    }
    string saveDir = "ToolPathData";
    ///offsetPar._offsetUnit.IsPathSmooth() = true;
    if(pointer->ui->cheBoxIsBatchCal->isChecked()){
        try{
            BatchCalculateToolPath(pointer,isDiscret,discPre,offsetPar);
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
        catch(string e1){
            std::cout << "Exception" << std::endl;
        }
        catch (const char* e2){
            std::cout << "Exception" << std::endl;
        }
       catch (...){
          std::cout << "Exception" << std::endl;
       }
        return;
    }
    bool isAnaSuc = false;
    BndIslPair pairLoop;
    string path = m_vCutFile[m_curFileIdx].m_filePath.toStdString();
    wicrttp_dat->AnalyticlePairLoopData(path,isAnaSuc,pairLoop);
    if(!isAnaSuc){
        str = "计算刀路时,解析导入的轮廓数据失败,请检查相应文件!";
        wicrttp_idtyp->SetCurrentUiStatusInfo(pointer,str,true);
        return;
    }
    ///****************************************************
    if(isDiscret){
        GeomArea BILoop(pairLoop.m_bndLoop,pairLoop.m_vIslLoop);
        GeometryLogic().TranslateBndIslToSegment(discPre,false,BILoop);
        pairLoop.m_bndLoop = BILoop.BndLoop();
        pairLoop.m_vIslLoop = BILoop.IslLoop();
    }
    ///****************************************************
    str = "刀路计算中...";
    shared_ptr<OffsetNode>rootNode;
    wicrttp_idtyp->SetCurrentUiStatusInfo(pointer,str,false);
    PreprocLoop().InputPreprocess(offsetPar,pairLoop.m_bndLoop,
                                      pairLoop.m_vIslLoop);
    wicrttp_exiAlg->ExicuteToolPathCalculate(pairLoop,offsetPar,rootNode);
    str = "刀路计算完成!";
    wicrttp_idtyp->SetCurrentUiStatusInfo(pointer,str,false);
    GeomArea bndIslLoop(pairLoop.m_bndLoop,pairLoop.m_vIslLoop);
    ///wicrttp_vieobj->DisplayCurrentToolPath(bndIslLoop,rootNode);
    ///emit SignalDisplayToolPath(oriRootNode,rootNode);
    string partName = m_vCutFile[m_curFileIdx].
            m_partName.toStdString();
    string savePath,modelName;
    wicrttp_tpsol->GetToolPathSavePath(saveDir,partName,
                                       offsetPar._offsetUnit.OffsetValue(),
                                       modelName,savePath);
    bool isSucSave = false;
    wicrttp_dat->SaveToolPathNode(savePath,partName,offsetPar,
                                  pairLoop,rootNode,isSucSave);
    str = "刀路计算完成,结果数据已保存至" +
            QString::fromStdString(saveDir);
    wicrttp_idtyp->SetCurrentUiStatusInfo(pointer,str,false);
    DisplayToolPath view;
    view.HandleDisplayRootNodeShape(bndIslLoop,rootNode,true,true);
}
//#include <tbb/tbb.h>
void CreateToolPath::BatchCalculateToolPath(ManipuWidget*pointer,
                                            bool isDiscret,double discPre,
                                            OffsetParam offsetPar)
{
    string areaClearDir = "areaClearToolPath";
    string areCleRepNam = areaClearDir +
            "/areaClearToolPath.txt";
    string modelCtDir = "ModelContourToolPath";
    string modelCtRepNam = modelCtDir +
            "/aModelContourToolPath.txt";
    string ctFinishDir = "ContourFinishingToolPath";
    string ctFinishRepNam = ctFinishDir +
            "/aContourFinishingToolPath.txt";
    string headStr = "fileName toolRdius offsetVal runTime status "
                     "loopNum EdgeNum toolPathLenth\n";
    bool isCalAllStra = pointer->ui->cheBoxCalAllStraTp->isChecked();
    isCalAllStra = false;
    ///offsetPar._mcStrategy = ModelAreaClearing;
    DataEdit datedi;
    CreatePathSolve patsol;
    GeometryLogic golg;CurrentIndexType curid;
    patsol.CreateToolPathSaveDir(isCalAllStra,areaClearDir,
                                 areCleRepNam,modelCtDir,
                                 modelCtRepNam,ctFinishDir,
                                 ctFinishRepNam,headStr,
                                 offsetPar._pathTech.McStrategy());
    int count = m_vCutFile.size();
    auto sTime = std::chrono::high_resolution_clock::now();
    m_curFileIdx = 0;
    vector<string> vOftValErr;
    vector<string>vAreCleFaiFil,vModCtFaiFil,vCtFinFaiFil;
    int areCleFaiNum = 0,modCtFaiNum = 0,ctFinFaiNum = 0;
    for (; m_curFileIdx < m_vCutFile.size(); ++m_curFileIdx) {
        if(m_listmodel != nullptr){
            QModelIndex index = pointer->ui->listViewListFile->model()->
                    index(m_curFileIdx,0);
            pointer->ui->listViewListFile->setCurrentIndex(index);
            pointer->ui->listViewListFile->update();
        }/*
        if(m_curFileIdx % 100 == 0){
            offsetPar._offsetUnit.SetFixedOffset(true);
            offsetPar._offsetUnit.SetOffsetNumber(3);
        }
        else{
            offsetPar._offsetUnit.SetFixedOffset(false);
            offsetPar._offsetUnit.SetOffsetNumber(0);
        }*/
        curid.GetCurrentIndexFile(pointer,CutContourData);
        QString path = m_vCutFile[m_curFileIdx].m_filePath;
        QString partName = m_vCutFile[m_curFileIdx].m_partName;
        if(m_curFileIdx == 6 ||
                partName == "prof_aliCncProductModel_54b2fb62227b49be9ff46687f3dcefa7.step.json"){
            int tem = 1;
        }
        std::cout << partName.toStdString() << endl;
        std::cout <<"执行文件总数:"<<count<<",当前正执行第"<< m_curFileIdx + 1
                 <<"个"<<",剩余:"
                <<count - m_curFileIdx - 1<<". --->";//usleep(100);
        bool isAnaSuc = false;
        BndIslPair pairLoop;
        datedi.AnalyticlePairLoopData(path.toStdString(),
                                      isAnaSuc,pairLoop);
        ///****************************************************
        if(isDiscret){
            GeomArea BILoop(pairLoop.m_bndLoop,pairLoop.m_vIslLoop);
            golg.TranslateBndIslToSegment(discPre,false,BILoop);
            pairLoop.m_bndLoop = BILoop.BndLoop();
            pairLoop.m_vIslLoop = BILoop.IslLoop();
        }
        ///****************************************************
        QString labStr;
        if(!isAnaSuc){
            labStr = "当前文件数据解析发生错误!";
            curid.SetCurrentUiStatusInfo(pointer,labStr,true);
            continue;
        }
        labStr = "当前文件数据解析完成!";
        shared_ptr<OffsetNode>rootNode;
        vector<double>vOffsetVal = pairLoop.m_vOffsetVal;
        if(vOffsetVal.size() != 3){
            vOftValErr.push_back(partName.toStdString());
            continue;
        }
        curid.SetCurrentUiStatusInfo(pointer,labStr,false);
        if(isCalAllStra){
            GetAreaClearToolPath(partName,offsetPar,pairLoop,
                                 rootNode,areaClearDir,areCleRepNam,
                                 vOffsetVal,areCleFaiNum,vOftValErr,
                                 vAreCleFaiFil);
            GetModelContourToolPath(partName,offsetPar,pairLoop,
                                    vOffsetVal,rootNode,modelCtDir,
                                    modelCtRepNam,modCtFaiNum,
                                    vAreCleFaiFil,vModCtFaiFil);
            GetModelProfFiniToolPath(partName,offsetPar,pairLoop,
                                     vOffsetVal,rootNode,ctFinishDir,
                                     ctFinishRepNam,ctFinFaiNum,
                                     vAreCleFaiFil,vCtFinFaiFil);
        }
        else{
            switch (offsetPar._pathTech.McStrategy()) {
            case ModelAreaClearing:
                GetAreaClearToolPath(partName,offsetPar,pairLoop,
                                     rootNode,areaClearDir,areCleRepNam,
                                     vOffsetVal,areCleFaiNum,vOftValErr,
                                     vAreCleFaiFil);
  
                break;
            case ModelContour:
                GetModelContourToolPath(partName,offsetPar,pairLoop,
                                        vOffsetVal,rootNode,modelCtDir,
                                        modelCtRepNam,modCtFaiNum,
                                        vAreCleFaiFil,vModCtFaiFil);
                break;
            case ContourFinishing:
                GetModelProfFiniToolPath(partName,offsetPar,pairLoop,
                                         vOffsetVal,rootNode,ctFinishDir,
                                         ctFinishRepNam,ctFinFaiNum,
                                         vAreCleFaiFil,vCtFinFaiFil);
                break;
            default:
                break;
            }
        }
        ///cout <<"),剩余:"<<count - m_curFileIdx - 1<<". --->";
        /**
        std::cout <<"执行文件总数:"<<count<<",当前正执行第"<< m_curFileIdx + 1
            <<"个("<<partName.toStdString()<<"),剩余:"
           <<count - m_curFileIdx - 1<<". --->";*//**
        cout <<"执行文件总数:"<<count<<",当前执行第"<< m_curFileIdx + 1
            <<"个，剩余:"<<count - m_curFileIdx - 1<<"."<<endl;*/
    }
    patsol.WriteFailureFileNameReport(isCalAllStra,offsetPar._pathTech.McStrategy(),
                                      areCleRepNam,modelCtRepNam,
                                      ctFinishRepNam,vAreCleFaiFil,
                                      vModCtFaiFil,vCtFinFaiFil,
                                      int(m_vCutFile.size()),
                                      areCleFaiNum,modCtFaiNum,
                                      ctFinFaiNum);
    auto eTime = std::chrono::high_resolution_clock::now();
    long double exeTime = std::chrono::duration<double>(eTime - sTime).count();
    std::cout<<"刀路已全部计算完成,轮廓数据刀路行距存在错误的零件总数:"<<vOftValErr.size()
       << ",执行时间(ms):"<< exeTime * 1000<<"("<< exeTime / 60<<"分钟)."<< std::endl;
    for(auto&it : vOftValErr)
    std::cout << it <<std::endl;
    std::cout<<"";
}
#pragma optimize("", off)
void CreateToolPath::GetAreaClearToolPath(QString partName,
                                          OffsetParam offsetPar,
                                          BndIslPair pairLoop,
                                          shared_ptr<OffsetNode>&rootNode,
                                          string areaClearDir,
                                          string areCleRepNam,
                                          vector<double>vOffsetVal,
                                          int& areCleFaiNum,
                                          vector<string>& vOftValErr,
                                          vector<string>&vAreCleFaiFil)
{
    for(int v = 0;v < vOffsetVal.size();v ++){
        string saveImgName;
        SetOffsetVal(ModelAreaClearing,vOffsetVal[v],offsetPar);
        wicrttp_tpsol->CalculateStrategyToolPath(partName,offsetPar,pairLoop,
                                                 rootNode,areaClearDir,
                                                 saveImgName,areCleFaiNum,
                                                 vAreCleFaiFil,areCleRepNam);
    }
    if(wicrttp_tpsol->CheckOffsetVal(vOffsetVal)){
        vOftValErr.push_back(partName.toStdString());
    }
    std::cout<<partName.toStdString()<<" 模型区域清除刀路计算完成.\n";
}
void CreateToolPath::GetModelContourToolPath(QString partName,
                                             OffsetParam offsetPar,
                                             BndIslPair pairLoop,
                                             vector<double> vOffsetVal,
                                             shared_ptr<OffsetNode>&rootNode,
                                             string modelCtDir,
                                             string modelCtRepNam,
                                             int& modCtFaiNum,
                                             vector<string> &vOftValErr,
                                             vector<string>&vModCtFaiFil)
{
    for(int v = 0;v < vOffsetVal.size();v ++){
        string saveImgName;
        SetOffsetVal(ModelContour,vOffsetVal[v],offsetPar);
        wicrttp_tpsol->CalculateStrategyToolPath(partName,offsetPar,pairLoop,
                                                 rootNode,modelCtDir,
                                                 saveImgName,modCtFaiNum,vModCtFaiFil,
                                                 modelCtRepNam);
    }
    if(wicrttp_tpsol->CheckOffsetVal(vOffsetVal)){
        vOftValErr.push_back(partName.toStdString());
    }
    std::cout<<"模型轮廓刀路计算完成.\n";
}
void CreateToolPath::GetModelProfFiniToolPath(QString partName,
                                              OffsetParam offsetPar,
                                              BndIslPair pairLoop,
                                              vector<double>vOffsetVal,
                                              shared_ptr<OffsetNode>&rootNode,
                                              string ctFinishDir,
                                              string ctFinishRepNam,
                                              int& ctFinFaiNum,
                                              vector<string>& vOftValErr,
                                              vector<string>&vCtFinFaiFil)
{
    for(int v = 0;v < vOffsetVal.size();v ++){
        string saveImgName;
        SetOffsetVal(ContourFinishing,vOffsetVal[v],offsetPar);
        wicrttp_tpsol->CalculateStrategyToolPath(partName,offsetPar,pairLoop,
                                                 rootNode,ctFinishDir,
                                                 saveImgName,ctFinFaiNum,vCtFinFaiFil,
                                                 ctFinishRepNam);
    }
    if(wicrttp_tpsol->CheckOffsetVal(vOffsetVal)){
        vOftValErr.push_back(partName.toStdString());
    }
    std::cout<<"轮廓精加工刀路计算完成.\n";
    /**
    emit SignalDisplayToolPath(oriRootNode,rootNode);
    usleep(300000);
    string imgSaveDir = ctFinishDir + "/CtFinishingTPImg";
    if (access(imgSaveDir.c_str(), F_OK) != 0){
        system(("mkdir -p " + imgSaveDir).c_str());
    }
    saveImgName += ".jpg";
    emit SignalScreen(imgSaveDir,saveImgName);*/
}
void CreateToolPath::SetOffsetVal(MC_Strategy msStratagy,double offsetVal,
                                  OffsetParam& offsetPar)
{
    switch (msStratagy) {
    case ModelAreaClearing:
        offsetPar._offsetUnit.SetOffsetValue(offsetVal);
        offsetPar._offsetUnit.SetToolRadius((offsetVal / OftValDiamMultiplier) / 2);
        break;
    case ModelContour:
        offsetPar._offsetUnit.SetFixedOffset(true);
        offsetPar._offsetUnit.SetOffsetNumber(3);
        ///double oftVal_ = offsetVal / 2;
        offsetPar._offsetUnit.SetOffsetValue(offsetVal / 2);
        offsetPar._offsetUnit.SetToolRadius(((offsetVal / 2) / OftValDiamMultiplier) / 2);
        break;
    case ContourFinishing:
        offsetPar._offsetUnit.SetFixedOffset(true);
        offsetPar._offsetUnit.SetOffsetNumber(1);
        offsetPar._offsetUnit.SetOffsetValue(offsetVal);
        offsetPar._offsetUnit.SetToolRadius((offsetVal / OftValDiamMultiplier) / 2);
        break;
    default:
        offsetPar._offsetUnit.SetOffsetValue(offsetVal);
        offsetPar._offsetUnit.SetToolRadius((offsetVal / OftValDiamMultiplier) / 2);
        break;
    }
    double toolRadius = offsetPar._offsetUnit.ToolRadius();
    offsetPar._offsetUnit.SetFirstOffsetValue(toolRadius);
}