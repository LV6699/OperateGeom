#include "RegreTest.h"
shared_ptr<HandleText>reg_text = make_shared<HandleText>();
shared_ptr<DataEdit>reg_edi = make_shared<DataEdit>();
shared_ptr<ElemCalculate>reg_num = make_shared<ElemCalculate>();
void RegreTest::PrintSplitErrStr(int i,string splitStr,vector<string>vStr)
{
    std::cout<<" slipErr "<<"i:"<<i<<" "<<splitStr<<"->>>";
    for(auto& it : vStr){
        std::cout<<it<<" ";
    }
    std::cout<<std::endl;
}
void RegreTest::GetDirFileTpData(string dir,
                                 vector<RegData>&vRepData)
{/*
    vRepData.clear();
    string suffix = "json";
    vector<string> vFile;
    reg_text->GetDirFileList(dir,suffix,vFile);
    vector<string> vSep{"_tp_"};
    for(int i = 0;i < vFile.size();++i){
        string path = dir + "/" + vFile[i],partName;
        if (access(path.c_str(), F_OK) != 0){
            cout<<"err->i:"<<i<<" "<<vFile[i]<<" file is not exist."<<endl;
        }
        double firOftVal,offsetVal;
        bool isAnaSuc = false;
        BndIslPair pairLoop;
        shared_ptr<OffsetNode>rootNode;
        reg_edi->AnalyticleToolPathJsonNode(path,partName,
                                            firOftVal,offsetVal,
                                            isAnaSuc,pairLoop,
                                            rootNode);
        vector<string>vStr;
        reg_text->SplitString(vFile[i],vSep,vStr);
        if(vStr.empty()){
            cout<<"err-> i:"<< i << " "<< vFile[i]<<" split error."<<endl;
            continue;
        }
        string modNam = vStr[0];
        int loopNum = 0,eleNum = 0;
        reg_num->GetNodeLoopNum(rootNode,loopNum);
        reg_num->GetNodeEleNum(rootNode,eleNum);
        RegData regData(vFile[i],modNam,offsetVal,loopNum,eleNum);
        vRepData.push_back(regData);
    }*/
}
void RegreTest::GetReportFileTpData(string repFile,
                                    vector<RegData>&vRepData)
{
    vector<string>vLine;
    reg_text->ReadFile(repFile,vLine);
    /**
    vector<string> vSep{"prof_",".step",".STEP",".stp",".STP",
                        ".json"," "};*/
    vector<string> vSep{"_tp_",".json"," "};
    for(int i = 0;i < vLine.size();++i){
        if(i == 0){
            continue;
        }
        vector<string>vStr;
        reg_text->SplitString(vLine[i],vSep,vStr);
        if(vStr.size() != 7){
            PrintSplitErrStr(i,vLine[i],vStr);
            continue;
        }
        double offsetVal = std::stold(vStr[1].c_str());
        int loopNum = std::atoi(vStr[5].c_str());
        int eleNum = std::atoi(vStr[6].c_str());
        RegData regData(vStr[0],vStr[0],offsetVal,loopNum,eleNum);
        vRepData.push_back(regData);
    }
}
void RegreTest::RegComTpData(string repDir,string repFile,
                             string tpDir,string regResDir)
{
    vector<RegData>vRepData,vDirData,vDifData;
    repFile = repDir + "/" + repFile;
    GetReportFileTpData(repFile,vRepData);
    GetDirFileTpData(tpDir,vDirData);
    vector<RegDataCom>vDifCom;
    for(int i = 0;i < vRepData.size();++i){
        RegData repData = vRepData[i];
        for(int j = 0;j < vDirData.size();++j){
            RegData dirData = vDirData[j];
            if(dirData.m_isVis || (dirData.m_modNam != repData.m_modNam) ||
                    (dirData._offsetVal != repData._offsetVal)){
                continue;
            }
            if((repData.m_loopNum != dirData.m_loopNum) ||
                    (repData.m_eleNum != dirData.m_eleNum)){
                vDifData.push_back(dirData);
            }
            RegDataCom difData(dirData.m_fileNam,dirData._offsetVal,
                               dirData.m_loopNum,dirData.m_eleNum,
                               repData.m_loopNum,repData.m_eleNum);
            vDifCom.push_back(difData);
            dirData.m_isVis = true;
        }
    }
    vector<string>vDifFile;
    for(int i = 0;i < vDifData.size();++i){
        RegData curData = vDifData[i];
        string file = repDir + "/" + curData.m_fileNam;
        vDifFile.push_back(file);
    }
    string diffDir = regResDir + "/DiffTpFile";
    system(("mkdir -p " + diffDir).c_str());
    for(int i = 0;i < vDifFile.size();++i){
        system(("mv " + vDifFile[i] + " " + diffDir).c_str());
    }
    string fileName = regResDir + "/1_DiffInfo.txt";
    system(("echo " + fileName).c_str());
    string filHead = "fileName offsetVal preLoopNum preEleNum "
                     "curLoopNum curEleNum\n";
    std::ofstream fileStrea;
    fileStrea.open(fileName,std::ios::out);
    fileStrea << filHead;
    fileStrea.close();
    for(int i = 0;i < vDifCom.size();++i){
        string str = vDifCom[i].m_fileNam +" " +
                std::to_string(vDifCom[i]._offsetVal) + " " +
                std::to_string(vDifCom[i].m_preLoopNum) + " " +
                std::to_string(vDifCom[i].m_preEleNum) + " " +
                std::to_string(vDifCom[i].m_curLoopNum) + " " +
                std::to_string(vDifCom[i].m_curEleNum)+ "\n";
        fileStrea.open(fileName,std::ios::out|std::ios::app);
        fileStrea << str;
        fileStrea.close();
    }
}