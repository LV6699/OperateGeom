#ifndef NUMSTRUCT_H
#define NUMSTRUCT_H
#include "NumPreErr.h"
#include <cmath>
namespace NumStruct {
class ReLoopErr{
public:
    ReLoopErr(){}
    ReLoopErr(double cp,double r) : _aCpErr(cp),_aRadErr(r){}
    ReLoopErr(double l,double ar,double ac,double close = PreErr5_10) :
        _lAglErr(l),_aRadErr(ar),_aCpErr(ac),_closeErr(close){}
    void SetAglErr(double v){_lAglErr = v;}
    void SetRadErr(double v){_aRadErr = v;}
    void SetCpErr(double v){_aCpErr = v;}
    void SetCloseErr(double v){_aCpErr = v;}
    double RelAglErr() const{return _lAglErr;}
    double RelRadErr() const{return _aRadErr;}
    double RelCpErr() const{return _aCpErr;}
    double RelCloseErr() const{return _closeErr;}
private:
    double _lAglErr = 0;              //线段角度平行精度
    double _aRadErr = 0;              //圆弧半径相等
    double _aCpErr = 0;               //相同圆心精度
    double _closeErr = PreErr_1;      //环封闭精度
};
//几何包含参数
class ContParam{
public:
    ContParam(){}
    ContParam(double intp = PreErr5_10,double coin = PreErr5_10,
              double incp = PreErr5_10) :
        _intPre(intp),_coinPre(coin),_incPre(incp){}
    ContParam(double intp,double coin,double incp,bool isb) :
        _intPre(intp),_coinPre(coin),_incPre(incp),_isMatchBord(isb){}
    bool MatchCoord()const{return _isMatchCoord;}
    void SetMatchCoord(bool b){_isMatchCoord = b;}

public:
    double _intPre = PreErr5_10;    //相交精度
    double _coinPre = PreErr5_10;   //重合精度
    double _incPre = PreErr5_10;    //包含精度
    double _onBordPre = PreErr5_10; //点在曲线上精度
    bool _isMatchBord = true;       //匹配当前图元或环在其他图元或环边界上
    bool _isMatchCoord = true;      //计算前提前判断坐标包含
};
class IntParam{
public:
    IntParam(){}
    IntParam(double llInt,double lcInt,double ccInt,double endPre,
             double lctan,double cctan,double onSeg,double onArc,
             double lP = PreErr_6,double cP = PreErr_6,
             double lpx = PreErr_6,double lpy = PreErr_6,
             double lpz = PreErr_6,
             bool calTanCS = true,bool calTanCC = true,
             bool ccseg = false,bool ccarc = false) :
        _llIntPre(llInt),_lcIntPre(lcInt),_ccIntPre(ccInt),
        _endptPre(endPre),_lcTanPre(lctan),_ccTanPre(cctan),
        _onSegPre(onSeg),_onArcPre(onArc),_lParalPre(lP),
        _cParalPre(cP),_lParalXPre(lpx),_lParalYPre(lpy),
        _lParalZPre(lpz),_cal_CL_Tan(calTanCS),
        _cal_CC_Tan(calTanCC),_calParalLine(ccseg),
        _calParalCir(ccarc){}
    void SetEndptRele(bool is,double e){_calEndpt = is;_endptPre = e;}
    void SetIntErr(double err){_llIntPre = err;_lcIntPre = err;_ccIntPre = err;}
    void SetOnCurveErr(double err){_onSegPre = err;_onArcPre = err;}
    void SetTanErr(double err){_lcTanPre = err;_ccTanPre = err;}
    void SetParalErr(double err){_lParalPre = err;_cParalPre = err;}
    void SetParalAxisErr(double err){_lParalXPre = err;_lParalYPre = err;_lParalZPre = err;}
    void SetCalcTan(bool b){_cal_CL_Tan = b;_cal_CC_Tan = b;}
    void SetCalcParal(bool b){_calParalLine = b;_calParalCir = b;}
    const double& LLIntPre()const{return _llIntPre;}
    const double& LCIntPre()const{return _lcIntPre;}
    const double& CCIntPre()const{return _ccIntPre;}
    const double& EndptPre()const{return _endptPre;}
    const double& LCTanPre()const{return _lcTanPre;}
    const double& CCTanPre()const{return _ccTanPre;}
    const double& OnSegPre()const{return _onSegPre;}
    const double& OnArcPre()const{return _onArcPre;}
    const double& LParalPre()const{return _lParalPre;}
    const double& CParalPre()const{return _cParalPre;}
    const double& LinePXPre()const{return _lParalXPre;}
    const double& LinePYPre()const{return _lParalYPre;}
    const double& LinePZPre()const{return _lParalZPre;}
    const bool& Cal_CL_Tan()const{return _cal_CL_Tan;}
    const bool& Cal_CC_Tan()const{return _cal_CC_Tan;}
    const bool& CalParalLine()const{return _calParalLine;}
    const bool& CalParalCir()const{return _calParalCir;}
    const bool& CalEndpt()const{return _calEndpt;}
public:
    double _llIntPre = PreErr_10;     //直线与直线相交精度
    double _lcIntPre = PreErr_10;     //直线与圆相交精度
    double _ccIntPre = PreErr_10;     //圆与圆相交精度
    double _endptPre = PreErr_10;     //曲线端点重合精度
    double _lcTanPre = PreErr_10;     //直线与圆相切精度
    double _ccTanPre = PreErr_10;     //圆与圆相切精度
    double _onSegPre = PreErr_10;     //点在线段上精度
    double _onArcPre = PreErr_10;     //点在圆弧上精度
    double _lParalPre = PreErr_6;     //线段平行精度
    double _cParalPre = PreErr_6;     //圆弧平行精度
    double _lParalXPre = PreErr_10;   //直线是平行x 轴精度
    double _lParalYPre = PreErr_10;   //直线是平行y 轴精度
    double _lParalZPre = PreErr_10;   //直线是平行z 轴精度
    bool _cal_CL_Tan = true;          //是否计算相切直线圆相交
    bool _cal_CC_Tan = true;          //是否计算相切圆圆相交
    bool _calParalLine = false;       //是否计算重合线段相交
    bool _calParalCir = false;        //是否计算重合圆弧相交
    bool _calEndpt = true;
};
class FitParam{
    //拟合结果主要受_bh 和_aVar 影响
public:
    FitParam(){}
    FitParam(double linear,double cpErr,double rErr) :
        _segLinear(linear),_cpErr(cpErr),_rErr(rErr){}
    FitParam(double bh,double aVar) : _bh(bh),_aVar(aVar){
        _bhSqu = bh * bh;
    }
    FitParam(double bh,double aVar,double cpVar,
             double rVar,double maxL,double minR,double maxR) :
        _bh(bh),_aVar(aVar),_cpVar(cpVar),_rVar(rVar),
        _maxLen(maxL),_minR(minR),_maxR(maxR){
        _maxLenSqu = maxL * maxL;
        _cpVarSqu = cpVar * cpVar;
        _bhSqu = bh * bh;
    }
    const double& SegLinear()const{return _segLinear;}
    const double& LinearSin()const{return _lineSin;}
    const double& CenterErr()const{return _cpErr;}
    const double& RadiusErr()const{return _rErr;}
    const double& Bh()const{return _bh;}
    const double& AglVar()const{return _aVar;}
    const double& CpVar()const{return _cpVar;}
    const double& RVar()const{return _rVar;}
    const double& MinR()const{return _minR;}
    const double& MaxR()const{return _maxR;}
    const double& MaxLen()const{return _maxLen;}
    const double& MaxLenSqur()const{return _maxLenSqu;}
    const double& CpVarSqur()const{return _cpVarSqu;}
    const double& BhSqur()const{return _bhSqu;}
    void  SetSegLinear(double v){_segLinear = v;}
    void  SetLinearSin(double v){_lineSin = v;}
    void  SetCenterErr(double v){_cpErr = v;}
    void  SetRadiusErr(double v){_rErr = v;}
    void  SetBhErr(double v){_bh = v;}
    void  SetAglValErr(double v){_aVar = v;}
    void  SetCpErr(double v){_cpVar = v;}
    void  SetRVarErr(double v){_rVar = v;}
    void  SetMinR(double v){_minR = v;}
    void  SetMaxR(double v){_maxR = v;}
    void  SetMaxLen(double v){_maxLen = v;}
    void  SetMaxLenSqu(double v){_maxLenSqu = v;}
    void  SetCpErrSqu(double v){_cpVarSqu = v;}
    void  SetBhErrSqu(double v){_bhSqu = v;}

private:
    double _segLinear = PreErr5_8;   //同一直线上线性误差(角度制)
    double _lineSin = PreErr5_8;     //_segLinear 的正弦值
    double _cpErr = PreErr5_6;       //同一圆上圆弧圆心误差
    double _rErr = PreErr5_6;        //同一圆上圆弧半径误差
    double _bh = 8e-3;            //弓高精度
    double _aVar = 10;            //拐角精度
    double _cpVar = 3;            //圆心变化精度(已设为经验值,别随意调整)
    double _rVar = 3;             //半径变化精度(已设为经验值,别随意调整)
    double _maxLen = 5;           //线段长度限制(已设为经验值,别随意调整)
    double _minR = PreErr_6;      //最小圆半径(已设为经验值,别随意调整)
    double _maxR = 5e3;           //最大圆半径(已设为经验值,别随意调整)
    double _bhSqu = 6.4e-5;       //内部计算数据,(需要初始化,别调整)
    double _maxLenSqu = 25;       //内部计算数据,(需要初始化,别调整)
    double _cpVarSqu = 9;         //内部计算数据,(需要初始化,别调整)
};
class DiscParam{
public:
    DiscParam(){}
    DiscParam(bool iss,double sstep,bool isa,bool isc,
              double cstep) : _isDiscSeg(iss),_segStep(sstep), _isDiscArc(isa),
        _isDiscCir(isc),_cirStep(cstep){}
    const bool& IsDiscSeg()const{return _isDiscSeg;}
    const bool& IsDiscArc()const{return _isDiscArc;}
    const bool& IsDiscCir()const{return _isDiscCir;}
    const double& SegStep()const{return _segStep;}
    const double& CirStep()const{return _cirStep;}
    void SetDiscSeg(bool b){_isDiscSeg = b;}
    void SetDiscArc(bool b){_isDiscArc = b;}
    void SetDiscCir(bool b){_isDiscCir = b;}
    void SetSegStep(double v){_segStep = v;}
    void SetCirStep(double v){_cirStep = v;}
public:
    bool _isDiscSeg = false;
    bool _isDiscCir = true;
    bool _isDiscArc = true;
    double _segStep = 2;   //点距离
    double _cirStep = 5;   //点间圆心角(角度制)
};
class LimCoord{
public:
    LimCoord(){}
    LimCoord(double x,double x1,double y,double y1) :
        _minx(x),_maxx(x1),_miny(y),_maxy(y1),_hasInit(true){}
    const double& MinX()const{return _minx;}
    const double& MaxX()const{return _maxx;}
    const double& MinY()const{return _miny;}
    const double& MaxY()const{return _maxy;}
    const bool& HasInit()const{return _hasInit;}

    bool IsInclude(double x,double y,double e)const{
        return (_minx - e < x) && (_maxx + e > x) &&
                (_miny - e < y) && (_maxy + e > y);
    }
    void SetMinX(double d){_minx = d;}
    void SetMaxX(double d){_maxx = d;}
    void SetMinY(double d){_miny = d;}
    void SetMaxY(double d){_maxy = d;}
    void SetInit(bool b){_hasInit = b;}

private:
    double _minx = 0;
    double _maxx = 0;
    double _miny = 0;
    double _maxy = 0;
    bool _hasInit = false;
};
}
namespace numst = NumStruct;
#endif // NUMSTRUCT_H
