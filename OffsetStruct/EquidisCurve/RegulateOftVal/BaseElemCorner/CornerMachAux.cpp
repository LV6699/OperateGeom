#include "CornerMachAux.h"
#if DebugModel
#pragma optimize("", off)
#endif
void CornerMachAux::GetAdjoinElementAngle(const DefElem& ele1,
                                          const DefElem& ele2,
                                          double& angle)
{
    CalculateAngle angl;
    if(ele1._elemType == LINETYPE){
        Point l1Sp = ele1._segment._segSp;
        Point l1Ep = ele1._segment._segEp;
        if(ele2._elemType == LINETYPE){
            angl.AdjoinSegmentSegmentAngle(l1Sp,l1Ep,ele2._segment._segSp,
                                      ele2._segment._segEp,angle);
        }
        else if(ele2._elemType == ARCTYPE){
            angl.AdjoinSegmentArcAngle(l1Sp,l1Ep,ele2._arc,
                                      true,angle);
        }
    }
    else if(ele1._elemType == ARCTYPE){
        if(ele2._elemType == LINETYPE){
            angl.AdjoinSegmentArcAngle(ele2._segment._segSp,
                                      ele2._segment._segEp,
                                      ele1._arc,false,angle);
        }
        else if(ele2._elemType == ARCTYPE){
            angl.AdjoinArcArcAngle(ele1._arc,ele2._arc,angle);
        }
    }
}