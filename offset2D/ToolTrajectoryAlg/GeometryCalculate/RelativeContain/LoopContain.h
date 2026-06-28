#ifndef LOOPCONTAIN_H
#define LOOPCONTAIN_H
#include"LoopContAux.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class LoopContain
{
public:
    LoopContain(){}
    ///判断loop 是否在outLoop 之外
    static bool IsLoopOutLoopCoord(const ofts::DefLoop& outLoop,
                                   const ofts::DefLoop& loop,double e)
    {
        auto out = LimitCoord2D::LoopLimitCoord(outLoop);
        auto in = LimitCoord2D::LoopLimitCoord(loop);
        return LimitCoord2D::IsLimitCoordOut(out,in,e);
    }
    static bool IsLoopContLoopCoord(const ofts::DefLoop& outLoop,
                                    const ofts::DefLoop& loop,double e)
    {
        auto out = LimitCoord2D::LoopLimitCoord(outLoop);
        auto in = LimitCoord2D::LoopLimitCoord(loop);
        return LimitCoord2D::IsLimitCoordCont(out,in,e);
    }
    static bool IsLoopsContLoopCoord(const std::vector<ofts::DefLoop>& loops,
                                     const ofts::DefLoop& loop,double pre)
    {
        for(const auto& l : loops){
            if(IsLoopContLoopCoord(l,loop,pre)){
                return true;
            }
        }
        return false;
    }
    static bool IsLoopContIntLoop(ofts::DefLoop& outLoop,
                                  ofts::DefLoop& loop,
                                  const numst::ContParam& cpa)
    {
        if(LimitCoord2D::IsLimitCoordOut(outLoop.GetLimCoord(),
                                         loop.GetLimCoord(),cpa._incPre)){
            return false;
        }
        for (size_t i = 0; i < loop.Size(); ++i) {
            auto& c = loop[i];
            if(LoopContAux::IsLoopContIntCurve(outLoop,c,cpa)){
                return true;
            }
        }
        return false;
    }
    static bool IsLoopContainLoop(ofts::DefLoop& outLoop,
                                  ofts::DefLoop& loop,
                                  const numst::ContParam& cpa)
    {
        if(outLoop.IsEmpty() || loop.IsEmpty()){return false;}
        if(!outLoop.GetLimCoord().HasInit()){
            LimitCoord2D::SetLoopLimCoord(outLoop);
        }
        if(!loop.GetLimCoord().HasInit()){
            LimitCoord2D::SetLoopLimCoord(loop);
        }
        if(!LimitCoord2D::IsLimitCoordCont(outLoop.GetLimCoord(),
                                           loop.GetLimCoord(),cpa._incPre)){
            return false;
        }
#if 0
        if(cpa.MatchCoord() && !LimitCoord2D::IsLimitCoordCont(outLoop.GetLimCoord(),
                                                               loop.GetLimCoord(),cpa._incPre)){
            return false;
        }
        PointInLoop pil;
        ofts::GeomIntRes iRes;
        auto ipa = LoopContAux::ContToIntParam(cpa);
        std::vector<size_t>ids;ids.reserve(5);
        bool isIntersect = false,onBord = false,inPoly = false;;
        for (size_t i = 0; i < outLoop.Size(); ++i) {
            const auto& c1 = outLoop[i];
            for (size_t j = 0; j < loop.Size(); j++){
                const auto& c2 = loop[j];
                CurveInt2D::GetCurveIntersect(c1,c2,ipa,iRes);
                if(!iRes.IsInt()){continue;}
                isIntersect = true;
                const auto p1 = iRes.P1();
                iRes.Reset();
                bool isSp1 = p1.IsEqual(c1.GetSp(),cpa._onBordPre);
                bool isEp1 = p1.IsEqual(c1.GetEp(),cpa._onBordPre);
                bool isSp2 = p1.IsEqual(c2.GetSp(),cpa._onBordPre);
                bool isEp2 = p1.IsEqual(c2.GetEp(),cpa._onBordPre);
                if(!isSp1 && !isEp1 && !isSp2 && !isEp2){
                    return false;
                }
                if(isSp2 && isEp2){
                    auto mp = BaseGeom2D::CurveMidpoint(c2);
                    pil.GetPointInsideLoop(loop,mp,cpa._onBordPre,onBord,inPoly);
                    if(onBord){
                        auto c3 = c2;
                        c3.ChangeEndpt(mp,true);
                        mp = BaseGeom2D::CurveMidpoint(c3);
                        pil.GetPointInsideLoop(loop,mp,cpa._onBordPre,onBord,inPoly);
                        if(onBord || inPoly){continue;}
                        return false;
                    }
                    if(inPoly){continue;}
                    return false;
                }
                if(isSp2 && !isEp2){
                    pil.GetPointInsideLoop(loop,c2.GetEp(),cpa._onBordPre,onBord,inPoly);
                    if(onBord){
                        auto mp = BaseGeom2D::CurveMidpoint(c2);
                        pil.GetPointInsideLoop(loop,mp,cpa._onBordPre,onBord,inPoly);
                        if(onBord){
                            auto c3 = c2;
                            c3.ChangeEndpt(mp,true);
                            mp = BaseGeom2D::CurveMidpoint(c3);
                            pil.GetPointInsideLoop(loop,mp,cpa._onBordPre,onBord,inPoly);
                            if(onBord || inPoly){continue;}
                            return false;
                        }
                        if(inPoly){continue;}
                        return false;
                    }
                    if(inPoly){continue;}
                    return false;
                }
                if(!isSp2 && isEp2){
                    pil.GetPointInsideLoop(loop,c2.GetSp(),cpa._onBordPre,onBord,inPoly);
                    if(onBord){
                        auto mp = BaseGeom2D::CurveMidpoint(c2);
                        pil.GetPointInsideLoop(loop,mp,cpa._onBordPre,onBord,inPoly);
                        if(onBord){
                            auto c3 = c2;
                            c3.ChangeEndpt(mp,true);
                            mp = BaseGeom2D::CurveMidpoint(c3);
                            pil.GetPointInsideLoop(loop,mp,cpa._onBordPre,onBord,inPoly);
                            if(onBord || inPoly){continue;}
                            return false;
                        }
                        if(inPoly){continue;}
                        return false;
                    }
                    if(inPoly){continue;}
                    return false;
                }
                pil.GetPointInsideLoop(loop,c2.GetSp(),cpa._onBordPre,onBord,inPoly);
                if(onBord){
                    pil.GetPointInsideLoop(loop,c2.GetEp(),cpa._onBordPre,onBord,inPoly);
                    if(onBord){
                        auto mp = BaseGeom2D::CurveMidpoint(c2);
                        pil.GetPointInsideLoop(loop,mp,cpa._onBordPre,onBord,inPoly);
                        if(onBord){
                            auto c3 = c2;
                            c3.ChangeEndpt(mp,true);
                            mp = BaseGeom2D::CurveMidpoint(c3);
                            pil.GetPointInsideLoop(loop,mp,cpa._onBordPre,onBord,inPoly);
                            if(onBord || inPoly){continue;}
                            return false;
                        }
                        if(inPoly){continue;}
                        return false;
                    }
                    if(inPoly){continue;}
                    return false;
                }
                if(inPoly){continue;}
                return false;
            }
        }
        if(!isIntersect){
            const auto& sp = loop.Endpoint(false);
            pil.GetPointInsideLoop(loop,sp,cpa._onBordPre,onBord,inPoly);
            return inPoly;
        }
        for (size_t i = 0; i < loop.Size(); ++i) {
            auto c = loop[i];
            c.SetLimCoord(LimitCoord2D::CurveLimitCoord(c));
            if(!LoopContAux::IsLoopContCurve(outLoop,c,cpa,true)){
                return false;
            }
        }
        return true;
#else
        for (size_t i = 0; i < loop.Size(); ++i) {
            if(!LoopContAux::IsLoopContCurve(outLoop,loop[i],cpa,true)){
                return false;
            }
        }
        return true;
#endif
    }
    static bool IsLoopsContIntLoop(std::vector<ofts::DefLoop>& loops,
                                   ofts::DefLoop& loop,
                                   const numst::ContParam& cpa)
    {
        for (size_t i = 0; i < loops.size(); ++i) {
            if(IsLoopContIntLoop(loops[i],loop,cpa)){
                return true;
            }
        }
        return false;
    }
    static bool IsLoopsContLoop(std::vector<ofts::DefLoop>& loops,
                                ofts::DefLoop& loop,
                                const numst::ContParam& cpa)
    {
        /**
        判断一个环是在其他环内,如果一个环在其他任一环内,则该环被包含在其他环,
        判断一个环是否包含在另一环内,当环的所有点均在另一环内(包含边界),则
        该环包含在另一环.*/
        for (size_t i = 0; i < loops.size(); ++i) {
            if(IsLoopContainLoop(loops[i],loop,cpa)){
                return true;
            }
        }
        return false;
    }
};
#endif // LOOPCONTAIN_H
