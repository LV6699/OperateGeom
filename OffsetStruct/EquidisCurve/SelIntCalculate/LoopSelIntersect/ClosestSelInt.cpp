#include "ClosestSelInt.h"
#if DebugModel
#pragma optimize("", off)
#endif
bool ClosestSelInt::IsValidSelIntPoint(int i1,int i2,const DefLoop &loop,
                                       const GeomIntRes &intRes)
{
    if(!intRes._isExistInt)
        return false;
    if((std::abs(i1 - i2) == 1 ||
        std::abs(i1 - i2) == loop.ElemNum()) &&
            intRes._intNum < 2)
        return false;
    return true;
}
void ClosestSelInt::ElementSelIntersectPoint(const DefLoop &loop,
                                             int id,vector<Point> &points)
{
    GeomIntersect goint;ElemCalculate numcal;
    const auto& ele = loop[id];
    for (int i = 0; i < loop.Size(); ++i){/**
        if((id == 10 && i == 11) || (id == 11 && i == 12)){
            int tem = 1;
        }*/
        if (i == id || (std::abs(i-id) == 1 &&
                        ele.ElemeType() == LINETYPE &&
                        loop[i].ElemeType() == LINETYPE)){
            continue;
        }
        GeomIntRes intRes;
        const auto& ele_ = loop[i];
        goint.CalculateCurveIntersect(ele,ele_,true,PreErr5_10,
                                      PreErr5_10,PreErr5_10,intRes);
        if(!IsValidSelIntPoint(id,i,loop,intRes))
            continue;
        bool isEndpt = false;
        isEndpt = numcal.IsElementEndpoint(ele,intRes.P1(),PreErr5_10);
        ///double dis1 = intRes.P1().Distance2D(ele.StarPt());
        ///double dis2 = intRes.P1().Distance2D(ele.EndPt());
        if(!isEndpt){
            points.push_back(intRes._intPnt1);
        }
        if(intRes._intNum < 2){
            continue;
        }else{
            ///double dis1_ = intRes.P2().Distance2D(ele.StarPt());
            ///double dis2_ = intRes.P2().Distance2D(ele.EndPt());
            isEndpt = false;
            isEndpt = numcal.IsElementEndpoint(ele,intRes.P2(),PreErr5_10);
            if(!isEndpt){
                points.push_back(intRes._intPnt2);
            }
        }
    }
}
void ClosestSelInt::ElemSelIntersectPoint(DefLoop &loop, int index,
                                          vector<Point> &vPoint)
{
    if(index >= loop.ElemNum() - 1)        return;
    GeomIntersect goint;ElemCalculate nuca;
    LoopSelIntSolve selsol;
    DefElem ele = loop.m_vElem[index];
    int sIndex = nuca.NextIndex(loop.ElemNum(),index);
    vector<pair<int,vector<Point>>>vIntElem;
    for (int i = sIndex; i < loop.ElemNum(); ++i){/**
        if((index == 53 )){
            int tem = 1;
        }*/
        GeomIntRes intRes;
        goint.CalculateCurveIntersect(ele,loop.m_vElem[i],true,
                                      PreErr_10,PreErr5_10,
                                      PreErr5_10,intRes);
        if(!intRes._isExistInt)
            continue;
        vector<Point>vPoint_;
        bool isEndpt1 = false,isEndpt2 = false,
                isEndpt1_ = false,isEndpt2_ = false;
        isEndpt1 = nuca.IsElementEndpoint(ele,intRes._intPnt1,
                                          PreErr5_10);
        isEndpt1_ = nuca.IsElementEndpoint(loop.IndexElem(i),
                                           intRes._intPnt1,
                                           PreErr5_10);
        if(!isEndpt1)
            vPoint.push_back(intRes._intPnt1);
        if(!isEndpt1_)
            vPoint_.push_back(intRes._intPnt1);
        if(intRes._intNum > 1){
            isEndpt2 = nuca.IsElementEndpoint(ele,intRes._intPnt2,
                                              PreErr5_10);
            isEndpt2_ = nuca.IsElementEndpoint(loop.IndexElem(i),
                                               intRes._intPnt2,
                                               PreErr5_10);
            if(!isEndpt2)
                vPoint.push_back(intRes._intPnt2);
            if(!isEndpt2_)
                vPoint_.push_back(intRes._intPnt2);
        }
        if(!vPoint_.empty())
            vIntElem.push_back(std::make_pair(i,vPoint_));
    }
    if(vIntElem.empty())
        return;
    int ptNum = 0;
    vector<int>vIndex;
    for(auto& p : vIntElem){
        vIndex.push_back(p.first);
        ptNum += p.second.size();
    }
    MathCalculate matcal;
    vector<DefElem> vElement;
    for (int i = 0; i < loop.ElemNum(); ++i) {
        if(!matcal.IsValueIn2DArray(vIndex,i)){
            vElement.push_back(loop.IndexElem(i));
            continue;
        }
        bool isFind = false;
        vector<Point> vPoint_;
        for (int j = 0; j < vIntElem.size(); ++j) {
            if(i == vIntElem[j].first){
                isFind = true;
                vPoint_ = vIntElem[j].second;                break;
            }
        }
        if(!isFind || vPoint_.empty()){
            vElement.push_back(loop.IndexElem(i));
            continue;
        }
        vector<DefElem>vElem_;
        DefElem ele_ = loop.IndexElem(i);
        selsol.ArrayPointSliptElement(ele_,vPoint_,vElem_);
        for(auto &e : vElem_){
            bool isVal = false;
            nuca.ElementValidity(e,PreErr5_10,PreErr5_10,isVal);
            if(isVal)
                vElement.push_back(e);
        }
    }
    loop.SetLoopElem(vElement);
}