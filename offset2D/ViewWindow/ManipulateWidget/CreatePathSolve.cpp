#include "CreatePathSolve.h"
#include"../TextOrganize/DataEdit.h"
#include"../ProgramCall/CreatePathCall.h"
#include <iostream>
#include <filesystem>
#include <ios>
shared_ptr<ElemCalculate>crttpsol_num = make_shared<ElemCalculate>();
shared_ptr<GeomCalculate>crttpsol_go = make_shared<GeomCalculate>();
shared_ptr<ExicuteProgram>crttpsol_pro = make_shared<ExicuteProgram>();
shared_ptr<DataEdit>crttpsol_datEdi = make_shared<DataEdit>();
shared_ptr<CreatePathCall>crttpsol_exiAlg = make_shared<CreatePathCall>();
shared_ptr<HandleText>crttpsol_text = make_shared<HandleText>();
#if defined(_WIN32)
#include <windows.h>
#endif
#pragma optimize("", off)
void CreatePathSolve::GetEleZValue(const DefElem& eleData,
                                   double& zValue)
{
    if(eleData._type == LINETYPE){
        zValue = eleData._segment._segSp._z;
    }
    else if(eleData._type == ARCTYPE){
        zValue = eleData._arc._arcSp._z;
    }
    else if(eleData._type == CIRCLETYPE){
        zValue = eleData._circle._cirCp._z;
    }
}
bool CreatePathSolve::CheckOffsetVal(vector<double>vOffsetVal)
{
    if(vOffsetVal.size() != 3){
        return true;
    }
    double val0 = vOffsetVal[0],val1 = vOffsetVal[1],val2 = vOffsetVal[2];
    if((val0 == val1) || (val0 == val2) || (val1 == val2)){
        return true;
    }
    return false;
}
void CreatePathSolve::GetToolPathSavePath(string &saveDir,string partName,
                                          double offsetVal,string &saveImgName,
                                          string& savePath)
{
#if defined(_WIN32)
#else
    if (access(saveDir.c_str(), F_OK) != 0){
        saveDir = "./" + saveDir;
        system(("mkdir -p " + saveDir).c_str());
    }
#endif
    vector<string>vSep{"prof_",".json",".stp",".STP",
                       ".step",".STEP",".stl",".STL"};
    vector<string>vStr;
    crttpsol_text->SplitString(partName,vSep,vStr);
    if(!vStr.empty()){
        partName = vStr[0];
        saveImgName = vStr[0];
    }
    std::stringstream strem;
    strem << std::setprecision(2) << offsetVal;
    string valStr = strem.str();
    saveImgName = saveImgName + "_img_" + valStr;
    savePath = saveDir + "/" + partName + "_tp_" + valStr + ".json";
#if defined(_WIN32)
#else
    if (access(savePath.c_str(), F_OK) == 0){
        system(("rm " + savePath).c_str());
    }
#endif
}
///创建文件
void CreatePathSolve::CreateToolPathSaveDir(bool isCalAllStraTp,
                                            string areaClearDir,
                                            string areaClearRepNam,
                                            string modelCtDir,
                                            string modelCtRepNam,
                                            string ctFinishingDir,
                                            string ctFinishingRepNam,
                                            string headStr,
                                            MC_Strategy mcStrategy)
{
    if(areaClearRepNam == ""){
        areaClearRepNam = "areaClearToolPathData.txt";
    }
    if(modelCtRepNam == ""){
        modelCtRepNam = "amodelContourToolPathData.txt";
    }
    if(ctFinishingRepNam == ""){
        ctFinishingRepNam = "acontourFinishingToolPathData.txt";
    }
    if(headStr == ""){
        headStr = "fileName toolRdius offsetVal runTime status "
                  "loopNum EdgeNum toolPathLenth\n";
    }
    if(isCalAllStraTp){
#ifdef _WIN32
#elif __linux__
        if (access(areaClearDir.c_str(), F_OK) != 0){
            areaClearDir = "./" + areaClearDir;
            system(("mkdir -p " + areaClearDir).c_str());
        }
        if (access(modelCtDir.c_str(), F_OK) != 0){
            modelCtDir = "./" + modelCtDir;
            system(("mkdir -p " + modelCtDir).c_str());
        }
        if (access(ctFinishingDir.c_str(), F_OK) != 0){
            modelCtDir = "./" + ctFinishingDir;
            system(("mkdir -p " + modelCtDir).c_str());
        }
        system(("echo " + areaClearRepNam).c_str());
        system(("echo " + modelCtRepNam).c_str());
        system(("echo " + ctFinishingRepNam).c_str());
#endif
        std::ofstream areaClearFile,modelCtFile,ctFinishingFile;
        areaClearFile.open(areaClearRepNam,std::ios::out|std::ios::app);
        areaClearFile << headStr;
        areaClearFile.close();
        modelCtFile.open(modelCtRepNam,std::ios::out|std::ios::app);
        modelCtFile << headStr;
        modelCtFile.close();
        ctFinishingFile.open(ctFinishingRepNam,std::ios::out|std::ios::app);
        ctFinishingFile << headStr;
        ctFinishingFile.close();
        return;
    }
    std::ofstream fileStrea;
    if(mcStrategy == ModelAreaClearing){
#ifdef _WIN32
        areaClearDir = "./" + areaClearDir;
        BOOL result = CreateDirectory(areaClearDir.c_str(), NULL);
        if (result || ERROR_ALREADY_EXISTS == GetLastError()) {
            if(result){
                std::cout << "Directory created successfully: " << areaClearDir << std::endl;
            } else {
                std::cout << "Directory already exists: " << areaClearDir << std::endl;
            }
        } else {
            DWORD error = GetLastError();
            std::cerr << "Failed to create directory. Error code: " << error << std::endl;
        }
#elif __linux__
        if (access(areaClearDir.c_str(), F_OK) != 0){
            areaClearDir = "./" + areaClearDir;
            system(("mkdir -p " + areaClearDir).c_str());
        }
        system(("echo " + areaClearRepNam).c_str());
#endif
        fileStrea.open(areaClearRepNam,std::ios::out|std::ios::app);
        fileStrea << headStr;
        fileStrea.close();
    }
    else if(mcStrategy == ModelContour){
#ifdef _WIN32
#elif __linux__
        if (access(areaClearDir.c_str(), F_OK) != 0){
            areaClearDir = "./" + areaClearDir;
            system(("mkdir -p " + areaClearDir).c_str());
        }
        system(("echo " + modelCtRepNam).c_str());
#endif
        fileStrea.open(modelCtRepNam,std::ios::out|std::ios::app);
        fileStrea << headStr;
        fileStrea.close();
    }
    else {
#ifdef _WIN32
#elif __linux__
        if (access(ctFinishingDir.c_str(), F_OK) != 0){
            ctFinishingDir = "./" + ctFinishingDir;
            system(("mkdir -p " + ctFinishingDir).c_str());
        }
        system(("echo " + ctFinishingRepNam).c_str());
#endif
        fileStrea.open(ctFinishingRepNam,std::ios::out|std::ios::app);
        fileStrea << headStr;
        fileStrea.close();
    }
}
void CreatePathSolve::WriteFailureFileNameReport(bool isCalAllStra,
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
                                                 int ctFinFaiNum)
{
    string failurStr = "";
    if(isCalAllStra){
        for(auto& it : vAreCleFaiFil){
            failurStr = failurStr + it + "\n";
        }
        failurStr = "total: " + std::to_string(totalNum) +
                ",fail: " + std::to_string(areCleFaiNum) + "\n" + failurStr;
        std::ofstream fileStrea;
        fileStrea.open(areaClearRepNam,std::ios::out|std::ios::app);
        fileStrea << "\n"<< failurStr << "\n";
        fileStrea.close();
        failurStr = "";
        for(auto& it : vModCtFaiFil){
            failurStr = failurStr + it + "\n";
        }
        failurStr = "total: " + std::to_string(totalNum) +
                ",fail: " + std::to_string(modCtFaiNum) + "\n" + failurStr;
        fileStrea.open(modelCtRepNam,std::ios::out|std::ios::app);
        fileStrea << "\n"<< failurStr << "\n";
        fileStrea.close();
        failurStr = "";
        for(auto& it : vCtFinFaiFil){
            failurStr = failurStr + it + "\n";
        }
        failurStr = "total: " + std::to_string(totalNum) +
                ",fail: " + std::to_string(ctFinFaiNum) + "\n" + failurStr;
        fileStrea.open(ctFinishingRepNam,std::ios::out|std::ios::app);
        fileStrea << "\n"<< failurStr << "\n";
        fileStrea.close();
        return;
    }
    std::ofstream fileStrea;
    if(mcStrategy == ModelAreaClearing){
        for(auto& it : vAreCleFaiFil){
            failurStr = failurStr + it + "\n";
        }
        failurStr = "total: " + std::to_string(totalNum) +
                ",fail: " + std::to_string(areCleFaiNum) + "\n" + failurStr;
        fileStrea.open(areaClearRepNam,std::ios::out|std::ios::app);
        fileStrea << "\n"<< failurStr << "\n";
        fileStrea.close();
    }
    else if(mcStrategy == ModelContour){
        for(auto& it : vModCtFaiFil){
            failurStr = failurStr + it + "\n";
        }
        failurStr = "total: " + std::to_string(totalNum) +
                ",fail: " + std::to_string(modCtFaiNum) + "\n" + failurStr;
        fileStrea.open(modelCtRepNam,std::ios::out|std::ios::app);
        fileStrea << "\n"<< failurStr << "\n";
        fileStrea.close();
    }
    else{
        for(auto& it : vCtFinFaiFil){
            failurStr = failurStr + it + "\n";
        }
        failurStr = "total: " + std::to_string(totalNum) +
                ",fail: " + std::to_string(ctFinFaiNum) + "\n" + failurStr;
        fileStrea.open(ctFinishingRepNam,std::ios::out|std::ios::app);
        fileStrea << "\n"<< failurStr << "\n";
        fileStrea.close();
    }
}
void CreatePathSolve::CreateToolPathReport(QString partName,
                                           OffsetParam offsetPar,
                                           BndIslPair& pairLoop,
                                           shared_ptr<OffsetNode>&rootNode,
                                           int& failurNum,
                                           vector<string> &vFailurFile,
                                           string fileName)
{
    clock_t startTime, finishTime;
    startTime = clock();
    crttpsol_exiAlg->CallCreateToolPath(pairLoop,offsetPar,rootNode);
    finishTime = clock();
    string calStatus = "1";
    if(rootNode->_loops.empty() &&
            rootNode->_nodes.empty()){
        vFailurFile.push_back(partName.toStdString());
        failurNum ++;
        calStatus = "0";
    }
    double totaltime = (double)(finishTime - startTime) / 1000;
    string timeStr = std::to_string(totaltime);
    double offsetVal = offsetPar._offsetUnit.OffsetValue();
    if(offsetPar._pathTech.McStrategy() == ModelContour){
        offsetVal = offsetPar._offsetUnit.FirstOftVal();
    }
    double lenth = 0;
    int loopNum = 0,eleNum = 0;
    crttpsol_num->GetNodeLoopNum(rootNode,loopNum);
    crttpsol_num->GetNodeEleNum(rootNode,eleNum);
    crttpsol_num->GetNodeEleLenth(rootNode,lenth);
    vector<string>vSep{"prof_",".json",".stp",".STP",
                       ".step",".STEP",".stl",".STL"};
    vector<string>vStr;
    string modNam;
    crttpsol_text->SplitString(partName.toStdString(),vSep,vStr);
    if(!vStr.empty()){
        modNam = vStr[0];
    }
    std::stringstream strem;
    strem << std::setprecision(2) << offsetVal;
    string valStr = strem.str();
    modNam = modNam + "_tp_" + valStr + ".json";
    string timeInfo = /*partName.toStdString()*/modNam + " " +
            std::to_string(offsetVal) + " " +
            timeStr + " " +
            calStatus + " " +
            std::to_string(loopNum) + " " +
            std::to_string(eleNum) + " " +
            std::to_string(lenth) + "\n";
    std::ofstream fileStrea;
    fileStrea.open(fileName,std::ios::out|std::ios::app);
    fileStrea << timeInfo;
    fileStrea.close();
}
void CreatePathSolve::CalculateStrategyToolPath(QString partName,
                                                OffsetParam offsetPar,
                                                BndIslPair pairLoop,
                                                shared_ptr<OffsetNode> &rootNode,
                                                string straSaveDir, string &modelName,
                                                int &failurNum, vector<string> &vFailurFile,
                                                string fileName)
{
    rootNode = make_shared<OffsetNode>();
    CreateToolPathReport(partName,offsetPar,pairLoop,rootNode,
                         failurNum,vFailurFile,fileName);
    modelName = "";
    string tpSavePath;
    double offsetVal = offsetPar._offsetUnit.OffsetValue();
    if(offsetPar._pathTech.McStrategy() == ModelContour){
        offsetVal = offsetPar._offsetUnit.FirstOftVal();
    }
    if(offsetPar._pathTech.McStrategy() != ModelAreaClearing){
        offsetPar._offsetUnit.SetIsSmooth(false);
    }
    //usleep(50);
    GetToolPathSavePath(straSaveDir,partName.toStdString(),
                        offsetVal,modelName,tpSavePath);
    //usleep(50);
    bool isSucSave = false;
    crttpsol_datEdi->SaveToolPathNode(tpSavePath,partName.toStdString(),
                                      offsetPar,pairLoop,rootNode,
                                      isSucSave);
}
void CreatePathSolve::GetToolPathDir(string tpPath,bool isSuc,
                                     string &mvDir,
                                     string &imgSaveDir,
                                     string &imgName,
                                     bool &isGet)
{
    /**
    home/j-472/TestWork/partCutTest/已成交/batch2/batch2-1/
    ContourData/ContourFinishingToolPathData/
    aliCncProductModel_0d72bf8c7c104459852ca333d22202fe_tp_0.38.json*/
    isGet = false;
    vector<string> vSep{"/","_tp_",".json"},vStr;
    crttpsol_text->SplitString(tpPath,vSep,vStr);
    if(vStr.size() < 2){
        return;
    }
    string modelNam = vStr[int(vStr.size() - 2)];
    imgName = modelNam + "_img_" + vStr[int(vStr.size() - 1)] + ".jpg";
    string areCleKeyStr = "AreaClearToolPathData";
    string modCtKeyStr = "ModelContourToolPathData";
    string ctFinKeyStr = "ContourFinishingToolPathData";
    string mcStraKeyStr;
    MC_Strategy mcStra;
    bool isMcStra = false;
    for(auto &lo : vStr){
        if(lo == areCleKeyStr){
            mcStra = ModelAreaClearing;
            isMcStra = true;
            mcStraKeyStr = areCleKeyStr;
            break;
        }
        else if(lo == modCtKeyStr){
            mcStra = ModelContour;
            isMcStra = true;
            mcStraKeyStr = modCtKeyStr;
            break;
        }
        else if(lo == ctFinKeyStr){
            mcStra = ContourFinishing;
            isMcStra = true;
            mcStraKeyStr = ctFinKeyStr;
            break;
        }
    }
    if(!isMcStra){
        return;
    }
    string mcStraDir = "/";
    for(int i = 0;i < vStr.size();i++){
        if(vStr[i] == mcStraKeyStr){
            mcStraDir = mcStraDir + vStr[i];
            break;
        }
        else{
            mcStraDir = mcStraDir + vStr[i] + "/";
        }
    }
    string imgDir;
    if(mcStra == ModelAreaClearing){
        imgDir = mcStraDir + "/AreCleTPImg";
    }
    else if(mcStra == ModelContour){
        imgDir = mcStraDir + "/ModelCtTPImg";
    }
    else {
        imgDir = mcStraDir + "/CtFinishingTPImg";
    }
    string imgSucDir = "/imgSuccess",imgFaiDir = "/imgFailure",
            tpSucDir = "/tpSuccess",tpFaiDir = "/tpFailure";
    imgSucDir = imgDir + imgSucDir;
    imgFaiDir = imgDir + imgFaiDir;
    tpSucDir = mcStraDir + tpSucDir;
    tpFaiDir = mcStraDir + tpFaiDir;
    if(isSuc){
#if defined(_WIN32)
#else
        if (access(tpSucDir.c_str(), F_OK) != 0){
            system(("mkdir -p " + tpSucDir).c_str());
        }
        system(("mv " + tpPath + " " + tpSucDir).c_str());
        if (access(imgSucDir.c_str(), F_OK) != 0){
            system(("mkdir -p " + imgSucDir).c_str());
        }
#endif
        mvDir = tpSucDir;
        imgSaveDir = imgSucDir;
        isGet = true;
        return;
    }
    mvDir = tpFaiDir;
#if defined(_WIN32)
#else
    if (access(tpFaiDir.c_str(), F_OK) != 0){
        system(("mkdir -p " + tpFaiDir).c_str());
    }
    system(("mv " + tpPath + " " + tpFaiDir).c_str());
    if (access(imgFaiDir.c_str(), F_OK) != 0){
        system(("mkdir -p " + imgFaiDir).c_str());
    }
#endif
    imgSaveDir = imgFaiDir;
    isGet = true;
}