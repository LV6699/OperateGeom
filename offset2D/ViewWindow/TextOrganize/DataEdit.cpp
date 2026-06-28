#include "DataEdit.h"
#include <sys/stat.h>
//#include <unistd.h>
#include <string>
#include <fstream>
shared_ptr<ToolPathJson>datedi_tpjso = make_shared<ToolPathJson>();
void DataEdit::GetPartViewData(const CutPartResult& cutResult,
                               vector<DefLoop>& vLoopData)
{
    vLoopData.clear();
    int par = 0;
    while (par < cutResult.m_vCutPartResult.size()) {
        int are = 0;
        PartCut curPart = cutResult.m_vCutPartResult[par].m_CutPartData;
        while (are < curPart.m_vAreaData.size()) {
            if(curPart.m_vAreaData.empty()){
                are ++;
                continue;
            }
            int lay = 0;
            PartAreaCut curArea = curPart.m_vAreaData[are];
            while (lay < curArea.m_vLayData.size()) {
                PartLayCut curLayLoop = curArea.m_vLayData[lay];
                BndIslPair curPairLoop = curLayLoop.m_vPairLoop[0];
                DefLoop bndLoop = curPairLoop.m_bndLoop;
                vector<DefLoop>vLoop;
                vLoop.push_back(bndLoop);
                vector<DefLoop>vIslLoop = curPairLoop.m_vIslLoop;
                for(auto& it : vIslLoop){
                    vLoop.push_back(it);
                }
                for(auto& it : vLoop){
                    vLoopData.push_back(it);
                }
                lay ++;
            }
            are ++;
        }
        par ++;
    }
}
void DataEdit::GetSinglePartViewData(const PartCutData& partData,
                                     vector<DefLoop>& vLoopData)
{
    int are = 0;
    PartCut curPart = partData.m_CutPartData;
    while (are < curPart.m_vAreaData.size()) {
        if(curPart.m_vAreaData.empty()){
            are ++;
            continue;
        }
        int lay = 0;
        PartAreaCut curArea = curPart.m_vAreaData[are];
        while (lay < curArea.m_vLayData.size()) {
            PartLayCut curLayLoop = curArea.m_vLayData[lay];
            BndIslPair curPairLoop = curLayLoop.m_vPairLoop[0];
            DefLoop bndLoop = curPairLoop.m_bndLoop;
            vector<DefLoop>vLoop;
            vLoop.push_back(bndLoop);
            vector<DefLoop>vIslLoop = curPairLoop.m_vIslLoop;
            for(auto& it : vIslLoop){
                vLoop.push_back(it);
            }
            for(auto& it : vLoop){
                vLoopData.push_back(it);
            }
            lay ++;
        }
        are ++;
    }
}
bool DataEdit::IsSuccessPartFile(string path)
{
    std::fstream file;
    file.open(path,std::ios::in);
    if(!file){
        std::cout<<"!file";
        return false;
    }
    std::ifstream inFile(path.c_str());
    Json::Reader jreader;
    Json::Value jValue;
    if(!jreader.parse(inFile,jValue)){
        std::cout<<"!jreader.parse(inFile,partRoot)";
        return false;
    }
    return true;
}
void DataEdit::SlipeFile(string str,char sep,vector<string>&vStr)
{
    int startIdx = 0, endIdx = 0;
    for (int i = 0; i <= str.size(); i++){
        if (str[i] == sep || i == str.size()){
            endIdx = i;
            string temp;
            temp.append(str,startIdx,endIdx - startIdx);
            vStr.push_back(temp);
            startIdx = endIdx + 1;
        }
    }
}
void DataEdit::SlipeQFile(QString qstr,char sep,
                          vector<QString>&vQstr)
{
    vQstr.clear();
    string str = qstr.toStdString();
    vector<string>vStr;
    SlipeFile(str,sep,vStr);
    for(auto& it : vStr){
        vQstr.push_back(QString::fromStdString(it));
    }
}
void DataEdit::SaveCutPartResult(const CutPartPara& cutPar,
                                 const CutPartResult& cutResult)
{
    Json::Value root;
    std::ofstream outFiles("PartCutResult.json");
    int par = 0,partNum = cutResult.m_vCutPartResult.size();
    while (par < cutResult.m_vCutPartResult.size()) {
        int are = 0;
        PartCut curPart = cutResult.m_vCutPartResult[par].m_CutPartData;
        string partName = cutResult.m_vCutPartResult[par].m_CutPartName;
        Json::Value PartRoot,PartInfo;
        PartInfo["partName"] = partName;
        PartInfo["partNum"] = partNum;
        PartInfo["partIdx"] = par;
        ///PartRoot["partInfo"].append(PartInfo);
        PartRoot["partInfo"] = PartInfo;
        while (are < curPart.m_vAreaData.size()) {
            if(curPart.m_vAreaData.empty()){
                are ++;
                continue;
            }
            int lay = 0;
            Json::Value AreaRoot,AreaInfo;
            AreaInfo["partNum"] = partNum;
            AreaInfo["partIdx"] = par;
            AreaInfo["areaNum"] = curPart.m_vAreaData.size();
            AreaInfo["areaIdx"] = are;
            AreaRoot["areaInfo"] = AreaInfo;
            PartAreaCut curArea = curPart.m_vAreaData[are];
            while (lay < curArea.m_vLayData.size()) {
                PartLayCut curLayLoop = curArea.m_vLayData[lay];
                BndIslPair curPairLoop = curLayLoop.m_vPairLoop[0];
                Json::Value LayRoot,LayInfo,pairRoot,pairInfo,
                        bndLoopRoot,vIslLoopRoot;
                LayInfo["areIdx"] = are;
                LayInfo["layNum"] = curArea.m_vLayData.size();
                LayInfo["layIdx"] = lay;
                LayRoot["LayInfo"] = LayInfo;
                DefLoop bndLoop = curPairLoop.m_bndLoop;
                for(int i = 0;i < bndLoop.m_vElem.size();i++){
                    Json::Value bndEle;
                    datedi_tpjso->TranslateEleToJson(bndLoop.m_vElem[i],i,bndEle);
                    bndLoopRoot.append(bndEle);
                }
                vector<DefLoop>vIslLoop = curPairLoop.m_vIslLoop;
                int lo = 0;
                while (lo < vIslLoop.size()) {
                    DefLoop curLoop = vIslLoop[lo];
                    Json::Value curLoopRoot,loopData,islLoopInfo;
                    islLoopInfo["loopIdx"] = lo;
                    curLoopRoot["loopInfo"] = islLoopInfo;
                    for(int e = 0;e < curLoop.m_vElem.size();e ++){
                        Json::Value islEle;
                        datedi_tpjso->TranslateEleToJson(curLoop.m_vElem[e],e,islEle);
                        loopData.append(islEle);
                    }
                    curLoopRoot["loopData"] = loopData;
                    vIslLoopRoot["vLoopRoot"].append(curLoopRoot);
                    lo ++;
                }
                pairRoot["bndLoopRoot"] = bndLoopRoot;
                pairRoot["vIslLoopRoot"] = vIslLoopRoot;
                LayRoot["pairRoot"].append(pairRoot);
                AreaRoot["layRoot"].append(LayRoot);
                lay ++;
            }
            PartRoot["areaRoot"].append(AreaRoot);
            are ++;
        }
        root["topRoot"].append(PartRoot);
        par ++;
    }
    Json::FastWriter ResultFile;
    string file = ResultFile.write(root);
    outFiles << file;
    outFiles.close();
}
void DataEdit::AnalyticCutPartResult(string path,
                                     CutPartResult& cutResult)
{
    std::fstream file;
    file.open(path,std::ios::in);
    if(!file){
        std::cout<<"!file";
        return;
    }
    std::ifstream inFile(path.c_str());
    Json::Reader jreader;
    Json::Value jValue;
    if(!jreader.parse(inFile,jValue)){
        std::cout<<"!jreader.parse(inFile,partRoot)";
        return;
    }
    vector<PartCutData> vCutPartResult;
    Json::Value topRoot = jValue["topRoot"];
    int partNum = topRoot.size();
    int pIdx = 0;
    while (pIdx < partNum) {
        Json::Value partInfo = topRoot[pIdx]["partInfo"];
        string partName = partInfo["partName"].asString();
        int partIdx = partInfo["partIdx"].asInt();
        vector<PartAreaCut> vAreaData;
        Json::Value areaRoot = topRoot[pIdx]["areaRoot"];
        int areaNum = areaRoot.size();
        int aIdx = 0;
        while (aIdx < areaNum) {
            Json::Value areaInfo = areaRoot[aIdx]["areaInfo"];
            int areaNum = areaInfo["areaNum"].asInt();
            int areaIdx = areaInfo["areaIdx"].asInt();
            vector<PartLayCut> vLayData;
            Json::Value layRoot = areaRoot[aIdx]["layRoot"];
            int layNum = layRoot.size();
            int lIdx = 0;
            while (lIdx < layNum) {
                Json::Value layInfo = layRoot[lIdx]["LayInfo"];
                Json::Value pairRoot = layRoot[lIdx]["pairRoot"];
                Json::Value bndRoot = pairRoot[0]["bndLoopRoot"];
                Json::Value vIslRoot = pairRoot[0]["vIslLoopRoot"];
                DefLoop bndLoop;
                for(int i = 0;i < bndRoot.size();i ++){
                    DefElem ele;
                    datedi_tpjso->TranslateJsonToEle(bndRoot[i],ele);
                    bndLoop.m_vElem.push_back(ele);
                }
                vector<DefLoop>vIslLoop;
                Json::Value vLoopRoot = vIslRoot["vLoopRoot"];
                if(!vLoopRoot.isNull() && vLoopRoot.size() > 0){
                    int lo = 0;
                    while (lo < vLoopRoot.size()) {
                        Json::Value curLoop = vLoopRoot[lo]["loopData"];
                        DefLoop islLoop;
                        for(int i = 0;i < curLoop.size();i++){
                            DefElem ele;
                            datedi_tpjso->TranslateJsonToEle(curLoop[i],ele);
                            islLoop.m_vElem.push_back(ele);
                        }
                        vIslLoop.push_back(islLoop);
                        lo ++;
                    }
                }
                BndIslPair loopPair(bndLoop,vIslLoop);
                vector<BndIslPair> vPartLayCut;
                vPartLayCut.push_back(loopPair);
                PartLayCut layData(vPartLayCut);
                vLayData.push_back(layData);
                lIdx ++;
            }
            PartAreaCut areaData(vLayData);
            vAreaData.push_back(areaData);
            aIdx ++;
        }
        PartCut cutData(vAreaData);
        PartCutData partData(partName,cutData);
        vCutPartResult.push_back(partData);
        pIdx ++;
    }
    cutResult.m_vCutPartResult = vCutPartResult;
}
void DataEdit::SaveCutPairLoopData(string path,const BndIslPair& pairLoop,
                                   bool &isSucSave)
{
    isSucSave = false;
    if(pairLoop.m_bndLoop.m_vElem.empty() &&
            pairLoop.m_vIslLoop.empty()){
        return;
    }
    bool isExiPath = true;
    /*if (access(path.c_str(), F_OK) != 0){
        system(("mkdir -p " + path).c_str());
        isExiPath = false;
    }*/
    Json::Value jRoot;
    datedi_tpjso->TransBndIslLoopToJson(pairLoop,jRoot);
    string fileName = pairLoop.m_partName;
    char sep = '/';
    vector<string> vFileName;
    SlipeFile(fileName,sep,vFileName);
    int len = vFileName.size() - 1;
    if(vFileName.empty()){
        std::cout<<"error! SaveCutUiSelDataSeparately->"
              "vFileName.empty()\n";
        return;
    }
    string partName = vFileName[len];
    if(isExiPath){
        partName = path  + "/" + "prof_" + partName + ".json";
    }
    else{
        partName = "./" + path  + "/" + "prof_" + partName + ".json";
    }
    Json::FastWriter resultFile;
    string fileData = resultFile.write(jRoot);
    std::ofstream outFiles(partName);
    outFiles << fileData;
    outFiles.close();
    isSucSave = true;
}
void DataEdit::AnalyticlePairLoopData(string path,
                                      bool& isAnaSuc,
                                      BndIslPair &pairLoop)
{
    isAnaSuc = false;
    std::fstream file;
    file.open(path,std::ios::in);
    if(!file){
        return;
    }
    std::ifstream inFile(path.c_str());
    Json::Reader jreader;
    Json::Value jValue;
    if(!jreader.parse(inFile,jValue)){
        return;
    }
    datedi_tpjso->TransJsonToBndIslLoop(jValue,pairLoop);
    vector<double>vOffsetVal;
    Json::Value pairInfo = jValue["apairInfo"];
    vOffsetVal.push_back(pairInfo["offsetVal0"].asDouble());
    vOffsetVal.push_back(pairInfo["offsetVal1"].asDouble());
    vOffsetVal.push_back(pairInfo["offsetVal2"].asDouble());
    pairLoop._toolRadius = pairInfo["toolRadius"].asDouble();
    pairLoop.m_vOffsetVal = vOffsetVal;
    isAnaSuc = true;
}
void DataEdit::GetRootNodeLoopNum(const shared_ptr<OffsetNode>&rootNode,
                                  int &num)
{
    num = rootNode->_loops.size();
    for(int i = 0;i < rootNode->_nodes.size();i++){
        int num_1 = 0;
        GetRootNodeLoopNum(rootNode->_nodes[i],num_1);
        num = num + num_1;
    }
}
void DataEdit::SaveToolPathNode(string path,string partName,
                                OffsetParam& offsetPar,
                                const BndIslPair&oriPairLoop,
                                const shared_ptr<OffsetNode>&rootNode,
                                bool& isSucSave)
{
    isSucSave = false;
    if(rootNode == nullptr){
        return;
    }
    int num = 0;
    Json::Value nodeRoot,a_nodeRootInfo,nodeData,pairLoopRoot;
    GetRootNodeLoopNum(rootNode,num);
    a_nodeRootInfo["partName"] = partName;
    a_nodeRootInfo["loopNum"] = num;
    a_nodeRootInfo["toolRadius"] = offsetPar._offsetUnit.ToolRadius();
    a_nodeRootInfo["firOffsetVal"] = offsetPar._offsetUnit.FirstOftVal();
    a_nodeRootInfo["offsetVal"] = offsetPar._offsetUnit.OffsetValue();
    datedi_tpjso->TransBndIslLoopToJson(oriPairLoop,pairLoopRoot);
    datedi_tpjso->TransToolPathNodeToJson(rootNode,nodeData);
    nodeRoot["a_nodeRootInfo"] = a_nodeRootInfo;
    nodeRoot["pairLoopRoot"] = pairLoopRoot;
    nodeRoot["nodeData"] = nodeData;
    Json::FastWriter resultFile;
    string fileData = resultFile.write(nodeRoot);
    std::ofstream outFiles(path);
    outFiles << fileData;
    outFiles.close();
    isSucSave = true;
}
void DataEdit::AnalyticleToolPathJsonNode(string path,string& partName,
                                          double& firOffsetVal,
                                          double& offsetVal,
                                          bool& isAnaSuc,
                                          BndIslPair& pairLoop,
                                          shared_ptr<OffsetNode>&rootNode)
{
    isAnaSuc = false;
    /*if (access(path.c_str(), F_OK) != 0){
        return;
    }*/
    if(rootNode == nullptr){
        rootNode = make_shared<OffsetNode>();
    }
    std::fstream file;
    file.open(path,std::ios::in);
    if(!file){
        return;
    }
    std::ifstream inFile(path.c_str());
    Json::Reader jreader;
    Json::Value jValue;
    if(!jreader.parse(inFile,jValue)){
        return;
    }
    Json::Value pairLoopRoot = jValue["pairLoopRoot"];
    datedi_tpjso->TransJsonToBndIslLoop(pairLoopRoot,pairLoop);
    Json::Value nodeRootInfo = jValue["a_nodeRootInfo"];
    firOffsetVal = nodeRootInfo["firOffsetVal"].asDouble();
    offsetVal = nodeRootInfo["offsetVal"].asDouble();
    partName = nodeRootInfo["partName"].asString();
    Json::Value nodeRoot = jValue["nodeData"];
    datedi_tpjso->TransJsonToToolPathNode(nodeRoot,rootNode);
    isAnaSuc = true;
}