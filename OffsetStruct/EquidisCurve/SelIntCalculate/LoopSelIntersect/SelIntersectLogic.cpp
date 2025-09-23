#include <tbb/tbb.h>
#include "SelIntersectLogic.h"
#include "../../../../OperateView/OperateView/DisplayGeom.h"
#if DebugModel
#pragma optimize("", off)
#endif
void SelIntersectLogic::AddIndexElement(const DefLoop& loop,
                                        vector<vector<int>>vArrIndex,
                                        vector<DefLoop>&vResLoop)
{
    for (int i = 0; i < vArrIndex.size(); ++i) {
        DefLoop loop_ = loop;
        loop_.ClearElem();
        for (int j = 0; j < vArrIndex[i].size(); ++j) {
            int index = vArrIndex[i][j];
            if(loop.IsValIndex(index)){
                loop_.PushBackElem(loop.IndexElem(index));
            }
        }
        if(!loop_.IsEmpty()){
            vResLoop.push_back(loop_);
        }
    }
}
void SelIntersectLogic::ClipSelfIntersectLoop(const DefLoop &oriLoop,
                                              const DefLoop &loop,
                                              double offsetVal,
                                              bool& isIntersect,
                                              DefLoop& resLoop)
{
    resLoop = loop;
    resLoop.ClearElem();
    ClosestSelInt cloInt;LoopSelIntSolve selsol;
    ElemCalculate elecal;
    /*auto arc1 = loop[10].ElemArc();
    auto sp = arc1._arcSp,ep = arc1._arcEp;
    loop[10] = DefElem(LINETYPE,DefSeg(sp,ep));
    auto arc2 = loop[11].ElemArc();
    double a1,a2;
    double dis1 = arc1._arcSp.Distance2D(arc1._arcEp);
    GeomCalculate().ArcCenterAngle(arc1,a1);
    double dis2 = arc2._arcSp.Distance2D(arc2._arcEp);
    GeomCalculate().ArcCenterAngle(arc2,a2);*/
    for (int i = 0; i < loop.ElemNum(); ++i){/**
        if(i == 12 || i == 31){
            int tem = 1;
        }*/
        vector<Point>points;
        const auto& ele = loop[i];
        cloInt.ElementSelIntersectPoint(loop,i,points);
        if (points.empty()){
            resLoop.PushBack(ele);
            continue;
        }
        isIntersect = true;
        vector<DefElem>elems;
        elecal.DeduplicationPoint(points,PreErr5_10);
        selsol.ArrayPointSliptElement(ele,points,elems);/**        selsol.HandleInterfereElement(oriLoop,elems,
                                      offsetVal,PreErr_3);*/
        selsol.SpecifyInvalidClipElem(elems);
        for (int e = 0; e < elems.size(); ++e) {
            elems[e]._elemLabel._isClipEle = true;
            resLoop.m_vElem.push_back(elems[e]);
        }
    }
}
void SelIntersectLogic::SplitSelfIntersectLoop(const DefLoop &oriLoop,
                                               DefLoop loop,
                                               double offsetVal,
                                               bool &isIntersect,
                                               DefLoop &resLoop)
{
    resLoop = loop;
    resLoop.ClearElem();
    ClosestSelInt cloInt;LoopSelIntSolve selsol;
    for (int i = 0; i < loop.ElemNum(); ++i) {/**
        if(i == 1066 ){
            int tem = 1;
        }*/
        vector<Point> vPoint;
        DefElem ele = loop.IndexElem(i);
        cloInt.ElemSelIntersectPoint(loop,i,vPoint);
        if (vPoint.empty()){
            resLoop.m_vElem.push_back(ele);
            continue;
        }
        isIntersect = true;
        vector<DefElem> vElem;
        selsol.ArrayPointSliptElement(ele,vPoint,vElem);
        selsol.HandleInterfereElement(oriLoop,vElem,
                                      offsetVal,PreErr_3);
        for (int e = 0; e < vElem.size(); ++e) {
            vElem[e]._elemLabel._isClipEle = true;
            resLoop.m_vElem.push_back(vElem[e]);
        }
    }
}
void SelIntersectLogic::DeleteInvalidClipElement(const DefLoop & oriLoop,
                                                 double offsetVal,
                                                 bool isOpenLoop,
                                                 bool& isExiDelete,
                                                 vector<DefElem>& elems)
{
    isExiDelete = false;
    if(oriLoop.IsEmpty() || elems.empty())
        return;
    vector<int>indexs;
    SelIntersectAux exesel;LoopSelIntSolve selsol;/**
    ///DisplayGeom().DisplayLoop(oriLoop,_colors.back(),1,true,false);
    ///DisplayGeom().DisplayLoop(DefLoop(elems),_colors[1],1,true,false);
    DefArc arc1 = oriLoop[0].ElemArc();
    DefArc arc2 = elems[1].ElemArc();
    double ddd;
    //DisplayGeom().DisplayArc(arc1,_colors.back(),2,true,false);
    ///DisplayGeom().DisplayPoint(arc1._arcSp,_colors.back());
    //DisplayGeom().DisplayArc(arc2,_colors[1],2,true,false);    ///DisplayGeom().DisplayPoint(arc2._arcSp,_colors.back());
    GeomDistance().ArcArcDistance(arc1,arc2,ddd);*/
    if(!isOpenLoop){
#if 0
        GeomDistance godis;
        vector<double>dists;
        for (int i = 0; i < elems.size(); ++i) {
            auto& ele = elems[i];
            if(!ele.IsValid() || ele._elemLabel._isAbsValid)
                continue;
            for (int j = 0; j < oriLoop.Size(); ++j) {
                if(i == 1 && j == 0){
                    int tem = 1 + 1;
                }
                double dist = 0;
                godis.ElementToElementDistance(ele,oriLoop[j],dist);
                dists.emplace_back(dist);
                if(dist + SeIn_Dis_Err < offsetVal){
                    ele.SetValidity(false);
                    break;
                }
            }
        }
#else
        tbb::parallel_for(tbb::blocked_range<std::size_t>(0, elems.size()),
                          [&](tbb::blocked_range<std::size_t>& r) {
            for (std::size_t i = r.begin(); i < r.end(); ++i) {
                if(!elems[i]._elemLabel._isValidity ||
                        elems[i]._elemLabel._isAbsValid)
                    continue;
                const auto& ele = elems[i];
                EleLoopDisType disType;
                exesel.ElemToLoopDistanceType(ele,oriLoop,offsetVal,disType);
                if(disType != EntiretyOutside){
                    elems[i].SetValidity(false);
                }
            }
        });
#endif
        for (int i = 0; i < elems.size(); ++i){
            if(!elems[i]._elemLabel._isValidity){
                indexs.push_back(i);
            }
        }
        if(!indexs.empty()){
            isExiDelete = true;
            ElemCalculate().DeleteIndexElement(indexs,elems);
        }
        return;
    }
    int lenth = elems.size();
    for (int id = 0; id < lenth; ++id) {/**
            if(i == 171){
                int tem = 1;
            }*/
        if(!elems[id]._elemLabel._isValidity){
            indexs.push_back(id);
            continue;
        }
        DefElem ele = elems[id];        EleLoopDisType disType;
        exesel.ElemToLoopDistanceType(ele,oriLoop,offsetVal,disType);
        if(disType != PartOutside){
            if(disType == EntiretyInside)
                indexs.push_back(id);
            continue;
        }
        if(ele._elemLabel._isConnectArc){
            indexs.push_back(id);
            continue;
        }
        vector<DefElem>vElem_;
        selsol.LoopEnvelopClipElement(oriLoop,ele,offsetVal,vElem_);
        if(vElem_.empty()){
            indexs.push_back(id);
            continue;
        }
        elems.erase(elems.begin() + id);
        lenth --;
        for (int e = 0; e < vElem_.size(); ++e) {
            elems.insert(elems.begin() + id + e,vElem_[e]);
        }
        id += vElem_.size();
        lenth += vElem_.size();
    }
    if(!indexs.empty()){
        isExiDelete = true;
        ElemCalculate().DeleteIndexElement(indexs,elems);
    }
}
void SelIntersectLogic::SplitElementLocalLoop(DefLoop loop,
                                              vector<DefLoop>&vResLoop)
{
    MathCalculate matcal;
    LoopSelIntSolve selsol;
    bool isOpeLoo = loop.IsOpenLoop();
    vector<DefElem>vElem = loop.LoopElem();
    vector<vector<int>>vArrIndex;
    for (int i = 0; i < vElem.size(); ++i) {
        if(matcal.IsValueIn3DArray(vArrIndex,i))
            continue;
        vector<int>vIndex{i};
        selsol.LocalSelIntLoopIndex(vElem[i],vElem,vArrIndex,
                                    isOpeLoo,i,vIndex);
        vArrIndex.push_back(vIndex);
    }
    AddIndexElement(loop,vArrIndex,vResLoop);
}
void SelIntersectLogic::OrgnizeElementToLoop(DefLoop loop,bool isOpen,
                                             vector<DefLoop>&vResLoop)
{
    LoopSelIntSolve selsol;MathCalculate matcal;
    ///OperateOftString().PrintLoopData(loop,false,0);
    vector<DefElem>vElem = loop.LoopElem();
    vector<vector<int>>vArrIndex;
    for (int i = 0; i < vElem.size(); ++i) {
        if(vElem[i]._elemLabel._eleHasMer)
            continue;
        vector<int>vIndex{i};
        ///bool isValidIndex = false;        vElem[i]._elemLabel._eleHasMer = true;
        selsol.CreateLocalIntersectLoop(i,vElem,isOpen,vIndex);
        if(!isOpen && !selsol.IsCloseIndexLoop(vElem,vIndex)){
            for (int j = 0; j < vElem.size(); ++j){
                if(j != i && !matcal.IsValueIn3DArray(vArrIndex,j))
                    vElem[j]._elemLabel._eleHasMer = false;
            }
            continue;
        }
        vArrIndex.push_back(vIndex);
    }
    AddIndexElement(loop,vArrIndex,vResLoop);
}