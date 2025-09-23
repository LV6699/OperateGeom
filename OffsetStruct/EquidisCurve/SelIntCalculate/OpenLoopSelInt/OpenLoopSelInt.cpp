#include "OpenLoopSelInt.h"
void OpenLoopSelInt::SpecifySelIntEleValidCutPart(const DefLoop& oriLoop,
                                                  const DefElem& ele,
                                                  double offsetVal,
                                                  const Point &P,
                                                  bool& isCurEleVal,
                                                  bool& isNexEleVal,
                                                  DefElem &valCutEle)
{
    ElemCalculate numcal;
    GeomCalculate gocal;
    OpenSelIntSolve opecal;
    DefElem curEle,nexEle;
    numcal.PointSplitElement(ele,P,true,curEle);
    numcal.PointSplitElement(ele,P,false,nexEle);
    double lenth1 = gocal.ElementLenth(curEle);
    double lenth2 = gocal.ElementLenth(nexEle);
    if(lenth1 > PreErr5_10){
        opecal.GetCutElementValidPart(oriLoop,curEle,true,
                                      offsetVal,isCurEleVal,
                                      valCutEle);
    }
    if(lenth2 > PreErr5_10){
        opecal.GetCutElementValidPart(oriLoop,nexEle,false,
                                      offsetVal,isNexEleVal,
                                      valCutEle);
    }
}
///获取ele有效的裁剪部分,且valPart是开放环的两端
void OpenLoopSelInt::HandleOpenLoopInvalidEle(const DefLoop& oriLoop,
                                              const DefElem& ele,
                                              double offsetVal,
                                              bool& isLocVal,
                                              DefElem& valPart)
{
    isLocVal = false;
    vector<Point>vPoint;
    vector<DefLoop>vEnvLoop;
    OpenSelIntSolve opesol;
    GeomDistance godis;
    opesol.ShortDistLoopElemEnvelope(oriLoop,ele,offsetVal,vEnvLoop);
    opesol.ElemEnvelopeIntPoint(oriLoop,ele,vEnvLoop,offsetVal,vPoint);
    valPart = ele;
    for (int i = 0; i < vPoint.size(); ++i) {
        bool isCurEleVal = false,isNexEleVal = false;
        SpecifySelIntEleValidCutPart(oriLoop,valPart,offsetVal,vPoint[i],
                                     isCurEleVal,isNexEleVal,valPart);
        if(isCurEleVal || isNexEleVal){
            double distance;
            godis.ElementToLoopDistance(oriLoop,valPart,distance);
            if(distance + SeIn_Dis_Err >= offsetVal){
                isLocVal = true;
            }
        }
    }
    if(ele._elemType == ARCTYPE && isLocVal){
        opesol.IsConnectArcSameOftDirection(oriLoop,ele._arc,
                                            offsetVal,isLocVal);
    }
}
bool OpenLoopSelInt::IsIntEleExistValCutPart(const DefLoop &oriLoop,
                                             const DefLoop &loop,                                             const DefElem &ele,
                                             double offsetVal,
                                             const Point &P,
                                             bool &isCurCutVal,
                                             DefElem &valCutEle)
{
    if(!loop.IsOpenLoop())
        return false;
    bool isCurEleVal = false,isNexEleVal = false;
    SpecifySelIntEleValidCutPart(oriLoop,ele,offsetVal,P,isCurEleVal,
                                 isNexEleVal,valCutEle);
    if(!isCurEleVal && !isNexEleVal){
        return false;
    }
    else if(isCurEleVal && !isNexEleVal){
        isCurCutVal = true;
    }
    else {
        isCurCutVal = false;
    }
    bool isExiValCut = false;
    HandleOpenLoopInvalidEle(oriLoop,valCutEle,offsetVal,
                             isExiValCut,valCutEle);
    return isExiValCut;
}