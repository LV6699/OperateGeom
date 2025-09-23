#ifndef CNTLINEPATHAUX_H
#define CNTLINEPATHAUX_H
#include<vector>
#include "Operation/Base/Operation.h"
class CntLinePathAux
{
public:
    CntLinePathAux(){}
    gp_Pnt ToPnt(const oft::Point& p){
        return gp_Pnt(p.X(),p.Y(),p.Z());
    }
    Point ToPt(const gp_Pnt& p){
        return oft::Point(p.X(),p.Y(),p.Z());
    }
    bool ArrayFirstPoint(const std::vector<JLC_Wire>&paths,Point& p){
        for(size_t t = 0;t < paths.size();++t){
            if(paths[t].vCurveList.empty())
                continue;
            p = JLC_Curve::ToPoint(paths[t].vCurveList[0]->startPnt);
            return true;
        }
        return false;
    }
    bool FirstDownCutPoint(const std::vector<std::vector<JLC_Wire>>&
                           loops,Point& p){
        for(auto& d : loops){
            if (ArrayFirstPoint(d,p)){return true;}
        }
        return false;
    }
    void AddSteerCutPath(const std::array<JLC_Wire,3>& paths,
                         JLC_Opera::JLC_ToolPath*& tpath){
    }
    void LiftFeedMovePath(double absSafeH,
                          double downCutH, bool isFirst,
                          const Point& p, bool isEntry,
                          vector<JLC_Curve *> &curves);
    void LoopSteerEntryPath(const std::vector<JLC_Leads::LeadBase*>& params,
                            const DefLoop& loop,
                            const JLC_Leads::ModelProtect& modelp,
                            JLC_Wire* &path);
    void LoopSteerExitPath(const std::vector<JLC_Leads::LeadBase*>& params,
                           const DefLoop& loop,
                           const JLC_Leads::ModelProtect& modelp,
                           JLC_Wire* &path);
    bool IsStraightConnect(const JLC_Leads::ModelProtect& modelp,
                           const Connect::connect &cntParam,
                           const Point &cntp0, const Point &cntp1);
    
    bool IsSafeDownUpLocation(const std::vector<std::vector<JLC_Wire>>& wires,
                              const std::shared_ptr<cls::model>& clsModel,
                              const Connect::connect& cntParam);
};
#endif // CNTLINEPATHAUX_H
