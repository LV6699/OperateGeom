/**
  @author: lvdong
  @date: 2025-03
**/
#ifndef GEOMETRYFITTING_H
#define GEOMETRYFITTING_H
#include"GeomFittingSolve.h"
#include"../../../CommonFile/Header.h"
#include"../../../../OperateView/OperateView/DisplayGeom.h"
using namespace OffsetStruct::GeomFitting;
class GeometryFitting
{
public:
    GeometryFitting(){}
    void ResetFittingIndex(OffsetStruct::GeomFitting::FittArc &fitData,
                           vector<OffsetStruct::GeomFitting::FittArc> &fittints)
    {
        if(fitData._indexs.size() > 1)
            fittints.push_back(fitData);
        fitData.ClearFittingData();
    }
    void InvalidCase(int i,vector<OffsetStruct::GeomFitting::FittArc> &fittints,
                     OffsetStruct::GeomFitting::FittArc &fitData){
        fitData._indexs.pop_back();
        fitData._arc = fitData._preArc;
        ResetFittingIndex(fitData,fittints);
        fitData._indexs.push_back(i);
    }
    void ExicuteSegmentFitting(const DefLoop& loop,
                               vector<OffsetStruct::GeomFitting::FittArc> 
&fittints){
        FittArc fitData;
        GeomFittingSolve sol;
        double hErr = 8e-3,aErr = 10;
        ///DisplayGeom().DisplayLoop(loop,_colors.back(),1.5);
        for (int i = 0; i < loop.ElemNum(); ++i) {/**
            if(i == 13){
                int tem = 1;
            }*/
            if(!sol.IsMeetFittingCase(loop.IndexElem(i))){
                ResetFittingIndex(fitData,fittints);
                continue;
            }
            if(fitData._indexs.empty()){
                fitData._indexs.push_back(i);
                continue;
            }
            if(!sol.IsMeetAdjoinAngle(loop,i,aErr)){
                ///DisplayGeom().DisplayArc(fitData._arc,_colors[1],1);
                ResetFittingIndex(fitData,fittints);
                fitData._indexs.push_back(i);
            }
            fitData._indexs.push_back(i);
            if(!sol.ReferenceArc(loop,fitData)){
                ///DisplayGeom().DisplayArc(fitData._arc,_colors[1],1);
                InvalidCase(i,fittints,fitData);
                continue;
            }
            if(!sol.CurrentIteratorValidity(loop,fitData,aErr,hErr)){                ///DisplayGeom().DisplayArc(fitData._arc,_colors[1],1);
                InvalidCase(i,fittints,fitData);
                continue;
            }
            if((i == loop.ElemNum() - 1) && fitData._indexs.size() > 2){
                fittints.push_back(fitData);
            }
        }
    }
    void OrganizeFittingStructure(const DefLoop& loop,
                                  const 
vector<OffsetStruct::GeomFitting::FittArc> &fittints,
                                  DefLoop &rloop){
        if(fittints.size() == 1){
            const auto& sp = fittints[0]._arc._arcSp;
            const auto& ep = fittints[0]._arc._arcEp;
            if(sp.IsSamePoint2D(ep,PreErr5_10)){
                const auto& cp = fittints[0]._arc._arcCp;
                double r = fittints[0]._arc.ArcRadius();
                rloop.PushBack(DefElem(CIRCLETYPE,DefCircle(cp,r)));
                return;
            }
        }
        MathCalculate matcal;
        for (int i = 0; i < loop.ElemNum(); ++i) {
            int index = 0;
            bool isIn = false;
            for (int f = 0; f < fittints.size(); ++f) {
                if(matcal.IsValueIn2DArray(fittints[f]._indexs,i)){
                    index = f;
                    isIn = true;
                    break;
                }
            }
            if(!isIn){
                rloop.PushBack(loop.IndexElem(i));
                continue;
            }
            FittArc fitArc = fittints[index];
            if(fitArc._arcCps.empty() || fitArc._arcRadius.empty() ||
                    fitArc._indexs.empty()){
                rloop.PushBack(loop.IndexElem(i));
                continue;
            }
            rloop.PushBack(DefElem(ARCTYPE,fitArc._arc));
            i = fitArc._indexs.back();
        }
        if(GeometryLogic().IsCircleStructureLoop(rloop,PreErr5_3,
                                                 PreErr5_3) &&
                rloop.ElemNum() == 2){
            rloop[0]._arc.ChoiceSetRadius(PreErr5_3);
            rloop[1]._arc.ChoiceSetRadius(PreErr5_3);
            const auto& cp0 = rloop[0]._arc._arcCp;
            const auto& cp1 =  rloop[1]._arc._arcCp;
            double r = (rloop[0]._arc._arcR + rloop[1]._arc._arcR) / 2;
            Point cp = cp0.Midpoint(cp1);
            rloop.ClearElem();
            rloop.PushBack(DefElem(CIRCLETYPE,DefCircle(cp,r)));
        }
    }};
#endif // GEOMETRYFITTING_H