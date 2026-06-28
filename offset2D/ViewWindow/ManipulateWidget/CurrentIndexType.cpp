#include "CurrentIndexType.h"
#include "ui_ManipuWidget.h"
#include"../../AlgorithmDeve/AutomaticCheck/AutoCheckPath.h"
#include"../../AlgorithmDeve/CheckToolPath/AreaClearCheck/ToolPathCheck.h"
#include"../ProgramCall/CreatePathCall.h"
#include"../DisplayShape/DisplayToolPath.h"
#include"../DisplayShape/DisplayModel.h"
shared_ptr<DataEdit>idtyp_dat = make_shared<DataEdit>();
shared_ptr<HandleText>idtyp_tex = make_shared<HandleText>();
shared_ptr<ElemCalculate>idtyp_num = make_shared<ElemCalculate>();
shared_ptr<DisplayToolPath>idtyp_view = make_shared<DisplayToolPath>();
shared_ptr<ViewCurObject>idtyp_vieobj = make_shared<ViewCurObject>();
#if DebugModel
#include<iostream>
#pragma optimize("", off)
#endif
void CurrentIndexType::SetCurrentUiStatusInfo(ManipuWidget *pointer,
                                              QString str,bool isWarning)
{
    if(str == ""){
        "There is currently no running status.";
        isWarning = true;
    }
    if(isWarning){
        pointer->ui->labRunStatus->setStyleSheet("background-color:rgb("
                                                 "211, 215, 207);"
                                                 "color: rgb(200,0,0);"
                                                 "font: 10pt Ubuntu");
    }
    else{
        pointer->ui->labRunStatus->setStyleSheet("background-color:rgb("
                                                 "211, 215, 207);"
                                                 "color: rgb(0,0,0);"
                                                 "font: 10pt Ubuntu");
    }
    pointer->ui->labRunStatus->setText(str);
}
void CurrentIndexType::GetCurrentIndexFile(ManipuWidget *pointer,
                                           CurViewType viewType)
{
    idtyp_vieobj->InitailUiStatus();
    QString strTotalNum = QString::number(m_totalFileNum);
    QString strCurIdx = QString::number(m_curFileIdx);
    QString reNum = QString::number(m_totalFileNum - m_curFileIdx);
    QString curFile = "无法定位到当前文件索引!";
    if(viewType == PartModel){
        strTotalNum = QString::number(m_vModelFile.size());
        if(m_curFileIdx < m_vModelFile.size() && m_curFileIdx >= 0){
            curFile = m_vModelFile[m_curFileIdx].m_filePath;
        }
    }
    else if(viewType == CutContourData){
        strTotalNum = QString::number(m_vCutFile.size());
        if(m_curFileIdx < m_vCutFile.size() && m_vCutFile.size() >= 0){
            curFile = m_vCutFile[m_curFileIdx].m_filePath;
        }
    }
    else if(viewType == ToolPathData){
        strTotalNum = QString::number(m_vToolPathFile.size());
        if(m_curFileIdx < m_vToolPathFile.size() && m_vToolPathFile.size() >= 0){
            curFile = m_vToolPathFile[m_curFileIdx].m_filePath;
        }
    }
    else if(viewType == ToolPathImage){
        strTotalNum = QString::number(m_vToolPathImg.size());
        if(m_curFileIdx < m_vToolPathImg.size() && m_vToolPathImg.size() >= 0){
            curFile = m_vToolPathImg[m_curFileIdx].m_filePath;
        }
    }
    QString str  = "总数:" + strTotalNum + ",当前:" + strCurIdx + ",剩余:" + reNum;
    pointer->ui->labListViewNum->setText(str);
    pointer->ui->lineEditCutPartName->setText(curFile);
}
void CurrentIndexType::LoadeViewFileList(ManipuWidget*pointer,
                                         CurViewType viewType)
{
    QFileDialog *fileDialog = new QFileDialog(pointer);
    fileDialog->setWindowTitle(QStringLiteral("请选择需要导入的模型轮廓数据"));
    fileDialog->setDirectory("1");
    if(viewType == PartModel){
        fileDialog->setNameFilter("file(*.step *.stp *.stl *.stls "
                                  "*.STEP *.STP *.STL *.STLS)");
    }
    else if(viewType == CutContourData || viewType == ToolPathData){
        fileDialog->setNameFilter("file(*.json *.JSON)");
    }
    else if(viewType == ToolPathImage){
        fileDialog->setNameFilter("file(*.jpg)");
    }
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
    }
    vector<FileInfo> vTemFileInfo;
    vector<QString>vPartName;
    vector<string>vSep{"/",".step",".STEP",".stp",".STP",
                       ".stl",".STL",".json",".JSON"};
    for(auto& it : fileNames){
        vector<QString>vFile;
        ///idtyp_dat->SlipeQFile(it,'/',vFile);
        vector<string>vSlipStr;
        idtyp_tex->SplitString(it.toStdString(),vSep,vSlipStr);
        for(auto& d: vSlipStr){
            vFile.push_back(QString::fromStdString(d));
        }
        QString partName = vFile.back();
        FileInfo file(it,partName);
        vTemFileInfo.push_back(file);
        vPartName.push_back(partName);
    }
    QStringList fileListView;
    for(auto& it : vPartName){
        fileListView.append(it);
    }
    m_listmodel = new QStringListModel(fileListView);
    pointer->ui->listViewListFile->setModel(m_listmodel);
    QString partNum = QString::number(vTemFileInfo.size());
    m_curViewType = viewType;
    if(m_curViewType == PartModel){
        m_vModelFile = vTemFileInfo;
        partNum = "成功导入" + partNum + "个零件.";
    }
    else if(m_curViewType == CutContourData){
        m_vCutFile = vTemFileInfo;
        partNum = "成功导入" + partNum + "个轮廓数据.";
    }
    else if(m_curViewType == ToolPathData){
        m_vToolPathFile = vTemFileInfo;
        partNum = "成功导入" + partNum + "个刀路数据.";
    }
    else if(m_curViewType == ToolPathImage){
        m_vToolPathImg = vTemFileInfo;
        partNum = "成功导入" + partNum + "个刀路图片.";
    }
    SetCurrentUiStatusInfo(pointer,partNum,false);
    m_totalFileNum = vTemFileInfo.size();
}
void CurrentIndexType::GetPreNexFileIndex(ManipuWidget *pointer, bool isNex)
{
    vector<FileInfo>vCurFile;
    if(m_curViewType == PartModel){
        vCurFile = m_vModelFile;
    }
    else if(m_curViewType == CutContourData){
        vCurFile = m_vCutFile;
    }
    else if(m_curViewType == ToolPathData){
        vCurFile = m_vToolPathFile;
    }
    else if(m_curViewType == ToolPathImage){
        vCurFile = m_vToolPathImg;
    }
    if(vCurFile.empty()){
        return;
    }
    if(isNex){
        m_curFileIdx = m_curFileIdx + 1;
    }
    else{
        m_curFileIdx = m_curFileIdx - 1;
    }
    if(m_curFileIdx < 0){
        m_curFileIdx = vCurFile.size() - 1;
    }
    if(m_curFileIdx > vCurFile.size() - 1){
        m_curFileIdx = 0;
    }
    GetCurrentIndexFile(pointer,m_curViewType);
    ViewCurrentIndexFileObject(pointer);
}
void CurrentIndexType::ViewCurrentIndexFileObject(ManipuWidget*pointer)
{
    switch (m_curViewType) {
    case PartModel:
        ViewCurrentIndexModelFile(pointer);
        break;
    case CutContourData:
        ViewCurrentIndexContour(pointer);
        break;
    case ToolPathData:
        ViewCurrentIndexToolPath(pointer);
        break;
    case ToolPathImage:
        ViewCurrentIndexToolPathPicture(pointer);
        break;
    default:
        break;
    }
    if(m_listmodel != nullptr){
        QModelIndex index = pointer->ui->listViewListFile->
                model()->index(m_curFileIdx,0);
        pointer->ui->listViewListFile->setCurrentIndex(index);
        pointer->ui->listViewListFile->update();
    }
}
void CurrentIndexType::ViewCurrentIndexModelFile(ManipuWidget*pointer)
{
    QString str;
    string path = m_vModelFile[m_curFileIdx].m_filePath.toStdString();
    /*if (access(path.c_str(), F_OK) != 0){
        str = "当前零件模型不存在！";
        SetCurrentUiStatusInfo(pointer,str,true);
        return;
    }*/
    SetCurrentUiStatusInfo(pointer,str,false);
    idtyp_vieobj->ReadCurrentModelFile(m_vModelFile[m_curFileIdx].m_filePath);
}
void CurrentIndexType::ViewCurrentIndexContour(ManipuWidget*pointer)
{
    QString str;
    BndIslPair cutPair;
    string path = m_vCutFile[m_curFileIdx].m_filePath.toStdString();
    /*if (access(path.c_str(), F_OK) != 0){
        str = "当前轮廓文件不存在！";
        SetCurrentUiStatusInfo(pointer,str,true);
        return;
    }*/
    bool isAnaSuc = false;
    idtyp_dat->AnalyticlePairLoopData(path,isAnaSuc,cutPair);
    QString val0,val1,val2,toolRadius;
    if(cutPair.m_vOffsetVal.size() == 3){
        val0 = QString::number(cutPair.m_vOffsetVal[0]);
        val1 = QString::number(cutPair.m_vOffsetVal[1]);
        val2 = QString::number(cutPair.m_vOffsetVal[2]);
        toolRadius = QString::number(cutPair._toolRadius,'f',2);;
    }
    if(isAnaSuc){
        str = "oftVal: " + val0 + "," + val1 +
                "," + val2 + ", R:" + toolRadius;
    }
    else{
        str = "存储文件转成轮廓数据失败. oftVal: " + val0 + "," +
                val1 + "," + val2 + ",R:" + toolRadius;
        SetCurrentUiStatusInfo(pointer,str,true);
        return;
    }
    ///idtyp_num->PrintLoopData(cutPair.m_bndLoop);
    for(auto &it : cutPair.m_vIslLoop){
        ///idtyp_num->PrintLoopData(it);
    }/**
    idtyp_elepre->InputPretreatment(cutPair.m_bndLoop,
                                    cutPair.m_vIslLoop,
                                    true,false,false,false,false);*/ 
    QString coordStr;
    GetSpecifyLoopLimitCoorStr(cutPair.m_bndLoop,coordStr);
    str = coordStr + "," + str;
    SetCurrentUiStatusInfo(pointer,str,false);
    GeomArea bndIslLoop(cutPair.m_bndLoop,cutPair.m_vIslLoop);
    idtyp_view->DisplayBndIslLoop(bndIslLoop,true,true);
}
void CurrentIndexType::ViewCurrentIndexToolPath(ManipuWidget*pointer)
{
    QString str;
    string path = m_vToolPathFile[m_curFileIdx].
            m_filePath.toStdString();
    /*if (access(path.c_str(), F_OK) != 0){
        str = "当前刀路文件不存在！";
        SetCurrentUiStatusInfo(pointer,str,true);
        return;
    }*/
    string partName;
    bool isAnaSuc = false;
    double firOffsetVal,offsetVal;
    BndIslPair pairLoop;
    shared_ptr<OffsetNode>rootNode;
    idtyp_dat->AnalyticleToolPathJsonNode(path,partName,firOffsetVal,
                                          offsetVal,isAnaSuc,pairLoop,
                                          rootNode);/**
    idtyp_elepre->InputPretreatment(pairLoop.m_bndLoop,
                                    pairLoop.m_vIslLoop,true,
                                    false,false,false, false);*/
    if(isAnaSuc){
        str = "打开刀路文件完成！";
        SetCurrentUiStatusInfo(pointer,str,false);
    }
    else{
        str = "打开刀路文件失败！";
        SetCurrentUiStatusInfo(pointer,str,true);
        return;
    }
    int loopNum = 0,eleNum=  0;
    idtyp_num->GetNodeLoopNum(rootNode,loopNum);
    idtyp_num->GetNodeEleNum(rootNode,eleNum);
    str = "环数量:" + QString::fromStdString(std::to_string(loopNum))
            + ",总元素数量:" + QString::fromStdString(std::to_string(eleNum));
    SetCurrentUiStatusInfo(pointer,str,false);
    GeomArea bndIslLoop(pairLoop.m_bndLoop,pairLoop.m_vIslLoop);
    ///idtyp_vieobj->DisplayCurrentToolPath(bndIslLoop,rootNode);
    DisplayToolPath viewtp;
    viewtp.HandleDisplayRootNodeShape(bndIslLoop,rootNode,true,true);
}
void CurrentIndexType::ViewCurrentIndexToolPathPicture(ManipuWidget*pointer)
{
    string path = m_vToolPathImg[m_curFileIdx].m_filePath.toStdString();
    /*if (access(path.c_str(), F_OK) != 0){
        QString str = "当前刀路图片不存在！";
        SetCurrentUiStatusInfo(pointer,str,true);
        return;
    }*/
    ///emit SignalSwitchWindDisplay(false,path);
}
void CurrentIndexType::GetSpecifyLoopLimitCoorStr(const DefLoop& loop,
                                                  QString& str)
{
    str = "";
    if(loop.m_vElem.empty()){
        return;
    }
    double minX,maxX,minY, maxY;/**
    idtyp_limcoo->LoopLimitCoordinate(loop,true,false,minX);
    idtyp_limcoo->LoopLimitCoordinate(loop,true,true,maxX);
    idtyp_limcoo->LoopLimitCoordinate(loop,false,false,minY);
    idtyp_limcoo->LoopLimitCoordinate(loop,false,true,maxY);
    str = "x:(" + QString::number(minX,'f',1) +
            "," + QString::number(maxX,'f',1) +
            "),y:(" + QString::number(minY,'f',1) +
            "," + QString::number(maxY,'f',1) + ")";*/
}
void CurrentIndexType::ViewSelectContourScale(ManipuWidget*pointer)
{
    if((m_curViewType != PartModel) || !m_isCurSelFind ||
            m_curSelPairLoop.m_bndLoop.m_vElem.empty()){
        return;
    }
    QString str;
    GetSpecifyLoopLimitCoorStr(m_curSelPairLoop.m_bndLoop,str);
    SetCurrentUiStatusInfo(pointer,str,false);
}