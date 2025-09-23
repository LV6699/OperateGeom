#include <tbb/tbb.h>
#include "ExicuteProgram.h"
#if DebugModel
#include<iostream>
#pragma optimize("", off)
#endif
void ExicuteProgram::CheckoutOffsetParameter(OffsetParam &param)
{
    if(std::abs(param._offsetUnit.OffsetValue()) < OftValLimit)
        param._offsetUnit.SetOffsetValue(OftValLimit);
    if(std::abs(param._offsetUnit.FirstOffsetValue()) < OftValLimit)
        param._offsetUnit.SetFirstOffsetValue(OftValLimit);
}
void ExicuteProgram::FirstOffsetSmooth(OffsetParam& oftPara,DefLoop& loop)
{
    if(!oftPara._offsetUnit.IsFirSmooth() ||
            loop.PastOffsetNum() > 1 ||
            oftPara._offsetUnit.FirstOffsetValue() < PreErr5_3)
        return;
    double pastOftNum = loop.PastOffsetNum();
#if 0
    string str = OperateOftString().LoopElementString(loop);
    std::cout<<str<<endl;
#endif
    PathSmooth smooth;
    vector<DefLoop>rloops;
    double R = oftPara._offsetUnit.PathSmoothValue();
    smooth.CalculateLoopSmooth(loop,R,rloops,true);
    if(rloops.empty())
        return;
    loop = rloops[0];
    ElementPrecision elepre;
    elepre.PretreatmentLoop(loop,true,true,true,true,true);
    loop.SetPastOftNum(pastOftNum);
}
void ExicuteProgram::ExicuteOffsetCalculate(const DefLoop& loop,
                                            OffsetParam param,
                                            vector<DefLoop>& locLoops,
                                            vector<DefLoop>& resLoops)
{
    resLoops.clear();
    locLoops.clear();
    if(!ElemCalculate().IsLegalCurveNumber(loop))
        return;
    LoopSelIntersect selIn;ElementPrecision elepre;
    CalculateValidity oftval;CreateEqudCurve crtequ;
    CalculateOftVal calval;OperateLoop opelo;
    vector<DefLoop>vOffsetLoop{loop};
    CheckoutOffsetParameter(param);
    bool isFixDist = param._offsetUnit.IsFixedOffset(),
            isAbldPreProc = param._offsetUnit.IsAblePreProcess(),
            isAblAdjust = param._offsetUnit.AlterableValue(),
            isAddOrigin = param._pathTech.IsAddOriginLoop();
    int numlimit = 200,iterNum = 0;
    const int allowNum = param._offsetUnit.OffsetNumber();
    double offsetVal = param._offsetUnit.OffsetValue(),
            R = param._offsetUnit.ToolRadius();
    while(!vOffsetLoop.empty()){        if(isFixDist && (iterNum >= allowNum))
            return;
        DefLoop inloop = vOffsetLoop.back();
        ///inloop.SetPastOftNum(1);
        bool hasAdjustVal = false,isValOffset = true;
        int thisOftNum = inloop.ThisOffsetNum(),
                pastOftNum = inloop.PastOffsetNum();
        while(isValOffset){
            if(iterNum >= numlimit)
                return;
            DefLoop rloop;
            ///string str = OperateOftString().LoopElementString(inloop);
            calval.AdjustCurrentOffsetValue(inloop,pastOftNum,hasAdjustVal,
                                            param,offsetVal);
            elepre.SpecifyPretreatmentLoop(isFixDist,isAbldPreProc,offsetVal,
                                           true,true,true,true,true,inloop);
            crtequ.CreateEquidistanceCurve(inloop,offsetVal,isValOffset,rloop);
            if(!isValOffset){
                if(hasAdjustVal || !isAblAdjust)
                    return;
                hasAdjustVal = true;
                offsetVal = R * LastOftValTimes;
                calval.AdjustCurrentOffsetValue(inloop,pastOftNum,hasAdjustVal,
                                                param,offsetVal);
                crtequ.CreateEquidistanceCurve(inloop,offsetVal,
                                               isValOffset,rloop);
                if(!isValOffset)
                    return;
                if(!vOffsetLoop.empty()){
                    vOffsetLoop.pop_back();
                }
            }
            vector<DefLoop>rloops;
            bool isSelIntersect = false;
            ///string str1 = OperateOftString().LoopElementString(rloop);
            selIn.CalculateLoopSelIntersect(inloop,rloop,param,offsetVal,
                                            true,isSelIntersect,rloops);
            //isSelIntersect = false;
            if(!isSelIntersect){
                bool isValidOft = false;
                oftval.IsCurrentValidOffset(param,vOffsetLoop,inloop,
                                            rloop,offsetVal,isValidOft);
                //isValidOft = true;
                if(!isValidOft){
                    ///string str2 =
                    OperateOftString().LoopElementString(inloop);
                    if(hasAdjustVal || !isAblAdjust)
                        break;
                    hasAdjustVal = true;
                    offsetVal = R * LastOftValTimes;
                    continue;
                }
                opelo.SetLoopElemBaseParam(R,offsetVal,rloop);
                inloop = rloop;
                elepre.SpecifyPretreatmentLoop(isFixDist,isAbldPreProc,
                                               offsetVal,true,true,true,
                                               true,true,inloop);
                inloop.SetThisOftNum(thisOftNum + 1);
                inloop.SetPastOftNum(pastOftNum + 1);
                ///string str3 = OperateOftString().LoopElementString(inloop);
                FirstOffsetSmooth(param,inloop);
                resLoops.push_back(inloop);
                ///string str4 = OperateOftString().LoopElementString(inloop);                if(isFixDist && (inloop._thisOftNum >= allowNum))
                return;
                thisOftNum ++;
                pastOftNum ++;
            }else{
                if(rloops.empty()){
                    ///string str5 =
                    OperateOftString().LoopElementString(inloop);
                    if(hasAdjustVal || !isAblAdjust ||
                            (!isAddOrigin && pastOftNum == 0))
                        return;
                    offsetVal = R * LastOftValTimes;
                    hasAdjustVal = true;
                    continue;
                }
                if(rloops.size() > 1){
                    for(auto &d : rloops){
                        ///string str6 =
                        OperateOftString().LoopElementString(d);
                        elepre.SpecifyPretreatmentLoop(isFixDist,isAbldPreProc,
                                                       offsetVal,true,true,true,
                                                       true,true,d);
                        d.SetThisOftNum(thisOftNum + 1);
                        d.SetPastOftNum(pastOftNum + 1);
                        FirstOffsetSmooth(param,d);
                        ///string str7= OperateOftString().LoopElementString(d);
                        locLoops.push_back(d);
                    }
                    return;
                }
                
                elepre.SpecifyPretreatmentLoop(isFixDist,isAbldPreProc,offsetVal,
                                               
                                               true,true,true,true,true,rloops[0]);
                rloops[0].SetThisOftNum(thisOftNum + 1);
                rloops[0].SetPastOftNum(pastOftNum + 1);
                FirstOffsetSmooth(param,rloops[0]);
                resLoops.push_back(rloops[0]);
                inloop = rloops[0];
                ///string str5 = OperateOftString().LoopElementString(inloop);
                if(isFixDist && ((rloops[0]._thisOftNum) >= allowNum))
                    return;
                thisOftNum ++;
                pastOftNum ++;
            }
            if(hasAdjustVal){
                if(!vOffsetLoop.empty()){
                    vOffsetLoop.pop_back();
                }
                break;
            }
            iterNum ++;
        }
    }
}
#if TimeStatistic
#include"../../../ViewWindow/MainWindow/mainwindow.h"
#endif
void ExicuteProgram::LoopOffsetNodeResult(const DefLoop& loop,                                          OffsetParam param,
                                          shared_ptr<OffsetNode> &pathNode,
                                          bool isStaticTime)
{
#if TimeStatistic
    auto sTime = std::chrono::high_resolution_clock::now();
#endif
    if(pathNode == nullptr){
        pathNode = make_shared<OffsetNode>();
    }/**
    if((loop.LoopType() == WITHINPROFILE || loop.IsOpenLoop())
            && !offsetPara._offsetUnit.IsFixedOffset()){
        return;
    }*/
    vector<DefLoop>siloops,resloops;
    ExicuteOffsetCalculate(loop,param,siloops,resloops);
    pathNode->_vLoopData = resloops;
#if 1
    for(int i = 0;i < siloops.size();i++){
        shared_ptr<OffsetNode> curNode = make_shared<OffsetNode>();
        if(param._offsetUnit.IsFixedOffset() &&
                (siloops[i]._thisOftNum >=
                 param._offsetUnit.OffsetNumber())){
            curNode->PushBackLoop(siloops[i]);
        }else{
            LoopOffsetNodeResult(siloops[i],param,curNode,false);
            curNode->InsertLoop(0,siloops[i]);
        }
        pathNode->_vOffsetNode.push_back(curNode);
    }
#else
    oftNode->_vOffsetNode.resize(siloops.size(), nullptr);
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, siloops.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); i++) {
            auto loop = siloops[i];
            shared_ptr<OffsetNode> curNode = make_shared<OffsetNode>();
            if(offsetPara._offsetUnit.IsFixedOffset() &&
                    (loop._thisOftNum >=
                     offsetPara._offsetUnit.OffsetNumber())){
                curNode->PushBackLoop(loop);
            }
            else{
                LoopOffsetNodeResult(loop,offsetPara,curNode,false);
                curNode->_vLoopData.insert(curNode->_vLoopData.begin(),
                                           loop);
            }
            oftNode->_vOffsetNode[i] = curNode;
            //oftNode->_vOffsetNode.push_back(curNode);
        }
    });
#endif
#if TimeStatistic
    if(isStaticTime){
        auto eTime = std::chrono::high_resolution_clock::now();
        double exeTime = std::chrono::duration<double>(eTime - sTime).count();
        Time_offset += exeTime * 1000;
    }
#endif
}void ExicuteProgram::GetLoopOffsetNode(OffsetParam param,
                                        const DefLoop& loop,
                                        shared_ptr<OffsetNode> &rootNode)
{
    if((!param._offsetUnit.IsFixedOffset()) ||
            (param._offsetUnit.IsFixedOffset() &&
             (loop.PastOffsetNum() < param._offsetUnit.OffsetNumber()))){
        bool allowNum = param._offsetUnit.OffsetNumber() - loop.PastOffsetNum();
        param._offsetUnit.SetOffsetNumber(allowNum);
        if(param._offsetUnit.IsFixedOffset() &&
                param._offsetUnit.OffsetNumber() < 0){
            return;
        }
        LoopOffsetNodeResult(loop,param,rootNode);
    }
}
void ExicuteProgram::GetMultipleLoopOffset(const vector<DefLoop>&loops,
                                           ToolPathType pathType,
                                           OffsetParam param,
                                           bool isAddOriLoop,
                                           shared_ptr<OffsetNode>& rootNode)
{
    if(rootNode == nullptr)
        rootNode = make_shared<OffsetNode>();
    RootNodeSolve rotsol;;
    for (int i = 0; i < loops.size(); ++i) {
        shared_ptr<OffsetNode> node = make_shared<OffsetNode>();
        GetLoopOffsetNode(param,loops[i],node);
        if(isAddOriLoop){
            node->_vLoopData.insert(node->_vLoopData.begin(),loops[i]);
        }
        if(node->_vLoopData.empty() && node->_vOffsetNode.empty()){
            continue;
        }
        rotsol.SetNodeToolPathType(pathType,node);
        rootNode->_vOffsetNode.push_back(node);
    }
}
