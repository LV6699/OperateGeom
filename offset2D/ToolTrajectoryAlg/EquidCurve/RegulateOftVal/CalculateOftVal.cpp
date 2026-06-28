#include "CalculateOftVal.h"
#include<iostream>
#if DebugModel
#pragma optimize("", off)
#endif
void CalculateOftVal::AdjustOffsetValue(const DefLoop& loop,int hasOftNum,
                                        bool hasChaOftVal,OffsetParam param,
                                        double& offsetVal)
{
    if(!param._offsetUnit.AlterableValue()){
        offsetVal = param._offsetUnit.OffsetValue();
        return;
    }
    if(!param._pathTech.IsAddOriginLoop() && hasOftNum == 0){
        offsetVal = param._offsetUnit.FirstOftVal();
        return;
    }
    if(hasChaOftVal)
        return;
    offsetVal = param._offsetUnit.OffsetValue();
    ///return;
    if((offsetVal - PreErr5_4) <= param._offsetUnit.ToolRadius()){
        return;
    }
    CornerMachine cormac;
    cormac.OffsetValueBaseCorner(loop,param._offsetUnit.
                                 ToolRadius(),offsetVal);
    SelIntMachine selmac;
    selmac.GetOftValueBaseSelInt(loop,param._offsetUnit.
                                 ToolRadius(),offsetVal);
}
