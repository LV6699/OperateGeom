#include <tbb/tbb.h>
#include "BaseMinPassage.h"
using namespace oft::ProgOftValue;
#if DebugModel
#pragma optimize("", off)
#endif
void BaseMinPassage::ExploreProbableUnprocess(const DefLoop &loop,
                                            const double R, const double 
oftValue,
                                            const int index,IndexDist 
&idDistance)
{
    if(index >= loop.ElemNum() - 2)
        return;
    GeomDistance godis;
    vector<ValidDist>vIndexDist;
    vIndexDist.resize(loop.ElemNum());
    int i = loop.NextIndex(index);
    i = loop.NextIndex(i);
    DefElem ele = loop.IndexElem(index);
    double thre1 = 2 * R * PreErr_9_,
            thre2 = (2 * oftValue) * 1.1,dErr = R * 0.35;
#if 1
    for (; i < loop.ElemNum(); ++i) {
        if(i == 14){
            int tem = 1;
        }
        double d = 0;
        godis.ElementToElementDistance(ele,loop.IndexElem(i),d);
        if(/*d >= thre1 && */d <= thre2){
            vIndexDist[i]._isValidDist = true;
            vIndexDist[i]._indexDist = std::make_pair(i,d);
            ///idDistance.vIndexDist.push_back(std::make_pair(i,d));
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,loop.ElemNum()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            double d = 0;
            godis.ElementToElementDistance(ele,loop.IndexElem(i),d);
            if(d >= thre1 && d <= thre2){
                vIndexDist[i]._isValidDist = true;
                vIndexDist[i]._indexDist = std::make_pair(i,d);
            }
        }
    });
#endif
    for(auto &d : vIndexDist){
        if(d._isValidDist){
            idDistance._vIndexDist.push_back(d);
        }
    }
    if(idDistance._vIndexDist.empty())
        return;
    MinPassageSolve sol;
    sol.IndexDistanceSort(idDistance._vIndexDist);
    double segStep = R * PreErr_2_,arcStep = 0,k = R * PreErr_2_;
    for (int i = 0; i < idDistance._vIndexDist.size(); ++i) {
        if(idDistance._vIndexDist[i]._indexDist.first == 14){
            int tem = 1;        }
        DefElem ele_ = 
loop.IndexElem(idDistance._vIndexDist[i]._indexDist.first);
        if(ele_.ElemeType() == ARCTYPE){
            ele_._arc.ChoiceSetRadius(PreErr5_2);
            arcStep = (k / (2 * M_PI * ele_._arc.ArcRadius())) * 360;
        }
        else{
            if(std::abs(idDistance._vIndexDist[i]._indexDist.first - index) == 1
||
                    std::abs(idDistance._vIndexDist[i]._indexDist.first - index)
==
                    loop.ElemNum() - 1)
                continue;
        }
        if(sol.IsValidExploreUnprocess(loop,index,segStep,arcStep,idDistance.
                                       
_vIndexDist[i]._indexDist.first,idDistance.
                                       _vIndexDist[i]._indexDist.second,dErr)){
            idDistance.SetIndex(index);
            idDistance.SetMeetDistance(true);
            idDistance.SetToIndex(idDistance._vIndexDist[i]._indexDist.first);
            
idDistance.SetToDistance(idDistance._vIndexDist[i]._indexDist.second);
            return;
        }
    }
}