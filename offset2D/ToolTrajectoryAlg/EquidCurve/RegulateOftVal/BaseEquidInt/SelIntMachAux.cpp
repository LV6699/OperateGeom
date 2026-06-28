#include "SelIntMachAux.h"
#include "../../EquidCurve/EquidCurve/EquidCurve2D.h"
#if DebugModel
#pragma optimize("", off)
#endif
void SelIntMachAux::IsEleEquExiSelInt(const DefElem& ele1,
                                      const DefElem& ele2,
                                      bool isAdjoin,
                                      bool leftward,
                                      double offsetVal,
                                      bool &isResetOftVal)
{
    isResetOftVal = false;
    if(ele1._type == LINETYPE &&
            ele2._type == LINETYPE && isAdjoin)
        return;
    EquidistantCurve equ;
    bool isEle1ExiEqu = false,isEle2ExiEqu = false;
    DefElem equEle1,equEle2;
    //equ.EquidistantElement(ele1,offsetVal,profType,isEle1ExiEqu,equEle1);
    bool isE1 = EquidCurve2D::EquidistantCurve(ele1,offsetVal,leftward,equEle1);
    //equ.EquidistantElement(ele2,offsetVal,profType,isEle2ExiEqu,equEle2);
    bool isE2 = EquidCurve2D::EquidistantCurve(ele2,offsetVal,leftward,equEle2);
    if(!isE1 || !isE2){return;}
    GeomIntRes intRes;
    GeomIntersect goInt;
    goInt.CalculateCurveIntersect(equEle1,equEle2,true,PreErr5_10,
                                  PreErr_10,PreErr_10,intRes);
    if(!intRes._isExistInt || (isAdjoin && intRes._intNum < 2))
        return;
    isResetOftVal =  true;
}
void SelIntMachAux::IsResetOftValBaseSelInt(const DefLoop& loop,
                                            const DefElem& ele,
                                            int index, double offsetVal,
                                            bool& isReset)
{
    if(index >= loop.Size() - 1){return;}
    isReset = false;
    int s = ElemCalculate().NexId(loop.Size(),index);
    for (int i = s; i < loop.m_vElem.size(); ++i){/**
        if(idx == 0 && i == 18){
            int tem = 1;
        }*/
        if(i == index)
            continue;
        bool isAdjoin = false;
        if(std::abs(i - index) == 1 || std::abs(i - index) ==
                int(loop.Size()) - 1){
            isAdjoin = true;
        }
        IsEleEquExiSelInt(ele,loop.m_vElem[i],isAdjoin,
                          loop.Leftward(),offsetVal,isReset);
        if(isReset || index == loop.m_vElem.size() - 1)
            return;
    }
}
