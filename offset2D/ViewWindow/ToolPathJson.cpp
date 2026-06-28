#include "ToolPathJson.h"
#include <sys/stat.h>
//#include <unistd.h>
#include <string>
#include <fstream>
void ToolPathJson::TranslateEleToJson(const DefElem& ele,int i,
                                      Json::Value& eleVal)
{
    if(ele._type == LINETYPE){
        Point lSp = ele._segment._segSp;
        Point lEp = ele._segment._segEp;
        eleVal["idx"] = i;
        eleVal["eleType"] = "LINETYPE";
        eleVal["lSp_x"] = lSp._x;
        eleVal["lSp_y"] = lSp._y;
        eleVal["lSp_z"] = lSp._z;
        eleVal["lEp_x"] = lEp._x;
        eleVal["lEp_y"] = lEp._y;
        eleVal["lEp_z"] = lSp._z;
    }
    else if(ele._type == ARCTYPE){
        Point arcSp = ele._arc._arcSp;
        Point arcEp = ele._arc._arcEp;
        Point arcCp = ele._arc._arcCp;
        bool isCW = ele._arc._isCW;
        double R = ele._arc._arcR;
        eleVal["idx"] = i;
        eleVal["eleType"] = "ARCTYPE";
        eleVal["arcSp_x"] = arcSp._x;
        eleVal["arcSp_y"] = arcSp._y;
        eleVal["arcSp_z"] = arcSp._z;
        eleVal["arcEp_x"] = arcEp._x;
        eleVal["arcEp_y"] = arcEp._y;
        eleVal["arcEp_z"] = arcEp._z;
        eleVal["arcCp_x"] = arcCp._x;
        eleVal["arcCp_y"] = arcCp._y;
        eleVal["arcCp_z"] = arcCp._z;
        eleVal["isCW"] = isCW;
        eleVal["R"] = R;
    }
    else if(ele._type == CIRCLETYPE){
        eleVal["idx"] = i;
        eleVal["eleType"] = "CIRCLETYPE";
        Point centerPnt = ele._circle._cirCp;
        eleVal["center_x"] = centerPnt._x;
        eleVal["center_y"] = centerPnt._y;
        eleVal["center_z"] = centerPnt._z;
        eleVal["R"] = ele._circle._cirR;
    }
}
void ToolPathJson::TranslateJsonToEle(const Json::Value& eleVal,
                                      DefElem& ele)
{
    string eleType = eleVal["eleType"].asString();
    if(eleType == "LINETYPE"){
        Point lSp,lEp;
        lSp._x = eleVal["lSp_x"].asDouble();
        lSp._y = eleVal["lSp_y"].asDouble();
        lSp._z = eleVal["lSp_z"].asDouble();
        lEp._x = eleVal["lEp_x"].asDouble();
        lEp._y = eleVal["lEp_y"].asDouble();
        lEp._z = eleVal["lEp_z"].asDouble();
        ele._segment._segSp = lSp;
        ele._segment._segEp = lEp;
        ele._type = LINETYPE;
    }
    else if(eleType == "ARCTYPE"){
        Point arcSp,arcEp,arcCp;
        arcSp._x = eleVal["arcSp_x"].asDouble();
        arcSp._y = eleVal["arcSp_y"].asDouble();
        arcSp._z = eleVal["arcSp_z"].asDouble();
        arcEp._x = eleVal["arcEp_x"].asDouble();
        arcEp._y = eleVal["arcEp_y"].asDouble();
        arcEp._z = eleVal["arcEp_z"].asDouble();
        arcCp._x = eleVal["arcCp_x"].asDouble();
        arcCp._y = eleVal["arcCp_y"].asDouble();
        arcCp._z = eleVal["arcCp_z"].asDouble();
        bool isCW = eleVal["isCW"].asBool();
        bool iscw1 = isCW;
        double R = eleVal["R"].asDouble();
        DefArc arc(arcSp,arcEp,arcCp,R,iscw1);
        ele._type = ARCTYPE;
        ele._arc = arc;
    }
    else if(eleType == "CIRCLETYPE"){
        Point centerPnt;
        centerPnt._x = eleVal["center_x"].asDouble();
        centerPnt._y = eleVal["center_y"].asDouble();
        centerPnt._z = eleVal["center_z"].asDouble();
        double R = eleVal["R"].asDouble();
        DefCircle cir(centerPnt,R);
        ele._circle = cir;
        ele._type = CIRCLETYPE;
    }
}
void ToolPathJson::TransBndIslLoopToJson(const BndIslPair& pairLoop,
                                         Json::Value& jRoot)
{
    Json::Value pairInfo;
    int offsetValNum = 3;
    if(offsetValNum > pairLoop.m_vOffsetVal.size()){
        offsetValNum = pairLoop.m_vOffsetVal.size();
    }
    for(int i = 0;i < offsetValNum;i++){
        string str = std::to_string(i);
        str = "offsetVal" + str;
        pairInfo[str] = pairLoop.m_vOffsetVal[i];
        if(i == 2){
            break;
        }
    }
    pairInfo["numOfIslLoop"] = pairLoop.m_vIslLoop.size();
    pairInfo["toolRadius"] = pairLoop._toolRadius;
    Json::Value bndLoopRoot,vIslLoopRoot;
    DefLoop bndLoop = pairLoop.m_bndLoop;
    for(int i = 0;i < bndLoop.m_vElem.size();i++){
        Json::Value bndEle;
        TranslateEleToJson(bndLoop.m_vElem[i],i,bndEle);
        bndLoopRoot.append(bndEle);
    }
    vector<DefLoop>vIslLoop = pairLoop.m_vIslLoop;
    int lo = 0;
    while (lo < vIslLoop.size()) {
        DefLoop curLoop = vIslLoop[lo];
        Json::Value curLoopRoot,loopData,islLoopInfo;
        islLoopInfo["loopIdx"] = lo;
        islLoopInfo["eleNum"] = curLoop.m_vElem.size();
        curLoopRoot["loopInfo"] = islLoopInfo;
        for(int e = 0;e < curLoop.m_vElem.size();e ++){
            Json::Value islEle;
            TranslateEleToJson(curLoop.m_vElem[e],e,islEle);
            loopData.append(islEle);
        }
        curLoopRoot["loopData"] = loopData;
        vIslLoopRoot.append(curLoopRoot);
        lo ++;
    }
    jRoot["apairInfo"] = pairInfo;
    jRoot["bndRoot"] = bndLoopRoot;
    jRoot["vIslRoot"] = vIslLoopRoot;
}
void ToolPathJson::TransJsonToBndIslLoop(const Json::Value &jValue,
                                         BndIslPair &pairLoop)
{
    DefLoop bndLoop;
    Json::Value bndRoot = jValue["bndRoot"];
    for(int i = 0;i < bndRoot.size();i++){
        Json::Value jCurEle = bndRoot[i];
        DefElem curEle;
        TranslateJsonToEle(jCurEle,curEle);
        bndLoop.m_vElem.push_back(curEle);
    }
    vector<DefLoop>vIslLoop;
    Json::Value vIslRoot = jValue["vIslRoot"];
    for(int lo = 0;lo < vIslRoot.size();lo++){
        Json::Value jCurLoop = vIslRoot[lo]["loopData"];
        DefLoop islLoop(true);
        for(int i = 0;i < jCurLoop.size();i++){
            DefElem curEle;
            TranslateJsonToEle(jCurLoop[i],curEle);
            islLoop.m_vElem.push_back(curEle);
        }
        vIslLoop.push_back(islLoop);
    }
    pairLoop.m_bndLoop = bndLoop;
    pairLoop.m_vIslLoop = vIslLoop;
}
void ToolPathJson::TransJsonFileToBndIslLoop(string path,
                                             bool& isAnaSuc,
                                             BndIslPair& pairLoop)
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
    TransJsonToBndIslLoop(jValue,pairLoop);
    vector<double>vOffsetVal;
    Json::Value pairInfo = jValue["apairInfo"];
    vOffsetVal.push_back(pairInfo["offsetVal0"].asDouble());
    vOffsetVal.push_back(pairInfo["offsetVal1"].asDouble());
    vOffsetVal.push_back(pairInfo["offsetVal2"].asDouble());
    pairLoop.m_vOffsetVal = vOffsetVal;
    isAnaSuc = true;
}
void ToolPathJson::TransTPTypeToString(ToolPathType type,
                                       string& str)
{
    switch (type) {
    case BndOffsetPath:
        str = "BndOffsetPath";
        break;
    case BndOftLocalPath:
        str = "BndOftLocalPath";
        break;
    case IslOffsetPath:
        str = "IslOffsetPath";
        break;
    case IslOftLocalPath:
        str = "IslOftLocalPath";
        break;
    case BndIslMerPath:
        str = "BndIslMerPath";
        break;
    case IslIslMerPath:
        str = "IslIslMerPath";
        break;
    default:
        break;
    }
}
void ToolPathJson::TransStringToTPType(string str,
                                       ToolPathType& type)
{
    if(str == "BndOffsetPath"){
        type = BndOffsetPath;
    }
    else if(str == "BndOftLocalPath"){
        type = BndOftLocalPath;
    }
    else if(str == "IslOffsetPath"){
        type = IslOffsetPath;
    }
    else if(str == "IslOftLocalPath"){
        type = IslOftLocalPath;
    }
    else if(str == "BndIslMerPath"){
        type = BndIslMerPath;
    }
    else if(str == "IslIslMerPath"){
        type = IslIslMerPath;
    }
}
void ToolPathJson::TransToolPathNodeToJson(const shared_ptr<OffsetNode>&rootNode,
                                           Json::Value& nodeRoot)
{
    if(rootNode == nullptr){
        return;
    }
    Json::Value vLoopRoot;
    vector<DefLoop>vLoop = rootNode->_loops;
    if(!vLoop.empty()){
        string tpStr;
        TransTPTypeToString(rootNode->_geneType,tpStr);
        for (int lo = 0; lo < vLoop.size(); ++lo) {
            DefLoop curLoop = vLoop[lo];
            Json::Value jLoopRoot,jLoop,jLoopInfo;
            jLoopInfo["loopIdx"] = lo;
            jLoopInfo["eleNum"] = curLoop.m_vElem.size();
            jLoopRoot["loopInfo"] = jLoopInfo;
            jLoopRoot["hasOffsetNum"] = curLoop.PastOftNum();
            for(int e = 0;e < curLoop.m_vElem.size();e ++){
                Json::Value islEle;
                TranslateEleToJson(curLoop.m_vElem[e],e,islEle);
                jLoop.append(islEle);
            }
            jLoopRoot["loopData"] = jLoop;
            vLoopRoot.append(jLoopRoot);
        }
        nodeRoot["vLoopRoot"] = vLoopRoot;
        nodeRoot["toolPathType"] = tpStr;
    }
    for(int i = 0;i < rootNode->_nodes.size();i ++){
        if(rootNode->_nodes[i]->_loops.empty() &&
                rootNode->_nodes[i]->_nodes.empty()){
            continue;
        }
        Json::Value curNodeRoot;
        TransToolPathNodeToJson(rootNode->_nodes[i],curNodeRoot);
        nodeRoot["nodeRoot"].append(curNodeRoot);
    }
}
void ToolPathJson::TransJsonToToolPathNode(const Json::Value& nodeRoot,
                                           shared_ptr<OffsetNode>&rootNode)
{
    Json::Value vLoopRoot = nodeRoot["vLoopRoot"];
    string tpTypeStr = nodeRoot["toolPathType"].asString();
    if(!nodeRoot["vLoopRoot"].empty()){
        ToolPathType tpType;
        TransStringToTPType(tpTypeStr,tpType);
        vector<DefLoop>vLoop;
        for(int i = 0;i < vLoopRoot.size();i ++){
            Json::Value jLoop = vLoopRoot[i]["loopData"];
            DefLoop loop(true);
            for(int i = 0;i < jLoop.size();i++){
                DefElem curEle;
                TranslateJsonToEle(jLoop[i],curEle);
                loop.m_vElem.push_back(curEle);
            }
            if(loop.m_vElem.empty()){
                continue;
            }
            //loop._pastOftNum = jLoop["hasOffsetNum"].asInt();
            vLoop.push_back(loop);
        }
        rootNode->_geneType = tpType;
        rootNode->_loops = vLoop;
    }
    Json::Value nodeRoot_ = nodeRoot["nodeRoot"];
    for(int i = 0;i < nodeRoot_.size();i ++){
        shared_ptr<OffsetNode>rootNode_ = make_shared<OffsetNode>();
        ///Json::Value curNodeRoot = nodeRoot_1[i]["nodeRoot"];
        Json::Value curNodeRoot = nodeRoot_[i];
        TransJsonToToolPathNode(curNodeRoot,rootNode_);
        rootNode->_nodes.push_back(rootNode_);
    }
}