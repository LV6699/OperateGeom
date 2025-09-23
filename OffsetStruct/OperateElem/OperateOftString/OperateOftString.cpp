#include<sstream>
#include<iostream>
#include <iomanip>
#include <fstream>
#include "OperateOftString.h"
#pragma optimize("", off)
void OperateOftString::SetNumPrecision(int pre,double& num)
{
    double data;
    stringstream ss;
    ss <<fixed <<setprecision(pre) << num;
    ss >> data;
    num = data;
}
void OperateOftString::EnumSwitchString(RotDir type,string& str)
{
    if(type == CLOCKWISE){str = "CLOCKWISE";}
    else{str = "ANTICLOCKWISE";}
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
        str = "DefSeg l" + to_string(i) + loIDStr +" {{"                + startX_str + "," + startY_str + "," + startZ_str + "},{"
                + endX_str + "," + endY_str+"," + endZ_str+"}};\n";
        str += loopStr + ".m_vElem.push_back(DefElem(LINETYPE, l"
                + to_string(i) + loIDStr +"));\n";
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
        EnumSwitchString(arc._arcRot,str_);
        str = "DefArc arc" + to_string(i) + loIDStr + " {{"
                + startX_str + "," + startY_str + "," + startZ_str+ "},{"
                + endX_str + "," + endY_str+ "," + endZ_str + "},{"
                + cdtX_str + "," + cdtY_str+ "," + cdtZ_str + "}" + ","+ str_ + 
"};\n";
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
        str = "DefCircle circle" + loIDStr + "{{"
                + strX + ","+ strY + "," + strZ + "}"
                + "," + strR + "};\n";
        str += loopStr +".m_vElem.push_back(DefElem(CIRCLETYPE,"
                + "circle" + loIDStr + "));\n";
        break;
    }
    }
    return str;}
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
        break;}    case ARCTYPE:{
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
        EnumSwitchString(arc._arcRot,str);
        std::cout <<"DefArc arc"<<to_string(i) << loIDStr <<" {{"
                 << startX_str << ","<<startY_str << ","<<startZ_str<< "},{"
                 << endX_str << ","<<endY_str<< ","<<endZ_str << "},{"
                 << cdtX_str << ","<<cdtY_str<< ","<<cdtZ_str << "}" << ","<< 
str << "};" << endl;
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
    DefArc arc;    DefSeg segment;
    DefCircle circle;
    string str = "\n";
    for (int i = 0; i < loop.ElemNum(); ++i) {
        switch (loop.IndexElem(i).ElemeType()) {
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
            + std::to_string(index) + " loop end.***************\n\n";
    return str;
}
void OperateOftString::PrintLoopData(const DefLoop& loop,bool isBoundary,int 
index)
{
    DefArc arc;
    DefSeg segment;
    DefCircle circle;
    cout<<"\n";
    for (int i = 0; i < loop.ElemNum(); ++i) {
        switch (loop.IndexElem(i).ElemeType()) {
        case LINETYPE:{
            PrintElemInfo(isBoundary,index,i,LINETYPE,
                          loop.m_vElem[i]._segment,arc,circle);
            break;
        }
        case ARCTYPE:{
            string str;
            EnumSwitchString(loop.m_vElem[i]._arc._arcRot,str);
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
    RotDir r = CLOCKWISE;
    DefArc arc1(S,E,C,r);    PrintElemInfo(isBoundary,index,0,ARCTYPE,segment,arc1,circle);
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