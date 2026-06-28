#include "BatchCutSolve.h"
#include"../TextOrganize/DataEdit.h"
#include"../ProgramCall/CreatePathCall.h"
shared_ptr<ElemCalculate>batsol_num = make_shared<ElemCalculate>();
shared_ptr<GeomCalculate>batsol_go = make_shared<GeomCalculate>();
shared_ptr<ExicuteProgram>batsol_pro = make_shared<ExicuteProgram>();
shared_ptr<DataEdit>batsol_datEdi = make_shared<DataEdit>();
shared_ptr<CreatePathCall>batsol_exiAlg = make_shared<CreatePathCall>();
shared_ptr<HandleText>batsol_text = make_shared<HandleText>();
void BatchCutSolve::CreateSpecifyDirectory(string dir,bool isRemove)
{/*
    if(isRemove){
        if (access(dir.c_str(), F_OK) == 0){
            system(("rm " + dir).c_str());
        }
    }
    if (access(dir.c_str(), F_OK) != 0){
        dir = "./" + dir;
        system(("mkdir -p " + dir).c_str());
    }*/
}
void BatchCutSolve::CreateCutModelDirectory()
{
    string saveDir = "./batchContour",cutFailureDir = "./cutFailure",
            hasCutModDir = "./hasCutModel";
    CreateSpecifyDirectory(saveDir,false);
    CreateSpecifyDirectory(cutFailureDir,false);
    CreateSpecifyDirectory(hasCutModDir,false);
}
void BatchCutSolve::GetBndIslContourWeight(BndIslPair& bndIslLoop)
{
    double lenWeight = 0.35,numWeight = 0.65,islNum = 0.15;
    int eleNum = bndIslLoop.m_bndLoop.m_vElem.size();
    for (int lo = 0; lo < bndIslLoop.m_vIslLoop.size(); ++lo) {
        eleNum += bndIslLoop.m_vIslLoop[lo].m_vElem.size();
    }
    double bndLenth = 0,islLenth = 0;
    batsol_num->LoopGeometryLenth(bndIslLoop.m_bndLoop,bndLenth);
    for (int lo = 0; lo < bndIslLoop.m_vIslLoop.size(); ++lo) {
        double lenth = 0;
        batsol_num->LoopGeometryLenth(bndIslLoop.m_vIslLoop[lo],lenth);
        islLenth += lenth;
    }
    double lenth = bndLenth + islLenth;
    double lenWei = lenth * lenWeight;
    double numWei = eleNum * numWeight;
    double islNumWei = bndIslLoop.m_vIslLoop.size() * islNum;
    double weight = lenWei + numWei;
    weight = weight * (1 + islNumWei);
    bndIslLoop.m_eleNum = eleNum;
    bndIslLoop.m_lenth = lenth;
    bndIslLoop.m_weight = weight;
}
void BatchCutSolve::SetBndIslMatchOffsetVal(BndIslPair& biLoop)
{
    double minX,maxX,minY, maxY;/**
    batsol_limcoo->LoopLimitCoordinate(biLoop.m_bndLoop,true,false,minX);
    batsol_limcoo->LoopLimitCoordinate(biLoop.m_bndLoop,true,true,maxX);
    batsol_limcoo->LoopLimitCoordinate(biLoop.m_bndLoop,false,false,minY);
    batsol_limcoo->LoopLimitCoordinate(biLoop.m_bndLoop,false,true,maxY);
    */double toolRadius;
    vector<double> vOffsetVal;
    bool isValScale = false;
    GetOffsetValBaseScale(minX,maxX,minY,maxY,toolRadius,
                          isValScale,vOffsetVal);
    biLoop._toolRadius = toolRadius;
    biLoop.m_vOffsetVal = vOffsetVal;
    biLoop.m_isValScale = isValScale;
}
void BatchCutSolve::GetOffsetValBaseScale(double minX, double maxX, double minY,
                                          double maxY, double &toolRadius,
                                          bool& isValScale,
                                          vector<double>& vOffsetVal)
{
    isValScale = false;
    double xScale = std::abs(maxX - minX);
    double yScale = std::abs(maxY - minY);
    double scale = (xScale * 2 + yScale * 2) / 4;
    double val0,val1,val2;
    if(scale < 25)
        return;
    else if(scale <= 40){
        val0 = 0.8;val1 = 1.5;val2 = 2;}
    else if(40 < scale && scale <= 60){
        val0 = 1;val1 = 2;val2 = 3;}
    else if(60 < scale &&  scale <= 80){
        val0 = 2;val1 = 3;val2 = 4;}
    else if(80 < scale &&  scale <= 100){
        val0 = 3;val1 = 4;val2 = 5;}
    else if(100 < scale && scale <= 130){
        val0 = 4;val1 = 5;val2 = 6;}
    else if(130 < scale && scale <= 160){
        val0 = 5;val1 = 7;val2 = 8;}
    else if(160 < scale && scale <= 180){
        val0 = 6;val1 = 8;val2 = 9;}
    else if(180 < scale && scale <= 200){
        val0 = 8;val1 = 9;val2 = 11;}
    else if(200 < scale && scale < 230){
        val0 = 9;val1 = 10;val2 = 13;}
    else{
        val0 = 9;val1 = 11;val2 = 14;}
    toolRadius = (val1 / OftValDiamMultiplier) / 2;
    vOffsetVal.push_back(val0);
    vOffsetVal.push_back(val1);
    vOffsetVal.push_back(val2);
    isValScale = true;
}
bool BatchCutSolve::IsMatchLoopSimilarProperty(const BndIslPair& biLoop1,
                                               const BndIslPair& biLoop2)
{
    if(std::abs(biLoop1.m_weight - biLoop2.m_weight) < PreErr5_1){
        return true;
    }
    if(std::abs(biLoop1.m_eleNum - biLoop2.m_eleNum) < 3){
        return true;
    }
    if(std::abs(biLoop1.m_lenth - biLoop2.m_lenth) < PreErr_10_){
        return true;
    }
    return false;
}