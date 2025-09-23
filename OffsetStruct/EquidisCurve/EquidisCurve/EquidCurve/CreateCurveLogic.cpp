#include "CreateCurveLogic.h"
#include "../../../../OperateView/OperateView/DisplayGeom.h"
#if 1
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
void CreateCurveLogic::OperateInvalidArc(vector<DefElem>& elems)
{
    ///DisplayGeom().DisplayLoop(DefLoop(elems),_colors[1],1,true,false);
    GeomCalculate gocal;
    vector<int>ids;
    for(size_t i = 0;i < elems.size();++i){
        if(elems[i].ElemeType() != OffsetEnum::ARCTYPE)
            continue;
        auto& arc = elems[i]._arc;
        if(!arc._arcSp.IsSamePoint2D(arc._arcEp,PreErr_4))
            continue;
        double agl = 0;
        gocal.ArcCenterAngle(arc,agl);
        if(agl > PreErr5_4 && agl < 360 - PreErr5_4)
            continue;
        if(agl >= 360 - PreErr5_4 && !elems[i].IsCntCurve())
            continue;
        const auto& sp = arc._arcSp;
        const auto& ep = arc._arcEp;
        if(sp.IsSamePoint2D(ep,PreErr_12)){
            ids.emplace_back(static_cast<int>(i));
            continue;
        }
        elems[i].SetElemType(OffsetEnum::LINETYPE);
        elems[i].ChangeEndpoint(sp,false);
        elems[i].ChangeEndpoint(ep,true);
    }
    if(!ids.empty()){
        ElemCalculate().DeleteIndexElement(ids,elems);
    }
}
void CreateCurveLogic::CalculateEquidistanceCurve(const 
vector<DefElem>&oriElems,
                                                  ProfType proType,
                                                  double offsetVal,
                                                  vector<DefElem>& elems)
{
    ///DisplayGeom().DisplayLoop(DefLoop(vOriElem),_colors.back(),1,true,false);
    EquidistantCurve equ;CreateCurveSolve cursol;
    for (int i = 0; i < oriElems.size(); ++i) {/**
        if(vElem.size() == 9){
            int tem = 1;
        }*/
        DefElem equEle;
        bool isExiEqud = false;
        equ.EquidistantElement(oriElems[i],offsetVal,
                               proType,isExiEqud,equEle);/**
        cursol.ArcGeomInfo(vOriElem[i].ElemArc());
        cursol.ArcGeomInfo(equEle.ElemArc());*/
        if(isExiEqud){
            equEle.SetSourceIndex(i);
            elems.push_back(equEle);
            continue;        }
        if(/*!cursol.IsCreateDoubleArc(vOriElem,vElem,proType,offsetVal,i)
                                                        || 
*/oriElems[i].ElemeType() != ARCTYPE)
            continue;
        DefArc arc1,arc2;
        cursol.NotExistEquidCurve(oriElems[i].ElemArc(),
                                  offsetVal,arc1,arc2);
        DefElem elem1(ARCTYPE,arc1),elem2(ARCTYPE,arc2);
        ///DisplayGeom().DisplayArc(arc1,_colors[1],2,true,false);
        ///DisplayGeom().DisplayArc(arc2,_colors[2],2,true,false);
        elem1.SetSourceIndex(i);
        elem2.SetSourceIndex(i);
        elems.push_back(elem1);
        elems.push_back(elem2);
    }
    ///DisplayGeom().DisplayLoop(DefLoop(vElem),_colors[1],1,true,false);
}
void CreateCurveLogic::ClosedEquidistanceCurve(const DefLoop &oriLoop,
                                               vector<DefElem> &equEles,
                                               double offsetVal,
                                               ProfType proType,
                                               vector<DefElem> &elems)
{
#define view 0
    vector<DefElem>equEles_ = equEles;
    ElemCalculate elecal;GeomIntersect goInt;
    CreateCurveSolve sursol;///GeomCalculate gocal;
    for(size_t i = 0;i < equEles.size();++i) {/**
        if(i == 15){
            int tem = 1;
        }*/
        if(oriLoop.IsOpenLoop() && i >= equEles.size() - 1){
            if(elecal.IsValidGeomeElement(equEles[i],PreErr5_5,
                                          PreErr5_5,0,PreErr5_5))
                elems.push_back(equEles[i]);
            break;
        }
        GeomIntRes intRes;
        int nex = elecal.NextIndex(equEles.size(),i);
        auto& ele1 = equEles[i];
        auto& ele2 = equEles[nex];
        const auto& sp1 = ele1.StarPt();
        const auto& ep1 = ele1.EndPt();
        const auto& sp2 = ele2.StarPt();
        if(ep1.IsSamePoint2D(sp2,PreErr_6)){
            ///double d = ep1.Distance2D(sp2);
            if(ep1.IsSamePoint2D(sp2,PreErr5_10)){
                Point mp = ep1.Midpoint(sp2);
                sursol.SetEquedElemEndpoint(mp,sp1,equEles[i],
                                            equEles[nex],elems);
            }else{
                sursol.ChangeEndpointCase(ele1,ele2,ep1,sp2,elems);
            }
#if view
            DisplayGeom().DisplayLoop(DefLoop(elems),_colors[2],1,true,false);
#endif
            continue;
        }/**
        sursol.ArcGeomInfo(oriLoop.IndexElem(i).ElemArc());        sursol.ArcGeomInfo(oriLoop.IndexElem(nex).ElemArc());
        sursol.ArcGeomInfo(ele1.ElemArc());
        sursol.ArcGeomInfo(ele2.ElemArc());*/
        goInt.CalculateCurveIntersect(ele1,ele2,true,PreErr5_14,
                                      PreErr5_10,PreErr5_10,intRes);
        if(intRes._isExistInt && (intRes._intNum == 1)){
            sursol.SetEquedElemEndpoint(intRes.P1(),sp1,equEles[i],
                                        equEles[nex],elems);
#if view
            DisplayGeom().DisplayLoop(DefLoop(elems),_colors[2],1,true,false);
            std::cout<<elems.size()<<endl;
#endif
            continue;
        }
        if(!sp1.IsSamePoint2D(ep1,PreErr5_10)){
            elems.push_back(ele1);
#if view
            DisplayGeom().DisplayLoop(DefLoop(elems),_colors[2],1,true,false);
            std::cout<<elems.size()<<endl;
#endif
        }
        DefArc arc;
        int id = ele1.SourceIndex();
        Point cp = oriLoop[id].EndPt();
        ///此时ele1可能端点重合,计算arc方向时可能结果错误,因此引入
        ///equEles_[i] 计算arc方向.
        sursol.CreateElemConnectArc(equEles_[i],ele1,ele2,
                                    cp,offsetVal,proType,arc);
#if 0
        double agl = 0;
        gocal.ArcCenterAngle(arc,agl);
        if(std::abs(agl-360) < PreErr5_3){
            if(!elems.empty()){elems.pop_back();}
            Point mp = ep1.Midpoint(sp2);
            sursol.SetEquedElemEndpoint(mp,sp1,equEles[i],
                                        equEles[nex],elems);
#if 1
            DisplayGeom().DisplayLoop(DefLoop(elems),_colors[1],1,true,true);
            std::cout<<elems.size()<<endl;
#endif
            continue;
        }
#endif
        DefElem ele_(ARCTYPE,arc);
        ele_.SetCntCurve(true);
        elems.push_back(ele_);
#if view
        DisplayGeom().DisplayLoop(DefLoop(elems),_colors[2],1,true,false);
        std::cout<<elems.size()<<endl;
#endif
    }
#if view
    DisplayGeom().DisplayLoop(DefLoop(elems),_colors[2],1,true,true);
    std::cout<<elems.size()<<endl;
#endif
    if(elems.empty() || oriLoop.IsOpenLoop())
        return;
    const auto& ep = elems.back().EndPt();
    elems[0].ChangeEndpoint(ep,false);
}