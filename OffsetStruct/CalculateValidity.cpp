#include "CalculateValidity.h"
#if DebugModel
#pragma optimize("", off)
#endif
void CalculateValidity::IsCurrentValidOffset(OffsetParam offsetPara,
                                             vector<DefLoop>& vOftloop,
                                             const DefLoop& oriLoop,
                                             const DefLoop& loop,
                                             double offsetVal,
                                             bool& isValCal)
{
    isValCal = false;
    if(offsetPara._offsetUnit.IsPureEqud() ||
            (loop.m_vElem.size() == 1 &&
             loop.m_vElem[0]._elemType == CIRCLETYPE)){
        isValCal = true;
        return;
    }
    double distance;
    GeomDistance godis;
    godis.LoopToLoopDistance(oriLoop,loop,distance);
    if(std::abs(distance - offsetVal) > PreErr5_3){
        if(!vOftloop.empty())
            vOftloop.pop_back();
        return;
    }
    if(loop.IsOpenLoop()){
        isValCal = true;
        return;
    }
    if(!IsVallidLocationRelation(oriLoop,loop)){
        if(!vOftloop.empty())
            vOftloop.pop_back();
        return;
    }
    RotDir rot = LoopDirection().LoopRotateDirection(loop);
    if(rot != CLOCKWISE){
        if(!vOftloop.empty())
            vOftloop.pop_back();
        return;
    }
    if(!IsMeetMinBoxMeasure(loop)){
        if(!vOftloop.empty())
            vOftloop.pop_back();
        return;
    }
    isValCal = true;
}
void CalculateValidity::IsValEquidistanceCurveLenth(const DefLoop& oriLoop,
                                                    const DefLoop& loop,
                                                    bool& isValCal)
{
    ElemCalculate numcal;
    double oriLenth,lenth;
    numcal.LoopGeometryLenth(oriLoop,oriLenth);
    numcal.LoopGeometryLenth(loop,lenth);
    if(oriLoop.LoopType() == OUTSIDEPROFILE)
        isValCal = lenth < oriLenth ? true : false;
    else
        isValCal = lenth > oriLenth ? true : false;
}bool CalculateValidity::IsVallidLocationRelation(const DefLoop& oriLoop,
                                                  const DefLoop& loop)
{
    if(loop.m_vElem.size() == 1 &&
            loop.m_vElem[0]._elemType == CIRCLETYPE)
        return true;
    GeomContain gocon;
    bool isInclude = false;
    gocon.IsLoopContainLoopCoord(oriLoop,loop,Base_LoInLo_Err,
                                 isInclude);
    if(oriLoop.LoopType() == OUTSIDEPROFILE){
        if(isInclude)
            return true;
        return false;
    }
    else{
        if(!isInclude)
            return true;
        return false;
    }
}
bool CalculateValidity::LoopMinBoundingBox(const DefLoop& loop)
{
    double minX,maxX,minY,maxY,w,h,l;
    LimitCoordinate().LoopLimitCoordinate(loop,minX,maxX,minY,maxY);
    w = std::abs((maxX + minX) / 2);
    h = std::abs((maxY + minY) / 2);
    l = w > h ? w : h;
    return l >= PreErr5_2;
}
bool CalculateValidity::IsMeetMinBoxMeasure(const DefLoop &loop)
{
    GeomCalculate gocal;
    double girth = 0;
    for (int i = 0; i < loop.ElemNum(); ++i) {
        girth += gocal.ElementLenth(loop.IndexElem(i));
        if(girth >= PreErr_1)
            return true;
    }
    return false;
}
