#ifndef PTINLOOPAUX_H
#define PTINLOOPAUX_H
#include "LimitCoord.h"
#include "../GeomCalculate/CurveLog2D.h"
#include "../../OperCurve2D/OperCurve2D/OperateCurve.h"
class InloopInt{
public:
    InloopInt(){}
    struct IntPt{
        IntPt(){}
        IntPt(size_t i,const ofts::GeomIntRes r) : id(i),ires(r){}
        size_t id;
        ofts::GeomIntRes ires;
    };
    void AddIntIds(size_t i,const ofts::GeomIntRes& res){iIds.emplace_back(IntPt(i,res));}
    void AddHorIds(size_t i){horIds.emplace_back(i);}
public:
    bool isComLastEp = false;      //是否比较最后曲线结束点
    bool hasComFirSp = false;      //是否已经比较第一个曲线起始点
    bool hasComPreEp = false;      //是否已经比较前一个曲线结束点
    bool isComNexSp = false;       //是否需要比较下一条曲线起点
    bool hasChStatus = false;      //是否已经改变前一论状态
    bool hasNotHor = false;        //是否已经出现过非水平线段
    size_t iNumber = 0;            //相交点数量
    double endPre = PreErr5_8;     //相交点是曲线端点精度
    double horPre = PreErr5_8;     //线段是水平精度
    double tanPre = PreErr_10;     //图元相切精度
    std::vector<size_t> horIds;    //所有水平线段索引
    std::vector<IntPt> iIds;       //所有相交曲线索引
};
class PtInLoopAux
{
public:
    PtInLoopAux(){}
    static void ExicuteSwitch(bool isLarge,size_t i,InloopInt& iData,bool& isInPoly)
    {
        if(isLarge){
            iData.iNumber ++;
            isInPoly = !isInPoly;
            //iData.AddIntIds(i);
        }
    }
    static bool IsArcEndptMaxCoord(const ofts::DefArc& arc,bool isSp){
        const auto& p = isSp ? arc.GetSp() : arc.GetEp();
        double cAgl = BaseGeom2D::ArcCenterAngle(arc),stepAlg = PreErr_6_;
        if(cAgl < 2 * stepAlg){
            stepAlg = cAgl / 2;
        }
        auto endpt = CurveLog2D::NearArcEndpt(arc,isSp,stepAlg);
        //ofts::Point endpt;
        //CurveLog2D::CloseArcEndpt(arc,isSp,true,stepAlg,endpt);
        bool is = p.Y() > endpt.Y() ? true : false;
        return is;
    }
    static bool IsEndptMaxCoord(const ofts::DefElem& c,size_t i,
                                InloopInt& iData,bool isSp){
        /**
        需要考虑下列情况:
        比较时可以等于,为了防止当下列情况,比如线段与i-1、i、i+1,其中i 是水平线段,
        i-1、i+1 是圆弧,与i-1 相切,与i+1 相交端点且该端点具有最大y 值,理论上点不在
        环内,然而与i+1 是有效相交,因此会出现误判.处理方式是可以相等,或滤掉水平线断.*/
        bool is = false;
        if(c._type == Ofte::LINETYPE){
            const auto& s = c._segment;
            for(auto& d : iData.horIds){if(i == d){return false;}}
            if(isSp){is = s.GetSp().Y() > s.GetEp().Y() ? true : false;
            }else{is = s.GetEp().Y() > s.GetSp().Y() ? true : false;}
            return is;
        }
        return IsArcEndptMaxCoord(c._arc,isSp);
    }
    //只需要围绕交点是endpoint、sp、ep 进行处理
    static void IntersectSegCase(const ofts::DefLoop& loop,size_t i,
                                 const ofts::Point& lsp,
                                 const ofts::Point& lep,
                                 const ofts::Point& ip,
                                 InloopInt& iData,bool& isInPoly)
    {
        if((i == loop.Size() - 1) && iData.isComLastEp){
            bool isLarge = lep.Y() > lsp.Y() ? true : false;
            ExicuteSwitch(isLarge,i,iData,isInPoly);
            return;
        }
        if(i == 0 && ip.IsEqual(lsp,iData.endPre)){
            iData.isComNexSp = true;
        }
        //如果i!=0,当且仅当iData.isComNexSp==true 时,
        //isEleSp = true,即isEleSp 取值由iData._isComNexSp 决定.
        bool isEndpt = false,isEleSp = false;
        if(!iData.hasNotHor){
            iData.isComNexSp = ip.IsEqual(lsp,iData.endPre);
        }
        if(iData.isComNexSp){
            isEleSp = true;isEndpt = true;
        }else{
            isEndpt = ip.IsEqual(lep,iData.endPre);
        }
        if(isEndpt){
            if(isEleSp){
                //这时需要比较前一元素的端点是否拥有最大纵坐标,这时要考虑前一元素是否
                //已经判断过了,如果已经判断过了,则不能再次判断.
                bool isLarge = lsp.Y() > lep.Y() ? true : false;
                ExicuteSwitch(isLarge,i,iData,isInPoly);
                if(i == 0){
                    iData.hasComFirSp = true;
                    iData.isComLastEp = true;
                }
                iData.hasComPreEp = false;
                iData.isComNexSp = false;
                return;
            }
            //线段结束点,isComNexSp 不可能是true.
            if(i == loop.Size() - 1 && !iData.hasComFirSp){
                bool isLarg_ = false;
                isLarg_ = IsEndptMaxCoord(loop[0],i,iData,true);
                ExicuteSwitch(isLarg_,i,iData,isInPoly);
            }
            bool isLarge = lep.Y() > lsp.Y() ? true : false;
            ExicuteSwitch(isLarge,i,iData,isInPoly);
            iData.hasComPreEp = true;
            iData.isComNexSp = true;
            return;
        }
        ExicuteSwitch(true,i,iData,isInPoly);
        iData.hasComPreEp = false;
        iData.isComNexSp = false;
    }
    static void OnePointCase(const ofts::DefLoop& loop,size_t i,
                             const ofts::DefArc& arc,InloopInt& iData,
                             bool isEp,bool &isInPoly)
    {
        //当两个交点之一是圆弧端点时,两相交点计算方式相同,且互不影响
        if(isEp){
            if((i == loop.Size()-1) && !iData.hasComFirSp){
                bool isLarg_ = false;
                isLarg_ = IsEndptMaxCoord(loop[0],i,iData,true);
                ExicuteSwitch(isLarg_,i,iData,isInPoly);
                iData.hasComFirSp = true;
                iData.isComLastEp = false;
            }
            bool isLarge = false;
            isLarge = IsArcEndptMaxCoord(arc,false);
            ExicuteSwitch(isLarge,i,iData,isInPoly);
            iData.hasComPreEp = true;
            iData.isComNexSp = true;
        }else{
            ExicuteSwitch(true,i,iData,isInPoly);
        }
    }
    static void TwoPointsCase(const ofts::DefLoop& loop,size_t i,
                              const ofts::DefArc& arc,
                              const ofts::Point& p1,const ofts::Point& p2,
                              InloopInt& iData,bool& isInPoly)
    {
        bool isLastEp = false;
        if((i == loop.Size() - 1) && iData.isComLastEp){
            isLastEp = true;
        }
        if(i == 0 && (p1.IsEqual(arc.GetSp(),iData.endPre) ||
                      p2.IsEqual(arc.GetSp(),iData.endPre))){
            iData.isComNexSp = true;
        }
        if(!iData.hasNotHor){
            iData.isComNexSp = p1.IsEqual(arc.GetSp(),iData.endPre) ||
                    p2.IsEqual(arc.GetSp(),iData.endPre);
        }
        if(iData.isComNexSp){
            //当最后元素既要比较开始点又要比较结束点时直接跳过.
            if(isLastEp){
                return;
            }
            bool isLarge = false;
            isLarge = IsArcEndptMaxCoord(arc,true);
            ExicuteSwitch(isLarge,i,iData,isInPoly);
            if(i == 0){
                iData.hasComFirSp = true;
                iData.isComLastEp = true;
            }
            double dis1 = CurveDist2D::PointToArcEndptDist(arc,p1,true);
            double dis2 = CurveDist2D::PointToArcEndptDist(arc,p2,true);
            const auto& noSp = dis1 > dis2 ? p1 : p2;
            bool isEp = false;
            //确定远离圆弧开始点的相交点noArcSp 是否跟圆弧结束点同一点
            isEp = noSp.IsEqual(arc.GetEp(),iData.endPre);
            if(isEp){
                if((i == loop.Size() - 1) && !iData.hasComFirSp){
                    bool isLarg_ = false;
                    isLarg_ = IsEndptMaxCoord(loop[0],i,iData,true);
                    ExicuteSwitch(isLarg_,i,iData,isInPoly);
                    iData.hasComFirSp = true;
                    iData.isComLastEp = false;
                }
                bool isLarge_ = false;
                isLarge_ = IsArcEndptMaxCoord(arc,false);
                ExicuteSwitch(isLarge_,i,iData,isInPoly);
                iData.isComNexSp = true;
            }else{
                //当isEp == false 时,noSp 不可能是开始点,即只能是非端点.
                ExicuteSwitch(true,i,iData,isInPoly);
                iData.isComNexSp = false;
            }
            return;
        }
        if(isLastEp){
            bool isLarge = false;
            isLarge = IsArcEndptMaxCoord(arc,false);
            ExicuteSwitch(isLarge,i,iData,isInPoly);
            //当intInfo.isComNexSp==false 时,noEp 不可能是开始点,即只能是非端点.
            ExicuteSwitch(true,i,iData,isInPoly);
            return;
        }
        const auto& arcEp = arc.GetEp();
        bool isEp1 = false,isEp2 = false;
        isEp1 = p1.IsEqual(arcEp,iData.endPre);
        isEp2 = p2.IsEqual(arcEp,iData.endPre);
        if(!isEp1 && !isEp2){
            iData.hasComPreEp = false;
            iData.isComNexSp = false;
            return;
        }
        //当两个交点之一是圆弧端点时,两相交点计算方式相同,且互不影响.
        OnePointCase(loop,i,arc,iData,isEp1,isInPoly);
        OnePointCase(loop,i,arc,iData,isEp2,isInPoly);
    }
    static void IntersectArcCase(const ofts::DefLoop& loop,
                                 const ofts::Point& lsp,
                                 const ofts::Point& lep, size_t i,
                                 const ofts::DefArc& arc,
                                 const ofts::GeomIntRes& iRes,
                                 InloopInt& iData,
                                 bool& isInPoly)
    {
        if(iRes.Num() == 2){
            TwoPointsCase(loop,i,arc,iRes.P1(),iRes.P2(),iData,isInPoly);
            return;
        }
        if(i == loop.Size() - 1 && iData.isComLastEp){
            bool isLarge = false;
            isLarge = IsArcEndptMaxCoord(arc,false);
            ExicuteSwitch(isLarge,i,iData,isInPoly);
            return;
        }
        if(i == 0 && iRes.P1().IsEqual(arc.GetSp(),iData.endPre)){
            iData.isComNexSp = true;
        }
        //如果i!=0,当且仅当intInfo.isComNexSp==true 时,
        //isEleSp = true,即isEleSp 取值由intInfo._isComNexSp 决定.
        const auto& ip = iRes.P1();
        bool isEndpt = false,isEleSp = false;
        if(!iData.hasNotHor){
            iData.isComNexSp = ip.IsEqual(arc.GetSp(),iData.endPre);
        }
        if(iData.isComNexSp){
            isEleSp = true;
            isEndpt = true;
        }else{
            isEndpt = ip.IsEqual(arc.GetEp(),iData.endPre);
        }
        if(isEndpt){
            bool isLarge = false;
            if(isEleSp){
                isLarge = IsArcEndptMaxCoord(arc,true);
                ExicuteSwitch(isLarge,i,iData,isInPoly);
                if(i == 0){
                    iData.hasComFirSp = true;
                    iData.isComLastEp = true;
                }
                iData.hasComPreEp = false;
                iData.isComNexSp = false;
                return;
            }else{
                //线段结束点,isComNexSp 不可能是true.
                if(i == loop.Size() - 1 && !iData.hasComFirSp){
                    bool isLarg_ = false;
                    isLarg_ = IsEndptMaxCoord(loop[0],i,iData,true);
                    ExicuteSwitch(isLarg_,i,iData,isInPoly);
                }
                isLarge = IsArcEndptMaxCoord(arc,false);
                ExicuteSwitch(isLarge,i,iData,isInPoly);
                iData.hasComPreEp = true;
                iData.isComNexSp = true;
            }
            return;
        }
        double dist = BaseGeom2D::PointToLineDist(lsp,lep,arc.GetCp());
        double sub = std::abs(dist - arc.GetOptR());
        if(sub <= iData.horPre){/**
            const auto& c = loop.PreElem(i);
            if(c.Type() == Ofte::LINETYPE &&
                    ip.IsEqual(arc.GetSp(),iData.endPre)){
                const auto& sp = c._segment.GetSp();
                const auto& ep = c._segment.GetEp();
                if(CurveQual2D::IsHorizontalLine(sp,ep,iData.endPre)){
                    bool isLarge = false;
                    isLarge = IsArcEndptMaxCoord(arc,true);
                    ExicuteSwitch(isLarge,iData,isInPoly);
                }
            }*/
            return;
        }
        ExicuteSwitch(true,i,iData,isInPoly);
        iData.hasComPreEp = false;
        iData.isComNexSp = false;
    }
    static bool PointInRange(const numst::LimCoord& coord,const ofts::Point& p,
                             const double& e,bool positive)
    {
        if(coord.MinY() - e > p.Y() || coord.MaxY() + e < p.Y()){return false;}
        if(positive){
            if(coord.MaxX() + e < p.X()){return false;}
            return true;
        }
        if(coord.MinX() - e > p.X()){return false;}
        return true;
    }
};
#endif // PTINLOOPAUX_H
