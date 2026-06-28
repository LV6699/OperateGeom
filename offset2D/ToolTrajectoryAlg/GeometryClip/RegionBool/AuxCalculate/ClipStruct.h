#ifndef GEOMSTRUCT_H
#define GEOMSTRUCT_H
#include "../../../CommonFile/DataStructure.h"
namespace clst {
enum class ClipType {NoClip,Intersection,Union,Difference,Xor};
enum class FillRule {EvenOdd,NonZero,Positive,Negative};
class CntCurve{
public:
    CntCurve(){}
    CntCurve(size_t i,bool b) : _id(i),_isSp(b){}
    CntCurve(size_t i,ofts::DefElem* c,bool b) : _id(i),_curve(c),_isSp(b){}

    const size_t& Id()const{return _id;}
    const bool& IsSp()const{return _isSp;}
    const double& Agl()const{return _agl;}
    const bool& Visited()const{return _visited;}
    const bool& IsValid()const{return _isValid;}
    const double& IncAgl()const{return _incAgl;}
    const ofts::DefElem* Curve()const{return _curve;}

    void SetId(size_t i){_id = i;}
    void SetSp(bool b){_isSp = b;}
    void SetAgl(double d){_agl = d;}
    void SetVisited(bool b){_visited = b;}
    void SetValid(bool b){_isValid = b;}
    void SetIncAgl(double d){_incAgl = d;}
    void SetCurve(ofts::DefElem* c){_curve = c;}

public:
    size_t _id;          //相连的曲线
    bool _isSp = false;  //当前曲线是否与_curve 起点重合
    double _agl = 0;     //连接曲线在连接点处的切线角度
    double _incAgl = 0;  //夹角
    bool _visited = false;
    bool _isValid = true;
    ofts::DefElem* _curve = nullptr;
}; 
class IntId{
public:
    IntId(){}
    IntId(size_t i,bool sp) : _id(i),_isSp(sp){}
public:
    size_t _id;
    bool _isSp = false;
    double _agl = 0;
    double _incAgl = 0;
};
class ClipCurve{
public:
    ClipCurve(){}
    ClipCurve(ofts::DefElem& c) : _curve(c){}
    ClipCurve(size_t i,ofts::DefElem& c) : _id(i),_curve(c){}

    const size_t& Id()const{return _id;}
    const int& VisNum()const{return _visNum;}
    const ofts::DefElem Curve()const{return _curve;}
    const double& Agl()const{return _agl;}
    const bool& Visited()const{return _visited;}
    const std::vector<CntCurve>& SpCnts()const{return _spCnts;}
    const std::vector<CntCurve>& EpCnts()const{return _epCnts;}
    bool IsEmpty()const{return _spCnts.empty() && _epCnts.empty();}
    const bool& IsValid()const{return _isValid;}
    const bool& Double()const{return _isDouble;}
    bool IsJudgeValid()const{
        if(_curve.IsDouClip()){return _visNum < 2;}
        return _visNum < 1;
    }
    void SetAgl(double d){_agl = d;}
    void SetVisited(bool b){_visited = b;}
    void SetCurve(ofts::DefElem& c){_curve = c;}
    void SetSpCnts(const std::vector<CntCurve>& d){_spCnts = d;}
    void SetEpCnts(const std::vector<CntCurve>& d){_epCnts = d;}
    void SetValid(bool b){_isValid = b;}
    void SetId(size_t i){_id = i;}
    void SetVisNum(int n){_visNum = n;}
    void SetDouble(bool b){_isDouble = b;}
    void IncVisNum(){_visNum ++;}
    void JudgeValid(){SetValid(IsJudgeValid());}
public:
    size_t _id;          //相连的曲线
    int _visNum = 0;
    double _agl = 0;               //曲线在连接点处的切线角度
    bool _isValid = true;
    bool _visited = false;
    bool _isDouble = false;
    ofts::DefElem _curve;
    std::vector<IntId>_iIds;
    std::vector<CntCurve>_spCnts;   //起点相连的曲线
    std::vector<CntCurve>_epCnts;   //起点相连的曲线
}; 
}
#endif // GEOMSTRUCT_H
