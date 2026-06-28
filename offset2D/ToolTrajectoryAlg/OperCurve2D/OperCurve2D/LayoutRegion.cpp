#include "LayoutRegion.h"
#include "../../OperCurve2D/NumeCalculate/ElemCalculate.h"
#include "../../DynamicProgram/GeneratePath/MergeRegAux.h"
#include "../../GeometryCalculate/RelativeContain/LoopContain.h"
#include "../../GeometryCalculate/GeomCalculate/LoopDirection.h"
#include "../../GeometryCalculate/GeomCalculate/LoopDirection.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
void LayoutRegion::OrganizeLoopToArea(const std::vector<ofts::DefLoop>& inloops,
                                      double intPre, double coinPre,
                                      double incPre,
                                      std::vector<ofts::GeomArea>& regions)
{
    ElemCalculate elecal;MergeRegAux maux;
    numst::ContParam cpa(intPre,coinPre,incPre);
#define _ToArea 0
#define _view 0
    int i = 0;
    std::vector<ofts::DefLoop>loops = inloops,loops1;
    for(auto& l : loops){l.SetLimCoord(LimitCoord2D::LoopLimitCoord(l));}

    while (!loops.empty()) {
        ofts::DefLoop loop_;
        std::vector<ofts::DefLoop>vLoop_;
        elecal.ExceptIndexLoop(loops,i,loop_,vLoop_);
#if _ToArea
        if(loop_._id == 6 || loop_._id == 7 || loop_._id == 8){
            int id = loop_._id;
        }
#endif 
        bool isInside = LoopContain::IsLoopsContLoop(vLoop_,loop_,cpa);
        if(!isInside){
            loops1.emplace_back(loop_);
            loops.erase(loops.begin() + i);
            i = 0;
        }else{
            i ++;
        }
        ///防止无限循环.
        if(i > inloops.size()){
            loops1 = inloops;
            break;
        }
    }
    i = 0;
    while (!loops1.empty()) {
        ofts::DefLoop loop_;
        std::vector<ofts::DefLoop>vLoop_;
        elecal.ExceptIndexLoop(loops1,i,loop_,vLoop_);
        std::vector<ofts::DefLoop>vIncLoop;
        LoopIncludeLoops(loop_,vLoop_,intPre,coinPre,
                         incPre,true,vIncLoop);
#if _ToArea
        if(loop_._id == 6){
            int id = loop_._id;
        }
#endif
        if(vIncLoop.empty()){
            bool isInside = LoopContain::IsLoopsContLoop(vLoop_,loop_,cpa);
            if(!isInside){
                regions.push_back(ofts::GeomArea(loop_));
                loops1.erase(loops1.begin() + i);
                i = 0;
            }else{
                i ++;
            }
            continue;
        }
        std::vector<ofts::DefLoop>vInsLoop,vOutLoop;
        maux.LoopsInsideOutsideLoop(vIncLoop,vInsLoop,vOutLoop);
        regions.push_back(GeomArea(loop_,vOutLoop));
        std::vector<ofts::DefLoop>leftLoops;
        loops1.erase(loops1.begin() + i);
        for(const auto& d : loops1){
            if(!elecal.IsLoopInArrayLoop(vOutLoop,d)){
                leftLoops.emplace_back(d);
            }
        }
        loops1 = leftLoops;
        i = 0;
    }
#if _ToArea
    int t = 0;
#endif
}
void LayoutRegion::LoopIncludeLoops(ofts::DefLoop& loop,
                                    std::vector<ofts::DefLoop>& vLoop,
                                    double intPre,double coinPre,
                                    double incPre,bool isDel,
                                    std::vector<ofts::DefLoop>& vIncLoop)
{
    std::vector<size_t>ids;
    ElemCalculate elecal;
    numst::ContParam cpa(intPre,coinPre,incPre);
    LimitCoord2D::SetLoopLimCoord(loop);
    for(auto& l : vLoop){LimitCoord2D::SetLoopLimCoord(l);}
    for (size_t i = 0; i < vLoop.size(); ++i) {
        bool isInclude = false;
        vLoop[i].SetLimCoord(LimitCoord2D::LoopLimitCoord(vLoop[i]));
        isInclude = LoopContain::IsLoopContainLoop(loop,vLoop[i],cpa);
        if(isInclude){
            vIncLoop.push_back(vLoop[i]);
            ids.push_back(i);
        }
    }
    if(isDel){
        elecal.DeleteIndexLoop(ids,vLoop);
    }
}
void LayoutRegion::LoopsToRegions(const std::vector<ofts::DefLoop>& loops,
                                  std::vector<ofts::GeomArea>& regions)
{
    LoopDirection ldir;
    std::vector<ofts::DefLoop>bloops,iloos;
    bloops.reserve(loops.size());iloos.reserve(loops.size());
    for (size_t i = 0; i < loops.size(); ++i) {
        bool iscw = ldir.IsClockwiseLoop(loops[i]);
        if(iscw){iloos.emplace_back(loops[i]);}
        else{bloops.emplace_back(loops[i]);}
    }
}
std::vector<std::shared_ptr<ofts::LoopTree>> LayoutRegion::LoopsToLoopTrees(numst::ContParam param,
                                                                            std::vector<ofts::DefLoop>& loops)
{
    std::vector<std::shared_ptr<ofts::LoopTree>>trees;
    if(loops.size() < 2){
        if(!loops.empty()){trees.emplace_back(LoopTree::BuildTree(loops[0]));}
        return trees;
    }
    struct IncInfo{
        IncInfo(){}
        IncInfo(size_t i,double d) : id(i),coord(d){}
        size_t id;
        double coord = 0;
        bool included = false;
        std::vector<size_t>incs;  //所包含的环
        std::vector<size_t>equs;  //被哪些环包含
    };
    std::vector<IncInfo>infs;infs.reserve(loops.size());
    for(auto& l : loops){l.SetLimCoord(LimitCoord2D::LoopLimitCoord(l));}

    for (size_t i = 0; i < loops.size(); i++){
        loops[i].SetId(static_cast<int>(i));
        double d = loops[i].GetLimCoord().MaxX();
        infs.emplace_back(IncInfo(i,d));
    }
    param.SetMatchCoord(false);
    std::sort(infs.begin(),infs.end(),[](const IncInfo& a,
              const IncInfo& b){return a.coord > b.coord;});
    for (size_t i = 0; i < infs.size(); i++){
        auto& inf = infs[i];
        for (size_t j = i+1; j < infs.size(); j++){
            auto& inf1 = infs[j];
            if(std::abs(inf.coord - inf1.coord) > PreErr_10){break;}
            inf.equs.emplace_back(j);
            inf1.equs.emplace_back(i);
        }
    }
    //如果A 包含B,B 又包含C,A 不会先判断包含C,因为提前做了坐标排序(B 在C 前面),因此
    //A 会先判断包含B,当判断包含C 时由于C 已经被包含,所以不会再次判断C
    for (size_t i = 0; i < infs.size(); i++){
        auto& inf = infs[i];
        auto& loop = loops[inf.id];
        for (size_t j = i+1; j < infs.size(); j++){
            auto& inf1 = infs[j];/*
            if(inf.id == 7 && inf1.id == 8){
                int tem = 1;
            }*/
            auto& loop1 = loops[inf1.id];
            if(LoopContain::IsLoopContainLoop(loop,loop1,param)){
                inf.incs.emplace_back(inf1.id);
                inf1.included = true;continue;
            }
            auto& equs = inf1.equs;
            if(equs.empty()){continue;}
            for(auto& d : equs){
                if(d != i){continue;}
                if(!LoopContain::IsLoopContainLoop(loop1,loop,param)){continue;}
                inf1.incs.emplace_back(inf.id);
                inf.included = true;break;
            }
        }
    }
    auto IdInfo = [&](const size_t& i)->const IncInfo&{
        for(auto& d : infs){if(d.id == i){return d;}}
        return infs[0];
    };
    std::sort(infs.begin(),infs.end(),[](const IncInfo& a,
              const IncInfo& b){return a.id < b.id;});
    for (size_t i = 0; i < infs.size(); i++){
        auto& inf = infs[i];
        auto& incs = inf.incs;
        std::vector<bool> bs(incs.size(),false);
        //判断incs 中某元素e 对应环,是否被除e 外的其他元素e1 对应环包含,
        //如果e 被e1 包含,说明e 被多重包含,则e 需要从incs 中移除
        for (size_t j = 0; j < incs.size(); j++){
            auto& id = incs[j];
            for (size_t k = 0; k < incs.size(); k++){
                if(j == k){continue;}
                auto& id1 = incs[k];
                for(auto& d : infs){
                    if(d.id != id1){continue;}
                    if(!pnum::IsInArray(id,d.incs)){continue;}
                    bs[j] = true;break;
                }
                if(bs[j]){break;}
            }
        }
        auto incs1 = incs;incs.clear();
        for (size_t j = 0; j < incs1.size(); j++){
            if(!bs[j]){incs.emplace_back(incs1[j]);}
        }
    }
    trees.reserve(infs.size());
    auto AddChilds = [&](std::shared_ptr<ofts::LoopTree>& tree,
            const IncInfo& inf,auto&& self)->void{
        auto& incs = inf.incs;
        for (size_t j = 0; j < incs.size(); j++){
            auto id = incs[j];
            const auto& inf1 = infs[id];
            auto t = LoopTree::BuildTree(loops[inf1.id]);
            t->Reserve(incs.size());
            if(!inf1.incs.empty()){
                self(t,inf1,self);
            }
            tree->AddNode(t);
        }
    };
    for(size_t i = 0; i < infs.size(); i++){
        const auto& inf = infs[i];
        if(inf.included){continue;}
        auto tree = LoopTree::BuildTree(loops[inf.id]);
        tree->SetId(inf.id);
        AddChilds(tree,inf,AddChilds);
        trees.emplace_back(tree);
    }
    for(auto& d : trees){d->InitId();}
#if 0
    auto DrawTree = [&](std::shared_ptr<LoopTree>& tree,
            Quantity_Color& c,double w,auto&& self)->void{
        ofts::DefLoop l(tree->Curves());
        DisplayGeom().DisplayLoop(l,c,w);
        for (size_t i = 0; i < tree->childs.size(); i++){
            self(tree->childs[i],_colors[i],w,self);
            std::cout<<"";
        }
    };
    for (size_t i = 0; i < trees.size(); i++){
        DrawTree(trees[i],_colors[i],1.5+i*0.5,DrawTree);
        std::cout<<"";
    }
#endif
    return trees;
}
