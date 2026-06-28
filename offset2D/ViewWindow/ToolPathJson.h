#ifndef TOOLPATHJSON_H
#define TOOLPATHJSON_H
#include"HearderFile.h"
class ToolPathJson
{
public:
    ToolPathJson(){}
    void TranslateEleToJson(const DefElem& ele, int i,
                            Json::Value& eleVal);
    void TranslateJsonToEle(const Json::Value& eleVal,
                            DefElem& ele);
    void TransBndIslLoopToJson(const BndIslPair& pairLoop,
                               Json::Value& jRoot);
    void TransJsonToBndIslLoop(const Json::Value &jValue,
                               BndIslPair &pairLoop);
    void TransJsonFileToBndIslLoop(string path, bool& isAnaSuc,
                                   BndIslPair &pairLoop);
    void TransTPTypeToString(ToolPathType type, string &str);
    void TransStringToTPType(string str,
                             ToolPathType& type);
    void TransToolPathNodeToJson(const shared_ptr<OffsetNode>&rootNode,
                                 Json::Value& nodeRoot);
    void TransJsonToToolPathNode(const Json::Value& nodeRoot,
                                 shared_ptr<OffsetNode>&rootNode);
};
#endif // TOOLPATHJSON_H