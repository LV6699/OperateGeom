#include "OpenSelIntSolve.h"
void OpenSelIntSolve::LogicalJudgmentValCutEle(const DefElem& curEle,
                                               const DefElem& nexEle,
                                               double curLenth,
                                               double nexLenth,
                                               bool isCurVal,
                                               bool isNexVal,
                                               DefElem& valPart)
{
    if(isCurVal && !isNexVal){
        valPart = curEle;
    }
    else if(!isCurVal && isNexVal){
        valPart = nexEle;
    }
    else{
        valPart = curLenth >= nexLenth ? curEle : nexEle;
    }
}
void OpenSelIntSolve::ShortDistLoopElemEnvelope(const DefLoop& oriLoop,
                                                const DefElem& ele,
                                                double offsetVal,
                                                vector<DefLoop>& vEnvLoop)
{
    vector<DefElem>vElem;
    ElementEquEnve eleenv;GeomDistance godis;
    for (int i = 0; i < oriLoop.m_vElem.size(); ++i) {
        double distance;
        godis.ElementToElementDistance(ele,oriLoop.m_vElem[i],distance);
        if(distance + SeIn_Dis_Err < offsetVal){
            vElem.push_back(oriLoop.m_vElem[i]);
        }
    }
    for (int i = 0; i < vElem.size(); ++i) {
        DefLoop loop;
        eleenv.GetEleSpecifyEnvelope(vElem[i],oriLoop.LoopType(),
                                     offsetVal,loop);
        vEnvLoop.push_back(loop);
    }
}
void OpenSelIntSolve::ElemEnvelopeIntPoint(const DefLoop& oriLoop,
                                           const DefElem& ele,
                                           const vector<DefLoop>& vEnvLoop,
                                           double offsetVal,
                                           vector<Point>& vPoint)
{
    vPoint.clear();
    GeomIntersect goInt;GeomTangency gotan;GeomDistance godis;
    for (int lo = 0; lo < vEnvLoop.size(); ++lo) {
        for (int i = 0; i < vEnvLoop[lo].m_vElem.size(); ++i) {
            if(gotan.IsEleEleTangency(ele,vEnvLoop[lo].m_vElem[i],
                                      true,PreErr5_10))
                continue;
            GeomIntRes intRes;
            goInt.CalculateElementIntersect(ele,vEnvLoop[lo].m_vElem[i],
                                            true,PreErr_12,PreErr_10,
                                            PreErr_10,intRes);
            if(!intRes._isExistInt)
                continue;
            double dis;            godis.PointToLoopDistance(oriLoop,intRes._intPnt1,dis);
            if(dis + SeIn_Dis_Err >= offsetVal){
                vPoint.push_back(intRes._intPnt1);
            }
            if(intRes._intNum < 2)
                continue;
            double dis_;
            godis.PointToLoopDistance(oriLoop,intRes._intPnt2,dis_);
            if(dis_ + SeIn_Dis_Err >= offsetVal){
                vPoint.push_back(intRes._intPnt2);
            }
        }
    }
}
void OpenSelIntSolve::GetCutElementValidPart(const DefLoop& oriLoop,
                                             const DefElem& cutEle,
                                             bool isCurCutEle,
                                             double offsetVal,
                                             bool& isCutEleVal,
                                             DefElem& valPart)
{
    isCutEleVal = false;
    Point cloEndpt;
    double stepVal = PreErr5_2;
    ElemCalculate().ElementEndpointStepVal(cutEle,PreErr5_2,stepVal);
    GeometryLogic().ApproachElementEndpoint(cutEle,stepVal,true,
                                            !isCurCutEle,cloEndpt);
    double dis;
    ///对于闭环,也可通过点是否在环内进行判断 .
    GeomDistance().PointToLoopDistance(oriLoop,cloEndpt,dis);
    if(dis + SeIn_Dis_Err >= offsetVal){
        valPart = cutEle;
        isCutEleVal = true;
    }
}
void OpenSelIntSolve::IsConnectArcSameOftDirection(const DefLoop& oriLoop,
                                                   const DefArc &arc,
                                                   double offsetVal,
                                                   bool& isValid)
{
    isValid = true;
    int pre,nex;
    ElemCalculate numcal;
    TupleQuality tuple;
    EquidistantCurve equcur;
    Point cp = arc._arcCp;
    bool isConnectArc = false;
    for (int i = 0; i < oriLoop.m_vElem.size(); ++i) {
        if(numcal.IsElementEndpoint(oriLoop.m_vElem[i],cp,
                                    PreErr5_10,true)){
            nex = i;
            isConnectArc = true;
            break;
        }
    }
    if(!isConnectArc){
        return;
    }
    pre = numcal.PreviousIndex(oriLoop.m_vElem.size(),nex);    DefElem preEle = oriLoop.m_vElem[pre];
    DefElem nexEle = oriLoop.m_vElem[nex];
    Point preSp,preEp,nexSp,nexEp,preSp_,preEp_,nexSp_,nexEp_;
    tuple.GetEleSpecifyTangentLine(preEle,false,true,
                                   preSp,preEp);
    tuple.GetEleSpecifyTangentLine(nexEle,true,false,
                                   nexSp,nexEp);
    double offsetVal_ = offsetVal / 100;
    ///这里preEp,preSp由于已经反向,因此产生等距时也需要反向 .
    equcur.EquidistantSegment(preEp,preSp,offsetVal_,
                              oriLoop.LoopType(),
                              preSp_,preEp_);
    equcur.EquidistantSegment(nexSp,nexEp,offsetVal_,
                              oriLoop.LoopType(),
                              nexSp_,nexEp_);
    GeomIntRes intRes;
    CheckGeomInt chInt;
    GeomIntersect goInt;
    goInt.CalculateSegSegIntersect(preSp_,preEp_,nexSp_,nexEp_,
                                   true,PreErr_10,PreErr_10,intRes);
    if(!intRes._isExistInt || !intRes._isExiGeoInt){
        return;
    }
    Point arcMid;
    GeomCalculate gocal;
    gocal.ArcMidpoint(arc,arcMid);
    DefElem sigEle(LINETYPE,DefSeg(arcMid,intRes._intPnt1));
    if(chInt.IsElementElementIntersect(preEle,sigEle,PreErr_10)){
        isValid = false;
        return;
    }
    if(chInt.IsElementElementIntersect(nexEle,sigEle,PreErr_10)){
        isValid = false;
    }
}