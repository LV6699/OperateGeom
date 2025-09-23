#include "OrganSubtrLoop.h"
void OrganSubtrLoop::TemplateLoopForwardIterate(DefLoop& tempLoop,
                                                vector<DefLoop>& vLoop,
                                                Point endpoint,int eleIndex,
                                                vector<DefElem>& vElement)
{
    if(tempLoop.IsEmpty())
        return;
    GeomCalculate gocal;
    ElemCalculate numcal;LoopSubtractSolve subsol;
    DefElem startEle = tempLoop.IndexElem(eleIndex);
    vElement.push_back(startEle);
    tempLoop.m_vElem[eleIndex]._elemLabel._eleHasMer = true;
    Point preEp,curSp;
    numcal.ElementEndpoint(startEle,true,preEp);
    if(subsol.IsTerminateSearch(endpoint,preEp,vElement)){
        return;
    }
    int i = numcal.NextIndex(tempLoop.ElemNum(),eleIndex);
    while (i < tempLoop.ElemNum()) {/**
        if(i == 5){
            int tem = 1;
        }*/
        DefElem ele = tempLoop.IndexElem(i);
        if(ele._elemLabel._eleHasMer){
            bool isFound__;
            int loIndex__,index__;
            subsol.ArrayLoopConnectElement(vLoop,preEp,isFound__,
                                           loIndex__,index__);
            if(isFound__){
                ArrayLoopReverseIterate(tempLoop,vLoop,endpoint,
                                        loIndex__,index__,vElement);
            }
            subsol.CloseLoopCase(vElement);
            return;
        }
        numcal.ElementEndpoint(ele,false,curSp);
        if(gocal.IsIdenticalPoint2D(preEp,curSp,PreErr5_8)){
            vElement.push_back(ele);
            tempLoop.m_vElem[i]._elemLabel._eleHasMer = true;
            numcal.ElementEndpoint(ele,true,preEp);
            if(subsol.IsTerminateSearch(endpoint,preEp,vElement))
                return;
            i = numcal.NextIndex(tempLoop.ElemNum(),i);
            continue;
        }
        if(subsol.IsTerminateSearch(endpoint,preEp,vElement))
            return;
        int loIndex_,index_;
        bool isFound_ = false;
        subsol.ArrayLoopConnectElement(vLoop,preEp,isFound_,
                                       loIndex_,index_);
        if(isFound_){
            ArrayLoopReverseIterate(tempLoop,vLoop,endpoint,
                                    loIndex_,index_,vElement);
        }
        subsol.CloseLoopCase(vElement);
        return;
    }}
void OrganSubtrLoop::ArrayLoopReverseIterate(DefLoop &tempLoop,
                                             vector<DefLoop> &vLoop,
                                             Point endpoint,
                                             int loIndex,int eleIndex,
                                             vector<DefElem> &vElement)
{
    GeomCalculate gocal;
    ElemCalculate numcal;
    LoopSubtractSolve subsol;
    DefLoop loop = vLoop[loIndex];
    DefElem startEle = loop.IndexElem(eleIndex);
    subsol.AddReverseElement(startEle,vElement);
    loop.m_vElem[eleIndex]._elemLabel._eleHasMer = true;
    Point preSp,curEp,curSp;
    numcal.ElementEndpoint(startEle,false,curSp);
    if(subsol.IsTerminateSearch(endpoint,curSp,vElement)){
        vLoop[loIndex] = loop;
        return;
    }
    numcal.ElementEndpoint(startEle,false,preSp);
    int i = numcal.PreviousIndex(loop.ElemNum(),eleIndex);
    while (i < loop.ElemNum()) {
        DefElem ele = loop.IndexElem(i);
        if(ele._elemLabel._eleHasMer){
            int index__;
            bool isFound__ = false;
            vLoop[loIndex] = loop;
            subsol.TempateConnectElement(tempLoop,preSp,
                                         isFound__,index__);
            if(!isFound__){
                vElement.clear();
                return;
            }
            TemplateLoopForwardIterate(tempLoop,vLoop,endpoint,
                                       index__,vElement);
            return;
        }
        numcal.ElementEndpoint(ele,true,curEp);
        if(gocal.IsIdenticalPoint2D(preSp,curEp,PreErr5_8)){
            subsol.AddReverseElement(ele,vElement);
            loop.m_vElem[i]._elemLabel._eleHasMer = true;
            numcal.ElementEndpoint(ele,false,curSp);
            if(subsol.IsTerminateSearch(endpoint,curSp,vElement)){
                vLoop[loIndex] = loop;
                return;
            }
            preSp = curSp;
            i = numcal.PreviousIndex(loop.ElemNum(),i);
            continue;
        }
        int index_;
        bool isFound_ = false;
        vLoop[loIndex] = loop;
        subsol.TempateConnectElement(tempLoop,curSp,isFound_,index_);
        if(!isFound_){
            vElement.clear();            return;
        }
        TemplateLoopForwardIterate(tempLoop,vLoop,endpoint,
                                   index_,vElement);
        return;
    }
}