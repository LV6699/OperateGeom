#include <tbb/tbb.h>
#include "LoopSelIntSolve.h"
#include "../../../../OperateView/OperateView/DisplayGeom.h"
#if DebugModel
#pragma optimize("", off)
#endif
///需要保证vPoint里面的点在ele上,vPoint中的点拆分ele成多段,
///且按照距离ele起点从近至远排列.
void LoopSelIntSolve::ArrayPointSliptElement(const DefElem& ele,
                                             const vector<Point>&vPoint,
                                             vector<DefElem>&vElem)
{
    if(vPoint.empty())
        return;
    ElemCalculate numcal;
    if(vPoint.size() == 1){
        DefElem preEle,nexEle;
        numcal.PointSplitElement(ele,vPoint[0],true,preEle);
        numcal.PointSplitElement(ele,vPoint[0],false,nexEle);
        vElem.push_back(preEle);
        vElem.push_back(nexEle);
        return;
    }
    GeomDistance godis;
    vector<double>vDistance,vDistance_;
    for (int i = 0; i < vPoint.size(); ++i) {
        double dis;
        godis.PointToEleEndpointDistance(ele,vPoint[i],true,dis);
        vDistance.push_back(dis);
        vDistance_.push_back(dis);
    }
    vector<Point>vPoint_;
    std::sort(vDistance_.begin(),vDistance_.end());
    for (int i = 0; i < vDistance_.size(); ++i) {
        int index = 0;
        for (int j = 0; j < vDistance.size(); ++j) {
            if(vDistance_[i] == vDistance[j]){
                index = j;
                break;
            }
        }
        vPoint_.push_back(vPoint[index]);
    }
    DefElem element = ele;
    for (int i = 0; i < vPoint_.size(); ++i) {
        DefElem preEle,nexEle;
        numcal.PointSplitElement(element,vPoint_[i],true,preEle);
        vElem.push_back(preEle);
        numcal.PointSplitElement(element,vPoint_[i],false,nexEle);
        element = nexEle;
        if(i == vPoint_.size() - 1){
            vElem.push_back(nexEle);
        }
    }
}
void LoopSelIntSolve::LocalSelIntLoopIndex(DefElem curEle,
                                           vector<DefElem>&vElem,
                                           vector<vector<int>>vArrIndex,
                                           bool isOpenLoop,
                                           int index,
                                           vector<int>& vIndex){
    Point startPt;
    GeomCalculate gocal;ElemCalculate numcal;MathCalculate matcal;
    numcal.ElementEndpoint(curEle,false,startPt);
    ///找与当前元素相连的下一元素 .
    for (int nex = index + 1; nex < vElem.size(); ++nex) {
        if(matcal.IsValueIn3DArray(vArrIndex,nex))
            continue;
        Point curEp,nexSp,nexEp;
        numcal.ElementEndpoint(curEle,true,curEp);
        numcal.ElementEndpoint(vElem[nex],false,nexSp);
        if(!gocal.IsIdenticalPoint2D(curEp,nexSp,PreErr5_6))
            continue;
        vIndex.push_back(nex);
        if(isOpenLoop && nex == vElem.size() - 1)
            break;
        numcal.ElementEndpoint(vElem[nex],true,nexEp);
        if(gocal.IsIdenticalPoint2D(nexEp,startPt,PreErr5_6))
            break;
        curEle = vElem[nex];
    }
}
void LoopSelIntSolve::LoopEnvelopClipElement(const DefLoop& oriLoop,
                                             const DefElem& ele,
                                             double offsetVal,
                                             vector<DefElem>&vElem)
{
    vector<Point>vPoint;
    vector<DefLoop>vEnvLoop;
    OpenSelIntSolve opesol;GeomDistance godis;
    opesol.ShortDistLoopElemEnvelope(oriLoop,ele,offsetVal,vEnvLoop);
    opesol.ElemEnvelopeIntPoint(oriLoop,ele,vEnvLoop,offsetVal,vPoint);
    vector<int>vIndex;
    ArrayPointSliptElement(ele,vPoint,vElem);
    for (int i = 0; i < vElem.size(); ++i) {
        double distance;
        godis.ElementToLoopDistance(oriLoop,vElem[i],distance);
        if(distance + SeIn_Dis_Err >= offsetVal)
            continue;
        vIndex.push_back(i);
    }
    ElemCalculate().DeleteIndexElement(vIndex,vElem);
}
///根据相邻元素的特性处理干扰元素 ,比如当元素i长度很短,且其相邻元素i+1或i-1较长
///且是有效元素,i理论上是无效的,且在判断元素i的有效性时,i容易识别有效,因此需要
///标识i无效.
void LoopSelIntSolve::HandleInterfereElement(const DefLoop& oriLoop,
                                             vector<DefElem>& vElem,
                                             double offsetVal,
                                             double errVal)
{
    ElemCalculate numcal;
    GeomCalculate gocal;SelIntersectAux exesel;
#if 0
    for (int i = 0; i < vElem.size(); ++i) {
        double lenth = gocal.ElementLenth(vElem[i]);
        if(lenth > errVal)
            continue;        if(lenth < PreErr5_8){
            vElem[i]._elemLabel._isValidity = false;
            continue;
        }
        int pre = numcal.PreviousIndex(vElem.size(),i),
                nex = numcal.NextIndex(vElem.size(),i);
        double preLen = gocal.ElementLenth(vElem[pre]);
        if(preLen > errVal){
            EleLoopDisType disType;
            exesel.ElemToLoopDistanceType(vElem[pre],oriLoop,
                                          offsetVal,disType);
            if(disType == EntiretyOutside){
                vElem[i]._elemLabel._isValidity = false;
                continue;
            }
        }
        double nexLen = gocal.ElementLenth(vElem[nex]);
        if(nexLen > errVal){
            EleLoopDisType disType;
            exesel.ElemToLoopDistanceType(vElem[nex],oriLoop,
                                          offsetVal,disType);
            if(disType == EntiretyOutside){
                vElem[i]._elemLabel._isValidity = false;
                continue;
            }
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, vElem.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            double lenth = gocal.ElementLenth(vElem[i]);
            if(lenth > errVal)
                continue;
            if(lenth < PreErr5_10){
                vElem[i]._elemLabel._isValidity = false;
                continue;
            }
            int pre = numcal.PreviousIndex(vElem.size(),i),
                    nex = numcal.NextIndex(vElem.size(),i);
            double preLen = gocal.ElementLenth(vElem[pre]);
            if(preLen > errVal){
                EleLoopDisType disType;
                exesel.ElemToLoopDistanceType(vElem[pre],oriLoop,
                                              offsetVal,disType);
                if(disType == EntiretyOutside){
                    vElem[i]._elemLabel._isValidity = false;
                    continue;
                }
            }
            if(vElem.size() <= 2)
                continue;
            double nexLen = gocal.ElementLenth(vElem[nex]);
            if(nexLen > errVal){
                EleLoopDisType disType;
                exesel.ElemToLoopDistanceType(vElem[nex],oriLoop,
                                              offsetVal,disType);
                if(disType == EntiretyOutside){
                    vElem[i]._elemLabel._isValidity = false;
                    continue;
                }
            }
        }    });
#endif
}
void LoopSelIntSolve::SpecifyInvalidClipElem(vector<DefElem>& elems)
{
    if(elems.size() < 2)
        return;
    ElementPrecision elepre;
    elepre.DeleteInvalidElement(PreErr5_10,PreErr5_10,elems);
}
void LoopSelIntSolve::CreateLocalIntersectLoop(int index,
                                               vector<DefElem>& vElem,
                                               bool isOpenLoop,
                                               vector<int>& vIndex)
{
    GeomCalculate gocal;ElemCalculate numcal;
    ///DisplayGeom().DisplayLoop(DefLoop(vElem),_colors.back(),1,true,true);
    Point endpoint,startEp;
    DefElem startEle = vElem[index];
    numcal.ElementEndpoint(startEle,false,endpoint);
    numcal.ElementEndpoint(startEle,true,startEp);
    int nex = 0;
    bool isFound = false,isTeminate = false;
    FindLoopConnectPoint(vElem,startEp,isFound,nex);
    if(!isFound)
        return;
    vIndex.push_back(nex);
    vElem[nex]._elemLabel._eleHasMer = true;
    if(vElem.size() < 3 || numcal.IsCloseLoop(
                DefLoop(vector<DefElem>{startEle,
                        vElem[nex]}),PreErr5_10))
        return;
    DefElem curEle = vElem[nex];
    while (!isTeminate) {/**
        if(nex == 43){
            int tem = 1;
        }*/
        isFound = false;
        Point curEp = numcal.ElementEndpoint(curEle,true);
        FindLoopConnectPoint(vElem,curEp,isFound,nex);
        if(!isFound){
            ///double dist = curEp.Distance2D(vElem[nex+1].StarPt());
            if(vIndex.size() < 2)
                break;
            vIndex.pop_back();
            int index_ = vIndex.back();
            curEle = vElem[index_];
            continue;
            ///break;
        }
        vIndex.push_back(nex);
        vElem[nex]._elemLabel._eleHasMer = true;
        if(isOpenLoop && nex == vElem.size() - 1){
            isTeminate = true;
        }
        Point nexEp = numcal.ElementEndpoint(vElem[nex],true);
        if(gocal.IsIdenticalPoint2D(nexEp,endpoint,PreErr5_9)){
            isTeminate = true;        }
        curEle = vElem[nex];
    }
}
void LoopSelIntSolve::FindLoopConnectPoint(vector<DefElem>& vElem,
                                           const Point& P,
                                           bool& isFound,int& idRes)
{
    ElemCalculate numcal;GeomCalculate gocal;
    isFound = false;
    for (int i = 0; i < vElem.size(); ++i) {/**
        if(i == 43){
            int tem = 1;
        }*/
        if(vElem[i]._elemLabel._eleHasMer)
            continue;
        Point sp = numcal.ElementEndpoint(vElem[i],false);
        if(gocal.IsIdenticalPoint2D(sp,P,PreErr5_9)){
            idRes = i;
            isFound = true;
            return;
        }
    }
}
bool LoopSelIntSolve::IsCloseIndexLoop(const vector<DefElem>& vElem,
                                       const vector<int>& vIndex)
{
    int s = vIndex.front(),e = vIndex.back();
    if(vElem.empty() || s > vElem.size() - 1 || e > vElem.size() - 1)
        return false;
    Point sp = ElemCalculate().ElementEndpoint(vElem[s],false),
            ep = ElemCalculate().ElementEndpoint(vElem[e],true);
    return GeomCalculate().IsIdenticalPoint2D(sp,ep,PreErr5_9);
}