#include "ExicuteCalc.h"
#include "../../ViewWindow/MainWindow.h"

void ExicuteCalc::LineTriangleClInt(const ofts::Point& p)
{
    grm::CutLocateZ cl;
    const auto& ts = _meshMap.TrianglesCl();
    vector<grm::Triangle>ts_;
    for(auto& t : ts){
        if(!t.IsInRange(p) || t.N().Z() < PreErr_8){continue;}
        ts_.push_back(t);
    }
    double z = Min_Val;
    for(size_t i = 0;i < ts_.size();++i){
        const auto& t = ts_[i];
<<<<<<< HEAD
        double z_ = cl.ProtectFaceZ(t,p);
=======
        double z_ = cl.ProtectFaceZ(_meshMap.Tool(),t,p);
>>>>>>> d8e13c22dbd9b21918161bd2a0a3146af0ea6396
        if(z < z_){z = z_;}
    }


}
void ExicuteCalc::GetRelaTris(double x,double y,
                              std::vector<grm::Triangle>& ts)
{


}

























