#include "TrajectoryCheck.h"
#include"../../AlgorithmDeve/AutomaticCheck/AutoCheckPath.h"
#include"../../AlgorithmDeve/CheckToolPath/AreaClearCheck/ToolPathCheck.h"
#include"../ProgramCall/CreatePathCall.h"
#include"../DisplayShape/DisplayToolPath.h"
#include"../DisplayShape/DisplayModel.h"
#include "ui_ManipuWidget.h"
//#include <tbb/tbb.h>
shared_ptr<DataEdit>widtpch_dat = make_shared<DataEdit>();
shared_ptr<AutoCheckPath>widtpch_tpauch = make_shared<AutoCheckPath>();
shared_ptr<ToolPathCheck>widtpch_tpche = make_shared<ToolPathCheck>();
shared_ptr<CurrentIndexType>widtpch_idtyp = make_shared<CurrentIndexType>();
shared_ptr<TrajectoryCheck>widtpch_tpch = make_shared<TrajectoryCheck>();
void TrajectoryCheck::ExecuteTrajectoryCheck(ManipuWidget*pointer)
{
    if(m_vToolPathFile.empty()){
        QString info = "检测数据为空,请导入待检测刀路!";
        widtpch_idtyp->SetCurrentUiStatusInfo(pointer,info,true);
        return;
    }
    if(!pointer->ui->isCheckAllTP->isChecked()){
        string path = m_vToolPathFile[m_curFileIdx].
                m_filePath.toStdString();
        widtpch_tpch->SingleToolPathCheck(pointer,path);
        return;
    }
    string saveDir = "./AutoCheck";
    string reportFile = saveDir + "/FailureReport.txt";
    string successDir = saveDir + "/" + "tp_success";
    string failuerDir = saveDir + "/" + "tp_failuer";
    DataEdit dated;
    AutoCheckPath autoch;
    ToolPathCheck tpch;
    autoch.InitialInputFile(saveDir,successDir,
                            failuerDir,reportFile);
    int i = 0;
    clock_t startTime = clock(), finishTime;
    startTime = clock();
    int tpInputErrNum = 0,failureNumber = 0;
    string inputErr = "\n";
    for (; i < m_vToolPathFile.size(); ++i) {
        string path = m_vToolPathFile[i].m_filePath.toStdString();
#ifdef _WIN32
#elif __linux__
        if (access(path.c_str(), F_OK) != 0){
            tpInputErrNum ++;
            inputErr = inputErr + m_vToolPathFile[i].
                    m_partName.toStdString() + "\n";
            std::cout<<"注意:当前刀路文件不存在!\n";
            continue;
        }
#endif
        string partName;
        bool isAnaSuc = false;
        BndIslPair pairLoop;
        double firOffsetVal,offsetVal;
        shared_ptr<OffsetNode>rootNode = make_shared<OffsetNode>();
        dated.AnalyticleToolPathJsonNode(path,partName,firOffsetVal,
                                         offsetVal,isAnaSuc,
                                         pairLoop,rootNode);
        std::cout << partName << std::endl;
        bool isSuccess = false;
        double toolRadius = offsetVal/*(offsetVal / OftValDiamMultiplier) / 2*/;
        CheckFailType failType;
        GeomArea bndIslLoop(pairLoop.m_bndLoop,pairLoop.m_vIslLoop);
        tpch.ExicuteToolPathCheck(bndIslLoop,rootNode,toolRadius,
                                  failType,isSuccess);
        std::cout <<"刀路总数:"<<m_vToolPathFile.size()<<",当前计算第"<< i + 1
            <<",剩余:"<<m_vToolPathFile.size() - i - 1<<", "
           << m_vToolPathFile[i].m_partName.toStdString()
           << " 已检测结束,"<<"结果:" << isSuccess <<",失败:"
           << failureNumber<<std::endl;
        autoch.HandleAutoCheckResult(path,m_vToolPathFile[i].
                                     m_partName.toStdString(),
                                     successDir,failuerDir,
                                     reportFile,failType,
                                     isSuccess,failureNumber);
    }
    finishTime = clock();
    double totaltime = (double)(finishTime - startTime) / 1000;
    std::cout<<"刀路已全部检测完成,失败个数:" << failureNumber
            << ",执行时间(ms):"<< totaltime<<"("<<totaltime / 60000 <<"分钟)."<< std::endl;
}
void TrajectoryCheck::SingleToolPathCheck(ManipuWidget*pointer,string path)
{
#ifdef _WIN32
#elif __linux__
    if (access(path.c_str(), F_OK) != 0){
        QString str = "当前刀路文件不存在！";
        widtpch_idtyp->SetCurrentUiStatusInfo(pointer,str,true);
        return;
    }
#endif
    string partName;
    BndIslPair pairLoop;
    bool isAnaSuc = false;
    double firOffsetVal,offsetVal;
    shared_ptr<OffsetNode>rootNode = make_shared<OffsetNode>();
    widtpch_dat->AnalyticleToolPathJsonNode(path,partName,firOffsetVal,
                                            offsetVal,isAnaSuc,
                                            pairLoop,rootNode);
    bool isSuccess = false;
    double toolRadius = offsetVal/*(offsetVal / OftValDiamMultiplier) / 2*/;
    CheckFailType failType;
    GeomArea bndIslLoop(pairLoop.m_bndLoop,pairLoop.m_vIslLoop);
    widtpch_tpche->ExicuteToolPathCheck(bndIslLoop,rootNode,toolRadius,
                                        failType,isSuccess);
    QString info = "已检测完成: 刀路计算正确!";
    if(!isSuccess){
        QString type;
        switch(failType){
        case ToolPathIsEmpty:
            type = "刀路数据为空!";
            break;
        case LoopExistSelfInt:
            type = "刀路存在自相交!";
            break;
        case LoopLoopExistInt:
            type = "刀路之间存在相交!";
            break;
        case ExistUnProcessedArea:
            type = "存在未加工区域!";
            break;
        case InputCheckParaErr:
            type = "检测输入存在错误,如:刀具半径、"
                   "边界轮廓、岛屿轮廓等输入错误!";
        }
        info = "已检测完成: 刀路计算存在错误: " + type;
    }
    widtpch_idtyp->SetCurrentUiStatusInfo(pointer,info,!isSuccess);
}