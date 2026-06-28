#include <iostream>
#include "ExicuteProgram.h"
#include "EquidCurve/EquidCurve/EquidLoop.h"
#include "EquidCurve/LoopSelInt/LoopSelIntersect.h"
#include "../OperateView/OperateView/DisplayGeom.h"

// #ifdef emit
// #pragma push_macro("emit")
// #undef emit
// #endif
#ifdef emit
#undef emit
#include <tbb/tbb.h>
#define emit Q_EMIT
#else
#include <tbb/tbb.h>
#endif
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif

void ExicuteProgram::CheckOffsetParam(ofts::OffsetParam &param)
{
    if(std::abs(param._offsetUnit.OffsetValue()) < OftValLimit)
        param._offsetUnit.SetOffsetValue(OftValLimit);
    if(std::abs(param._offsetUnit.FirstOftVal()) < OftValLimit)
        param._offsetUnit.SetFirstOffsetValue(OftValLimit);
}
void ExicuteProgram::FirstOffsetSmooth(ofts::OffsetParam& param,ofts::DefLoop& loop)
{
    if(!param._offsetUnit.IsFirSmooth() ||
            loop.PastOftNum() > 1 ||
            param._offsetUnit.FirstOftVal() < PreErr5_3){
        return;
    }
    int pastOftNum = loop.PastOftNum();
#if 0
    string str = OperateOftString().LoopElementString(loop);
    std::cout<<str<<endl;
#endif
    PathSmooth smooth;
    std::vector<ofts::DefLoop>rloops;
    double R = param._offsetUnit.PathSmoothValue();
    auto param_ = param;
    param_._prepOpti.SetOftValue(R);
    smooth.CalculateLoopSmooth(loop,R,rloops,true);
    if(rloops.empty()){return;}
    loop = rloops[0];
    PreprocLoop elepre;
    elepre.PreprocessLoop(param_,loop);
    loop.SetPastOftNum(pastOftNum);
}
void ExicuteProgram::ExicuteLoopOffset(const ofts::DefLoop& loop,
                                       ofts::OffsetParam param,
                                       std::vector<ofts::DefLoop>& locLoops,
                                       std::vector<ofts::DefLoop>& resLoops,
                                       bool preprocess)
{
    resLoops.clear();locLoops.clear();
    if(!loop.IsLegalNum()){return;}
    LoopSelIntersect selIn;PreprocLoop elepre;
    CalculateValidity oftval;EquidLoop crtequ;
    CalculateOftVal calval;
    std::vector<ofts::DefLoop>oft_loops{loop};
    CheckOffsetParam(param);
    int numlimit = 200,iterNum = 0;
    int allowNum = param._offsetUnit.OffsetNumber();
    double R = param._offsetUnit.ToolRadius();
    double oftD = param._offsetUnit.OffsetValue();
    bool isFixDist = param._offsetUnit.IsFixedOffset();
    bool isAblAdjust = param._offsetUnit.AlterableValue();
    bool isAddOrigin = param._pathTech.IsAddOriginLoop();
    while(!oft_loops.empty()){
        if(isFixDist && (iterNum >= allowNum)){return;}
        auto inloop = oft_loops.back();
        ///inloop.SetPastOftNum(1);
        bool hasAdjust = false,isValid = true;
        int thisOftNum = inloop.ThisOftNum();
        int pastOftNum = inloop.PastOftNum();
        while(isValid){
            if(iterNum >= numlimit){return;}
            ///string str = OperateOftString().LoopElementString(inloop);
            calval.AdjustOffsetValue(inloop,pastOftNum,hasAdjust,param,oftD);
            param._prepOpti.SetOftValue(oftD);
            if(preprocess){elepre.OptPreprocessLoop(param,inloop);}
            auto rloop = crtequ.GetEquidistanceCurve(param,inloop,oftD,isValid);
            if(!isValid){
                if(hasAdjust || !isAblAdjust){return;}
                hasAdjust = true;
                oftD = R * LastOftValTimes;
                calval.AdjustOffsetValue(inloop,pastOftNum,hasAdjust,param,oftD);
                rloop = crtequ.GetEquidistanceCurve(param,inloop,oftD,isValid);
                if(!isValid){return;}
                if(!oft_loops.empty()){
                    oft_loops.pop_back();
                }
            }
            std::vector<ofts::DefLoop>rloops;
            ///string str1 = OperateOftString().LoopElementString(rloop,true);
            if(!selIn.OperateLoopSelInt(inloop,rloop,param,oftD,rloops)){
                bool isValidOft = false;
                oftval.IsCurrentValidOffset(param,oft_loops,inloop,rloop,oftD,isValidOft);
                //isValidOft = true;
                if(!isValidOft){
                    ///string str2 = OperateOftString().LoopElementString(inloop,true);
                    if(hasAdjust || !isAblAdjust){break;}
                    hasAdjust = true;
                    oftD = R * LastOftValTimes;
                    continue;
                }
                //opelo.SetLoopElemBaseParam(R,oftD,rloop);
                inloop = rloop;
                elepre.OptPreprocessLoop(param,inloop);
                inloop.SetThisOftNum(thisOftNum + 1);
                inloop.SetPastOftNum(pastOftNum + 1);
                ///string str3 = OperateOftString().LoopElementString(inloop);
                FirstOffsetSmooth(param,inloop);
                resLoops.push_back(inloop);
                ///string str4 = OperateOftString().LoopElementString(inloop);
                if(isFixDist && (inloop.ThisOftNum() >= allowNum)){return;}
                thisOftNum ++;
                pastOftNum ++;
            }else{
                if(rloops.empty()){
                    ///string str5 = OperateOftString().LoopElementString(inloop,true);
                    ///std::cout<<str5<<std::endl;
                    if(hasAdjust || !isAblAdjust || (!isAddOrigin && pastOftNum == 0)){
                        return;
                    }
                    oftD = R * LastOftValTimes;
                    hasAdjust = true;
                    continue;
                }
                if(rloops.size() > 1){
                    for(auto &d : rloops){
                        ///string str6 = OperateOftString().LoopElementString(d);
                        elepre.OptPreprocessLoop(param,d);
                        d.SetThisOftNum(thisOftNum + 1);
                        d.SetPastOftNum(pastOftNum + 1);
                        FirstOffsetSmooth(param,d);
                        ///string str7= OperateOftString().LoopElementString(d);
                        locLoops.push_back(d);
                    }
                    return;
                }
                ///DisplayGeom().DisplayLoop(loop,_colors.back(),1,true,false);
                ///DisplayGeom().DisplayLoop(rloops[0],_colors[1],1,true,false);
                elepre.OptPreprocessLoop(param,rloops[0]);
                rloops[0].SetThisOftNum(thisOftNum + 1);
                rloops[0].SetPastOftNum(pastOftNum + 1);
                FirstOffsetSmooth(param,rloops[0]);
                resLoops.push_back(rloops[0]);
                inloop = rloops[0];
                ///string str5 = OperateOftString().LoopElementString(inloop);
                if(isFixDist && ((rloops[0].ThisOftNum()) >= allowNum)){
                    return;
                }
                thisOftNum ++;
                pastOftNum ++;
            }
            if(hasAdjust){
                if(!oft_loops.empty()){
                    oft_loops.pop_back();
                }
                break;
            }
            iterNum ++;
        }
    }
}
void ExicuteProgram::LoopOffsetNodeResult(const ofts::DefLoop& loop,
                                          ofts::OffsetParam param,
                                          std::shared_ptr<ofts::OffsetNode> &pathNode,
                                          bool isPrecocess)
{
    if(pathNode == nullptr){pathNode = make_shared<ofts::OffsetNode>();}
    std::vector<ofts::DefLoop>siloops,resloops;
    ExicuteLoopOffset(loop,param,siloops,resloops,isPrecocess);
    pathNode->_loops = resloops;
    for(int i = 0;i < siloops.size();i++){
        auto curNode = make_shared<ofts::OffsetNode>();
        if(param._offsetUnit.IsFixedOffset() &&
                (siloops[i].ThisOftNum() >=
                 param._offsetUnit.OffsetNumber())){
            curNode->AddLoop(siloops[i]);
        }else{
            LoopOffsetNodeResult(siloops[i],param,curNode,false);
            curNode->InsertLoop(0,siloops[i]);
        }
        pathNode->_nodes.push_back(curNode);
    }
}
void ExicuteProgram::GetLoopOffsetNode(ofts::OffsetParam param,
                                       const ofts::DefLoop& loop,
                                       std::shared_ptr<ofts::OffsetNode> &rootNode)
{
    if((!param._offsetUnit.IsFixedOffset()) ||
            (param._offsetUnit.IsFixedOffset() &&
             (loop.PastOftNum() < param._offsetUnit.OffsetNumber()))){
        bool allowNum = param._offsetUnit.OffsetNumber() - loop.PastOftNum();
        param._offsetUnit.SetOffsetNumber(allowNum);
        if(param._offsetUnit.IsFixedOffset() &&
                param._offsetUnit.OffsetNumber() < 0){
            return;
        }
        LoopOffsetNodeResult(loop,param,rootNode);
    }
}
void ExicuteProgram::GetMultipleLoopOffset(const std::vector<ofts::DefLoop>&loops,
                                           Ofte::ToolPathType pathType,
                                           ofts::OffsetParam param,
                                           bool isAddOriLoop,
                                           std::shared_ptr<ofts::OffsetNode>& rootNode)
{
    if(rootNode == nullptr)
        rootNode = make_shared<ofts::OffsetNode>();
    RootNodeSolve rotsol;;
    for (int i = 0; i < loops.size(); ++i) {
        auto node = make_shared<ofts::OffsetNode>();
        GetLoopOffsetNode(param,loops[i],node);
        if(isAddOriLoop){
            node->_loops.insert(node->_loops.begin(),loops[i]);
        }
        if(node->_loops.empty() && node->_nodes.empty()){
            continue;
        }
        rotsol.SetNodeToolPathType(pathType,node);
        rootNode->_nodes.push_back(node);
    }
}
