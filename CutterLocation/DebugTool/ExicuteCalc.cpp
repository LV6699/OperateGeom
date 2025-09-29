#include "ExicuteCalc.h"
#include "../../ViewWindow/MainWindow.h"

void ExicuteCalc::LineTriangleClInt(const oft::Point& p)
{
    grm::CutterLocationZ cl;
    const auto& ts = _meshMap.TrianglesCl();
    vector<grm::Triangle>ts_;
    for(auto& t : ts){
        if(!t.IsInRange(p) || t.N().Z() < PreErr_8){continue;}
        ts_.push_back(t);
    }
    double z = Min_Val;
    for(size_t i = 0;i < ts_.size();++i){
        const auto& t = ts_[i];
        double z_ = cl.CutterLocation(t,p);
        if(z < z_){z = z_;}
    }


}

























