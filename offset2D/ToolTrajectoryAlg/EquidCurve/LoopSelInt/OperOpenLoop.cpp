#include "OperOpenLoop.h"
#include "../../OperCurve2D/OperCurve2D/OperateCurve.h"
#include "../../GeometryClip/ClipIntersect/CurveIntClip.h"
void OperOpenLoop::CreateEndClipArc(const ofts::DefLoop& oloop,
                                    const ofts::DefElem& sc,
                                    const ofts::DefElem& ec,double R,
                                    ofts::DefArc& arc1,
                                    ofts::DefArc& arc2)
{
    if(oloop.IsEndToEnd(PreErr5_10)){return;}
    const auto& cp1 = oloop[0].GetSp();
    const auto& cp2 = oloop.Last().GetEp();
    bool isCW1 = false,isCW2 = false;

    if(sc.Type() == Ofte::LINETYPE){
        isCW1 = BaseGeom2D::IsTriaRotDireCW(cp1,sc.GetEp(),sc.GetSp());
    }else if(sc.Type() == Ofte::ARCTYPE){
        isCW1 = !sc.GetArc().IsCW();
    }else{return;}
    if(ec.Type() == Ofte::LINETYPE){
        isCW2 = BaseGeom2D::IsTriaRotDireCW(cp2,ec.GetSp(),ec.GetEp());
    }else if(ec.Type() == Ofte::ARCTYPE){
        isCW2 = ec.GetArc().IsCW();
    }else{return;}
    auto ep1 = BaseGeom2D::PointRotate2D(sc.GetSp(),cp1,180,isCW1);
    arc1 = ofts::DefArc(sc.GetSp(),ep1,cp1,isCW1);
    arc1.Reverse();
    auto ep2 = BaseGeom2D::PointRotate2D(ec.GetEp(),cp2,180,isCW2);
    arc2 = ofts::DefArc(ec.GetEp(),ep2,cp2,isCW2);
}
void OperOpenLoop::MergeOpenLoops(ofts::DefLoop& loop,double e)
{
    if(loop.Size() < 2){return;}
    for (size_t i = 0; i < loop.Size(); i++){loop[i].SetId(i);}
    struct EndCnt{
        EndCnt(){}
        EndCnt(size_t i) : id(i){}
        size_t id,sid,eid;
        bool ssp = false,esp = false;  //是否与sid 和eid 对应曲线起点相连
        bool issp = false,isep = false;
        bool IsDoubCnt()const{return issp && isep;}
    };
    std::vector<EndCnt> cnts;cnts.reserve(loop.Size());
    for (size_t i = 0; i < loop.Size(); i++){
        cnts.emplace_back(EndCnt(i));
    }
    //先判断相连曲线连接性
    for (size_t i = 0; i < cnts.size()-1; i++){
        auto& cnt = cnts[i];
        auto& id = cnt.id;
        auto i1 = cnts[pnum::NexId(cnts.size(),i)].id;
        auto& cnt1 = cnts[i1];
        if(loop[id].Endpoint(true).IsEqual(loop[id].Endpoint(false),e)){
            cnt.eid = i1;cnt.ssp = true;
            cnt1.sid = id;cnt1.esp = false;
        }
    }

} 
void OperOpenLoop::MergeOpenLoops(std::vector<ofts::DefLoop>& loops,double e)
{
    for (size_t i = 0; i < loops.size(); i++){loops[i].SetId(i);}
    for (size_t i = 0; i < loops.size(); i++){
        auto& loop = loops[i];
        if(loop.IsEndToEnd(e)){continue;}
        if(!loop.IsValid()){continue;}
        auto& sp = loop.Endpoint(false);
        auto& ep = loop.Endpoint(true);
        for (size_t j = 0; j < loops.size(); j++){
            if(i == j){continue;}
            auto& l = loops[j];
            if(!l.IsValid()){continue;}
            auto& sp1 = l.Endpoint(false);
            auto& ep1 = l.Endpoint(true);
            if(ep.IsEqual(sp1,e)){
                loop.Reserve(loop.Size() + l.Size());
                for(auto& c : l){loop.AddElem(c);}
                l.SetValid(false);
            }else if(ep.IsEqual(ep1,e)){
                l.Reverse();
                loop.Reserve(loop.Size() + l.Size());
                for(auto& c : l){loop.AddElem(c);}
                l.SetValid(false);
            }else if(sp.IsEqual(sp1,e)){
                loop.Reverse();
                loop.Reserve(loop.Size() + l.Size());
                for(auto& c : l){loop.AddElem(c);}
                l.SetValid(false);
            }else if(sp.IsEqual(ep1,e)){
                l.Reserve(loop.Size() + l.Size());
                for(auto& c : loop){l.AddElem(c);}
                loop.SetValid(false);
            }
            //std::cout<<"";
        }
    }
    loops.erase(std::remove_if(loops.begin(),loops.end(),[](const ofts::DefLoop& l){
        return !l.IsValid();}),loops.end());

}
