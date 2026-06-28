#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
/**
  @author: lvdong
  @date: 2023-05
**/
#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include <numeric>
#include "NumStruct.h"
#include "PureNumCalc.h"
#include "EnumType.h"
using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
///using std::make_unique;
using std::make_shared;
using namespace Ofte;
const bool isPrintEInfo = true;
const bool isPrintInfo = false;
#ifdef _WIN64
#include <corecrt_math_defines.h>
#ifdef CAMALGORITHM_WIN_DLL
#ifdef func_shared_EXPORTS
#define CAMALGORITHM_EXPORT __declspec(dllexport)
#else
#define CAMALGORITHM_EXPORT __declspec(dllimport)
#endif // CAMALGORITHM
#else
#define CAMALGORITHM_EXPORT
#endif
#else
#define CAMALGORITHM_EXPORT __attribute__((visibility("default")))
#endif // _WIN64
#ifndef oft_nopti_code
#define oft_nopti_code 0
#endif
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
#define DebugModel 0
#define TimeStatistic 0
#define _ViewDebug 0
namespace OffsetStruct {
class CAMALGORITHM_EXPORT Coord
{
public:
    Coord(){}
    Coord(double x,double y):_x(x),_y(y){}
    Coord(double x,double y,double z):_x(x),_y(y),_z(z){}
private:
    double _x;
    double _y;
    double _z = 0;
};
class Point;
class DefPlane;
class PntLable{
public:
    PntLable(){}
    ///PntLable(double v) : _norAngle(v){}
    ///PntLable(bool is) : _isOnBorder(is){}
    PntLable(double v,bool is) : _norAngle(v),_isOnBorder(is){}
    void SetNormalAngle(double v){_norAngle = v;}
    void SetOnBorder(bool v){_isOnBorder = v;}
    void SetOnSteep(bool v){_isOnSteep = v;}
    void SetVisited(bool v){_hasVisited = v;}
    void SetContactAngle(double v){_cpAngle = v;}
    void SetContactNormal(Point* v){_normal = v;}
    bool IsOnBorder() const{return _isOnBorder;}
    bool IsOnSteep() const{return _isOnSteep;}
    bool HasVisited() const{return _hasVisited;}
    double NormalAngle() const{return _norAngle;}
    double ContactAngle() const{return _cpAngle;}
    Point* Normal() const{
        return _normal;}
public:
    int _id = -1;
    double _norAngle = 0;         /// 与xy 平面夹角.
    bool _isOnBorder = false;     /// 是否位于边界线上.
    bool _isOnSteep = false;      /// 是否位于陡峭面上.
    bool _hasVisited = false;
    double _cpAngle;              /// 对应接触点法线与xy 平面角度.
    Point *_aglRefPt[2];
    Point *_normal = nullptr;
    std::shared_ptr<Point>_cnt = nullptr;      ///在曲线上相邻的某个点.
    std::shared_ptr<DefPlane>_plan = nullptr;  ///所在曲线的平面.
    std::shared_ptr<DefPlane>_nplan = nullptr; ///所在曲线的法平面.
};
class CAMALGORITHM_EXPORT  Point
{
public:
    Point(){}
    ///Point(double x,double y):Coord(x,y){}
    ///Point(double x,double y,double z):Coord(x,y,z){}
    Point(double x,double y):_x(x),_y(y){}
    Point(double x,double y,double z):_x(x),_y(y),_z(z){}
    Point(double x,double y,double z,const PntLable& l) :
        _x(x),_y(y),_z(z),_lable(l){}
    Point(const PntLable& l) : _lable(l){}
    void SetX(double x){_x = x;}
    void SetY(double y){_y = y;}
    void SetZ(double z){_z = z;}
    double X() const{return _x;}
    double Y() const{return _y;}
    double Z() const{return _z;}
    void SetNormal(const Point& p) {
        _lable._normal = new Point(p.X(),p.Y(),p.Z());}
    void SetPlan(const std::shared_ptr<DefPlane>& p){_lable._plan = p;}
    void SetNPlan(const std::shared_ptr<DefPlane>& p){_lable._nplan = p;}
    void SetCnt(const Point& p){
        _lable._cnt = std::make_shared<Point>(p.X(),p.Y(),p.Z());
    }
    void Normalize(){
        double l = Length();
        if(l < PreErr_15){_x = 0;_y = 0;_z = 0;return;}
        _x /= l,_y /= l,_z /= l;
    }
    void Normalize2D(){
        double l = Length2D();
        if(l < PreErr_15){_x = 0;_y = 0;return;}
        _x /= l,_y /= l;
    }
    PntLable PointLable()const{return _lable;}
    Point operator+(const Point& p)const{
        return Point(_x + p.X(),_y + p.Y(),_z + p.Z());
    }
    Point operator-(const Point& p)const{
        return Point(_x - p.X(),_y - p.Y(),_z - p.Z());
    }
    Point operator*(double d)const{
        return Point(_x * d,_y * d,_z * d);
    }
    double operator*(const Point& p)const{
        return _x * p.X() + _y * p.Y() + _z * p.Z();
    }
    ///friend Point operator*(double scal,const Point& p){
    ///return Point(p._x * scal, p._y * scal, p._z * scal);
    ///}
    Point operator/(double a) const{
        return Point(_x/a,_y/a,_z/a);
    }
    bool operator==(const Point& p)const{
        return _x == p.X() && _y == p.Y() && _z == p.Z();
    }
    Point Subtract(const Point& v)const {
        return {_x - v.X(), _y - v.Y(), _z - v.Z()};
    }
    double Dot(const Point& v)const {
        return _x * v.X() + _y * v.Y() + _z * v.Z();
    }
    double Dot2D(const Point& p)const {
        return _x * p.X() + _y * p.Y();
    }
    Point Cross(const Point &v)const{
        return {_y * v.Z() - _z * v.Y(), _z * v.X() - _x * v.Z(),
                    _x * v.Y() - _y * v.X()};
    }
    double Cross2D(const Point& p)const {
        return _x * p.Y() - _y * p.X();
    }
    double LengthSquare()const{return _x*_x + _y*_y + _z*_z;}
    double LengthSquare2D()const{return _x*_x + _y*_y;}
    double Length()const{return std::sqrt(_x*_x + _y*_y + _z*_z);}
    double Length2D()const{return std::sqrt(_x*_x + _y*_y);}
    Point Normalized()const {
        double l = Length();
        if(l < PreErr_15){return {_x,_y,_z};}
        return {_x/l,_y/l,_z/l};
    }
    Point Normalized2D()const {
        double l = Length2D();
        if(l < PreErr_15){return {_x,_y};}
        return {_x/l,_y/l};
    }
    ///叉积,P^Q>0,P 在Q 的顺时针方向<0,P 在Q 的逆时针方向=0,P,Q 共线,可能同向或反向.
    double operator^(const Point&p)const{
        return _x * p.Y() - _y * p.X();
    }
    double PointSqrt2D() const{return _x * _x + _y * _y;}
    double PointAbs2D() const{return std::sqrt(PointSqrt2D());}
    double DistSquare2D(const Point& P)const{
        double dis = 0;double d;
        d = _x;d -= P._x;d *= d;dis += d;
        d = _y;d -= P._y;d *= d;dis += d;
        return dis;
    }
    double DistSquare3D(const Point& P)const{
        double dis = 0;double d;
        d = _x;d -= P._x;d *= d;dis += d;
        d = _y;d -= P._y;d *= d;dis += d;
        d = _z;d -= P._z;d *= d;dis += d;
        return dis;
    }
    double Distance2D(const Point& P)const{
        return std::sqrt(DistSquare2D(P));
    }
    double Distance3D(const Point& P)const{
        return std::sqrt(DistSquare3D(P));
    }
    double DistSquare(const Point& p,bool is3d = false)const{
        if(is3d){return DistSquare3D(p);}
        return DistSquare2D(p);
    }
    double Distance(const Point& p,bool is3d = false)const{
        if(is3d){return Distance3D(p);}
        return Distance2D(p);
    }
    bool IsSamePoint2D(const Point &P,double e)const{
        double d = DistSquare2D(P);
        return d <= (e * e) ? true : false;
    }
    bool IsSamePoint3D(const Point &P,double e)const{
        double d = DistSquare3D(P);
        return d <= (e * e) ? true : false;
    }
    bool IsSameCoord2D(const Point &p, double e)const{
        return std::abs(_x - p._x) <= e && std::abs(_y - p._y) <= e;
    }
    bool IsSameCoord3D(const Point &p, double e)const{
        return std::abs(_x - p._x) <= e && std::abs(_y - p._y) <= e &&
                std::abs(_z - p._z) <= e;
    }
    bool IsEqual(const Point& p,double err,bool is3d = false)const{
        if(is3d){return IsSamePoint3D(p,err);}
        return IsSamePoint2D(p,err);
    }
    double ManhattanDist2D(const Point &p)const{
        return std::abs(_x - p._x) + std::abs(_y - p._y);
    }
    double ManhattanDist3D(const Point &p)const{
        return std::abs(_x - p._x) + std::abs(_y - p._y) + std::abs(_z - p._z);
    }
    Point Midpoint(const Point &p)const{
        return {(p.X() + _x)/2,(p.Y() + _y)/2,(p.Z() + _z)/2};
    }
    void SetPointLable(const PntLable& l){_lable = l;}
    PntLable Lable() const{return _lable;}
    bool IsOnBorder(const Point& p) const{
        return _lable.IsOnSteep() != p._lable.IsOnSteep();
    }
    Point Normal()const{return *_lable._normal;}
    Point Cnt()const{
        if(_lable._cnt == nullptr){return Point();}
        return *_lable._cnt;
    }
    std::shared_ptr<DefPlane> Plan()const{return _lable._plan;}
    std::shared_ptr<DefPlane> NPlan()const{return _lable._nplan;}
    bool IsZero(double e)const {
        return std::abs(_x) < e && std::abs(_y) < e && std::abs(_z) < e;
    }
    bool IsLegal()const{
        return pnum::IsLegalNum(_x) && pnum::IsLegalNum(_y) && pnum::IsLegalNum(_z);
    }
    bool IsParallel(const Point& p,double e = PreErr5_10)const{
        double normSq1 = LengthSquare(),normSq2 = p.LengthSquare(),ee = e*e;
        if(normSq1 < ee || normSq2 < ee) {return true;}
        double dot = Dot(p);
        double cosSqTheta = (dot * dot) / (normSq1 * normSq2);
        return std::abs(cosSqTheta - 1.0) < e;
    }
public:
    double _x;
    double _y;
    double _z = 0;
    PntLable _lable;
};
class CAMALGORITHM_EXPORT  DefSeg
{
public:
    DefSeg(){}
    DefSeg(const Point& s,const Point& e,bool is3d = false) :
        _segSp(s),_segEp(e),_is3d(is3d){}
    void SetSp(const Point& p) {_segSp = p;}
    void SetEp(const Point& p) {_segEp = p;}
    void SetEndpoint(const Point& p,bool issp){
        if(issp){SetSp(p);}
        else{SetEp(p);}
    }
    void Reverse() {Point p = _segSp;_segSp = _segEp;_segEp = p;}
    void PointSplit(const Point& p,bool isChEp){
        if(isChEp){_segEp = p;}else{_segSp = p;}
    }
    void SetEndptZ(double z){_segSp.SetZ(z);_segEp.SetZ(z);}
    void Set3D(bool b){_is3d = b;}
    const Point& GetSp()const{return _segSp;}
    const Point& GetEp()const{return _segEp;}
    const Point& Endpoint(bool isEp) const{
        if(isEp){return _segEp;}return _segSp;
    }
    double Angle2D() const{
        double dx = _segEp.X() - _segSp.X(),dy = _segEp.Y() - _segSp.Y();
        double agl = std::atan2(dy,dx);agl *= _AlgRad;
        if(agl < 0){agl += 360;}
        return agl;
    }
    bool IsHorizon2D(double v) const{
        double agl = Angle2D();
        if(std::abs(agl) < v || std::abs(agl-180) < v){
            return true;}
        return false;
    }
    bool IsVertical2D(double v) const{
        double agl = Angle2D();
        if(std::abs(agl-90) < v || std::abs(agl-270) < v){return true;}
        return false;
    }
    bool IsValid2D(double l) const{return !_segSp.IsSamePoint2D(_segEp,l);}
    bool IsValid3D(double l) const{return !_segSp.IsSamePoint3D(_segEp,l);}
    bool IsEndpoint(const Point& p,double e,bool is2d = true)const{
        if(is2d){return p.IsSamePoint2D(_segSp,e) || p.IsSamePoint2D(_segEp,e);}
        return p.IsSamePoint3D(_segSp,e) || p.IsSamePoint3D(_segEp,e);
    }
    bool Is3D()const{return _is3d;}
    double Lenth2D()const{return _segSp.Distance2D(_segEp);}
    double Lenth3D()const{return _segSp.Distance3D(_segEp);}
    double Lenth()const{return _segSp.Distance(_segEp,_is3d);}
    double LenSquare2D()const{return _segSp.DistSquare2D(_segEp);}
    double LenSquare3D()const{return _segSp.DistSquare3D(_segEp);}
    double LenSquare()const{return _segSp.DistSquare(_segEp,_is3d);}
public:
    Point _segSp;
    Point _segEp;
    bool _is3d = false;
};
class CAMALGORITHM_EXPORT  DefArc
{
public:
    DefArc(){}
    /*DefArc(const Point& s,const Point& e,const Point& c,Ofte::RotDir r) :
        _arcSp(s),_arcEp(e),_arcCp(c),_isCW(r){
        InitR();
        _isCW = (r == Ofte::CLOCKWISE);
    }*/
    DefArc(const Point& s,const Point& e,const Point& c,bool isCW) :
        _arcSp(s),_arcEp(e),_arcCp(c),_isCW(isCW){
        //_isCW = isCW ? Ofte::CLOCKWISE : Ofte::ANTICLOCKWISE;
        InitR();
    }
    DefArc(const Point& s,const Point& e,const Point& c,
           bool isCW,const Point& n,bool is3d = false) :
        _arcSp(s),_arcEp(e),_arcCp(c),_isCW(isCW),_n(n),_is3d(is3d){
        //_isCW = isCW ? Ofte::CLOCKWISE : Ofte::ANTICLOCKWISE;
        InitR3D();
    }
    DefArc(const Point& s,const Point& e,const Point& c,double r,bool isCW) :
        _arcSp(s),_arcEp(e),_arcCp(c),_arcR(r),_isCW(isCW){
        //_isCW = isCW ? Ofte::CLOCKWISE : Ofte::ANTICLOCKWISE;
    }
    /*DefArc(const Point& s,const Point& e,const Point& c,double r,Ofte::RotDir rot):
        _arcSp(s),_arcEp(e),_arcCp(c),_arcR(r),_isCW(rot){
        _isCW = (rot == Ofte::CLOCKWISE);
    }*/
    void SetSp(const Point& p){_arcSp = p;}
    void SetEp(const Point& p){_arcEp = p;}
    void SetCp(const Point& p){_arcCp = p;}
    void SetR(double R){_arcR = R;}
    /*void SetRot(Ofte::RotDir r){
        _isCW = r;_isCW = (r == Ofte::CLOCKWISE);}*/
    void SetCW(bool b){_isCW = b;/*_isCW = b ?
                                                                                                              Ofte::CLOCKWISE : Ofte::ANTICLOCKWISE;*/}
    void SetEndpoint(const Point& p,bool issp){
        if(issp){SetSp(p);}
        else{SetEp(p);}
    }
    void SetNormal(const Point& n){_n = n;}
    void InitR(){_arcR = GetCalR();}
    void InitR3D(){_arcR = GetCalR3D();}
    void OptInitR(double value = PreErr5_3){
        if(_arcR < value) InitR();}
    void Reverse(){
        Point p = _arcSp;_arcSp = _arcEp;_arcEp = p;
        /*_isCW = _isCW == Ofte::CLOCKWISE ?
                    Ofte::ANTICLOCKWISE : Ofte::CLOCKWISE;*/
        _isCW = !_isCW;
    }
    void PointSplit(const Point& p,bool isChEp){
        if(isChEp){_arcEp = p;}else{_arcSp = p;}
    }
    void SetEndptZ(double z){_arcSp.SetZ(z);_arcEp.SetZ(z);_arcCp.SetZ(z);}
    void Set3D(bool b){_is3d = b;}
    double GetCal2D()const{
        double R1 = _arcSp.Distance2D(_arcCp);
        double R2 = _arcEp.Distance2D(_arcCp);
        return (R1 + R2) / 2;
    }
    double GetCalR3D()const{
        double R1 = _arcSp.Distance3D(_arcCp);
        double R2 = _arcEp.Distance3D(_arcCp);
        return (R1 + R2) / 2;
    }
    double GetCalR()const{
        if(_is3d){return GetCalR3D();}
        return GetCal2D();
    }
    double GetOptR(double v = PreErr5_3)const{
        if(_arcR > v){return _arcR;}return GetCalR();
    }
    double GetOptR3D(double v = PreErr5_3)const{
        if(_arcR > v){return _arcR;}return GetCalR3D();
    }
    const Point& Endpoint(bool isEp)const{if(isEp){return _arcEp;}return _arcSp;}
    const Point& GetSp()const{return _arcSp;}
    const Point& GetEp()const{return _arcEp;}
    const Point& GetCp()const{return _arcCp;}
    const double& R()const{return _arcR;}
    //RotDir ArcRot()const{return _isCW;}
    bool IsCW()const{return _isCW/*_isCW == Ofte::CLOCKWISE*/;}
    const Point& Normal()const{return _n;}
    bool IsEndpoint(const Point& p,double e,bool is2d = true)const{
        if(is2d){return p.IsSamePoint2D(_arcSp,e) || p.IsSamePoint2D(_arcEp,e);}
        return p.IsSamePoint3D(_arcSp,e) || p.IsSamePoint3D(_arcEp,e);
    }
    bool Is3D()const{return _is3d;}
public:
    Point _arcSp;
    Point _arcEp;
    Point _arcCp;
    double _arcR = 0;
    bool _isCW;
    bool _is3d = false;
    //Ofte::RotDir _isCW;
    Point _n = Point(0,0,1);
};
class CAMALGORITHM_EXPORT  DefCircle
{
public:
    DefCircle(){}
    DefCircle(const Point& p,double r):_cirCp(p),_cirR(r){}
    DefCircle(const Point& p,double r,bool isCw):
        _cirCp(p),_cirR(r),_isCW(isCw){}
    DefCircle(const Point& p,double r,const Point& n,bool isCw = true):
        _cirCp(p),_cirR(r),_n(n),_isCW(isCw){}
    void SetCp(const Point& p){_cirCp = p;}
    void SetR(double r){_cirR = r;}
    void SetCW(bool v){_isCW = v;}
    void SetEndptZ(double z){_cirCp.SetZ(z);}
    void SetNormal(const Point& n){_n = n;}
    const Point& GetCp()const {return _cirCp;}
    const double& R()const {return _cirR;}
    const Point& Normal()const{return _n;}
    Point UpPt() const{
        return Point(_cirCp.X(),_cirCp.Y()+_cirR,_cirCp.Z());}
    Point DownPt() const{
        return Point(_cirCp.X(),_cirCp.Y()-_cirR,_cirCp.Z());}
    Point LeftPt() const{
        return Point(_cirCp.X()-_cirR,_cirCp.Y(),_cirCp.Z());}
    Point RightPt() const{
        return Point(_cirCp.X()+_cirR,_cirCp.Y(),_cirCp.Z());}
    bool IsContPt(const Point& p,double e) const{
        return p.Distance2D(_cirCp) - e <= _cirR;
    }
    double Length() const{return 2 * Num_PI * _cirR;}
    bool IsCW() const{return _isCW;}
public:
    Point _cirCp;
    double _cirR;
    bool _isCW = true;
    Point _n = Point(0,0,1);  ///所在平面法向量
};
class CAMALGORITHM_EXPORT DefCurve
{
public:
    DefCurve(){}
    DefCurve(ElemType t,const DefSeg& s) : _type(t),_segment(s){}
    DefCurve(ElemType t,const DefArc& a) : _type(t),_arc(a){}
public:
    DefArc _arc;
    DefSeg _segment;
    ElemType _type;
};
class OftBasePara{
public:
    OftBasePara(){}
public:
    OftBasePara(double r,double v) :
        _toolRadius(r),_offsetVal(v){_isInitail = true;}
    void SetToolRadius(double r){_toolRadius = r;}
    void SetOffsetValue(double v){_offsetVal = v;}
    void SetIsInitail(bool v){_isInitail = v;}
    double ToolRadius() const{return _toolRadius;}
    double OffsetValue() const{return _offsetVal;}
    bool IsInitail() const{return _isInitail;}
private:
    double _offsetVal = 0;
    double _toolRadius = 0;
    bool _isInitail = false;
};
class DefPlane{
public:
    DefPlane(){}
    DefPlane(double a,double b,double c,const Point& n) :
        _A(a), _B(b), _C(c), _n(n){}
    DefPlane(double a,double b,double c,double d) :
        _A(a), _B(b), _C(c), _D(d){
        _n.SetX(a);_n.SetY(b);_n.SetZ(c);
        _n.Normalize();
    }
    DefPlane(const Point& n,const Point& p) :
        _A(n.X()), _B(n.Y()), _C(n.Z()),_n(n){
        _D = -(n.X()*p.X() + n.Y()*p.Y() + n.Z()*p.Z());
    }
    void SetA(double v){_A = v;}
    void SetB(double v){_B = v;}
    void SetC(double v){_C = v;}
    void SetD(double v){_D = v;}
    void SetN(const Point& n){_n = n;}
    double A() const{return _A;}
    double B() const{return _B;}
    double C() const{return _C;}
    double D() const{return _D;}
    Point N() const{return _n;}
    bool IsHorizontal(double e) const{
        return std::abs(_n.X()) < e && std::abs(_n.Y()) < e &&
                std::abs(std::abs(_n.Z()) - 1) < e;
    }
    Point Normal() const {return Point(_A,_B,_C);}
    void Normalize() {
        double norm = std::sqrt(_A*_A + _B*_B + _C*_C);
        if (norm > PreErr5_12) {
            _A /= norm;_B /= norm;_C /= norm;_D /= norm;}
    }
    bool Contains(const Point& p, double e = PreErr5_10) const{
        return std::abs(_A*p.X() + _B*p.Y() + _C*p.Z() + _D) <= e;
    }
    bool IsValid(double e = PreErr_8){
        return (_A*_A + _B*_B + _C*_C) >= PreErr_8;
    }
public:
    Point _n;                 ///平面法向量
    bool _isGeneral = true;   ///是否由一般式定义
    double _A,_B,_C,_D;       ///平面系数
};
class CAMALGORITHM_EXPORT ElemLabel
{
public:
    ElemLabel() {}
public:
    int _mark = 0;
    bool _hasMerge = false;
    bool _isCirDisc = false;
    bool _isValid = true;
    bool _isClip = false;
    bool _isAbsValid = false;
    bool _isAbsInvalid = false;
    bool _isCntArc = false;
    Ofte::ToolPathType _sourceType;

    bool _isSpClip = false;  //开始点是裁剪(相交)点
    bool _isEpClip = false;  //结束点是裁剪(相交)点

    numst::LimCoord _limCoord;
    OftBasePara _baseParam;
};
class CAMALGORITHM_EXPORT  DefElem
{
public:
    DefElem(){}
    DefElem(ElemType type,const DefSeg& s):_type(type),_segment(s){}
    DefElem(ElemType type,const DefArc& a):_type(type),_arc(a){}
    DefElem(ElemType type,const DefCircle& c):_type(type),_circle(c){}
    DefElem(const DefSeg& s):_type(Ofte::LINETYPE),_segment(s){}
    DefElem(const DefArc& a):_type(Ofte::ARCTYPE),_arc(a){}
    DefElem(const DefCircle& c):_type(Ofte::CIRCLETYPE),_circle(c){}
    void SetSeg(const DefSeg& segment) {_segment = segment;}
    void SetArc(const DefArc& arc) {_arc = arc;}
    void SetCircle(const DefCircle& circle) {_circle = circle;}
    void SetType(ElemType t) {_type = t;}
    void SetSourceId(int i){_sourceId = i;}
    void SetMoveType(Ofte::MovePathType t){_moveType = t;}
    void SetValid(bool v){_label._isValid = v;}
    void SetIsClip(bool v){_label._isClip = v;}
    void SetAbsValid(bool v){_label._isAbsValid = v;}
    void SetCntCurve(bool v){_label._isCntArc = v;}
    void ChangeEndpt(const Point& p,bool isChEp){
        switch (_type) {
        case Ofte::LINETYPE: _segment.PointSplit(p,isChEp);break;
        case Ofte::ARCTYPE: _arc.PointSplit(p,isChEp);break;
        default: break;}
    }
    void SetSp(const Point& p){
        switch (_type) {
        case Ofte::LINETYPE: _segment.SetSp(p);break;
        case Ofte::ARCTYPE: _arc.SetSp(p);break;
        default:;break;}
    }
    void SetEndpoint(const Point& p,bool issp){
        switch (_type) {
        case Ofte::LINETYPE: _segment.SetEndpoint(p,issp);break;
        case Ofte::ARCTYPE: _arc.SetEndpoint(p,issp);break;
        default:;break;}
    }
    void SetEp(const Point& p){
        switch (_type) {
        case Ofte::LINETYPE: _segment.SetEp(p);break;
        case Ofte::ARCTYPE: _arc.SetEp(p);break;
        default:;break;}
    }
    void SetEndptZ(double z){
        switch (_type) {
        case Ofte::LINETYPE: _segment.SetEndptZ(z);break;
        case Ofte::ARCTYPE: _arc.SetEndptZ(z);break;
        default: _circle.SetEndptZ(z);break;}
    }
    void Reverse(){
        switch (_type) {
        case Ofte::LINETYPE:return _segment.Reverse();break;
        case Ofte::ARCTYPE:return _arc.Reverse();break;
        default: _circle.SetCW(!_circle.IsCW());break;}
    }
    void SetSourceType(ToolPathType t){_label._sourceType = t;}
    void SetId(size_t i){_id = i;}
    void SetSpClip(bool b){_label._isSpClip = b;}
    void SetEpClip(bool b){_label._isEpClip = b;}
    void SetLimCoord(const numst::LimCoord& c){_label._limCoord = c;}
    void SetLable(const ElemLabel& l){_label = l;}
    void SetMark(int i){_label._mark = i;}
    const DefSeg& GetSeg()const {return _segment;}
    const DefArc& GetArc()const {return _arc;}
    const DefCircle& GetCircle()const {return _circle;}
    ElemType Type()const {return _type;}
    int RootId()const {return _sourceId;}
    Ofte::MovePathType MoveType()const {return _moveType;}
    bool IsValid() const{return _label._isValid;}
    bool IsClipCurve() const{return _label._isClip;}
    bool IsAbsValid()const{return _label._isAbsValid;}
    bool IsCntCurve()const{return _label._isCntArc;}
    const Point& GetSp() const{
        switch (_type) {
        case Ofte::LINETYPE:return _segment.GetSp();break;
        case Ofte::ARCTYPE:return _arc.GetSp();break;
        default:return _circle.GetCp();}
    }
    const Point& GetEp() const{
        switch (_type) {
        case Ofte::LINETYPE:return _segment.GetEp();break;
        case Ofte::ARCTYPE:return _arc.GetEp();break;
        default:return _circle.GetCp();}
    }
    const Point& Endpoint(bool isEp) const{
        if(isEp){return GetEp();}return GetSp();
    }
    bool IsEndpoint(const Point& p,double e,bool is2D = true) const{
        switch (_type) {
        case Ofte::LINETYPE: return _segment.IsEndpoint(p,e,is2D);
        case Ofte::ARCTYPE: return _arc.IsEndpoint(p,e,is2D);
        default: return p.IsSamePoint2D(_circle._cirCp,e);}
    }
    bool IsSameCurve(const DefElem& c,double e)const{
        if(_type != c._type){return false;}
        if(_type == OffsetEnum::LINETYPE){
            return _segment._segSp.IsSamePoint2D(c._segment._segSp,e) &&
                    _segment._segEp.IsSamePoint2D(c._segment._segEp,e);
        }else if(_type == OffsetEnum::ARCTYPE){
            return  _arc._isCW == c._arc._isCW &&
                    _arc._arcSp.IsSamePoint2D(c._arc._arcSp,e) &&
                    _arc._arcEp.IsSamePoint2D(c._arc._arcEp,e) &&
                    _arc._arcCp.IsSamePoint2D(c._arc._arcCp,e);
        }else if(_type == OffsetEnum::CIRCLETYPE){
            return  std::abs(_circle._cirR - c._circle._cirR) <= e &&
                    _circle._cirCp.IsSamePoint2D(c._circle._cirCp,e);
        }
        return false;
    }
    bool IsValidEnd(double e = PreErr_10){return !GetSp().IsEqual(GetEp(),e);}
    size_t Id()const{return _id;}
    ToolPathType SetSourceType()const{return _label._sourceType;}
    bool IsSpClip()const{return _label._isSpClip;}
    bool IsEpClip()const{return _label._isEpClip;}
    bool IsClip()const{return IsSpClip() || IsEpClip();}
    bool IsDouClip()const{return IsSpClip() && IsEpClip();}
    const numst::LimCoord& GetLimCoord()const{return _label._limCoord;}
    const ElemLabel& Lable()const{return _label;}
    DefElem GetSplit(const Point& p,bool isChEp)const {
        auto c = *this;if(isChEp){c.ChangeEndpt(p,true);}
        else{c.ChangeEndpt(p,false);}
        return c;
    }
    int Mark()const{return _label._mark;}
public:
    DefArc _arc;
    DefSeg _segment;
    DefCircle _circle;
    ElemType _type;
    ElemLabel _label;
    DefPlane _plane;           //所在平面
    size_t _id = -1;
    Ofte::ToolPathType _elemSource;

private:
    int _sourceId;
    Ofte::MovePathType _moveType;
};
class GeomIntRes
{
public:
    GeomIntRes() {}
    GeomIntRes(const Point& p,bool isGeo = true) :
        _intNum(1),_isExistInt(true),_isExiGeoInt(isGeo),_intPnt1(p),_intPnt2(p) {}
    GeomIntRes(const Point& p1,const Point& p2,bool isGeo = true) :
        _intNum(2),_isExistInt(true),_isExiGeoInt(isGeo),_intPnt1(p1),_intPnt2(p2) {}
    void SetInt(){_isExistInt = true;}
    void SetGeoInt(){_isExiGeoInt = true;}
    void SetNum(int n){_intNum = n;}
    void SetP1(const Point& p){_intPnt1 = p;}
    void SetP2(const Point& p){_intPnt2 = p;}
    void SetInt1XY(double x,double y){_intPnt1.SetX(x);_intPnt1.SetY(y);}
    void SetInt2XY(double x,double y){_intPnt2.SetX(x);_intPnt2.SetY(y);}
    void SetIntXY(double x,double y){SetInt1XY(x,y);SetInt2XY(x,y);}
    void SetExistAll(){_isExistInt = true;_isExiGeoInt = true;}
    void SetIntPt(const Point& p){_intPnt1 = p;_intPnt2 = p;}
    void SetIntPts(const Point& p1,const Point& p2){_intPnt1 = p1;_intPnt2 = p2;}
    void Reset(){_isExistInt = false;_isExiGeoInt = false;_intNum = 0;}
    bool IsInt() const{return _isExistInt;}
    bool IsGeoInt() const{return _isExiGeoInt;}
    int Num() const{return _intNum;}
    const Point& P1() const{return _intPnt1;}
    const Point& P2() const{return _intPnt2;}
public:
    int _intNum = 0;
    bool _isExistInt = false;
    bool _isExiGeoInt = false;
    Point _intPnt1,_intPnt2;
};
class DefLoop;
class OffsetNode;
class GenerateParam{
public:
    GenerateParam() {}
    GenerateParam(const std::shared_ptr<DefLoop>& l) : _sourLoop(l){}
    GenerateParam(double r,double v) :
        _toolRadius(r),_offsetValue(v){}
    GenerateParam(double r,double v,const std::shared_ptr<DefLoop>& l) :
        _toolRadius(r),_offsetValue(v),_sourLoop(l){}
    void SetToolRadius(double r){_toolRadius = r;}
    void SetOffsetValue(double v){_offsetValue = v;}
    void SetSourceLoop(const std::shared_ptr<DefLoop>& l){_sourLoop = l;}
    void SetBaseParam(const OftBasePara& p){_baseParam = p;}
    double ToolRadius() const{return _toolRadius;}
    double OffsetValue() const{return _offsetValue;}
    OftBasePara BaseParameter(){return _baseParam;}
    std::shared_ptr<DefLoop> SourceLoop() const{return _sourLoop;}
private:
    double _toolRadius;
    double _offsetValue;
    OftBasePara _baseParam;
    std::shared_ptr<DefLoop>_sourLoop;
};
class LoopLabel
{
public:
    LoopLabel() {}

public:
    bool _isCW = false;
    bool _isValid = true;
    bool _isOffset = true;
    bool _isPreReverse = false;
    bool _isIntersect = false;
    bool _isMixLoop = false;
    bool _isBoundary = true;
    bool _hasClipDel = false;
    bool _hasReverse = false;
    //bool _leftward = true;

    int _windNum = 0;
    double _miter_lim = Max_Value;

    numst::LimCoord _limCoord;
    std::vector<DefLoop>_roots;
    //RotDir _loRotDir = CLOCKWISE;
    Ofte::ToolPathType _loPathType = BndOffsetPath;
    std::shared_ptr<OffsetNode>m_hasExiNode;
    Ofte::JoinType _joinType = Ofte::JoinType::Round;  //连接处连接类型
    Ofte::EndType _endType = Ofte::EndType::Polygon;   //开环端点处连接类型
};
class CAMALGORITHM_EXPORT  DefLoop
{
public:
    DefLoop() {}
    //DefLoop(ProfType type):_loopType(type){}
    DefLoop(bool left) : _leftward(left){}
    DefLoop(const std::vector<DefElem>& v):m_vElem(v){}
    /*DefLoop(const std::vector<DefElem>& v,ProfType ct):m_vElem(v),
        _loopType(ct){}*/
    DefLoop(const std::vector<DefElem>& v,bool left):m_vElem(v),_leftward(left){}
    //DefLoop(const std::vector<DefElem>& v,bool isOpen) : m_vElem(v),_isOpen(isOpen){}
    DefLoop(const std::vector<Point>& ps,bool isClose = true) {
        size_t l = ps.size();if(l < 2){return;}
        m_vElem.reserve(ps.size());
        for (size_t i = 0; i < l - 1; i++){
            m_vElem.emplace_back(DefElem(DefSeg(ps[i],ps[i+1])));
        }
        if(isClose){m_vElem.emplace_back(DefElem(DefSeg(ps[l-1],ps[0])));}
    }
    void SetThisOftNum(int num){_thisOftNum = num;}
    void SetPastOftNum(int num){_pastOftNum = num;}
    void SetCloseMode(bool value){_isCloseMode = value;}
    void AddElem(const DefElem& c){m_vElem.emplace_back(c);}
    void PopBack(){m_vElem.pop_back();}
    void AddFirst(const DefElem& e){m_vElem.emplace(m_vElem.begin(),e);}
    void Clear(){m_vElem.clear();}
    void SetIdElem(const DefElem& e,int i){m_vElem[i] = e;}
    //void SetLoopType(ProfType type){_loopType = type;}
    void SetLeftward(bool b){_leftward = b;}
    void SetFirst(const DefElem& e){if(!m_vElem.empty()){m_vElem[0] = e;}}
    void SetLast(const DefElem& e){if(!m_vElem.empty()){m_vElem[m_vElem.size()-1] = e;}}
    void SetCurves(const std::vector<DefElem>& cs){m_vElem = cs;}
    void SetMoveType(Ofte::MovePathType t){_moveType = t;}
    void SetCurvesMoveType(Ofte::MovePathType t){
        for(auto& d : m_vElem){d.SetMoveType(t);}
    }
    void SetOffsetType(Ofte::ToolPathType t){
        _label._loPathType = t;
    }
    void InitArcR(){
        for(auto& c : m_vElem){
            if(c.Type() != Ofte::ARCTYPE){continue;}
            c._arc.InitR();
        }
    }
    void OptInitArcR(double r){
        for(auto& c : m_vElem){
            if(c.Type() != Ofte::ARCTYPE){continue;}
            c._arc.OptInitR(r);
        }
    }
    void SetClipDele(bool v){_label._hasClipDel = v;}
    std::vector<DefElem>::iterator begin() {return m_vElem.begin();}
    std::vector<DefElem>::iterator end() {return m_vElem.end(); }
    std::vector<DefElem>::const_iterator begin() const{return m_vElem.begin();}
    std::vector<DefElem>::const_iterator end() const{return m_vElem.end();}
    DefElem& operator[](size_t i) {return m_vElem[i];}
    const DefElem& operator[](size_t i) const{return m_vElem[i];}
    //void IniDir(){_label._leftward = _loopType == Ofte::OUTSIDEPROFILE ? true : false;}
    void SetHasReverse(bool b){_label._hasReverse = b;}
    void Reverse(){
        for(auto& d : m_vElem){d.Reverse();}
        std::reverse(m_vElem.begin(),m_vElem.end());
    }
    void SetEndptZ(double z){for(auto& d : m_vElem){d.SetEndptZ(z);}}
    void SetBoundary(bool b){_label._isBoundary = b;}
    void Insert(size_t i,const DefElem& c){m_vElem.insert(m_vElem.begin()+i,c);}
    void Erase(size_t i){m_vElem.erase(m_vElem.begin() + i);}
    void Reserve(size_t i){m_vElem.reserve(i);}
    void DelInvalid(){
        m_vElem.erase(std::remove_if(m_vElem.begin(),m_vElem.end(),[](const DefElem& c){
            return !c.IsValid();}),m_vElem.end());
    }
    void SetCW(bool b){_label._isCW = b;}
    void SetValid(bool b){_label._isValid = b;}
    void SetWindNum(int i){
        _label._windNum = i;
    }
    void SetRoots(const std::vector<DefLoop>& ls){_label._roots = ls;}
    std::vector<DefElem>& Curves(){return m_vElem;}
    const std::vector<DefElem>& Curves()const{return m_vElem;}
    void SetId(int i){_id = i;}
    void SetLimCoord(const numst::LimCoord& c){_label._limCoord = c;}
    void ReverseType(){
        _leftward = !_leftward;
        //_loopType = (_loopType == Ofte::OUTSIDEPROFILE) ? WITHINPROFILE : OUTSIDEPROFILE;
    }
    void SetMiterLimit(double d){_label._miter_lim = d;}
    void SetEndType(Ofte::EndType t){_label._endType = t;}
    void SetJoinType(Ofte::JoinType t){_label._joinType = t;}
    void MakeClose(double e = PreErr5_14){
        if(IsEndToEnd(e)){return;}
        const auto& sp = Endpoint(true);
        const auto& ep = Endpoint(false);
        AddElem(DefElem(DefSeg(sp,ep)));
    }
    double ZCoord()const {return Endpoint(false).Z();}
    int ThisOftNum()const {return _thisOftNum;}
    int PastOftNum()const {return _pastOftNum;}
    bool IsEmpty()const {return m_vElem.empty();}
    const std::vector<DefElem>& LoopElem()const {return m_vElem;}
    const DefElem& IndexElem(int i)const {return m_vElem[i];}
    DefElem& First(){return m_vElem.front();}
    DefElem& Last(){return m_vElem.back();}
    const DefElem& First()const{return m_vElem.front();}
    const DefElem& Last()const{return m_vElem.back();}
    size_t Size() const{return m_vElem.size();}
    bool IsValIndex(size_t i)const{
        if(m_vElem.empty() || i < 0 || i > m_vElem.size() - 1){return false;}
        return true;
    }
    size_t PreId(size_t i)const{return pnum::PreId(m_vElem.size(),i);}
    size_t NexId(size_t i)const{return pnum::NexId(m_vElem.size(),i);}
    const DefElem& PreElem(size_t i)const{return m_vElem[PreId(i)];}
    const DefElem& NexElem(size_t i)const{return m_vElem[NexId(i)];}
    bool IsCloseMode()const {return _isCloseMode;}
    bool Leftward() const{return _leftward;}
    //ProfType LoopType()const {return _loopType;}
    Ofte::MovePathType MoveType()const {return _moveType;}
    Ofte::ToolPathType OffsetType() const{return _label._loPathType;}
    const Point& CurveEndpt(size_t i,bool isEp) const{return m_vElem[i].Endpoint(isEp);}
    const Point& Endpoint(bool isEp) const{
        if(isEp){return m_vElem.back().Endpoint(true);}
        return m_vElem.front().Endpoint(false);}
    bool IsEndToEnd(double v,bool is2d = true) const{
        if(is2d){return Endpoint(false).IsSamePoint2D(Endpoint(true),v);}
        return Endpoint(false).IsSamePoint3D(Endpoint(true),v);
    }
    bool IsConnect(double err,bool isOpen = false,bool is3D = false) const{
        if(m_vElem.empty()) {return false;}
        size_t n = 0,s = m_vElem.size();
        if(isOpen){s -= 1;}
        for(size_t i = 0;i < s;++i){
            n = NexId(i);
            if(!m_vElem[i].GetEp().IsEqual(m_vElem[n].GetSp(),err,is3D)){
                return false;
            }
        }
        return true;
    }
    bool HasReverse() const{return _label._hasReverse;}
    bool IsOffset() const{return _label._isOffset;}
    bool IsBoundary()const{return _label._isBoundary;}
    bool IsSameCurves(const std::vector<DefElem>& cs,double e)const{
        if(m_vElem.size() != cs.size() || IsEmpty()){return false;}
        size_t s = 0;bool is = false;
        for(size_t i = 0;i < cs.size();++i){
            if(m_vElem[0].IsSameCurve(cs[i],e)){
                is = true;s = i;break;}}
        if(!is){return false;}
        std::vector<DefElem>cs1;
        for(size_t i = s; i < cs.size(); ++i){cs1.push_back(cs[i]);}
        for(size_t i = 0; i < s; ++i){cs1.push_back(cs[i]);}
        for(size_t i = 0; i < m_vElem.size(); ++i){
            if(!m_vElem[i].IsSameCurve(cs[i],e)){return false;}
        }return true;
    }
    bool HasClipDele()const{return _label._hasClipDel;}
    bool IsCircleLoop()const{return Size() == 1 && m_vElem[0]._type == Ofte::CIRCLETYPE;}
    bool IsExistObjPt(const Point& p,double e)const{
        for(const auto& c : m_vElem){if(c.IsEndpoint(p,e)){return true;}}
        return false;
    }
    bool IsCW()const{return _label._isCW;}
    bool IsValid()const{return _label._isValid;}
    int WindNum()const{return _label._windNum;}
    const std::vector<DefLoop>& Roots()const{return _label._roots;}
    bool IsLegalNum()const{
        if(Size() > 2 || IsCircleLoop() || ((GetEndType() != Ofte::EndType::Polygon) &&
                                            !IsEmpty())){return true;}
        for(const auto& d : m_vElem){
            if(d.Type() != Ofte::ARCTYPE){continue;}
            if(Size() > 1){return true;}break;
        }
        return false;
    }
    int Id()const{return _id;}
    const numst::LimCoord& GetLimCoord()const{return _label._limCoord;}
    bool IsIncPtCoord(const Point& p,double e)const{
        return _label._limCoord.IsInclude(p.X(),p.Y(),e);
    }
    const double& MiterLimit()const {return _label._miter_lim;}
    Ofte::JoinType GetJoinType()const {return _label._joinType;}
    Ofte::EndType GetEndType()const {return _label._endType;}
    bool IsOpen()const{
        return GetEndType() != Ofte::EndType::Polygon &&
                GetEndType() != Ofte::EndType::Joined;
    }
    void SetZCoord(double z){
        for(auto& d : m_vElem){
            if(d._type == Ofte::LINETYPE){d._segment._segSp.SetZ(z);d._segment._segEp.SetZ(z);}
            else if (d._type == Ofte::ARCTYPE) {d._arc._arcSp.SetZ(z);d._arc._arcEp.SetZ(z);d._arc._arcCp.SetZ(z);}
            else if(d._type == Ofte::CIRCLETYPE){d._circle._cirCp.SetZ(z);}
        }
    }
public:
    int _id = -1;
    int _thisOftNum = 0;
    LoopLabel _label;
    std::vector<DefElem> m_vElem;
private:
    bool _leftward = false;
    int _pastOftNum = 0;
    bool _isOpen = false;
    bool _isCloseMode = false;
    //Ofte::ProfType _loopType = Ofte::OUTSIDEPROFILE;
    Ofte::MovePathType _moveType;
};
class CAMALGORITHM_EXPORT  OffsetNode
{
public:
    OffsetNode() {}
    OffsetNode(Ofte::ToolPathType t):_geneType(t){}
    OffsetNode(Ofte::ToolPathType t,const std::vector<DefLoop>& v):
        _geneType(t),_loops(v){}
    OffsetNode(const std::vector<DefLoop>& ls):_loops(ls){}
    OffsetNode(Ofte::ToolPathType t,const std::vector<DefLoop>& ls,
               const std::vector<std::shared_ptr<OffsetNode>>& ns):
        _geneType(t),_loops(ls),_nodes(ns){}
    void SetPathType(Ofte::ToolPathType t){_geneType = t;}
    void SetLoops(const std::vector<DefLoop>& ls){_loops = ls;}
    void SetNode(const std::vector<std::shared_ptr<OffsetNode>>& ns){_nodes = ns;}
    void ClearNode(){_loops.clear();_nodes.clear();}
    void ClearLoop(){_loops.clear();}
    void ClearData(){ClearNode();ClearLoop();}
    void InsertNode(int i,const std::shared_ptr<OffsetNode>& n){
        _nodes.insert(_nodes.begin() + i,n);}
    void InsertLoop(int i,const DefLoop& loop){
        _loops.insert(_loops.begin() + i,loop);}
    void SetId(int i){_id = i;}
    void AddNode(const std::shared_ptr<OffsetNode>& n){
        if(n != nullptr){_nodes.emplace_back(n);}
    }
    void AddNodes(const std::vector<std::shared_ptr<OffsetNode>>& ns){
        _nodes.insert(_nodes.end(),ns.begin(),ns.end());
    }
    void AddLoop(const DefLoop& loop){_loops.emplace_back(loop);}
    void AddLoops(const std::vector<DefLoop>& ls){
        _loops.insert(_loops.end(),ls.begin(),ls.end());
    }
    void PopBackLoop(){_loops.pop_back();}
    void PopBackNode(){_nodes.pop_back();}
    void ReverseLoops(){std::reverse(_loops.begin(),_loops.end());}
    void ReverseNodes(){std::reverse(_nodes.begin(),_nodes.end());}
    Ofte::ToolPathType PathType()const {return _geneType;}
    bool IsLoopsEmpty()const {return _loops.empty();}
    bool IsNodeEmpty()const {return _nodes.empty();}
    bool IsEmpty()const {return _loops.empty() && _nodes.empty();}
    const DefLoop& LastLoop()const {return _loops.back();}
    const std::shared_ptr<OffsetNode>& LastOftNode()const {return _nodes.back();}
    const DefLoop IndexLoop(int i)const {return _loops[i];}
    const std::shared_ptr<OffsetNode>& IndexAreaNode(int i)const {return _nodes[i];}
    const std::vector<DefLoop>& Loops()const {return _loops;}
    const std::vector<std::shared_ptr<OffsetNode>>& Nodes()const {return _nodes;}
    std::vector<DefLoop>AllLoops()const{
        auto ls = this->_loops;
        for(auto& d : this->_nodes){
            if(d == nullptr){break;}
            auto ls1 = d->AllLoops();
            ls.insert(ls.end(),ls1.begin(),ls1.end());
        }
        return ls;
    }
    int Id() const{return _id;}
public:
    Ofte::ToolPathType _geneType;
    std::vector<DefLoop> _loops;
    std::vector<std::shared_ptr<OffsetNode>> _nodes;
private:
    int _id = 0;
};
class LoopTree : public DefLoop, public std::enable_shared_from_this<LoopTree>{
public:
    LoopTree(){}
    LoopTree(const DefLoop& l) : DefLoop(l){}
    LoopTree(const DefLoop& l,int id) : DefLoop(l),_id(id){}
    std::vector<std::shared_ptr<LoopTree>>::iterator begin() {return _nodes.begin();}
    std::vector<std::shared_ptr<LoopTree>>::iterator end() {return _nodes.end(); }
    std::vector<std::shared_ptr<LoopTree>>::const_iterator begin() const{return _nodes.begin();}
    std::vector<std::shared_ptr<LoopTree>>::const_iterator end() const{return _nodes.end();}
    std::shared_ptr<LoopTree>& operator[](size_t i) {return _nodes[i];}
    static std::shared_ptr<LoopTree> BuildTree(const DefLoop& l){
        return std::make_shared<LoopTree>(l);
    }
    const std::shared_ptr<LoopTree>& Parent()const{return _parent;}
    const DefLoop& GetLoop()const{return *this;}
    std::vector<DefLoop>ToLoops()const{
        std::vector<DefLoop>ls{GetLoop()};
        ls.reserve(_nodes.size() * 3 + 1);
        for(const auto& d : _nodes){
            auto ls1 = ToLoops();
            for(auto& l : ls1){ls.emplace_back(l);}
        }
        return ls;
    }
    bool IsExiValid()const{
        if(IsValid()){return true;}
        for(const auto& d : _nodes){if(d->IsValid()){return true;}}
        return false;
    }
    int Id(bool sel = true)const{return sel ? _id : DefLoop::_id;}
    void SetParent(const std::shared_ptr<LoopTree>& p){_parent = p;}
    void Reserve(size_t n){_nodes.reserve(n);}
    void AddNode(const DefLoop& l){
        if(!l.IsEmpty()){_nodes.emplace_back(std::make_shared<LoopTree>(l));}
    }
    void AddNode(const std::shared_ptr<LoopTree>& d){
        if(d != nullptr && !d->IsEmpty()){_nodes.emplace_back(d);}
    }
    void InitParent(){
        auto self = shared_from_this();
        for(auto& d : _nodes){
            d->_parent = self;
            d->InitParent();
        }
    }
    void InitWindNum(){
        int wn = this->IsCW() ? -1 : 1;
        if(this->_parent != nullptr){wn += this->Parent()->_label._windNum;}
        this->_label._windNum = wn;
        for(auto& d : _nodes){
            d->InitWindNum();
        }
    };
    void InitId(){
        _id = DefLoop::_id;
        for(auto& d : _nodes){
            d->InitId();
        }
    }
    void SetId(int i,bool sel = true){if(sel){_id = i;}else{DefLoop::_id = i;}}
public: 
    int _id = -1;
    std::shared_ptr<LoopTree> _parent = nullptr;
    std::vector<std::shared_ptr<LoopTree>> _nodes;
};
class CAMALGORITHM_EXPORT LoopNode
{
public:
    LoopNode() {}
    LoopNode(const DefLoop& l,const std::shared_ptr<OffsetNode>& n) :
        m_loop(l),_offsetNode(n){}
    LoopNode(const std::shared_ptr<OffsetNode>& n){_offsetNode = n;}
public:
    DefLoop m_loop;
    std::shared_ptr<OffsetNode>_offsetNode = std::make_shared<OffsetNode>();
};
class NodeForm{
public:
    NodeForm(){}
public:
    std::vector<DefLoop>_bLocLoops;
    std::vector<DefLoop>_iLocLoops;
    std::vector<DefLoop>_biMerLoops;
    std::vector<DefLoop>_iiMerLoops;
    std::vector<std::shared_ptr<OffsetNode>>_bOftNodes;
    std::vector<std::shared_ptr<OffsetNode>>_iOftNodes;
};
class InOutLoop
{
public:
    InOutLoop() {}
    InOutLoop(const std::vector<DefLoop>& in,
              const std::vector<DefLoop>& out) :
        _inLoops(in),_outLoops(out){}
public:
    std::vector<DefLoop>_inLoops;
    std::vector<DefLoop>_outLoops;
};
class MillDirction
{
public:
    MillDirction() {}
    MillDirction(bool toolClock,bool profClock,bool areaClock):
        _isToolClock(toolClock),_isProfClockMill(profClock),
        _isAreaClockMill(areaClock){}
    void SetToolClock(bool isClockwise){_isToolClock = isClockwise;}
    void SetProfClockMill(bool isClockwise){_isProfClockMill = isClockwise;}
    void SetAreaClockMill(bool isClockwise){_isAreaClockMill = isClockwise;}
    bool IsToolClock()const {return _isToolClock;}
    bool IsProfClockMill()const {return _isProfClockMill;}
    bool IsAreaClockMill()const {return _isAreaClockMill;}
private:
    bool _isToolClock = true;       ///加工刀具是顺时针.
    bool _isProfClockMill = true;   ///轮廓是顺铣.
    bool _isAreaClockMill = true;   ///区域是顺铣.
};
class PathTechnic
{
public:
    PathTechnic() {}
    PathTechnic(bool isBlank,bool isAccOut,bool isBndOft,
                OffsetStyle style,double angle) :
        _isBlankBnd(isBlank),_isAcceptBorOut(isAccOut),
        _isBOffset(isBndOft),_profOffsetStyle(style),
        _styleAngle(angle){}
    PathTechnic(bool isBlank,bool isAccOut,bool isBndOft) :
        _isBlankBnd(isBlank),_isAcceptBorOut(isAccOut),
        _isBOffset(isBndOft){}
    PathTechnic(bool isBlank,bool isAccOut,bool isBndOft,bool isAddOri) :
        _isBlankBnd(isBlank),_isAcceptBorOut(isAccOut),
        _isBOffset(isBndOft),_isAddOriLoop(isAddOri){}
    void SetIsBlankBoundary(bool value){_isBlankBnd = value;}
    void SetIsAcceptBoundaryOut(bool value){_isAcceptBorOut = value;}
    void SetIsBoundaryOffset(bool value){_isBOffset = value;}
    void SetProfOffsetStyle(OffsetStyle value){_profOffsetStyle = value;}
    void SetStyleAngle(double value){_styleAngle = value;}
    ///void SetMillDirection(MillDirction value){_millDirection = value;}
    void SetMcStrategy(MC_Strategy value){_mcStrategy = value;}
    void SetAddOriginLoop(bool value){_isAddOriLoop = value;}
    void SetExpandSmall(bool value){_isExpandSmall = value;}
    bool IsBlankBoundary()const {return _isBlankBnd;}
    bool IsAcceptBoundaryOut()const {return _isAcceptBorOut;}
    bool IsBoundaryOffset()const {return _isBOffset;}
    OffsetStyle ProfOffsetStyles()const {return _profOffsetStyle;}
    double OffsetStyleAngle()const {return _styleAngle;}
    MC_Strategy McStrategy()const {return _mcStrategy;}
    bool IsAddOriginLoop()const {return _isAddOriLoop;}
    bool IsExpandSmall()const {return _isExpandSmall;}
private:
    double _styleAngle = 45;             ///角度制[0,360)
    bool _isBlankBnd = true;             ///边界轮廓是否是毛坯
    bool _isAcceptBorOut = false;        ///是否接受刀具走原始边界外部
    bool _isBOffset = true;            ///边界轮廓是否偏置
    bool _isAddOriLoop = false;          ///是否添加原始轮廓至刀路结果中.
    bool _isExpandSmall = false;         ///是否针对较小环进行扩大.
    ///刀路样式，目前统一为边界轮廓和岛屿轮廓同时偏置
    Ofte::OffsetStyle _profOffsetStyle = Ofte::Simultaneously_Offset;
    Ofte::MC_Strategy _mcStrategy = Ofte::ModelAreaClearing;
};
class OffsetUnit
{
public:
    OffsetUnit() {}
    OffsetUnit(bool isFixOffset,int offsetNum,
               double toolRadius,double firstOffsetVal,
               double offsetVal,bool isSmooth,double smoothVal):
        _isOftFixNum(isFixOffset),_offsetNum(offsetNum),
        _toolRadius(toolRadius),_firOffsetVal(firstOffsetVal),
        _offsetVal(offsetVal),_isSmooth(isSmooth),
        _smoothVal(smoothVal){}
    OffsetUnit(bool isFixOffset,int offsetNum,double firstOffsetVal,
               double offsetVal,bool isSmooth,double smoothVal):
        _isOftFixNum(isFixOffset),_offsetNum(offsetNum),
        _firOffsetVal(firstOffsetVal),_offsetVal(offsetVal),
        _isSmooth(isSmooth),_smoothVal(smoothVal){}
    void SetFixedOffset(bool value){_isOftFixNum = value;}
    void SetOffsetNumber(int value){_offsetNum = value;}
    void SetFirstOffsetValue(double value){_firOffsetVal = value;}
    void SetOffsetValue(double value){_offsetVal = value;}
    void SetToolRadius(double value){_toolRadius = value;}
    void SetIsSmooth(bool value){_isSmooth = value;}
    void SetSmoothValue(double value){_smoothVal = value;}
    void SetAlterableValue(bool value){_alterableValue = value;}
    void SetIsPureEqud(bool value){_isPureEqud = value;}
    void SetAblePreProcess(bool value){_isAblePreProc = value;}
    void SetIsFirSmooth(bool value){_isFirSmooth = value;}
    void SetAbsPreProcess(bool value){_isAbsPreProc = value;}
    bool IsFixedOffset()const {return _isOftFixNum;}
    int OffsetNumber()const {return _offsetNum;}
    double ToolRadius()const {return _toolRadius;}
    double FirstOftVal()const {return _firOffsetVal;}
    double OffsetValue()const {return _offsetVal;}
    bool IsPathSmooth()const {return _isSmooth;}
    double PathSmoothValue()const {return _smoothVal;}
    bool AlterableValue()const {return _alterableValue;}
    bool IsPureEqud()const {return _isPureEqud;}
    bool IsAblePreProcess()const {return _isAblePreProc;}
    bool IsFirSmooth()const {return _isFirSmooth;}
    bool IsAbsPreProcess()const{return _isAbsPreProc;}

private:
    bool _isOftFixNum = false;                   //是否固定偏置
    bool _isSmooth = false;                      //是否光顺
    bool _isFirSmooth = false;                   //第一次偏置是否光顺
    bool _alterableValue = true;                 //是否可改变偏置距离
    bool _isPureEqud = false;                    //true 不会计算自相交
    bool _isAblePreProc = false;                 //预处理
    bool _isAbsPreProc = false;                  //预处理
    int _offsetNum = 1;                          //偏置次数
    double _firOffsetVal = 2.2;                  //首次偏置距离
    double _offsetVal = 2.2;                     //偏置距离
    double _toolRadius = (_offsetVal / 0.7) / 2; //刀具半径
    double _smoothVal = 0.1;                     //光顺半径
};
class PrepOption{
public:
    PrepOption(){}
    PrepOption(double oft,bool delInv,bool opeSup,bool ete,
               bool close,bool ablPre = true,bool absPre = false,
               bool isfit = true):
        _oftValue(oft), _isDelInval(delInv),_isOperSupe(opeSup),
        _isEndtoend(ete),_isClose(close),_isAblePrep(ablPre),
        _isAbsPrep(absPre),_isFitArc(isfit){}
    double OftValue()const{return _oftValue;}
    double IsDelInval()const{return _isDelInval;}
    double IsOperSupe()const{return _isOperSupe;}
    double IsEndtoend()const{return _isEndtoend;}
    double IsClose()const{return _isClose;}
    double IsAblePrep()const{return _isAblePrep;}
    double IsAbsPrep()const{return _isAbsPrep;}
    bool IsFitting()const{return _isFitArc;}
    void SetOftValue(double v){_oftValue = v;}
    void SetIsDelInval(bool v){_isDelInval = v;}
    void SetIsOperSupe(bool v){_isOperSupe = v;}
    void SetIsEndtoend(bool v){_isEndtoend = v;}
    void SetIsClose(bool v){_isClose = v;}
    void SetIsAblePrep(bool v){_isAblePrep = v;}
    void SetIsAbsPrep(bool v){_isAbsPrep = v;}
    void SetFitting(bool v){_isFitArc = v;}
private:
    double _oftValue = 0;
    bool _isDelInval = true;
    bool _isOperSupe = true;
    bool _isEndtoend = true;
    bool _isClose = true;
    bool _isAblePrep = true;
    bool _isAbsPrep= false;
    bool _isFitArc = true;
};
class CAMALGORITHM_EXPORT  OffsetParam
{
public:
    OffsetParam(){}
    ///不带岛屿轮廓的偏置参数设定
    OffsetParam(bool isFixOftNum,int offsetNum,double firOffsetVal,
                double offsetVal,bool isSmooth,double smoothVal)
    {
        OffsetUnit oftUnit(isFixOftNum,offsetNum,firOffsetVal,
                           offsetVal,isSmooth,smoothVal);
        _offsetUnit = oftUnit;
    }
    ///带岛屿轮廓的偏置参数设置
    OffsetParam(bool isFixOftNum,int offsetNum,double firOffsetVal,
                double offsetVal,bool isSmooth,double smoothVal,
                OffsetStyle offsetStyle,MC_Strategy mcStrategy)
    {
        OffsetUnit oftUnit(isFixOftNum,offsetNum,
                           firOffsetVal,offsetVal,
                           isSmooth,smoothVal);
        _offsetUnit = oftUnit;
    }
    ///新增刀具半径、刀具是否允许走原始边界
    OffsetParam(bool isFixOftNum,int offsetNum,double firOffsetVal,
                double offsetVal,bool isSmooth,double smoothVal,
                double toolRadius,bool isBlankBnd,bool isAccBorOut,
                OffsetStyle offsetStyle,MC_Strategy mcStrategy)
    {
        _pathTech.SetIsBlankBoundary(isBlankBnd);
        _pathTech.SetIsAcceptBoundaryOut(isAccBorOut);
        _pathTech.SetProfOffsetStyle(offsetStyle);
        _pathTech.SetMcStrategy(mcStrategy);
        OffsetUnit oftUnit(isFixOftNum,offsetNum,toolRadius,firOffsetVal,
                           offsetVal,isSmooth,smoothVal);
        _offsetUnit = oftUnit;
    }
    OffsetParam(OffsetUnit oftUni) : _offsetUnit(oftUni){}
    OffsetParam(OffsetUnit oftUni,PathTechnic pathTech,MillDirction millDir) :
        _offsetUnit(oftUni),_pathTech(pathTech),_millDirection(millDir){}
    OffsetParam(bool isFix,int oftNum,double oftVal,bool isPreproc = true){
        _offsetUnit = OffsetUnit(isFix,oftNum,oftVal,oftVal,oftVal,false,0);
        _prepOpti.SetIsAblePrep(isPreproc);
        _prepOpti.SetIsAbsPrep(isPreproc);
    }
    OffsetParam(bool isFix,int oftNum,double oftVal,bool isPre,bool isFit,bool isAftProc){
        _offsetUnit = OffsetUnit(isFix,oftNum,oftVal,oftVal,oftVal,false,0);
        _prepOpti = PrepOption(oftVal,isPre,isPre,isPre,isPre,isPre,isPre,isFit);
        _isAfterProc = isAftProc;
    }
    void SetIsFirstOffset(bool value){_isFirstOffset = value;}
    void SetAfterProc(bool v){_isAfterProc = v;}

    bool IsFirstOffset()const {return _isFirstOffset;}
    bool IsAfterProc()const {return _isAfterProc;}
public:
    OffsetUnit _offsetUnit;
    PathTechnic _pathTech;
    MillDirction _millDirection;
    PrepOption _prepOpti;
    numst::FitParam _fitParam;
private:
    bool _isFirstOffset = true;
    bool _isAfterProc = true;
};
class GeomArea
{
public:
    GeomArea() {}
    GeomArea(const DefLoop& b) : _bndLoop(b){}
    GeomArea(const DefLoop& b,const std::vector<DefLoop>& v) : _bndLoop(b),_vIslLoop(v){}
    GeomArea(const DefLoop& b,const std::vector<DefLoop>& v,bool isBOft,bool isIOft) :
        _bndLoop(b),_vIslLoop(v),_isBOffset(isBOft),_isIOffset(isIOft) {}
    const DefLoop& BndLoop()const{return _bndLoop;}
    DefLoop& BLoop(){return _bndLoop;}
    const DefLoop& BLoop()const{return _bndLoop;}
    const DefLoop& OriBndLoop()const{return _obLoop;}
    const std::vector<DefLoop>& IslLoop()const{return _vIslLoop;}
    std::vector<DefLoop>& ILoops(){return _vIslLoop;}
    const std::vector<DefLoop>& ILoops()const{return _vIslLoop;}
    const std::vector<DefLoop>& OriIslLoop()const{return _vOriIslLoop;}
    std::vector<DefLoop>::iterator begin() {return _vIslLoop.begin();}
    std::vector<DefLoop>::iterator end() {return _vIslLoop.end(); }
    std::vector<DefLoop>::const_iterator begin() const{return _vIslLoop.begin();}
    std::vector<DefLoop>::const_iterator end() const{return _vIslLoop.end();}
    DefLoop& operator[](size_t i) {return _vIslLoop[i];}
    const DefLoop& operator[](size_t i) const{return _vIslLoop[i];}
    bool IsBndOffset()const {return _isBOffset;}
    bool IsIslOffset()const {return _isIOffset;}
    void SetBLoop(const DefLoop& l) {_bndLoop = l;}
    void SetILoops(const std::vector<DefLoop>& ls) {_vIslLoop = ls;}
    void SetAddILoop(bool b){_isAddILoop = b;}
    void SetAddBLoop(bool b){_isAddBLoop = b;}
    void SetValidMatch(bool b){_isValidMatch = b;}
    void SetOriBLoop(const DefLoop& b) {
        _obLoop = b;
        _obLoop.SetCloseMode(true);
    }
    void SetOriILoop(const std::vector<DefLoop>& ls) {
        _vOriIslLoop = ls;for(auto& l : _vOriIslLoop){l.SetCloseMode(true);}
    }
    void SetEndType(Ofte::EndType t){
        _bndLoop.SetEndType(t);
        for(auto& d : _vIslLoop){d.SetEndType(t);}
    }
    void SetJoinType(Ofte::JoinType t){
        _bndLoop.SetJoinType(t);
        for(auto& d : _vIslLoop){d.SetJoinType(t);}
    }
    void SetMiterLimit(double d){
        _bndLoop.SetMiterLimit(d);
        for(auto& l : _vIslLoop){l.SetMiterLimit(d);}
    }
    void AddILoop(const DefLoop& l){_vIslLoop.emplace_back(l);}
    void Clear(){_bndLoop.Clear();_obLoop.Clear();_vIslLoop.clear();_vOriIslLoop.clear();}
    void InitR(){_bndLoop.InitArcR();for(auto& l : _vIslLoop){l.InitArcR();}}
    bool IsAddBndLoop()const{return _isAddBLoop;}
    bool IsAddIslLoop()const{return _isAddILoop;}
    bool IsValidMatch()const{return _isValidMatch;}
    bool IsLegal()const{return _bndLoop.IsEmpty();}
public:
    DefLoop _bndLoop,_obLoop;
    std::vector<DefLoop> _vIslLoop;
    std::vector<DefLoop> _vOriIslLoop;
    bool _isBOffset = true;
    bool _isIOffset = true;
private:
    bool _isAddBLoop = false;
    bool _isAddILoop = false;
    bool _isValidMatch = true;
};
class AreaNode
{
public:
    AreaNode(){}
    AreaNode(const std::vector<GeomArea>& areas,
             const std::vector<std::shared_ptr<AreaNode>>& nodes) :
        _vGeomArea(areas),_vAreaNode(nodes){}
    void SetGeomArea(const std::vector<GeomArea>& areas){_vGeomArea = areas;}
    void SetAreaNode(const std::vector<std::shared_ptr<AreaNode>>& nodes){
        _vAreaNode = nodes;}
    void SetWindNum(int i){_windNum = i;}
    void WindNumPlusOne(){_windNum ++;}
    void WindNumMinusOne(){_windNum --;}
    void WindNumPlus(int i){_windNum += i;}
    void WindNumMinus(int i){_windNum -= i;}
    void ResetWindNum(){_windNum = 0;}
    void PushBackArea(const GeomArea& area){
        _vGeomArea.push_back(area);}
    void PopBackArea(){_vGeomArea.pop_back();}
    void InsertArea(int i,const GeomArea& area){
        _vGeomArea.insert(_vGeomArea.begin() + i,area);}
    void PushBackAreaNode(const std::shared_ptr<AreaNode>& node){
        _vAreaNode.push_back(node);}
    void PopBackAreaNode(){_vAreaNode.pop_back();}
    void InsertArea(int i,const std::shared_ptr<AreaNode>& node){
        _vAreaNode.insert(_vAreaNode.begin() + i,node);}
    void ClearGeomArea(){_vGeomArea.clear();}
    void ClearAreaNode(){_vAreaNode.clear();}
    void ClearAreaNodeData(){
        ClearGeomArea();ClearAreaNode();}
    int GeomAreaNum()const {return static_cast<int>(_vGeomArea.size());}
    int AreaNodeNum()const {return static_cast<int>(_vAreaNode.size());}
    int WindNumber()const {return _windNum;}
    bool IsValAreaIndex(int i)const {
        if(_vGeomArea.empty() || i < 0 || i > _vGeomArea.size() - 1)
            return false;
        return true;}
    bool IsValNodeIndex(int i)const {
        if(_vAreaNode.empty() || i < 0 || i > _vAreaNode.size() - 1)
            return false;
        return true;}
    GeomArea LastArea()const {return _vGeomArea.back();}
    std::shared_ptr<AreaNode> LastAreaNode()const {return _vAreaNode.back();}
    GeomArea IndexArea(int i)const {return _vGeomArea[i];}
    std::shared_ptr<AreaNode> IndexAreaNode(int i)const {return _vAreaNode[i];}
    bool IsEmptyArea()const {return _vGeomArea.empty();}
    bool IsEmptyAreaNode()const {return _vAreaNode.empty();}
private:
    int _windNum = 0;
    std::vector<GeomArea>_vGeomArea;
    std::vector<std::shared_ptr<AreaNode>>_vAreaNode;
};
class EquWitOutLoop
{
public:
    EquWitOutLoop() {}
    EquWitOutLoop(const std::vector<DefLoop>& vWitLoop,
                  const std::vector<DefLoop>& vOutLoop) :
        m_vWitLoop(vWitLoop),m_vOutLoop(vOutLoop){}
    std::vector<DefLoop>m_vWitLoop;
    std::vector<DefLoop>m_vOutLoop;
};
class EleCutLabel{
public:
    EleCutLabel(){}
    EleCutLabel(const DefElem& curValPart,const DefElem& curInValPart,
                const DefElem& anoValPart,const DefElem& anoInValPart):
        m_curValPart(curValPart),m_curInValPart(curInValPart),
        m_anoValPart(anoValPart),m_anoInValPart(anoInValPart){}
    EleCutLabel(bool isExistClip,int loopIndex,int eleIndex):
        m_isExistClip(isExistClip),m_loopIndex(loopIndex),
        m_eleIndex(eleIndex){}
public:
    bool m_isExistClip = false;
    bool m_isPrePartVal = false;
    bool m_isTwoPartVal = false;
    int m_loopIndex,m_eleIndex;
    DefElem m_curValPart,m_curInValPart,
    m_anoValPart,m_anoInValPart;
};
class CylinderHelix
{
public:
    CylinderHelix() {}
    CylinderHelix(const Point& c,double r,double h,double angle,bool isRight) :
        _helCenterPt(c),_helRadius(r),_helHeight(h),
        _helAngle(angle),_isRightHelix(isRight),_hasInitHelix(true){}
    void SetHelixCenter(Point c) {_helCenterPt = c;}
    void SetHelixRadius(double r) {_helRadius = r;}
    void SetHelixHeight(double h) {_helHeight = h;}
    void SetHelixAngle(double angle) {_helAngle = angle;}
    void SetHelixDirection(bool isRight) {_isRightHelix = isRight;}
    void SetAlterableHelix(bool v) {_alterableHelix = v;}
    Point HelixCenter() const{return _helCenterPt;}
    double HelixRadius() const{return _helRadius;}
    double HelixHeight() const{return _helHeight;}
    double HelixAngle() const{return _helAngle;}
    bool HelixDirection() const{return _isRightHelix;}
    bool HasInitHelix() const{return _hasInitHelix;}
    bool AlterableHelix() const{return _alterableHelix;}
private:
    Point _helCenterPt;
    double _helRadius = 0;
    double _helHeight = 0;
    double _helAngle = 0;
    bool _isRightHelix = false;
    bool _hasInitHelix = false;
    bool _alterableHelix = true;
};
class DefAxis{
public:
    DefAxis(){}
    DefAxis(const Point& loc,const Point& dir) : _loc(loc),_dir(dir){}
    void SetLocation(const Point& loc){_loc = loc;}
    void SetDirection(const Point& dir){_dir = dir;}
    void Reverse(){SetDirection(ReversedDire());}
    const Point& Location()const{return _loc;}
    const Point& Direction()const{return _dir;}
    Point ReversedDire()const{
        auto d = _dir;d.SetX(-d.X());d.SetY(-d.Y());d.SetZ(-d.Z());
        return d;
    }
    bool IsParallel(const DefAxis& ax,double e)const{
        return _dir.IsParallel(ax.Direction(),e);}
    double AngleWithOther(const DefAxis& ax)const{  //返回锐角
        double dot = _dir.Dot(ax.Direction());
        if(dot < -1){dot = -1.0;}
        if(dot > 1){dot = 1.0;}
        double agl = std::acos(dot);
        if(agl > M_PI_2){agl = M_PI_2;}
        double r = agl * _AlgRad;
        return r;
    }
    bool IsSameDire(const DefAxis& ax,double e)const{
        double a = AngleWithOther(ax);
        return std::fabs(a) < e ? true : false;
    }
    bool IsOppositeDire(const DefAxis& ax,double e)const{
        double a = AngleWithOther(ax);
        return std::fabs(a - 180.0) < e ? true : false;
    }
private:
    Point _loc = Point(0,0,0);
    Point _dir = Point(0,0,1);
};
class RotParam{
public:
    RotParam(){}
    RotParam(const DefAxis& ax,double agl,bool isCW) :
        _axis(ax),_agl(agl),_isCW(isCW){}
    void SetAxis(const DefAxis& ax){_axis = ax;}
    void SetAngle(double agl){_agl = agl;}
    void SetCW(double b){_isCW = b;}
    const DefAxis& Axis()const{return _axis;}
    double Angle()const{return _agl;}
    bool IsCW()const{return _isCW;}
public:
    double _agl = 0;
    bool _isCW = false; ///从_axis 原点朝其正方向看
    DefAxis _axis;
};
class ParalParam{
public:
    ParalParam(){}
    ParalParam(double d,double a,Ofte::ParalCntType t) :
        _dist(d),_agl(a),_cntType(t){}
    double Dist()const{return _dist;}
    double Agl()const{return _agl;}
    Ofte::ParalCntType PType()const{return _cntType;}
public:
    double _dist = 2;
    double _agl = 0;
    Ofte::ParalCntType _cntType = Ofte::SingleDirection;
};
namespace Fitt2D { 
class FittArc
{
public:
    FittArc() {}

    const DefArc& FitArc()const{return _arc;}
    DefArc& FitArc(){return _arc;}
    const DefArc& PreArc()const{return _preArc;}
    bool IsCW()const{return _arc.IsCW();}
    const std::vector<size_t>& Ids()const{return _ids;}
    const std::vector<Point>& Centers()const{return _cps;}
    const std::vector<Point>& Nodes()const{return _nodes;}
    const std::vector<double>& Rs()const{return _rs;}
    const std::vector<double>& AglVars()const{return _aglVars;}
    bool HasInit()const{return _hasInit;}
    double AverRadius()const{
        if(_rs.empty()){return 0;}
        double s = std::accumulate(_rs.begin(),_rs.end(),0.0);
        double n = static_cast<double>(_cps.size());
        return s / n;
    }
    Point AverCenter()const{
        if(_cps.empty()){return Point(0,0,0);}
        double x = 0,y = 0,z = 0;
        for(auto& d : _cps){x += d.X();y += d.Y();z += d.Z();}
        double n = static_cast<double>(_cps.size());
        x /= n;y /= n;y /= n;
        return Point(x,y,z);
    }

    void SetFitArc(const DefArc& arc){_arc = arc;}
    void SetPreArc(const DefArc& arc){_preArc = arc;}
    void SetCW(bool b){_arc.SetCW(b);}
    void SetHasInit(bool b){_hasInit = b;}
    void AddId(size_t i){_ids.emplace_back(i);}
    void AddCenter(const Point& p){_cps.emplace_back(p);}
    void AddNode(const Point& p){_nodes.emplace_back(p);}
    void AddRs(double r){_rs.emplace_back(r);}
    void AddAglVar(double a){_aglVars.emplace_back(a);}

    void IdsPopBack(){_ids.pop_back();}
    void CpsPopBack(){_cps.pop_back();}
    void RsPopBack(){_rs.pop_back();}

public:
    DefArc _arc;
    DefArc _preArc;
    std::vector<size_t> _ids;
    std::vector<Point> _cps;
    std::vector<Point> _nodes;
    std::vector<double> _rs;
    std::vector<double> _aglVars;

    bool _hasInit = false;
};
}
namespace OffsetResCheck{
class CheckDistance{
public:
    CheckDistance(){}
    CheckDistance(double maxD,double minD,double aD):
        _maxDis(maxD), _minDis(minD),_aevDis(aD){}
public:
    double _maxDis = 0;
    double _minDis = 0;
    double _aevDis = 0;
};
class CheckPoint{
public:
    CheckPoint(){}
    CheckPoint(const Point& p) : _p(p){}
    CheckPoint(const Point& p,double d) : _p(p),_distance(d){}
    CheckPoint(const Point& p,int i) : _p(p),_pIndex(i){}
    CheckPoint(const Point& p,int i,int eleID,double d) :
        _p(p),_pIndex(i),_eleIndex(eleID),_distance(d){}
public:
    Point _p;
    int _pIndex;
    int _eleIndex;
    double _distance = 0;
};
class CheckElem {
public:
    CheckElem() {}
    CheckElem(const DefElem& e) : _element(e){}
    CheckElem(const DefElem& e,int i) : _element(e), _eleIndex(i){}
    CheckElem(const DefElem& e,int i,const std::vector<CheckPoint>& vp,
              std::vector<double>vDis) :
        _element(e),_eleIndex(i), _vCheckPnt(vp),_vDistance(vDis){}
    CheckElem(const DefElem& e,int i,const std::vector<CheckPoint>& vPt,
              const std::vector<double>&vDis,CheckDistance& dis) :
        _element(e), _eleIndex(i),_vCheckPnt(vPt),
        _vDistance(vDis),_checkDis(dis){}
public:
    int _eleIndex;
    DefElem _element;
    CheckDistance _checkDis;
    std::vector<double>_vDistance;
    std::vector<CheckPoint> _vCheckPnt;
};
class CheckLoop {
public:
    CheckLoop(){}
    CheckLoop(const DefLoop& loop) : _loop(loop){}
    CheckLoop(const DefLoop& loop,double ref) :
        _loop(loop),_refValue(ref){}
    void SetMaxError(){
        _maxErr = std::abs(_checkDis._maxDis - _refValue) >=
                std::abs(_checkDis._minDis - _refValue) ?
                    std::abs(_checkDis._maxDis - _refValue) :
                    std::abs(_checkDis._minDis - _refValue);
    }
public:
    DefLoop _loop;
    double _refValue = 0;
    double _maxErr = 0;
    CheckDistance _checkDis;
    std::vector<CheckElem>_vCheckElem;
    std::vector<CheckElem>_vMaxDisEle;
    std::vector<CheckElem>_vMinDisEle;
    double _maxDistance;
    double _minDistance;
    std::vector<double>_vDistance;
    bool isExistDis = false;
};
}
namespace ProgOftValue{
class ValidDist
{
public:
    ValidDist() {}
    ValidDist(bool v,const std::pair<int,double> &p) :
        _isValidDist(v),_indexDist(p) {}
public:
    bool _isValidDist = false;
    std::pair<int,double>_indexDist;
};
class IndexDist
{
public:
    IndexDist() {}
    IndexDist(int i) : _index(i) {}
    IndexDist(int i,int toi) : _index(i),_toIndex(toi) {}
    IndexDist(int i,int toi,double d) :
        _index(i),_toIndex(toi),_distance(d) {}
    void SetIndex(int i){_index = i;}
    void SetToIndex(int i){_toIndex = i;}
    void SetToDistance(double d){_distance = d;}
    void SetMeetDistance(double v){_isMeetDist = v;}
    int Index() const{return _index;}
    int ToIndex() const{return _toIndex;}
    double ToDistance() const{return _distance;}
    bool IsMeetDistance() const{return _isMeetDist;}
public:
    int _nexIndex = 0;
    double _nexDistance = 0;
    std::vector<ValidDist>_vIndexDist;
private:
    int _index = 0;
    int _toIndex = 0;
    double _distance = 0;
    bool _isMeetDist = false;
};
}
}
namespace ofts = OffsetStruct;
namespace gfit = OffsetStruct::Fitt2D;
#endif // DATASTRUCTURE_H
