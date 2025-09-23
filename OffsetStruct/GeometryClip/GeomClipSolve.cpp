#include "GeomClipSolve.h"
#include "../CommonFile/NumStruct.h"
#include"../OperateElem/LoopEleHandle/ReconstructLoop.h"
void GeomClipSolve::IslCircleLoopCoincide(const DefLoop& loop1,
                                          const DefLoop& loop2,
                                          bool &isCirCoin)
{
    ///需要保证bndLoop和islLoop是圆组成.
    isCirCoin = false;
    if(loop1.IsEmpty() || loop2.IsEmpty())
        return;
    Point cp1,cp2;
    double R1,R2;
    if(loop1.ElemNum() == 1){
        cp1 = loop1.m_vElem[0]._circle._cirCp;
        R1 = loop1.m_vElem[0]._circle._cirR;
    }
    else{
        cp1 = loop1.m_vElem[0]._arc._arcCp;
        R1 = loop1.m_vElem[0]._arc._arcR;
    }
    if(loop2.ElemNum() == 1){
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
        IslCircleLoopCoincide(temLoop,vLoop[lo],isExiCoin);        if(isExiCoin){
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
void GeomClipSolve::HandleCoincideLoops(vector<DefLoop>& loops)
{
    ReconstructLoop rel;
    NumStruct::ReLoopErr err(PreErr5_8,PreErr5_8,
                             PreErr5_8,PreErr5_5);
    for(auto& l : loops){
        rel.OperateReconstructLoop(err,l);
    }
    size_t i = 0;
    std::vector<DefLoop>ls;
    while (i < loops.size()) {
        ls.clear();
        const auto& l = loops[i];
        for (size_t j = 0; j < loops.size(); ++j) {
            if(j != i){
                ls.emplace_back(loops[j]);
            }
        }
        if(IsCoincideWithOthers(l,ls)){
            loops.erase(loops.begin() + i);
            continue;
        }
        i ++;
    }
}