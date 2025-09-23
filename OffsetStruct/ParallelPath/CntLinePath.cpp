#include "CntLinePath.h"
#include "CntLinePathAux.h"
#include "../../LeadsAndLinks/Arc/Horizon_arc.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
using namespace std;
void CntLinePath::AddLoopSteerPath(const 
                                   std::vector<JLC_Leads::LeadBase*>paramsIn,
                                   const
                                   std::vector<JLC_Leads::LeadBase*>paramsOut,
                                   const JLC_Leads::ModelProtect &modelp,
                                   const DefLoop& loop,
                                   std::vector<JLC_Wire>& path)
{
    ElemCalculate elecal;
    JLC_Wire* entryWire = nullptr;
#if 0
    string str = OperateOftString().LoopElementString(loop);
    std::cout<<str<<endl;
#endif
    CntLinePathAux caux;
    ///创建切入运动刀路
    caux.LoopSteerEntryPath(paramsIn,loop,modelp,entryWire);
    if(entryWire != nullptr && !entryWire->vCurveList.empty()){
        path.emplace_back(JLC_Wire(entryWire->vCurveList,true,false));
    }
    ///创建切削运动刀路
    vector<JLC_Curve*>cutCurves = JLC_Curve::LoopToJLC_Curves(loop);
    path.emplace_back(JLC_Wire(cutCurves,true,false));
    ///创建切出运动刀路
    JLC_Wire* exitWire = nullptr;
    caux.LoopSteerExitPath(paramsOut,loop,modelp,exitWire);
    if(exitWire != nullptr && !exitWire->vCurveList.empty()){
        path.emplace_back(JLC_Wire(exitWire->vCurveList,true,false));
    }
#if 0
    DefLoop inloop,ouloop;
    if(entryWire != nullptr){
        inloop = JLC_Curve::ConvertJLCurveToLoopData(entryWire->vCurveList);}
    if(exitWire != nullptr){
        ouloop = JLC_Curve::ConvertJLCurveToLoopData(exitWire->vCurveList);}
    std::vector<DefLoop>ls{inloop,loop,ouloop};
    string str1 = OperateOftString().ArrayLoopString(ls);
    std::cout<<str1<<endl;
#endif
}
void CntLinePath::CreatePathNodeConnect(const Connect::connect& cntParam,
                                        const JLC_Leads::ModelProtect& modelp,
                                        const
                                        std::vector<std::vector<JLC_Wire>>& wiress,
                                        JLC_Opera::JLC_ToolPath*& tpath)
{
    Point firpt;
    CntLinePathAux caux;
    vector<JLC_Curve*>firDownCurves;
    if(!caux.FirstDownCutPoint(wiress,firpt)){return;}
    double absSafeH = cntParam.safe_plane.rapidHeight;
    double downCutH = cntParam.safe_plane.plungeHeight;    ///创建首次下切运动刀路
    caux.LiftFeedMovePath(absSafeH,downCutH,true,
                          firpt,true,firDownCurves);
    tpath->push_back(new JLC_Opera::ToolPathNode(
                         new JLC_Wire(firDownCurves,true,false)));
    for (size_t i = 0; i < wiress.size()-1; ++i){
        const std::vector<JLC_Wire>& paths = wiress[i];
        for(size_t t = 0;t < paths.size();++t){
            const auto& cs = paths[t].vCurveList;
            if(!cs.empty()){
                tpath->push_back(new JLC_Opera::ToolPathNode(
                                     new JLC_Wire(cs,true,false)));
            }
        }
        Point curEp = JLC_Curve::ToPoint(tpath->tail->data->
                                         vCurveList.back()->endPnt),nexSp;
        const std::vector<JLC_Wire>& paths_ = wiress[i+1];
        caux.ArrayFirstPoint(paths_,nexSp);
#if 0
        Point ob(14.894499786943195,-5.123099982738495,5.119098663330078);
        Point ob1(56.02894215164899,53.67407612020352,12.432741457030337);
        if(curEp.IsSamePoint3D(ob,PreErr5_6)){
            double dist = curEp.Distance3D(nexSp);
            std::cout<<dist<<endl;
        }
#endif
        if(caux.IsStraightConnect(modelp,cntParam,curEp,nexSp)){
            vector<JLC_Curve*>cnts{
                new JLC_Curve(curEp,nexSp,JLC_path::Lead_in)};
            tpath->push_back(new JLC_Opera::ToolPathNode(
                                 new JLC_Wire(cnts,true,false)));
            continue;
        }
        ///创建向上和向下运动刀路
        vector<JLC_Curve*>upCurves,downCurves,skims;
        caux.LiftFeedMovePath(absSafeH,downCutH,false,curEp,false,upCurves);
        caux.LiftFeedMovePath(absSafeH,downCutH,false,nexSp,true,downCurves);
        skims = vector<JLC_Curve*>{new JLC_Curve(upCurves.back()->endPnt,
                                                 downCurves.front()->startPnt,
                                                 JLC_path::Skim)};
        tpath->push_back(new JLC_Opera::ToolPathNode(
                             new JLC_Wire(upCurves,true,false)));
        tpath->push_back(new JLC_Opera::ToolPathNode(
                             new JLC_Wire(skims,true,false)));
        tpath->push_back(new JLC_Opera::ToolPathNode(
                             new JLC_Wire(downCurves,true,false)));
    }{
        const std::vector<JLC_Wire>& lasPath = wiress.back();
        for(size_t t = 0;t < lasPath.size();++t){
            tpath->push_back(new JLC_Opera::ToolPathNode(
                                 new
                                 JLC_Wire(lasPath[t].vCurveList,true,false)));
        }
        Point curEp_ = JLC_Curve::ToPoint(tpath->tail->data->
                                          vCurveList.back()->endPnt);
        vector<JLC_Curve*>upCurves_/*,skims_*/;
        caux.LiftFeedMovePath(absSafeH,downCutH,false,curEp_,false,upCurves_);/
        **
        gp_Pnt upEp = upCurves_.back()->endPnt;
        gp_Pnt firPt = tpath->head->data->vCurveList.front()->startPnt;
        skims_ = vector<JLC_Curve*>{
                new JLC_Curve(upEp,firPt,JLC_path::Skim)};*/
        tpath->push_back(new JLC_Opera::ToolPathNode(                             new JLC_Wire(upCurves_,true,false)));/**
        tpath->push_back(new JLC_Opera::ToolPathNode(
                             new JLC_Wire(skims_,true,false)));*/
    }
}
///54.365739698791280  -52.640929948265729 10.995744507221790
