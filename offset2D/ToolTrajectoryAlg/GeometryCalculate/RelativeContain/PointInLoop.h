#ifndef POINTINLOOP_H
#define POINTINLOOP_H
#include "PtInLoopAux.h"
#include"../../../OperateView/OperateView/DisplayGeom.h"
#ifdef emit
#undef emit
#include <tbb/tbb.h>
#define emit Q_EMIT
#else
#include <tbb/tbb.h>
#endif
#define _paralell_inloop 0
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class PointInLoop
{
public:
    PointInLoop(){}
    void NotIntCase(const ofts::DefLoop& loop,size_t i,bool& isInPoly);
    void GetPointInsideLoop(const ofts::DefLoop& loop,
                            const ofts::Point& P,double bPre,
                            bool& isOnBord,bool& isInPoly,bool positive = true);
    InloopInt InLoopIntData(const ofts::DefLoop& loop,
                            const ofts::Point& P,double bPre)
    {
        bool isOnBord = false,isInPoly = false;
        GetPointInsideLoop(loop,P,bPre,isOnBord,isInPoly);
        return _iData;
    }
    void InitParam(double e){
        _iData = InloopInt();
        _iparam._endptPre = PreErr5_8;_iparam._onSegPre = PreErr_10;
        _iparam._onArcPre = PreErr_10;_iparam._lcIntPre = PreErr_10;
        _iparam._lcTanPre = PreErr_10;
        _iData.endPre = _iparam._endptPre;_iData.horPre = PreErr5_8;
        _iData.tanPre = _iparam._lcTanPre;
    }
    //const std::vector<size_t>& IntIds()const{return _iData.iIds;}

    bool IsPointInLoopNeg(const ofts::DefLoop& loop,const ofts::Point& P,
                          double bPre,bool& onBorder);
    bool IsPointInLoop(const ofts::DefLoop& loop,const ofts::Point& P,
                       double bPre,bool& onBorder,bool positive = true);

public:
    ofts::Point _p;
    InloopInt _iData;
    numst::IntParam _iparam;
    bool _isInLoop = false;
    bool _isOnBorder = false;
};
inline void PointInLoop::NotIntCase(const ofts::DefLoop& loop,size_t i,bool& isInPoly)
{
    auto& iData = _iData;
    bool isMax = false;
    if(iData.isComNexSp){
        isMax = PtInLoopAux::IsEndptMaxCoord(loop[i],i,iData,true);
    }else{
        if(i == loop.Size() - 1 && iData.isComLastEp){
            isMax = PtInLoopAux::IsEndptMaxCoord(loop[i],i,iData,false);
        }
    }
    if(isMax){
        iData.iNumber ++;
        isInPoly = !isInPoly;
        const auto& p = iData.isComNexSp ? loop[i].GetSp() : loop[i].GetEp();
        iData.AddIntIds(i,ofts::GeomIntRes(p));
    }
    iData.hasComPreEp = false;
    iData.isComNexSp = false;
    //laterStatus = isInPoly;
    //hasPreChStatus = preStatus == laterStatus ? false : true;
}
#endif // POINTINLOOP_H
