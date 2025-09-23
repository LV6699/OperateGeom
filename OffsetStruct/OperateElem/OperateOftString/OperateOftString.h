#ifndef OPERATEOFTSTRING_H
#define OPERATEOFTSTRING_H
#include<string>
#include"../../CommonFile/Header.h"
class OperateOftString
{
public:
    OperateOftString(){}
    void SetNumPrecision(int pre,double& num);
    void EnumSwitchString(RotDir type, std::string& str);
    string ElementString(const bool isBoundary,
                         const int loIndex,
                         const int i,
                         const ElemType& type,
                         const DefSeg& segment,
                         const DefArc& arc,
                         const DefCircle& circle);
    void WriteStringToFile(const string& str,bool isAppend = false,
                           string path = "./",string file = "Data.txt");
    void PrintElemInfo(const bool isBoundary,
                       const int loIndex,
                       const int i,
                       const ElemType& type,
                       const DefSeg& segment,
                       const DefArc& arc,
                       const DefCircle& circle);
    string LoopElementString(const DefLoop& loop,bool isBoundary = false,
                             int index = 0,string path = "./",
                             string file = "Data.txt");
    void PrintLoopData(const DefLoop& loop,bool isBoundary = false,
                       int index = 0);
    string BndIslLoopString(const GeomArea& data,
                            string path = "./",string file = "Data.txt");
    void PrintBndIslLoop(const GeomArea& data);
    void PrintArrayLoop(const vector<DefLoop>& vLoop);
    string ArrayLoopString(const vector<DefLoop>& vLoop,
                           string path = "./",string file = "Data.txt");
    string LoopDataString(const DefLoop& loop);
    void PathNodeString(const std::shared_ptr<OffsetNode>& pathNode,
                        int& n,string &str );
};
#endif // OPERATEOFTSTRING_H