#include "CreateEqudCurve.h"
#include "../../../../OperateView/OperateView/DisplayGeom.h"
#if DebugModel
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
#define _EquDebug 0
#if _EquDebug
#include<iostream>
#include"../../../../../../ViewWindow/DisplayShape/DisplayToolPath.h"
DisplayToolPath dis;
#endif
void CreateEqudCurve::EquidCurvePretreatment(DefLoop oriLoop,
                                             double offsetVal,
                                             bool &isIterminal,
                                             bool &isValOffset,
                                             DefLoop &resLoop)
{
    isValOffset = false;
    isIterminal = false;
    if(oriLoop.IsEmpty()){
        isIterminal = true;
        return;
    }
    resLoop.SetLoopType(oriLoop.LoopType());
    auto CircleOffset = [](DefLoop oriLoop,double offsetVal,
            bool &isIterminal,bool &isValOffset,DefLoop &resLoop) -> void{
        if(oriLoop.m_vElem.size() == 1 &&
                oriLoop.m_vElem[0]._elemType == CIRCLETYPE){
            isIterminal = true;
            double R = oriLoop.m_vElem[0]._circle.CircleRadius();
            if((oriLoop.LoopType() == OUTSIDEPROFILE) &&
                    ((std::abs(R - offsetVal) <= Oft_CircleVal_Err) ||
                     (R <= offsetVal))){
                return;
            }
            double radius;
            if(oriLoop.LoopType() == OUTSIDEPROFILE){
                radius = R - offsetVal;
            }else{
                radius = R + offsetVal;
            }
            resLoop = oriLoop;
            resLoop.m_vElem[0]._circle.SetCircleRadius(radius);
            isValOffset = true;
            return;
        }
    };
    CircleOffset(oriLoop,offsetVal,isIterminal,isValOffset,resLoop);
    if(isIterminal)
        return;
    GeometryLogic golg;
    golg.CircleArcLoopTransform(false,oriLoop);
    CircleOffset(oriLoop,offsetVal,isIterminal,isValOffset,resLoop);
}
void CreateEqudCurve::CreateEquidistanceCurve(DefLoop oriLoop,
                                              double offsetVal,
                                              bool &isValOffset,
                                              DefLoop &resLoop){
#define view 0
#if view
    DisplayGeom().DisplayLoop(oriLoop,_colors.back(),1,false,false);
    string s = OperateOftString().LoopElementString(oriLoop);
    std::cout<<s<<endl;
#endif
    bool isIterminal = false;
    EquidCurvePretreatment(oriLoop,offsetVal,isIterminal,
                           isValOffset,resLoop);
    if(isIterminal)
        return;
    CreateCurveLogic curlg;CreateCurveSolve cursol;
    vector<DefElem> eCurves,curves;
    curlg.CalculateEquidistanceCurve(oriLoop.m_vElem,
                                     oriLoop.LoopType(),
                                     offsetVal,eCurves);
#if view
    DisplayGeom().DisplayLoop(DefLoop(eCurves),_colors[1],1,false,false);
    string s1 = OperateOftString().LoopElementString(DefLoop(eCurves));
    std::cout<<s1<<endl;
#endif
    curlg.ClosedEquidistanceCurve(oriLoop,eCurves,offsetVal,
                                  oriLoop.LoopType(),curves);
#if view
    DisplayGeom().DisplayLoop(DefLoop(curves),_colors[2],1,true,false);
    string s2 = OperateOftString().LoopElementString(DefLoop(curves));
    std::cout<<s2<<endl;
#endif
    curlg.OperateInvalidArc(curves);
    resLoop.SetLoopElem(curves);
    cursol.ResetInvalidElem(curves,PreErr5_10);
    resLoop.SetOpenLoop(oriLoop.IsOpenLoop());
#if view
    DisplayGeom().DisplayLoop(DefLoop(curves),_colors[3],2,true,false);
    string s3 = OperateOftString().LoopElementString(DefLoop(curves));
    std::cout<<s3<<endl;
#endif
    isValOffset = true;
}