#include "LoopOftPrecision.h"
#if DebugModel
#pragma optimize("", off)
#endif
using namespace OffsetResCheck;
void LoopOftPrecision::LoopCheckStruct(const DefLoop &oriLoop,
                                       const DefLoop &loop,
                                       bool isStepAngle,
                                       double segStep,
                                       double arcStep,
                                       CheckLoop &checkLoop)
{
    GeomDistance godis;GeomCalculate gocal;
    for (int i = 0; i < loop.ElemNum(); ++i) {
        vector<Point>vPoint;
        DefElem ele = loop.IndexElem(i);
        gocal.TranslateElemToPoint(ele,isStepAngle,segStep,
                                   arcStep,vPoint);
        if(vPoint.empty())
            continue;
        vector<CheckPoint>vChePnt;
        ///ele上每一点到oriLoop距离.
        for (int j = 0; j < vPoint.size(); ++j) {
            vector<double>vDis;
            /// 每一点到oriLoop距离.
            for (int e = 0; e < oriLoop.ElemNum(); ++e) {
                double d;
                godis.PointToElementDistance(vPoint[j],oriLoop.IndexElem(e),d);
                vDis.push_back(d);
                if(std::abs(d - checkLoop._refValue) > PreErr5_3){
                    int tem = 1;
                }
            }
            auto iter = std::min_element(vDis.begin(),vDis.end());
            int id = static_cast<int>(std::distance(vDis.begin(), iter));
            double distance = vDis[id];
            if(std::abs(distance - checkLoop._refValue) > PreErr5_3){
                int tem = 1;
            }
            vChePnt.push_back(CheckPoint(vPoint[j],j,i,distance));
        }
        vector<double>vDistance_;
        for(auto& d : vChePnt){
            vDistance_.push_back(d._distance);
            checkLoop._vDistance.push_back(d._distance);
        }
        checkLoop._vCheckElem.push_back(
                    CheckElem(ele,i,vChePnt,vDistance_));
        checkLoop.isExistDis = true;
    }
    if(checkLoop._vDistance.empty())
        return;
    checkLoop._loop = loop;
    vector<double>vDistance = checkLoop._vDistance;
    auto maxIter = std::max_element(vDistance.begin(),vDistance.end());
    auto minIter = std::min_element(vDistance.begin(),vDistance.end());
    int maxId = static_cast<int>(std::distance(vDistance.begin(),maxIter));
    int minId = static_cast<int>(std::distance(vDistance.begin(),minIter));
    double maxVal = vDistance[maxId];    double minVal = vDistance[minId];
    checkLoop._maxDistance = maxVal;
    checkLoop._minDistance = minVal;
}
#if DebugModel
#include"../../../../../ViewWindow/DisplayShape/DisplayToolPath.h"
#endif
void LoopOftPrecision::ResultCheckStruct(const DefLoop &oriLoop,
                                         const vector<DefLoop> &vLoop,
                                         double refValue,
                                         double &error)
{
    if (oriLoop.IsEmpty() || vLoop.empty()) {
        error = 0;
        return;
    }
    vector<CheckLoop>vCheckLoop;
    for(const auto &l : vLoop){
        CheckLoop cheLoop;
        cheLoop._refValue = refValue;
        LoopCheckStruct(oriLoop,l,false,PreErr5_3,PreErr5_3,cheLoop);
        vCheckLoop.push_back(cheLoop);
    }
    error = 0;
    for (int i = 0; i < vCheckLoop.size(); ++i) {
        AnalysisCheckLoop(vCheckLoop[i],true);
        AnalysisCheckLoop(vCheckLoop[i],false);
        double err = 0;
        LoopCheckDistanceError(vCheckLoop[i],refValue,err);
        error = err > error ? err : error;
    }
    vector<Point>vPoint;
    vector<DefElem>vComEle;
    for (int i = 0; i < vCheckLoop.size(); ++i) {
        CheckLoop loop = vCheckLoop[i];
        for (int j = 0; j < loop._vMaxDisEle.size(); ++j) {
            CheckElem ele = loop._vMaxDisEle[j];
            for (int p = 0; p < ele._vCheckPnt.size(); ++p) {
                vPoint.push_back(ele._vCheckPnt[p]._p);
            }
            if(!ele._vCheckPnt.empty()){
                vComEle.push_back(ele._element);
            }
#if DebugModel
            DisplayToolPath().DisplayElement(ele._element);
#endif
        }
    }
#if DebugModel
    for (int i = 0; i < vPoint.size(); ++i) {
        DisplayToolPath().DisplayPoint(vPoint[i]);
    }
    for (int i = 0; i < vComEle.size(); ++i) {
        DisplayToolPath().DisplayElement(vComEle[i]);
    }
#endif
}
void LoopOftPrecision::AnalysisCheckLoop(CheckLoop& checkLoop,                                         bool isMax)
{
    if(!checkLoop.isExistDis)
        return;
    double value;
    if(isMax){
        value = checkLoop._maxDistance;
    }
    else{
        value = checkLoop._minDistance;
    }
    for (int i = 0; i < checkLoop._vCheckElem.size(); ++i) {
        CheckElem eleRes;
        CheckElem checkEle = checkLoop._vCheckElem[i];
        for (int j = 0; j < checkEle._vCheckPnt.size(); ++j) {
            if(checkEle._vCheckPnt[j]._distance == value) {
                eleRes._element = checkEle._element;
                eleRes._eleIndex = checkEle._eleIndex;
                eleRes._vDistance.push_back(value);
                eleRes._vCheckPnt.push_back(checkEle._vCheckPnt[j]);
            }
        }
        if(eleRes._vDistance.empty())
            continue;
        if(isMax){
            checkLoop._vMaxDisEle.push_back(eleRes);
        }
        else{
            checkLoop._vMinDisEle.push_back(eleRes);
        }
    }
}
void LoopOftPrecision::LoopCheckDistanceError(CheckLoop& checkLoop,
                                              double refValue,
                                              double &error)
{
    if(checkLoop._vMaxDisEle.empty() && checkLoop._vMinDisEle.empty())
        return;
    error = 0;
    if(!checkLoop._vMaxDisEle.empty()){
        error = std::abs(checkLoop._maxDistance - refValue);
    }
    if(!checkLoop._vMinDisEle.empty()){
        double value = std::abs(checkLoop._minDistance - refValue);
        if(value > error){
            error = value;
        }
    }
    checkLoop._maxErr = error;
    double v = 0;
    for(auto& d : checkLoop._vDistance){
        v += d;
    }
    double a = v / checkLoop._vDistance.size();
    checkLoop._checkDis._aevDis = a;
}