#ifndef CREATEFITTING_H
#define CREATEFITTING_H
#include "CreateFitting.h"
#include"../../CommonFile/Header.h"
#include "./GeomFitting/GeometryFitting.h"
#include"../../../OperateView/OperateView/DisplayGeom.h"
#if DebugModel
#pragma optimize("", off)
#endif
using namespace OffsetStruct::GeomFitting;
class CreateFitting
{
public:
    CreateFitting(){}
    void BndIslLoopGeomFitting(GeomArea& biLoop,
                               double lenLim = Fitting_LenLim,
                               double valErr = Fitting_DisErr,
                               double curvLim = Fitting_AngVarErr){
        DefLoop bLoop = biLoop.BndLoop();
        
///DisplayGeom().DisplayLoop(biLoop._bndLoop,_colors.back(),1,false,true);
        LoopGeometryFitting(bLoop,biLoop._bndLoop,
                            lenLim,valErr,curvLim);
        ///DisplayGeom().DisplayLoop(biLoop._bndLoop,_colors[0],1);
        for (int i = 0; i < biLoop.IslLoop().size(); ++i) {
            DefLoop loop;
            
///DisplayGeom().DisplayLoop(biLoop._vIslLoop[i],_colors.back(),1,false,true);
            LoopGeometryFitting(biLoop._vIslLoop[i],loop,
                                lenLim,valErr,curvLim);
            ///DisplayGeom().DisplayLoop(loop,_colors[0],1);
            biLoop._vIslLoop[i] = loop;
        }
    }
    void LoopGeometryFitting(const DefLoop& loop,
                             DefLoop &resLoop,
                             double lenLim = Fitting_LenLim,
                             double valErr = Fitting_DisErr,
                             double curvLim = Fitting_AngVarErr){
#if DebugModel
        auto sTime = std::chrono::high_resolution_clock::now();
#endif
        resLoop = loop;
        resLoop.ClearElem();
        vector<FittArc>fittints;
#if 0
        ExicuteFittingSegmentToArc(loop,lenLim,disVar,angleVar,fittints);
        if(fittints.empty()){
            resLoop = loop;
            return;
        }
        FittingArcLogic fitlg;
        fitlg.LoopSegmentFittingArc(loop,fittints,resLoop);
#else
        GeometryFitting gofit;
        gofit.ExicuteSegmentFitting(loop,fittints);
        if(fittints.empty()){            resLoop = loop;
            return;
        }
        gofit.OrganizeFittingStructure(loop,fittints,resLoop);
#endif
#if DebugModel
        auto eTime = std::chrono::high_resolution_clock::now();
        double time = std::chrono::duration<double>(eTime - sTime).count();
        time *= 1000;
#endif
#if DebugModel
        vector<int>vIndex;
        double e = PreErr5_12;
        EqudDebug().NotConnectElemIndex(resLoop.LoopElem(),
                                        e,vIndex);
        EqudDebug().NotMeetArcIndex(resLoop,e,vIndex);
        double mx1 = 0,mx2 = 0,my1 = 0,my2 = 0,mx1_ = 0,mx2_ = 0,my1_ = 0,my2_ =
0;
        LimitCoordinate().LoopLimitCoordinate(loop,mx1,mx2,my1,my2);
        LimitCoordinate().LoopLimitCoordinate(resLoop,mx1_,mx2_,my1_,my2_);
        double ex1 = std::abs(mx1 - mx1_),ex2 = std::abs(mx1 - mx1_);
        double ey1 = std::abs(my1 - my1_),ey2 = std::abs(my1 - my1_);
        double ex = ex1 > ex2 ? ex1 : ex2,ey = ey1 > ey2 ? ey1 : ey2;
        double me = ex > ey ? ex : ey;
        double v = 0.0001;
        if(me > v){
            string str = 
OperateOftString().LoopElementString(loop,false,0,"1","1");
            //resLoop = loop;
            int tem = 1;
        }
#endif
    }
};
#endif // CREATEFITTING_H