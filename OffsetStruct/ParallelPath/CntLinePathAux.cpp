#include "CntLinePathAux.h"
#include "../../LeadsAndLinks/Arc/Horizon_arc.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
using namespace std;
void CntLinePathAux::LiftFeedMovePath(double absSafeH,
                                      double downCutH,
                                      bool isFirst,
                                      const Point& p,
                                      bool isEntry,
                                      vector<JLC_Curve*>&curves)
{
    Point safePt = p,downPt = p;
    safePt.SetZ(absSafeH);
    if(isFirst){
        downPt.SetZ(downCutH);
    }else{
        if(isEntry){
            downPt.SetZ(p.Z()+1);
        }
    }
#if 0
    if(absSafeH > downCutH && downCutH > p.Z()){
        curves.push_back(new JLC_Curve(safePt,downPt,JLC_path::Safe));
        curves.push_back(new JLC_Curve(downPt,p,JLC_path::Rapid));
    }else if(downCutH > p.Z()){
        curves.push_back(new JLC_Curve(downPt,p,JLC_path::Rapid));
    }
#else
    if(isEntry){
        curves.push_back(new JLC_Curve(safePt,downPt,JLC_path::Safe));
        curves.back()->curveLength = safePt.Distance3D(downPt);
        curves.push_back(new JLC_Curve(downPt,p,JLC_path::Rapid));
        curves.back()->curveLength = downPt.Distance3D(p);
    }else{
        curves.push_back(new JLC_Curve(p,downPt,JLC_path::Rapid));
        curves.back()->curveLength = p.Distance3D(downPt);
        curves.push_back(new JLC_Curve(downPt,safePt,JLC_path::Safe));
        curves.back()->curveLength = downPt.Distance3D(safePt);
    }
#endif
}
void CntLinePathAux::LoopSteerEntryPath(const std::vector<JLC_Leads::LeadBase*>&
                                        params,
                                        const DefLoop& loop,
                                        const JLC_Leads::ModelProtect& modelp,
                                        JLC_Wire* &path)
{
    path = nullptr;
    JLC_Wire* wire = new JLC_Wire();
    wire->vCurveList = JLC_Curve::LoopToJLC_Curves(loop);
    for (size_t i = 0; i < params.size(); i++){
        if(params[i] == nullptr)
            continue;
        path = params[i]->CreateLoopSteerPath(wire,modelp,true);
        if(path == nullptr)
            continue;
        break;    }
    delete wire;
    wire = nullptr;
#if 0
    if(path != nullptr){
        DefLoop l = JLC_Curve::ConvertJLCurveToLoopData(path->vCurveList);
        string str = OperateOftString().LoopElementString(l);
        std::cout<<str<<endl;
    }
#endif
}
void CntLinePathAux::LoopSteerExitPath(const std::vector<JLC_Leads::LeadBase*>& 
                                       params,
                                       const DefLoop& loop,
                                       const JLC_Leads::ModelProtect& modelp,
                                       JLC_Wire* &path)
{
    path = nullptr;
    JLC_Wire* wire = new JLC_Wire();
    wire->vCurveList = JLC_Curve::LoopToJLC_Curves(loop);
    for (size_t i = 0; i < params.size(); i++){
        if(params[i] == nullptr)
            continue;
        path = params[i]->CreateLoopSteerPath(wire,modelp,false);
        if(path == nullptr)
            continue;
        break;
    }
    delete wire;
    wire = nullptr;
#if 0
    if(path != nullptr){
        DefLoop l = JLC_Curve::ConvertJLCurveToLoopData(path->vCurveList);
        string str = OperateOftString().LoopElementString(l);
        std::cout<<str<<endl;
    }
#endif
}
bool CntLinePathAux::IsStraightConnect(const JLC_Leads::ModelProtect& modelp,
                                       const Connect::connect &cntParam,
                                       const Point& cntp0,const Point& cntp1)
{
    const auto& cnts = cntParam.Links();
    if(cnts.empty() || cnts[0].type != Connect::Type::Straight ||
            cnts[0].constrains.empty()){
        return false;
    }
    if(cntp0.Distance3D(cntp1) > cnts[0].constrains[0].dis){
        return false;
    }
#if 0
    Point pt(15.333479938940616,-15.977110402458205,5.870259947244112);
    Point pt1(16.815079411315857,-14.41065811633992,6.5468649257880385);
    bool is = false,is1 = false;
    is = pt.IsSamePoint3D(cntp0,PreErr5_4) &&
            pt1.IsSamePoint3D(cntp1,PreErr5_4);
    is1 = pt1.IsSamePoint3D(cntp0,PreErr5_4) &&
            pt.IsSamePoint3D(cntp1,PreErr5_4);
    double dist = cntp0.Distance3D(cntp1);
    if(is || is1){
        Point mp = pt.Midpoint(pt1);        std::cout<<dist<<endl;
    }
#endif
    vector<oft::Point>points;
    GeomCalculate().DiscreteLineToPoint3D(cntp0,cntp1,PreErr5_2,points);
    double z = 0;
    ///std::vector<double>errs;
    for(auto& d : points){
        rmath::vec2 p(d.X(),d.Y());
        z = cls::get_cutter_location(p,*modelp.cls_model);
        if(d.Z() < z - PreErr5_3){
            ///errs.push_back(std::abs(d.Z() - z));
            return false;
        }
    }/**
    if(!errs.empty()){
        auto max_it = std::max_element(errs.begin(), errs.end());
        double max_val = *max_it;
        std::cout<<max_val<<endl;
        return false;
    }*/
    return true;
}
bool CntLinePathAux::IsSafeDownUpLocation(const 
                                          std::vector<std::vector<JLC_Wire>>& wires,
                                          const std::shared_ptr<cls::model>&
                                          clsModel,
                                          const Connect::connect& cntParam)
{
    double z = 0,z1 = 0;
    double h = cntParam.safe_plane.plungeHeight;
    vector<oft::Point>points;
    for(const auto& d : wires){
        for(const auto& w : d){
            const auto& curves = w.vCurveList;
            if(curves.empty())
                continue;
            const auto& s = curves.front()->startPnt;
            const auto& e = curves.back()->endPnt;
            rmath::vec2 s_(s.X(),s.Y());
            rmath::vec2 e_(e.X(),e.Y());
            z = cls::get_cutter_location(s_,*clsModel);
            z1 = cls::get_cutter_location(e_,*clsModel);
            if(h < z - PreErr5_3 || h < z1 - PreErr5_3){
                return false;
            }
        }
    }
    return true;
}
