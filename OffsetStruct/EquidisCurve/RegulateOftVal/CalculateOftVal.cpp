#include "CalculateOftVal.h"
#include<iostream>
#if DebugModel
#pragma optimize("", off)
#endif
void CalculateOftVal::AdjustCurrentOffsetValue(const DefLoop& loop,
                                               int hasOftNum,
                                               bool hasChaOftVal,
                                               OffsetParam offsetPara,
                                               double& offsetVal)
{
    if(!offsetPara._offsetUnit.AlterableValue()){
        offsetVal = offsetPara._offsetUnit.OffsetValue();
        return;
    }
    if(!offsetPara._pathTech.IsAddOriginLoop() && hasOftNum == 0){
        offsetVal = offsetPara._offsetUnit.FirstOffsetValue();
        return;
    }
    if(hasChaOftVal)
        return;
    offsetVal = offsetPara._offsetUnit.OffsetValue();
    ///return;
    if((offsetVal - PreErr5_4) <= offsetPara._offsetUnit.ToolRadius()){
        return;
    }
    CornerMachine cormac;
    cormac.OffsetValueBaseCorner(loop,offsetPara._offsetUnit.
                                 ToolRadius(),offsetVal);
    SelIntMachine selmac;
    selmac.GetOftValueBaseSelInt(loop,offsetPara._offsetUnit.
                                 ToolRadius(),offsetVal);
                                 
}