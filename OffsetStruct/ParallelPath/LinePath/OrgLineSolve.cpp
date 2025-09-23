#include "OrgLineSolve.h"
#include "../../../../offset2D/ToolTrajectoryAlg/CommonFile/Header.h"
using namespace OftStr;
using namespace GridArea;
using namespace GeomStruct;
using namespace GeomUnit;
#if 0
#pragma optmize("", off)
#pragma GCC optimize ("O0")
#endif
void OrgLineSolve::PointsBoundary(const OftStr::DefLoop& loop,
                                  const IntPnt* const pre,
                                  const IntPnt* const nex,
                                  bool isNex,vector<DefElem>& eles)
{
    DefElem ele;
    ElemCalculate elecal;
    if(pre->_eId == nex->_eId){
        DefElem ele_;
        if(isNex){
            if(loop.IndexElem(pre->_eId).ElemeType() == CIRCLETYPE){
                const DefCircle &c = loop.IndexElem(pre->_eId)._circle;
                eles.emplace_back(DefElem(ARCTYPE,DefArc(pre->P(),nex-
>P(),c._cirCp,
                                                         c._cirR,CLOCKWISE)));
                return;
            }
            elecal.PointSplitElement(loop.IndexElem(pre->_eId),
                                     pre->P(),false,ele);
            elecal.PointSplitElement(ele,nex->P(),true,ele_);
        }
        else{
            if(loop.IndexElem(pre->_eId).ElemeType() == CIRCLETYPE){
                const DefCircle &c = loop.IndexElem(pre->_eId)._circle;
                eles.emplace_back(DefElem(ARCTYPE,DefArc(pre->P(),nex-
>P(),c._cirCp,
                                                         
c._cirR,ANTICLOCKWISE)));
                return;
            }
            elecal.PointSplitElement(loop.IndexElem(pre->_eId),
                                     pre->P(),true,ele);
            elecal.PointSplitElement(ele,nex->P(),false,ele_);
            elecal.ReverseEleDirection(ele_);
        }
        eles.emplace_back(ele_);
        return;
    }
    int id = pre->_eId;
    if(isNex){
        elecal.PointSplitElement(loop.IndexElem(pre->_eId),
                                 pre->P(),false,ele);
        eles.emplace_back(ele);
        while (true) {
            id = loop.NextIndex(id);
            if(id == nex->_eId)
                break;
            eles.emplace_back(loop.IndexElem(id));
        }
        elecal.PointSplitElement(loop.IndexElem(nex->_eId),
                                 nex->P(),true,ele);        eles.emplace_back(ele);
        return;
    }
    elecal.PointSplitElement(loop.IndexElem(pre->_eId),
                             pre->P(),true,ele);
    elecal.ReverseEleDirection(ele);
    eles.emplace_back(ele);
    while(true){
        id = loop.PreviousIndex(id);
        if(id == nex->_eId)
            break;
        ele = loop.IndexElem(id);
        elecal.ReverseEleDirection(ele);
        eles.emplace_back(ele);
    }
    elecal.PointSplitElement(loop.IndexElem(nex->_eId),
                             nex->P(),false,ele);
    elecal.ReverseEleDirection(ele);
    eles.emplace_back(ele);
}