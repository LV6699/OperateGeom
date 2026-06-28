#include <iostream>
#include "LoopBoolInt.h"
#include "../../GeometryCalculate/GeomCalculate/GeometryLogic.h"
#include "../../OperCurve2D/OperateOftString/OperateOftString.h"
#include "../ClipIntersect/CurveIntClip.h"
#include "../ClipIntersect/OperClipCurve.h"
using std::vector;
using namespace OffsetStruct ;
#if 0
#pragma optimize("", off)
#endif
void LoopBoolInt::InputPreprocesse(vector<DefLoop>& inloops,numst::ReLoopErr err)
{
    GeomClipSolve().HandleCoincideLoops(inloops);
    for(auto& l : inloops){
        OperateLoop().OptCircleArcTrans(l,true,false,0,0);
        l.SetClipDele(false);
    }
}
void LoopBoolInt::LoopBoolIntersection(vector<DefLoop> inloops,
                                       const NumStruct::IntParam& param,
                                       vector<DefLoop>& rloops, bool isOnce)
{
    if(inloops.size() < 2){return;}
    numst::ReLoopErr err(PreErr5_8,PreErr5_8,PreErr5_8,PreErr5_5);
    InputPreprocesse(inloops,err);
    if(inloops.size() < 2){return;}
    CurveIntClip icl;OperClipCurve opc;
    vector<DefLoop>clloops;
    icl.LoopsMutualClip(inloops,param,clloops);
    if(inloops.size() != clloops.size()){return;}
    for (size_t i = 0; i < clloops.size(); ++i) {
        clloops[i].SetBoundary(inloops[i].IsBoundary());
    }
#if 0
    string s = OperateOftString().ArrayLoopString(inloops);
    string s1 = OperateOftString().ArrayLoopString(clloops);
    std::cout<<s<<s1<<endl;
    for (size_t i = 0; i < clloops.size(); ++i) {
        DisplayGeom().DisplayLoop(clloops[i],_colors[i],1,true);
    }
#endif
    opc.DeleteInvalidClipCurve(inloops,clloops,PreErr5_10,true,isOnce);
#if 0
    string s2 = OperateOftString().ArrayLoopString(clloops);
    std::cout<<s2<<endl;
    for (size_t i = 0; i < clloops.size(); ++i) {
        DisplayGeom().DisplayLoop(clloops[i],_colors[i],2);
    }
#endif
    DefLoop tLoop;
    for(auto& l : clloops){
        for(auto& c : l.m_vElem){
            tLoop.AddElem(c);
        }
    }
    opc.ArrangeCurveToLoop(tLoop,PreErr5_8,true,rloops);
    for(auto& l : rloops){
        GeometryLogic().CircleArcLoopTransform(false,l);
    }
#if 0
    string s3 = OperateOftString().ArrayLoopString(rloops);
    std::cout<<s3<<endl;
    for (size_t i = 0; i < rloops.size(); ++i) {
        DisplayGeom().DisplayLoop(rloops[i],_colors.back(),2);
    }
    int tem = 1;
#endif
}
