#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "OperateOftString.h"
#include <sys/stat.h>
using std::to_string;
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
///#pragma optimize("", off)
void OperateOftString::SetNumPrecision(int pre,double& num)
{
    double data;
    std::stringstream ss;
    ss <<std::fixed <<std::setprecision(pre) << num;
    ss >> data;
    num = data;
}
string OperateOftString::PointToString(const ofts::Point& p,int pre){
    std::stringstream x,y,z;
    x<<std::setprecision(pre)<<p.X();
    y<<std::setprecision(pre)<<p.Y();
    z<<std::setprecision(pre)<<p.Z();
    std::string x_ = x.str();
    std::string y_ = y.str();
    std::string z_ = z.str();
    return "ofts::Point("+x_+","+y_+","+z_+")";
}
void OperateOftString::EnumSwitchString(bool iscw,string& str)
{
    if(iscw){str = "true";}
    else{str = "false";}
}
string OperateOftString::ElementString(const bool isBoundary,
                                       const int loIndex,
                                       const int i,
                                       const ElemType& type,
                                       const DefSeg& segment,
                                       const DefArc& arc,
                                       const DefCircle& circle)
{
    string str = "";
    int pre = 18;
    string loIDStr = "",loopStr = "";
    if(isBoundary){
        loIDStr = "_";
        loopStr = "loop_";
    }else{
        loIDStr = "_" + std::to_string(loIndex);
        if(loIndex == 0){loopStr = "loop";}
        else{loopStr = "loop" + std::to_string(loIndex);}
    }
    switch (type) {
    case LINETYPE:{
        std::stringstream startX,startY,startZ,endX,endY,endZ;
        string startX_str,startY_str,startZ_str,
                endX_str,endY_str,endZ_str;
        startX << std::setprecision(pre) <<segment._segSp.X();
        startY << std::setprecision(pre) <<segment._segSp.Y();
        startZ << std::setprecision(pre) <<segment._segSp.Z();
        endX << std::setprecision(pre) <<segment._segEp.X();
        endY << std::setprecision(pre) <<segment._segEp.Y();
        endZ << std::setprecision(pre) <<segment._segEp.Z();
        startX_str = startX.str();
        startY_str = startY.str();
        startZ_str = startZ.str();
        endX_str = endX.str();
        endY_str = endY.str();
        endZ_str = endZ.str();
        str = "DefSeg l" + std::to_string(i) + loIDStr +" {{"
                + startX_str + "," + startY_str + "," + startZ_str + "},{"
                + endX_str + "," + endY_str+"," + endZ_str+"}};\n";
        str += loopStr + ".m_vElem.push_back(DefElem(LINETYPE, l"
                + std::to_string(i) + loIDStr +"));\n";
        break;}
    case ARCTYPE:{
        std::stringstream satrtX,startY,startZ,endX,
                endY,endZ,cdtX,cdtY,cdtZ;
        string startX_str,startY_str,startZ_str,endX_str,endY_str,endZ_str;
        satrtX << std::setprecision(pre) <<arc._arcSp.X();
        startY << std::setprecision(pre) <<arc._arcSp.Y();
        startZ << std::setprecision(pre) <<arc._arcSp.Z();
        endX << std::setprecision(pre) <<arc._arcEp.X();
        endY << std::setprecision(pre) <<arc._arcEp.Y();
        endZ << std::setprecision(pre) <<arc._arcEp.Z();
        cdtX << std::setprecision(pre) <<arc._arcCp.X();
        cdtY << std::setprecision(pre) <<arc._arcCp.Y();
        cdtZ << std::setprecision(pre) <<arc._arcCp.Z();
        string cdtX_str,cdtY_str,cdtZ_str,str_;
        startX_str = satrtX.str();
        startY_str = startY.str();
        startZ_str = startZ.str();
        endX_str = endX.str();
        endY_str = endY.str();
        endZ_str = endZ.str();
        cdtX_str = cdtX.str();
        cdtY_str = cdtY.str();
        cdtZ_str = cdtZ.str();
        EnumSwitchString(arc._isCW,str_);
        str = "DefArc arc" + to_string(i) + loIDStr + " {{"
                + startX_str + "," + startY_str + "," + startZ_str+ "},{"
                + endX_str + "," + endY_str+ "," + endZ_str + "},{"
                + cdtX_str + "," + cdtY_str+ "," + cdtZ_str + "}" + ","+ str_ + "};\n";
        str += loopStr +".m_vElem.push_back(DefElem(ARCTYPE, arc"
                + to_string(i) + loIDStr +"));\n";
        break;}
    default:{
        std::stringstream cdtX,cdtY,cdtZ,R;
        double radius = circle._cirR;
        cdtX << std::setprecision(pre) <<circle._cirCp.X();
        cdtY << std::setprecision(pre) <<circle._cirCp.Y();
        cdtZ << std::setprecision(pre) <<circle._cirCp.Z();
        R << std::setprecision(pre) << radius;
        string strX = cdtX.str();
        string strY = cdtY.str();
        string strZ = cdtZ.str();
        string strR = R.str();
        str = "DefCircle circle" + to_string(i) + loIDStr + "{{"
                + strX + ","+ strY + "," + strZ + "}"
                + "," + strR + "," + PointToString(circle.Normal(),pre) + "};\n";
        str += loopStr +".m_vElem.push_back(DefElem(CIRCLETYPE,"
                + "circle" + to_string(i) + loIDStr + "));\n";
        break;
    }
    }
    return str;
}
void OperateOftString::WriteStringToFile(const string& str, bool isAppend,
                                         string path, string file)
{
    string filePath = path + "/" + file;
    std::ofstream fileStream;
    if (isAppend) {
        fileStream.open(filePath, std::ios::out | std::ios::app);
    } else {
        fileStream.open(filePath, std::ios::out | std::ios::trunc);
    }
    if (!fileStream.is_open())
        return;
    fileStream << str;
    fileStream.close();
}
void OperateOftString::PrintElemInfo(const bool isBoundary,
                                     const int loIndex,
                                     const int i,
                                     const ElemType& type,
                                     const DefSeg& segment,
                                     const DefArc& arc,
                                     const DefCircle& circle)
{
    int pre = 18;
    string loIDStr = "",loopStr = "";
    if(isBoundary){
        loIDStr = "_";
        loopStr = "loop_";
    }
    else{
        loIDStr = "_" + std::to_string(loIndex);
        if(loIndex == 0){loopStr = "loop";}
        else{loopStr = "loop" + std::to_string(loIndex);}
    }
    switch (type) {
    case LINETYPE:{
        std::stringstream startX,startY,startZ,endX,endY,endZ;
        string startX_str,startY_str,startZ_str,
                endX_str,endY_str,endZ_str;
        startX << std::setprecision(pre) <<segment._segSp.X();
        startY << std::setprecision(pre) <<segment._segSp.Y();
        startZ << std::setprecision(pre) <<segment._segSp.Z();
        endX << std::setprecision(pre) <<segment._segEp.X();
        endY << std::setprecision(pre) <<segment._segEp.Y();
        endZ << std::setprecision(pre) <<segment._segEp.Z();
        startX_str = startX.str();
        startY_str = startY.str();
        startZ_str = startZ.str();
        endX_str = endX.str();
        endY_str = endY.str();
        endZ_str = endZ.str();
        std::cout <<"DefSeg l"<<to_string(i) << loIDStr <<" {{"
                 << startX_str << ","<< startY_str << ","<< startZ_str<< "},{"
                 << endX_str<<","<< endY_str<<","<< endZ_str<<"}};\n";
        std::cout<< loopStr << ".m_vElem.push_back(DefElem(LINETYPE, l"
                 << to_string(i) << loIDStr <<"));\n";
        break;}
    case ARCTYPE:{
        std::stringstream satrtX,startY,startZ,endX,
                endY,endZ,cdtX,cdtY,cdtZ;
        string startX_str,startY_str,startZ_str
                ,endX_str,endY_str,endZ_str;
        satrtX << std::setprecision(pre) <<arc._arcSp.X();
        startY << std::setprecision(pre) <<arc._arcSp.Y();
        startZ << std::setprecision(pre) <<arc._arcSp.Z();
        endX << std::setprecision(pre) <<arc._arcEp.X();
        endY << std::setprecision(pre) <<arc._arcEp.Y();
        endZ << std::setprecision(pre) <<arc._arcEp.Z();
        cdtX << std::setprecision(pre) <<arc._arcCp.X();
        cdtY << std::setprecision(pre) <<arc._arcCp.Y();
        cdtZ << std::setprecision(pre) <<arc._arcCp.Z();
        string cdtX_str,cdtY_str,cdtZ_str,str;
        startX_str = satrtX.str();
        startY_str = startY.str();
        startZ_str = startZ.str();
        endX_str = endX.str();
        endY_str = endY.str();
        endZ_str = endZ.str();
        cdtX_str = cdtX.str();
        cdtY_str = cdtY.str();
        cdtZ_str = cdtZ.str();
        EnumSwitchString(arc._isCW,str);
        std::cout <<"DefArc arc"<<to_string(i) << loIDStr <<" {{"
                 << startX_str << ","<<startY_str << ","<<startZ_str<< "},{"
                 << endX_str << ","<<endY_str<< ","<<endZ_str << "},{"
                 << cdtX_str << ","<<cdtY_str<< ","<<cdtZ_str << "}" << ","<< str << "};" << std::endl;
        std::cout<< loopStr <<".m_vElem.push_back(DefElem(ARCTYPE, arc"
                 << to_string(i) << loIDStr <<"));\n";
        break;}
    default:{
        std::stringstream cdtX,cdtY,cdtZ,R;
        double radius = circle._cirR;
        cdtX << std::setprecision(pre) <<circle._cirCp.X();
        cdtY << std::setprecision(pre) <<circle._cirCp.Y();
        cdtZ << std::setprecision(pre) <<circle._cirCp.Z();
        R << std::setprecision(pre) << radius;
        string strX = cdtX.str();
        string strY = cdtY.str();
        string strZ = cdtZ.str();
        string strR = R.str();
        std::cout << "DefCircle circle" << loIDStr << "{{"
                  << strX <<","<< strY << "," << strZ << "}"
                  << "," << strR << "};\n";
        std::cout<< loopStr <<".m_vElem.push_back(DefElem(CIRCLETYPE,"
                 << "circle" << loIDStr<<"));\n";
        break;
    }
    }
}
string OperateOftString::LoopElementString(const DefLoop& loop,bool isBoundary,
                                           int index,string path,string file)
{
    DefArc arc;
    DefSeg segment;
    DefCircle circle;
    string str = "\n";
    for (int i = 0; i < loop.Size(); ++i) {
        switch (loop.IndexElem(i).Type()) {
        case LINETYPE:{
            str += ElementString(isBoundary,index,i,LINETYPE,
                                 loop.m_vElem[i]._segment,arc,circle);
            break;
        }
        case ARCTYPE:{
            str += ElementString(isBoundary,index,i,ARCTYPE,segment,
                                 loop.m_vElem[i]._arc,circle);
            break;
        }
        default:{
            str += ElementString(isBoundary,index,i,CIRCLETYPE,segment,
                                 arc,loop.m_vElem[i]._circle);
            break;
        }
        }
    }
    str +=  "///*************** print "
            + std::to_string(index) + " loop end.***************\n";
    return str;
}
void OperateOftString::PrintLoopData(const DefLoop& loop,bool isBoundary,int index)
{
    DefArc arc;
    DefSeg segment;
    DefCircle circle;
    std::cout<<"\n";
    for (int i = 0; i < loop.Size(); ++i) {
        switch (loop.IndexElem(i).Type()) {
        case LINETYPE:{
            PrintElemInfo(isBoundary,index,i,LINETYPE,
                          loop.m_vElem[i]._segment,arc,circle);
            break;
        }
        case ARCTYPE:{
            string str;
            EnumSwitchString(loop.m_vElem[i]._arc._isCW,str);
            PrintElemInfo(isBoundary,index,i,ARCTYPE,segment,
                          loop.m_vElem[i]._arc,circle);
            break;
        }
        default:{
            PrintElemInfo(isBoundary,index,i,CIRCLETYPE,segment,
                          arc,loop.m_vElem[i]._circle);
            break;
        }
        }
    }
    std::cout << "///*************** print "
              << std::to_string(index) << " loop end.***************\n\n";
    Point S(0,0),E(0,0), C(0,0);
    bool iscw = true;
    DefArc arc1(S,E,C,iscw);
    PrintElemInfo(isBoundary,index,0,ARCTYPE,segment,arc1,circle);
}
string OperateOftString::ArrayLoopString(const vector<DefLoop>& vLoop,
                                         string path,string file)
{
    string str = "";
    for (int i = 0; i < vLoop.size(); ++i){
        string str_ = "";
        str_ = LoopElementString(vLoop[i],false,i,path,file);
        str += str_;
    }
    return str;
}
void OperateOftString::PrintArrayLoop(const vector<DefLoop>& vLoop)
{
    for (int i = 0; i < vLoop.size(); ++i)
        PrintLoopData(vLoop[i],false,i);
}
string OperateOftString::BndIslLoopString(const GeomArea& data,
                                          string path,string file)
{
    string bStr = "",iStr = "";
    bStr = LoopElementString(data.BndLoop(),true,0,path,file);
    iStr = ArrayLoopString(data.IslLoop(),path,file);
    return bStr + iStr;
}
void OperateOftString::PrintBndIslLoop(const GeomArea& data)
{
    PrintLoopData(data.BndLoop(),true,0);
    PrintArrayLoop(data.IslLoop());
}
void OperateOftString::PathNodeString(const std::shared_ptr<OffsetNode>& pathNode,
                                      int& n,string &str)
{
    for (int i = 0; i < pathNode->Loops().size(); ++i) {
        string s = LoopElementString(pathNode->IndexLoop(i),false,n);
        str += s;
        n++;
    }
    for(auto& d : pathNode->_nodes){
        PathNodeString(d,n,str);
    }
}
void OperateOftString::RegionssString(const vector<vector<GeomArea>>& regionss,
                                      string& str)
{
    auto LoopsStr = [](size_t n,std::string& s)->void{
        ///s = "\n";
        ///s += "ofts::DefLoop ";
        for(size_t i = 0;i < n;++i){
            if(i == 0){
                s += "loop";
            }else{
                s += ",loop" + std::to_string(i);
            }
        }
        ///s += ";\n";
    };
    str = "";
    ///str = "std::vector<std::vector<ofts::GeomArea>>regionss;\n";
    ///vector<vector<string>>vss;
    for(size_t i = 0;i < regionss.size();++i){
        string str_ = "if(1){\n";
        str_ += "vector<GeomArea>regions" + std::to_string(i) + ";\n";
        for(size_t j = 0;j < regionss[i].size();++j){
            const auto& region = regionss[i][j];
            const auto& bl = region._bndLoop;
            if(bl.IsEmpty()){continue;}
            const auto& ils = region._vIslLoop;
            string bStr = "",iStr = "";
            bStr = LoopElementString(bl,true,0);
            iStr = ArrayLoopString(ils);
            str_ += "if(1){\n";
            std::string ls = "";
            ls += "ofts::DefLoop loop_";
            if(!ils.empty()){ls += ",";}
            LoopsStr(ils.size(),ls);
            ls += ";\n";
            str_ += ls;
            str_ += "if(1){\n";
            str_ += bStr;
            str_ += iStr;
            str_ += "\n}\n";
            string ls_ = "";
            LoopsStr(ils.size(),ls_);
            str_ += "GeomArea region" + std::to_string(i) +
                    "(loop_,std::vector<ofts::DefLoop>{" + ls_ + "});\n";
            str_ += "regions" + std::to_string(i) + ".push_back(region" + std::to_string(i) + ");\n";
            str_ += "}\n";
        }
        str_ += "regionss.push_back(regions" + std::to_string(i) + ");\n";
        str_ += "}\n";
        str += str_;
    }
}
bool OperateOftString::SaveStringToFile(const std::string& str, const std::string& path) {
    std::ofstream file(path, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }
    file << str;
    if (file.fail()) {
        file.close();
        return false;
    }
    file.close();
    return true;
}
