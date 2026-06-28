#include "GeomClipSolve.h"
#include "../CommonFile/NumStruct.h"
#include"../OperCurve2D/OperCurve2D/ReconstructLoop.h"
using namespace std;
using namespace NumStruct;
using namespace OffsetStruct ;
#if 0
#pragma optimize("", off)
#endif
void GeomClipSolve::IslCircleLoopCoincide(const DefLoop& loop1,
                                          const DefLoop& loop2,
                                          bool &isCirCoin)
{
    ///需要保证bndLoop 和islLoop 是圆组成.
    isCirCoin = false;
    if(loop1.IsEmpty() || loop2.IsEmpty())
        return;
    Point cp1,cp2;
    double R1,R2;
    if(loop1.Size() == 1){
        cp1 = loop1.m_vElem[0]._circle._cirCp;
        R1 = loop1.m_vElem[0]._circle._cirR;
    }
    else{
        cp1 = loop1.m_vElem[0]._arc._arcCp;
        R1 = loop1.m_vElem[0]._arc._arcR;
    }
    if(loop2.Size() == 1){
        cp2 = loop2.m_vElem[0]._circle._cirCp;
        R2 = loop2.m_vElem[0]._circle._cirR;
    }
    else{
        cp2 = loop2.m_vElem[0]._arc._arcCp;
        R2 = loop2.m_vElem[0]._arc._arcR;
    }
    if(!GeomCalculate().IsIdenticalPoint(cp1,cp2,PreErr5_10))
        return;
    if(!MathCalculate().IsNumericEqual(R1,R2,PreErr5_10))
        return;
    isCirCoin = true;
}
void GeomClipSolve::SubtractCircleCoincide(const DefLoop& temLoop,
                                           vector<DefLoop>& vLoop,
                                           bool &isCoincide)
{
    if(temLoop.IsEmpty())
        return;
    GeometryLogic golg;
    bool isCircleLoop = false;
    golg.IsCircleStructureLoop(temLoop,true,isCircleLoop);
    if(!isCircleLoop)
        return;
    int lo = 0;
    while (lo < vLoop.size()) {
        if(vLoop[lo].IsEmpty()){
            vLoop.erase(vLoop.begin() + lo);
            continue;
        }
        bool isCircle = false;
        golg.IsCircleStructureLoop(vLoop[lo],true,isCircle);
        if(!isCircle){
            lo ++;
            continue;
        }
        bool isExiCoin = false;
        IslCircleLoopCoincide(temLoop,vLoop[lo],isExiCoin);
        if(isExiCoin){
            vLoop.erase(vLoop.begin() + lo);
            isCoincide = true;
            continue;
        }
        lo ++;
    }
}
void GeomClipSolve::HandleSubtractCoincide(const DefLoop& temLoop,
                                           vector<DefLoop>& vLoop)
{
    bool isCoincide = false;
    SubtractCircleCoincide(temLoop,vLoop,isCoincide);
}
bool GeomClipSolve::IsCoincideWithOthers(const DefLoop& loop,
                                         const std::vector<DefLoop>& loops)
{
    ReconstructLoop rel;
    for(auto& l : loops){
        if(rel.IsCoincideLoop(loop,l))
            return true;
    }
    return false;
}
void GeomClipSolve::DeleteCoincideLoops(DefLoop loop,vector<DefLoop>& loops,
                                        ReLoopErr err)
{
    auto ls = loops;
    ReconstructLoop rel;
    rel.OperateReconstructLoop(err,loop);
    for(auto& l : ls){
        rel.OperateReconstructLoop(err,l);
    }
    HandleCoincideLoops(ls,err,false);
    vector<size_t>ids;
    for (size_t i = 0; i < ls.size(); ++i) {
        if(rel.IsCoincideLoop(loop,ls[i])){
            ids.push_back(i);
        }
    }
    if(ids.empty()){return;}
    vector<DefLoop>ls_;
    for (size_t i = 0; i < loops.size(); ++i) {
        bool is = false;
        for(auto& d : ids){
            if(i == d){
                is = true;break;
            }
        }
        if(!is){ls_.push_back(loops[i]);}
    }
    loops = ls_;
}
void GeomClipSolve::HandleCoincideLoops(vector<DefLoop>& loops,
                                        ReLoopErr err,bool isRestruct)
{
    ReconstructLoop rel;
    auto ls = loops;
    if(isRestruct){
        for(auto& l : ls){
            rel.OperateReconstructLoop(err,l);
        }
    }
    vector<size_t>ids;
    for (size_t i = 0; i < ls.size(); ++i) {
        for (size_t j = i+1; j < ls.size(); ++j) {
            if(rel.IsCoincideLoop(ls[i],ls[j])){
                ids.push_back(i);break;
            }
        }
    }
    if(ids.empty()){return;}
    vector<DefLoop>ls_;
    for (size_t i = 0; i < loops.size(); ++i) {
        bool is = false;
        for(auto& d : ids){
            if(i == d){
                is = true;break;
            }
        }
        if(!is){ls_.push_back(loops[i]);}
    }
    loops = ls_;
}
