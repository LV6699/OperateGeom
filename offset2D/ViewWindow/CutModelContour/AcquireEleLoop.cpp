#include "AcquireEleLoop.h"
shared_ptr<GeomCalculate>eleInlo_go = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate>eleInlo_num = make_shared<ElemCalculate>();
shared_ptr<ExicuteProgram>eleInlo_expro = make_shared<ExicuteProgram>();
void AcquireEleLoop::GetEleZValue(const DefElem& ele,double& zValue)
{
    if(ele._type == LINETYPE){
        zValue = ele._segment._segSp._z;
    }
    else if(ele._type == ARCTYPE){
        zValue = ele._arc._arcSp._z;
    }
    else if(ele._type == CIRCLETYPE){
        zValue = ele._circle._cirCp._z;
    }
}
void AcquireEleLoop::IsEleInLoop(const DefLoop& loop,
                                 const DefElem& ele,
                                 double pre,bool& isIn)
{
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        if(loop.m_vElem[i]._type != ele._type){
            continue;
        }
        if(ele._type == LINETYPE){
            Point lSp = ele._segment._segSp;
            Point lEp = ele._segment._segEp;
            Point lSp1 = loop.m_vElem[i]._segment._segSp;
            Point lEp1 = loop.m_vElem[i]._segment._segEp;
            if(!eleInlo_go->IsIdenticalPoint(lSp,lSp1,pre)){
                continue;
            }
            if(!eleInlo_go->IsIdenticalPoint(lEp,lEp1,pre)){
                continue;
            }
        }
        else if(ele._type == ARCTYPE){
            Point startPnt = ele._arc._arcSp;
            Point endPnt = ele._arc._arcEp;
            Point cp = ele._arc._arcCp;
            Point startPnt1 = loop.m_vElem[i]._arc._arcSp;
            Point endPnt1 = loop.m_vElem[i]._arc._arcEp;
            Point cp1 = loop.m_vElem[i]._arc._arcCp;
            if(!eleInlo_go->IsIdenticalPoint(startPnt,startPnt1,pre) &&
                    !eleInlo_go->IsIdenticalPoint(startPnt,endPnt1,pre)){
                continue;
            }
            if(!eleInlo_go->IsIdenticalPoint(endPnt,endPnt1,pre) &&
                    !eleInlo_go->IsIdenticalPoint(endPnt,startPnt1,pre)){
                continue;
            }
            if(!eleInlo_go->IsIdenticalPoint(cp,cp1,pre)){
                continue;
            }
        }
        else if(ele._type == CIRCLETYPE){
            Point cp = ele._circle._cirCp;
            double R = ele._circle._cirR;
            Point cp1 = loop.m_vElem[i]._circle._cirCp;
            double R1= loop.m_vElem[i]._circle._cirR;
            if(std::abs(R - R1) > pre){
                continue;
            }
            if(!eleInlo_go->IsIdenticalPoint(cp,cp1,pre)){
                continue;
            }
        }
        isIn = true;
        return;
    }
}
void AcquireEleLoop::GetLoopFromePartCutData(const vector<vector<BndIslPair> > &modCutData,
                                             const DefElem& ele, bool &isGetLoop,
                                             BndIslPair &pairData)
{
    isGetLoop = false;
    ElemCalculate elecal;
    double zValue;
    GetEleZValue(ele,zValue);
    for (int a = 0; a < modCutData.size(); ++a) {
        for (int l = 0; l < modCutData[a].size(); ++l) {
            BndIslPair biLoop = modCutData[a][l];
            double zValue_;
            zValue_ = elecal.LoopZCoordinate(biLoop.m_bndLoop);
            if(std::abs(zValue - zValue_) > PreErr_2){
                continue;
            }
            IsEleInLoop(biLoop.m_bndLoop,ele,PreErr_2,isGetLoop);
            if(isGetLoop){
                pairData = biLoop;
                pairData.m_areaIdx = a;
                pairData.m_layerIdx = l;
                return;
            }
            for (int lo = 0; lo < biLoop.m_vIslLoop.size(); ++lo) {
                IsEleInLoop(biLoop.m_vIslLoop[lo],ele,PreErr_2,isGetLoop);
                if(isGetLoop){
                    pairData = biLoop;
                    pairData.m_areaIdx = a;
                    pairData.m_layerIdx = l;
                }
                return;
            }
        }
    }
}