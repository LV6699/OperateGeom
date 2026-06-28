#include "OrgLineSolve.h"
#include "../../OperateView/DisplayGeom.h"
#include "../../offset2D/ToolTrajectoryAlg/CommonFile/Header.h"

using namespace ofts;
using namespace GridArea;
using namespace GeomStruct;
using namespace GeomUnit;

#if 0
#pragma optmize("", off)
#pragma GCC optimize ("O0")
#endif

void OrgLineSolve::PointsBoundary(const ofts::DefLoop& loop,
                                  const IntPnt* const pre,
                                  const IntPnt* const nex,
                                  bool isNex,vector<DefElem>& eles)
{
    DefElem ele;
    int pid = pre->_eId,nid = nex->_eId;
    if(pid == nid){
        ele = loop[pid];
        if(isNex){
            if(loop[pid].Type() == Ofte::CIRCLETYPE){
                const auto &c = loop[pid]._circle;
                ofts::DefArc a(pre->P(),nex->P(),c.GetCp(),c.R(),true);
                eles.emplace_back(DefElem(ARCTYPE,a));
                return;
            }
            ele.ChangeEndpt(pre->P(),false);
            ele.ChangeEndpt(nex->P(),true);
        }else{
            if(loop[pid].Type() == Ofte::CIRCLETYPE){
                const auto &c = loop[pid]._circle;
                ofts::DefArc a(pre->P(),nex->P(),c.GetCp(),c.R(),false);
                eles.emplace_back(DefElem(Ofte::ARCTYPE,a));
                return;
            }
            ele.ChangeEndpt(pre->P(),true);
            ele.ChangeEndpt(nex->P(),false);
            ele.Reverse();
        }
        eles.emplace_back(ele);
        return;
    }
    int id = pre->_eId;
    if(isNex){
        ele = loop[pid];
        ele.ChangeEndpt(pre->P(),false);
        eles.emplace_back(ele);
        while (true) {
            id = loop.NexId(id);
            if(id == nid)
                break;
            eles.emplace_back(loop[id]);
        }
        ele = loop[nid];
        ele.ChangeEndpt(nex->P(),true);
        eles.emplace_back(ele);
        return;
    }
    ele = loop[pid];
    ele.ChangeEndpt(pre->P(),true);
    ele.Reverse();
    eles.emplace_back(ele);
    while(true){
        id = loop.PreId(id);
        if(id == nid)
            break;
        ele = loop[id];
        ele.Reverse();
        eles.emplace_back(ele);
    }
    ele = loop[nid];
    ele.ChangeEndpt(nex->P(),false);
    ele.Reverse();
    eles.emplace_back(ele);
}