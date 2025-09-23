#ifndef PARALLELFINISHING_H
#define PARALLELFINISHING_H
#include "../Base/Operation.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
namespace JLC_Opera{
class ParallelFinishingParam : public MachiningParam{
public:
    ParallelFinishingParam() {}
public:
    double _agl = 0;
    bool _isOneDir = false;
    bool _isAglDir = false;
};
class ParallelFinishing : public Operation
{
public:
    gp_Pnt ToPnt(const oft::Point& p){
        return gp_Pnt(p.X(),p.Y(),p.Z());
    }
    oft::Point ToPt(const gp_Pnt& p){
        return oft::Point(p.X(),p.Y(),p.Z());
    }
    ParallelFinishing(ParallelFinishingParam* param,
                      const string& operateName);
    bool InitialParameters();
    virtual bool CheckMachiningParam() override;
    virtual JLC_ToolPath* CreateToolPath() override ;
    virtual OperationType getType() override{return
                OperationType::ParallelFinishing;}
    void WiresToGeomArea(const vector<JLC_Wire>& wires,
                         vector<GeomArea>& areas);
    void CreatePathRange(vector<DefLoop>& loops);
    void GenerateToolPaths() override;
    void AnalysisPathToAllMachiningArea(
            const std::vector<std::vector<DefLoop>>&loopss);
    void ConnectLinePath(vector<vector<DefLoop>>&loopss,
                         JLC_ToolPath *&tpath);
public:
    
    cls::cutter _clsTool;
    TopoDS_Shape _curveModel;
    ParallelFinishingParam _pathParam;
    std::shared_ptr<cls::model> _clsModel = nullptr;
    std::vector<JLC_Wire>_wires;
    DefLoop _blankLoop;
    std::vector<oft::GeomArea>_areas;
};
}
#endif // PARALLELFINISHING_H
