#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
/**
  @author: lvdong
  @date: 2023-05
 **/
#include <cmath>
#include <math.h>
#include <algorithm>
#include <memory>
#include <vector>
#include"EnumType.h"
#include"NumPreErr.h"

using namespace std;
using namespace OffsetEnum;
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

#define DebugModel 0
#define TimeStatistic 0
#define _ViewDebug 0

namespace oft {
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
    void SetContactNormal(oft::Point* v){_normal = v;}
    bool IsOnBorder() const{return _isOnBorder;}
    bool IsOnSteep() const{return _isOnSteep;}
    bool HasVisited() const{return _hasVisited;}
    double NormalAngle() const{return _norAngle;}
    double ContactAngle() const{return _cpAngle;}
    oft::Point* Normal() const{
        return _normal;}
public:
    int _id = -1;
    double _norAngle = 0;         /// 与xy平面夹角.
    bool _isOnBorder = false;     /// 是否位于边界线上.
    bool _isOnSteep = false;      /// 是否位于陡峭面上.
    bool _hasVisited = false;
    double _cpAngle;              /// 对应接触点法线与xy平面角度.
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
    PntLable PointLable()const{return _lable;}
    oft::Point operator+(const oft::Point &p) const{
        return Point(_x + p.X(),_y + p.Y(),_z + p.Z());
    }
    oft::Point operator-(const oft::Point& p)const{
        return Point(_x - p.X(),_y - p.Y(),_z - p.Z());
    }
    oft::Point operator*(double d) const{
        return Point(_x * d,_y * d,_z * d);
    }
    double operator*(const oft::Point&p)const{
        return _x * p.X() + _y * p.Y() + _z * p.Z();
    }
    friend Point operator*(double scalar, const Point& p) {
        return Point(p._x * scalar, p._y * scalar, p._z * scalar);
    }
    oft::Point operator/(double a) const{
        return oft::Point(_x/a,_y/a,_z/a);
    }
    bool operator==(const oft::Point& p) const{
        return _x == p.X() && _y == p.Y() && _z == p.Z();
    }
    oft::Point Subtract(const oft::Point& v) const {
        return {_x - v.X(), _y - v.Y(), _z - v.Z()};
    }
    double Dot(const oft::Point& v) const {
        return _x * v.X() + _y * v.Y() + _z * v.Z();
    }
    oft::Point Cross(const oft::Point &v) const{
        return {_y * v.Z() - _z * v.Y(), _z * v.X() - _x * v.Z(),
                    _x * v.Y() - _y * v.X()};
    }
    double Cross2D(const Point& p) const {
        return _x * p.Y() - _y * p.X();
    }
    oft::Point Normalize() const {
        double l = Length();
        if(std::abs(l) < PreErr5_10)
            return {_x, _y, _z};
        return {_x / l, _y / l, _z / l};
    }
    double Length() const{
        return std::sqrt(_x * _x + _y * _y + _z * _z);
    }
    double Length2D() const{
        return std::sqrt(_x * _x + _y * _y);
    }
    ///叉积,P^Q>0,P在Q的顺时针方向<0,P在Q的逆时针方向=0,
    ///P,Q 共线,可能同向或反向.
    double operator^(const oft::Point&p) const{
        return _x * p.Y() - _y * p.X();
    }
    double PointSqrt2D() const{return _x * _x + _y * _y;}
    double PointAbs2D() const{return std::sqrt(PointSqrt2D());}
    double Distance2D(const oft::Point& P) const{
        double dis = 0;double d;
        d = _x;d -= P._x;d *= d;dis += d;
        d = _y;d -= P._y;d *= d;dis += d;
        return std::sqrt(dis);
    }
    double Distance3D(const oft::Point& P) const{
        double dis = 0;double d;
        d = _x;d -= P._x;d *= d;dis += d;
        d = _y;d -= P._y;d *= d;dis += d;
        d = _z;d -= P._z;d *= d;dis += d;
        return std::sqrt(dis);
    }
    double DistSquare2D(const oft::Point& P) const{
        double dis = 0;double d;
        d = _x;d -= P._x;d *= d;dis += d;
        d = _y;d -= P._y;d *= d;dis += d;
        return dis;
    }
    double DistSquare3D(const oft::Point& P) const{
        double dis = 0;double d;
        d = _x;d -= P._x;d *= d;dis += d;
        d = _y;d -= P._y;d *= d;dis += d;
        d = _z;d -= P._z;d *= d;dis += d;
        return dis;
    }
    bool IsSamePoint2D(const oft::Point &P,double e) const{
        double d = DistSquare2D(P);
        return d <= (e * e) ? true : false;
    }
    bool IsSamePoint3D(const oft::Point &P,double e) const{
        double d = DistSquare3D(P);
        return d <= (e * e) ? true : false;
    }
    bool IsSameCoord2D(const oft::Point &p, double e) const{
        return std::abs(_x - p._x) <= e && std::abs(_y - p._y) <= e;
    }
    bool IsSameCoord3D(const oft::Point &p, double e) const{
        return std::abs(_x - p._x) <= e && std::abs(_y - p._y) <= e &&
                std::abs(_z - p._z) <= e;
    }
    double ManhattanDist2D(const oft::Point &p) const{
        return std::abs(_x - p._x) + std::abs(_y - p._y);
    }
    double ManhattanDist3D(const oft::Point &p) const{
        return std::abs(_x - p._x) + std::abs(_y - p._y) + std::abs(_z - p._z);
    }
    oft::Point Midpoint(const oft::Point &p) const{
        return {(p.X() + _x)/2,(p.Y() + _y)/2,(p.Z() + _z)/2};
    }
    void SetPointLable(const PntLable& l){_lable = l;}
    oft::PntLable Lable() const{return _lable;}
    bool IsOnBorder(const oft::Point& p) const{
        return _lable.IsOnSteep() != p._lable.IsOnSteep();
    }
    Point Normal() const{return *_lable._normal;}
    Point Cnt() const{
        if(_lable._cnt == nullptr){return Point();}
        return *_lable._cnt;
    }
    std::shared_ptr<DefPlane> Plan() const{return _lable._plan;}
    std::shared_ptr<DefPlane> NPlan() const{return _lable._nplan;}
    bool IsZero(double e) const {
        return std::abs(_x) < e && std::abs(_y) < e && std::abs(_z) < e;
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
    DefSeg(const Point& s,const Point& e):_segSp(s),_segEp(e){}
    void SetSegStartPt(const Point& p) {_segSp = p;}
    void SetSegEndPt(const Point& p) {_segEp = p;}
    void Reverse() {Point p = _segSp;_segSp = _segEp;_segEp = p;}
    void PointSplit(const Point& p,bool isChangeEp){
        if(isChangeEp){_segEp = p;}else{_segSp = p;}
    }
    void SetEndpointZ(double z){_segSp.SetZ(z);_segEp.SetZ(z);}
    const Point& SegStartPt() const{return _segSp;}
    const Point& SegEndPt() const{return _segEp;}
    const Point& Endpoint(bool isEp) const{
        if(isEp){return _segEp;}return _segSp;
    }
    double Angle2D() const{
        double dx = _segEp.X() - _segSp.X(),dy = _segEp.Y() - _segSp.Y();
        double agl = std::atan2(dy,dx);agl *= _AlgRad;
        if(agl < 0){agl += 360;}
        return agl;
    }
    bool IsHorizon(double v) const{
        double agl = Angle2D();
        if(std::abs(agl) < v || std::abs(agl-180) < v){
            return true;}
        return false;
    }
    bool IsVertical(double v) const{
        double agl = Angle2D();
        if(std::abs(agl-90) < v || std::abs(agl-270) < v){
            return true;}
        return false;
    }
    bool IsValid2D(double l) const{return !_segSp.IsSamePoint2D(_segEp,l);}
    bool IsValid3D(double l) const{return !_segSp.IsSamePoint3D(_segEp,l);}
public:
    Point _segSp;
    Point _segEp;
};
class CAMALGORITHM_EXPORT  DefArc
{
public:
    DefArc(){}
    DefArc(const Point& s,const Point& e,
           const Point& c,RotDir r):_arcSp(s),_arcEp(e),
        _arcCp(c),_arcRot(r){
        IniArcRadius();
    }
    DefArc(const Point& S,const Point& E,const Point& C,double radius,RotDir r):
        _arcSp(S),_arcEp(E),_arcCp(C),_arcR(radius),_arcRot(r){}
    void SetArcStartPt(const Point& P){_arcSp = P;}
    void SetArcEndPt(const Point& P){_arcEp = P;}
    void SetArcCenterPt(const Point& P){_arcCp = P;}
    void SetArcRadius(double R){_arcR = R;}
    void SetArcRot(RotDir rot){_arcRot = rot;}
    void IniArcRadius(){
        double R1 = std::sqrt(std::pow(_arcSp.X() - _arcCp.X(), 2) +
                              std::pow(_arcSp.Y() - _arcCp.Y(), 2));
        double R2 = std::sqrt(std::pow(_arcEp.X() - _arcCp.X(), 2) +
                              std::pow(_arcEp.Y() - _arcCp.Y(), 2));
        _arcR = (R1 + R2) / 2;
    }
    void ChoiceSetRadius(double value = PreErr5_3){
        if(_arcR < value) IniArcRadius();}
    void Reverse(){
        Point p = _arcSp;_arcSp = _arcEp;_arcEp = p;
        _arcRot = _arcRot == OffsetEnum::CLOCKWISE ?
                    OffsetEnum::ANTICLOCKWISE : OffsetEnum::CLOCKWISE;
    }
    void PointSplit(const Point& p,bool isChangeEp){
        if(isChangeEp){_arcEp = p;}else{_arcSp = p;}
    }
    const Point& Endpoint(bool isEp) const{if(isEp){return _arcEp;}return
                _arcSp;}
    const Point& ArcStartPt() const{return _arcSp;}
    const Point& ArcEndPt() const{return _arcEp;}
    const Point& ArcCenterPt() const{return _arcCp;}
    double ArcRadius() const{return _arcR;}
    RotDir ArcRot() const{return _arcRot;}
public:
    Point _arcSp;
    Point _arcEp;
    Point _arcCp;
    double _arcR;
    RotDir _arcRot;
};
class CAMALGORITHM_EXPORT  DefCircle
{
public:
    DefCircle(){}
    DefCircle(const Point& p,double r):_cirCp(p),_cirR(r){}
    DefCircle(const Point& p,double r,bool isCw):
        _cirCp(p),_cirR(r),_isCW(isCw){}
    void SetCircleCenter(const Point& p){_cirCp = p;}
    void SetCircleRadius(double r){_cirR = r;}
    void SetClockwise(bool v){_isCW = v;}
    const Point& CircleCenter()const {return _cirCp;}
    Point UpPoint() const{
        return Point(_cirCp.X(),_cirCp.Y()+_cirR,_cirCp.Z());}
    Point DownPoint() const{
        return Point(_cirCp.X(),_cirCp.Y()-_cirR,_cirCp.Z());}
    Point LeftPoint() const{
        return Point(_cirCp.X()-_cirR,_cirCp.Y(),_cirCp.Z());}
    Point RightPoint() const{
        return Point(_cirCp.X()+_cirR,_cirCp.Y(),_cirCp.Z());}
    double CircleRadius()const {return _cirR;}
    bool IsContainPoint(const Point& p,double e) const{
        return p.Distance2D(_cirCp) - e <= _cirR;
    }
    double Length() const{return 2 * M_PI * _cirR;}
    bool IsCW() const{return _isCW;}
public:
    Point _cirCp;
    double _cirR;
    bool _isCW = true;
};
class CAMALGORITHM_EXPORT DefCurve
{
public:
    DefCurve(){}
    DefCurve(ElemType t,const DefSeg& s) : _elemType(t),_segment(s){}
    DefCurve(ElemType t,const DefArc& a) : _elemType(t),_arc(a){}
public:
    DefArc _arc;
    DefSeg _segment;
    ElemType _elemType;
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
class CAMALGORITHM_EXPORT ElemLabel
{
public:
    ElemLabel() {}
public:
    bool _eleHasMer = false;
    bool _eleHasComMer = false;
    bool _isCirDisc = false;
    bool _isValidity = true;
    bool _isIslElem = false;
    bool _isClipEle = false;
    bool _isAbsValid = false;
    bool _isAbsInvalid = false;
    bool _isConnectArc = false;
    OftBasePara _baseParam;
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
public:
    Point _n;                 ///平面法向量
    bool _isGeneral = true;   ///是否由一般式定义
    double _A,_B,_C,_D;       ///平面系数
};
class CAMALGORITHM_EXPORT  DefElem
{
public:
    DefElem(){}
    DefElem(const DefElem& other) = default;
    DefElem(ElemType type,const DefSeg& s):_elemType(type),_segment(s){}
    DefElem(ElemType type,const DefArc& a):_elemType(type),_arc(a){}
    DefElem(ElemType type,const DefCircle& c):_elemType(type),_circle(c){}
    void SetElemSeg(const DefSeg& segment) {_segment = segment;}
    void SetElemArc(const DefArc& arc) {_arc = arc;}
    void SetElemCircle(const DefCircle& circle) {_circle = circle;}
    void SetElemType(ElemType eleType) {_elemType = eleType;}
    void SetSourceIndex(int i){_sourceId = i;}
    void SetMoveType(OffsetEnum::MovePathType t){_moveType = t;}
    void SetValidity(bool v){_elemLabel._isValidity = v;}
    void SetIsClip(bool v){_elemLabel._isClipEle = v;}
    void SetAbsValid(bool v){_elemLabel._isAbsValid = v;}
    void SetCntCurve(bool v){_elemLabel._isConnectArc = v;}
    void ChangeEndpoint(const Point& p,bool isChangeEp){
        switch (_elemType) {
        case OffsetEnum::LINETYPE: _segment.PointSplit(p,isChangeEp);break;
        case OffsetEnum::ARCTYPE: _arc.PointSplit(p,isChangeEp);break;
        default: break;}
    }
    void Reverse(){
        switch (_elemType) {
        case OffsetEnum::LINETYPE:return _segment.Reverse();break;
        case OffsetEnum::ARCTYPE:return _arc.Reverse();break;
        default: break;}
    }
    DefSeg ElemSeg()const {return _segment;}
    DefArc ElemArc()const {return _arc;}
    DefCircle ElemCircle()const {return _circle;}
    ElemType ElemeType()const {return _elemType;}
    int SourceIndex()const {return _sourceId;}
    OffsetEnum::MovePathType MoveType()const {return _moveType;}
    bool IsValid() const{return _elemLabel._isValidity;}
    bool IsClipCurve() const{return _elemLabel._isClipEle;}
    bool isAbsValid()const{return _elemLabel._isAbsValid;}
    bool IsCntCurve()const{return _elemLabel._isConnectArc;}
    const Point& StarPt() const{
        switch (_elemType) {
        case OffsetEnum::LINETYPE:return _segment.SegStartPt();break;
        case OffsetEnum::ARCTYPE:return _arc.ArcStartPt();break;
        case OffsetEnum::CIRCLETYPE:return _circle._cirCp;break;
        default:return _segment.SegStartPt();}
    }
    const Point& EndPt() const{
        switch (_elemType) {
        case OffsetEnum::LINETYPE:return _segment.SegEndPt();break;
        case OffsetEnum::ARCTYPE:return _arc.ArcEndPt();break;
        case OffsetEnum::CIRCLETYPE:return _circle._cirCp;break;
        default:return _segment.SegEndPt();}
    }
    const Point& Endpoint(bool isEp) const{
        if(isEp){return EndPt();}return StarPt();
    }
    bool IsEndpoint(const Point& p,double e) const{
        switch (_elemType) {
        case OffsetEnum::LINETYPE: return p.IsSamePoint2D(_segment._segSp,e) ||
                    p.IsSamePoint2D(_segment._segEp,e);
        case OffsetEnum::ARCTYPE: return p.IsSamePoint2D(_arc._arcSp,e) ||
                    p.IsSamePoint2D(_arc._arcEp,e);
        case OffsetEnum::CIRCLETYPE: return p.IsSamePoint2D(_circle._cirCp,e);
        default: return p.IsSamePoint2D(_segment._segSp,e) ||
                    p.IsSamePoint2D(_segment._segEp,e);}
    }
public:
    int _id = -1;
    DefArc _arc;
    DefSeg _segment;
    DefCircle _circle;
    ElemType _elemType;
    OffsetEnum::ToolPathType _elemSource;
    ElemLabel _elemLabel;
    DefPlane _plane;           ///所在平面
private:
    int _sourceId;
    OffsetEnum::MovePathType _moveType;
};
class GeomIntRes
{
public:
    GeomIntRes() {}
    int _intNum = 0;
    bool _isExistInt = false;
    bool _isExiGeoInt = false;
    Point _intPnt1,_intPnt2;
public:
    void SetInt(){_isExistInt = true;}
    void SetGeoInt(){_isExiGeoInt = true;}
    void SetNum(int n){_intNum = n;}
    void SetP1(const Point& p){_intPnt1 = p;}
    void SetP2(const Point& p){_intPnt2 = p;}
    bool IsInt() const{return _isExistInt;}
    bool IsGeoInt() const{return _isExiGeoInt;}
    int Num() const{return _intNum;}
    const Point& P1() const{return _intPnt1;}
    const Point& P2() const{return _intPnt2;}
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
    bool _loHasCutMer = false;
    bool _isLoopOffset = true;
    bool _bndLoopAdd = false;
    bool _isIslMerOutLoop = false;
    bool _isPreReverse = false;
    bool _loopIntersect = false;
    bool _loopValidity = true;
    bool _isMixLoop = false;
    RotDir _loRotDir = CLOCKWISE;
    OffsetEnum::ToolPathType _loPathType = BndOffsetPath;
    /**
岛屿环偏移后可能会产生特定的局部环,该环旋向与原始的不一致,如果该环是由岛屿环偏移
产生的,那么该环就是边界还,m_isMatchTypeTrans就是用来表示与一个岛屿环配对的边
界还是否是这样的边界还,如果是,m_isMatchTypeTrans = true.*/
    ///当前状态(当前次偏移后)是否与其他环相交.
    std::shared_ptr<OffsetNode>m_hasExiNode;
public:
    void SetSelIntGenerate(bool v){_isSelIntGene = v;}
    void SetClipGenerate(bool v){_isClipGenerate = v;}
    void SetGenerateParam(const GenerateParam &p){_generateParam = p;}
    void SetHasReverse(bool b){_hasReverse = b;}
    bool IsSelIntGenerate() const{return _isSelIntGene;}
    bool IsClipGenerate() const{return _isClipGenerate;}
    GenerateParam GenerateParameter() const{return _generateParam;}
    bool HasReverse() const{return _hasReverse;}
private:
    bool _isSelIntGene = false;
    bool _isClipGenerate = false;
    bool _hasReverse = false;
    GenerateParam _generateParam;
};
class CAMALGORITHM_EXPORT  DefLoop
{
public:
    DefLoop() {}
    DefLoop(ProfType type):_loopType(type){}
    DefLoop(const std::vector<DefElem>& v):m_vElem(v){}
    DefLoop(const std::vector<DefElem>& v,ProfType ct):m_vElem(v),
        _loopType(ct){}
    DefLoop(const std::vector<DefElem>& v,bool isOpeloo) :
        m_vElem(v),_isOpenLoop(isOpeloo){}
    void SetThisOftNum(int num){_thisOftNum = num;}
    void SetPastOftNum(int num){_pastOftNum = num;}
    void SetCloseMode(bool value){_isCloseMode = value;}
    void PushBackElem(const DefElem& e){m_vElem.push_back(e);}
    void PushBack(const DefElem& e){m_vElem.push_back(e);}
    void EmplaceBack(const DefElem& e){m_vElem.emplace_back(e);}
    void PopBack(){m_vElem.pop_back();}
    void PushFirst(const DefElem& e){m_vElem.emplace(m_vElem.begin(),e);}
    void ClearElem(){m_vElem.clear();}
    void SetIndexElem(const DefElem& e,int i){m_vElem[i] = e;}
    void SetLoopElem(const std::vector<DefElem>& vElem){m_vElem = vElem;}
    void SetLoopType(ProfType type){_loopType = type;}
    void SetFirstElem(const DefElem& e){
        if(!m_vElem.empty()){m_vElem[0] = e;}
    }
    void SetLastElem(const DefElem& e){
        if(!m_vElem.empty()){m_vElem[m_vElem.size()-1] = e;}
    }
    void SetOpenLoop(bool value){_isOpenLoop = value;}
    void SetElem(const std::vector<DefElem>& elems){m_vElem = elems;}
    void SetMoveType(OffsetEnum::MovePathType t){_moveType = t;}
    void SetCurvesMoveType(OffsetEnum::MovePathType t){
        for(auto& d : m_vElem){d.SetMoveType(t);}
    }
    void SetOffsetType(OffsetEnum::ToolPathType t){
        _loopLabel._loPathType = t;
    }
    void InitialRadius(){
        for(auto& e : m_vElem){
            if(e.ElemeType() == OffsetEnum::ARCTYPE){
                e._arc.IniArcRadius();}}
    }
    void ChoiceSetRadius(double r){
        for(auto& e : m_vElem){
            if(e.ElemeType() == OffsetEnum::ARCTYPE){
                e._arc.ChoiceSetRadius(r);}}
    }
    DefElem& operator[](size_t i) {return m_vElem[i];}
    const DefElem& operator[](size_t i) const{return m_vElem[i];}
    void SetInward(bool v){_isInward = v;}
    void SetHasReverse(bool b){_loopLabel.SetHasReverse(b);}
    void Reverse(){
        for(auto& d : m_vElem){d.Reverse();}
        std::reverse(m_vElem.begin(),m_vElem.end());
    }
    std::vector<DefElem>Elems(){return m_vElem;}
    int ThisOffsetNum()const {return _thisOftNum;}
    int PastOffsetNum()const {return _pastOftNum;}
    bool IsEmpty()const {return m_vElem.empty();}
    std::vector<DefElem> LoopElem()const {return m_vElem;}
    DefElem IndexElem(int i)const {return m_vElem[i];}
    DefElem FirstElem()const {
        if(IsEmpty()){return DefElem();}
        return m_vElem.front();
    }
    DefElem LastElem()const {
        if(IsEmpty()){return DefElem();}
        return m_vElem.back();
    }
    int ElemNum() const{return m_vElem.size();}
    int Size() const{return m_vElem.size();}
    bool IsValIndex(int i)const {
        if(m_vElem.empty() || i < 0 || i > m_vElem.size() - 1)
            return false;
        return true;
    }
    int NextIndex(int i)const {
        if(m_vElem.size() <= 0){return 0;}
        if(i >= m_vElem.size() - 1){return 0;}
        return i + 1;
    }
    int PreviousIndex(int i)const {
        if(m_vElem.size() <= 0){return 0;}
        if(i <= 0){return m_vElem.size() - 1;}
        return i - 1;
    }
    bool IsInward() const{return _isInward;}
    const DefElem& PreviousElem(int i) const{return m_vElem[PreviousIndex(i)];}
    const DefElem& NextElem(int i) const{return m_vElem[NextIndex(i)];}
    bool IsCloseMode()const {return _isCloseMode;}
    bool IsOpenLoop()const {return _isOpenLoop;}
    ProfType LoopType()const {return _loopType;}
    OffsetEnum::MovePathType MoveType()const {return _moveType;}
    OffsetEnum::ToolPathType OffsetType() const{return _loopLabel._loPathType;}
    const Point& CurveEndpoint(size_t i,bool isEp) const{return
                m_vElem[i].Endpoint(isEp);}
    const Point& Endpoint(bool isEp) const{
        if(isEp){return m_vElem.back().Endpoint(true);}
        return m_vElem.front().Endpoint(false);}
    bool IsEndToEnd(double v,bool is2d = true) const{
        if(is2d){return Endpoint(false).IsSamePoint2D(Endpoint(true),v);}
        return Endpoint(false).IsSamePoint3D(Endpoint(true),v);
    }
    bool IsConnect(double err,bool is2D = true) const{
        if(m_vElem.empty()) return false;
        size_t n = 0;
        for(size_t i = 0;i < m_vElem.size();++i){
            n = NextIndex(i);
            if(is2D){if(!
                        m_vElem[i].EndPt().IsSamePoint2D(m_vElem[n].StarPt(),err)){
                    return false;}
            }else{if(!m_vElem[i].EndPt().IsSamePoint3D(m_vElem[n].StarPt(),err))
                {
                    return false;}}
        }
        return true;}
    bool HasReverse() const{return _loopLabel.HasReverse();}
    bool IsOffset() const{return _loopLabel._isLoopOffset;}
public:
    int _id = -1;
    int _thisOftNum = 0;
    LoopLabel _loopLabel;
    std::vector<DefElem> m_vElem;
private:
    int _pastOftNum = 0;
    bool _isOpenLoop = false;
    bool _isCloseMode = false;
    bool _isInward = true;
    ProfType _loopType = OUTSIDEPROFILE;
    OffsetEnum::MovePathType _moveType;
};
class CAMALGORITHM_EXPORT  OffsetNode
{
public:
    OffsetNode() {}
    OffsetNode(OffsetEnum::ToolPathType t):_geneType(t){}
    OffsetNode(OffsetEnum::ToolPathType t,const std::vector<DefLoop>& v):
        _geneType(t),_vLoopData(v){}
    OffsetNode(const std::vector<DefLoop>& ls):_vLoopData(ls){}
    OffsetNode(OffsetEnum::ToolPathType t,const std::vector<DefLoop>& ls,
               const std::vector<std::shared_ptr<OffsetNode>>& ns):
        _geneType(t),_vLoopData(ls),_vOffsetNode(ns){}
    void SetPathType(OffsetEnum::ToolPathType t){_geneType = t;}
    void SetArrayLoop(const std::vector<DefLoop>& ls){_vLoopData = ls;}
    void SetArrayNode(const std::vector<std::shared_ptr<OffsetNode>>& ns)
    {_vOffsetNode = ns;}
    void ClearNode(){_vLoopData.clear();_vOffsetNode.clear();}
    void ClearLoop(){_vLoopData.clear();}
    void ClearData(){ClearNode();ClearLoop();}
    void InsertNode(int i,const std::shared_ptr<OffsetNode>& n){
        _vOffsetNode.insert(_vOffsetNode.begin() + i,n);}
    void InsertLoop(int i,const DefLoop& loop){
        _vLoopData.insert(_vLoopData.begin() + i,loop);}
    void SetNodeIndex(int i){_nodeIndex = i;}
    void PushBackNode(const std::shared_ptr<OffsetNode>& n)
    {_vOffsetNode.push_back(n);}
    void PushBackLoop(const DefLoop& loop){_vLoopData.push_back(loop);}
    void PopBackLoop(){_vLoopData.pop_back();}
    void PopBackNode(){_vOffsetNode.pop_back();}
    void ReverseLoops(){std::reverse(_vLoopData.begin(),_vLoopData.end());}
    void ReverseNodes(){std::reverse(_vOffsetNode.begin(),_vOffsetNode.end());}
    OffsetEnum::ToolPathType PathType()const {return _geneType;}
    bool IsLoopsEmpty()const {return _vLoopData.empty();}
    bool IsNodeEmpty()const {return _vOffsetNode.empty();}
    bool IsEmpty()const {return _vLoopData.empty() && _vOffsetNode.empty();}
    DefLoop LastLoop()const {return _vLoopData.back();}
    std::shared_ptr<OffsetNode> LastOftNode()const {return _vOffsetNode.back();}
    DefLoop IndexLoop(int i)const {return _vLoopData[i];}
    std::shared_ptr<OffsetNode> IndexAreaNode(int i)const {return
                _vOffsetNode[i];}
    std::vector<DefLoop> ArrayLoop()const {return _vLoopData;}
    std::vector<std::shared_ptr<OffsetNode>> ArrayNode()const {return
                _vOffsetNode;}
    int PathNodeIndex() const{return _nodeIndex;}
public:
    OffsetEnum::ToolPathType _geneType;
    std::vector<DefLoop> _vLoopData;
    std::vector<std::shared_ptr<OffsetNode>> _vOffsetNode;
private:
    int _nodeIndex = 0;
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
        _isBndOffset(isBndOft),_profOffsetStyle(style),
        _styleAngle(angle){}
    PathTechnic(bool isBlank,bool isAccOut,bool isBndOft) :
        _isBlankBnd(isBlank),_isAcceptBorOut(isAccOut),
        _isBndOffset(isBndOft){}
    PathTechnic(bool isBlank,bool isAccOut,bool isBndOft,bool isAddOri) :
        _isBlankBnd(isBlank),_isAcceptBorOut(isAccOut),
        _isBndOffset(isBndOft),_isAddOriLoop(isAddOri){}
    void SetIsBlankBoundary(bool value){_isBlankBnd = value;}
    void SetIsAcceptBoundaryOut(bool value){_isAcceptBorOut = value;}
    void SetIsBoundaryOffset(bool value){_isBndOffset = value;}
    void SetProfOffsetStyle(OffsetStyle value){_profOffsetStyle = value;}
    void SetStyleAngle(double value){_styleAngle = value;}
    ///void SetMillDirection(MillDirction value){_millDirection = value;}
    void SetMcStrategy(MC_Strategy value){_mcStrategy = value;}
    void SetAddOriginLoop(bool value){_isAddOriLoop = value;}
    void SetExpandSmall(bool value){_isExpandSmall = value;}
    bool IsBlankBoundary()const {return _isBlankBnd;}
    bool IsAcceptBoundaryOut()const {return _isAcceptBorOut;}
    bool IsBoundaryOffset()const {return _isBndOffset;}
    OffsetStyle ProfOffsetStyles()const {return _profOffsetStyle;}
    double OffsetStyleAngle()const {return _styleAngle;}
    MC_Strategy McStrategy()const {return _mcStrategy;}
    bool IsAddOriginLoop()const {return _isAddOriLoop;}
    bool IsExpandSmall()const {return _isExpandSmall;}
private:
    double _dodgeAllowance = PreErr5_5;  ///避让余量
    double _styleAngle = 45;             ///角度制[0,360)
    bool _isBlankBnd = true;             ///边界轮廓是否是毛坯
    bool _isAcceptBorOut = false;        ///是否接受刀具走原始边界外部
    bool _isBndOffset = true;            ///边界轮廓是否偏置
    bool _isAddOriLoop = false;          ///是否添加原始轮廓至刀路结果中.
    bool _isExpandSmall = false;         ///是否针对较小环进行扩大.
    ///刀路样式，目前统一为边界轮廓和岛屿轮廓同时偏置
    OffsetStyle _profOffsetStyle = Simultaneously_Offset;
    MC_Strategy _mcStrategy = ModelAreaClearing;
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
    bool IsFixedOffset()const {return _isOftFixNum;}
    int OffsetNumber()const {return _offsetNum;}
    double ToolRadius()const {return _toolRadius;}
    double FirstOffsetValue()const {return _firOffsetVal;}
    double OffsetValue()const {return _offsetVal;}
    bool IsPathSmooth()const {return _isSmooth;}
    double PathSmoothValue()const {return _smoothVal;}
    bool AlterableValue()const {return _alterableValue;}
    bool IsPureEqud()const {return _isPureEqud;}
    bool IsAblePreProcess()const {return _isAblePreProc;}
    bool IsFirSmooth()const {return _isFirSmooth;}
private:
    bool _isOftFixNum = false;                   ///是否固定偏置
    bool _isSmooth = false;                      ///是否光顺
    bool _isFirSmooth = false;                   ///第一次偏置是否光顺
    bool _alterableValue = true;                 ///是否可改变偏置距离
    bool _isPureEqud = false;                    ///true不会计算自相交
    bool _isAblePreProc = false;
    int _offsetNum = 1;                          ///偏置次数
    double _firOffsetVal = 2.2;                  ///首次偏置距离
    double _offsetVal = 2.2;                     ///偏置距离
    double _toolRadius = (_offsetVal / 0.7) / 2; ///刀具半径
    double _smoothVal = 0.1;                     ///光顺半径
};
class CAMALGORITHM_EXPORT  OffsetParam
{
public:
    OffsetParam() {}
    ///不带岛屿轮廓的偏置参数设定
    OffsetParam(bool isFixOftNum,int offsetNum,
                double firOffsetVal,double offsetVal,
                bool isSmooth,double smoothVal)
    {
        OffsetUnit oftUnit(isFixOftNum,offsetNum,firOffsetVal,
                           offsetVal,isSmooth,smoothVal);
        _offsetUnit = oftUnit;
    }
    ///带岛屿轮廓的偏置参数设置
    OffsetParam(bool isFixOftNum,int offsetNum,
                double firOffsetVal,double offsetVal,
                bool isSmooth,double smoothVal,
                OffsetStyle offsetStyle,
                MC_Strategy mcStrategy)
    {
        OffsetUnit oftUnit(isFixOftNum,offsetNum,
                           firOffsetVal,offsetVal,
                           isSmooth,smoothVal);
        _offsetUnit = oftUnit;
    }
    ///新增刀具半径、刀具是否允许走原始边界
    OffsetParam(bool isFixOftNum,int offsetNum,
                double firOffsetVal,double offsetVal,
                bool isSmooth,double smoothVal,
                double toolRadius,bool isBlankBnd,
                bool isAccBorOut,OffsetStyle offsetStyle,
                MC_Strategy mcStrategy)
    {
        _pathTech.SetIsBlankBoundary(isBlankBnd);
        _pathTech.SetIsAcceptBoundaryOut(isAccBorOut);
        _pathTech.SetProfOffsetStyle(offsetStyle);
        _pathTech.SetMcStrategy(mcStrategy);
        OffsetUnit oftUnit(isFixOftNum,offsetNum,
                           toolRadius,firOffsetVal,offsetVal,
                           isSmooth,smoothVal);
        _offsetUnit = oftUnit;
    }
    OffsetParam(OffsetUnit oftUni) : _offsetUnit(oftUni){}
    OffsetParam(OffsetUnit oftUni,PathTechnic pathTech,
                MillDirction millDir) : _offsetUnit(oftUni),
        _pathTech(pathTech),_millDirection(millDir){}
    void SetDodgeAllowance(double value){_dodgeAllowance = value;}
    void SetIsFirstOffset(bool value){_isFirstOffset = value;}
    double DodgeAllowance()const {return _dodgeAllowance;}
    bool IsFirstOffset()const {return _isFirstOffset;}
public:
    OffsetUnit _offsetUnit;
    PathTechnic _pathTech;
    MillDirction _millDirection;
private:
    bool _isFirstOffset = false;
    double _dodgeAllowance = PreErr5_6; ///避让余量
};
class GeomArea
{
public:
    GeomArea() {}
    GeomArea(const DefLoop& b) : _bndLoop(b){}
    GeomArea(const DefLoop& b,const std::vector<DefLoop>& v) :
        _bndLoop(b),_vIslLoop(v){}
    GeomArea(const DefLoop& b,const std::vector<DefLoop>& v,
             bool isBOft,bool isIOft) :
        _bndLoop(b),_vIslLoop(v),_isBndOffset(isBOft),_isIslOffset(isIOft) {}
    GeomArea(const DefLoop& b,const std::vector<DefLoop>& v,
             const std::vector<DefLoop>&block) :
        _bndLoop(b),_vIslLoop(v),_vBlockLoop(block){}
    DefLoop BndLoop()const {return _bndLoop;}
    DefLoop OriBndLoop()const {return _oriBndLoop;}
    std::vector<DefLoop>IslLoop()const {return _vIslLoop;}
    std::vector<DefLoop>OriIslLoop()const {return _vOriIslLoop;}
    std::vector<DefLoop>BlockLoop()const {return _vBlockLoop;}
    bool IsBndOffset()const {return _isBndOffset;}
    bool IsIslOffset()const {return _isIslOffset;}\
    void SetBndLoop(const DefLoop& b) {_bndLoop = b;}
    void SetIslLoop(const std::vector<DefLoop>& vIsl) {_vIslLoop = vIsl;}
    void SetBlockLoop(const std::vector<DefLoop>& vBlo) {_vBlockLoop = vBlo;}
    void SetAddIslLoop(bool value){_isAddIslLoop = value;}
    void SetAddBndLoop(bool value){_isAddBndLoop = value;}
    void SetValidMatch(bool value){_isValidMatch = value;}
    void SetOriBndLoop(const DefLoop& b) {
        _oriBndLoop = b;
        _oriBndLoop.SetCloseMode(true);
    }
    void SetOriIslLoop(const std::vector<DefLoop>& vIsl) {
        _vOriIslLoop = vIsl;
        for(auto& l : _vOriIslLoop)
            l.SetCloseMode(true);
    }
    void SetIndexIslLoop(const DefLoop& l,int i){_vIslLoop[i] = l;}
    void SetIndexOriIslLoop(const DefLoop& l,int i){_vOriIslLoop[i] = l;}
    void PushBackIslLoop(const DefLoop& loop){_vIslLoop.push_back(loop);}
    void PushBackOriIslLoop(const DefLoop& loop){_vOriIslLoop.push_back(loop);}
    void PushBackBlockLoop(const DefLoop& loop){_vBlockLoop.push_back(loop);}
    void SetBndLoopOffset(bool isOffset){_isBndOffset = isOffset;}
    void SetIslLoopOffset(bool isOffset){_isIslOffset = isOffset;}
    void ClearBndIslElem()
    {
        _bndLoop.ClearElem();
        for(auto& lo : _vIslLoop)
            lo.ClearElem();
        for(auto& lo : _vBlockLoop)
            lo.ClearElem();
    }
    bool IsAddBndLoop()const{return _isAddBndLoop;}
    bool IsAddIslLoop()const{return _isAddIslLoop;}
    bool IsValidMatch()const{return _isValidMatch;}
public:
    DefLoop _bndLoop,_oriBndLoop;
    std::vector<DefLoop>_vIslLoop;
    std::vector<DefLoop>_vOriIslLoop;
    std::vector<DefLoop>_vBlockLoop;     ///干涉
    bool _isBndOffset = true;
    bool _isIslOffset = true;
private:
    bool _isAddBndLoop = false;
    bool _isAddIslLoop = false;
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
    int GeomAreaNum()const {return _vGeomArea.size();}
    int AreaNodeNum()const {return _vAreaNode.size();}
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
namespace GeomFitting {
class FittArc
{
public:
    FittArc() {}
    void ClearFittingData()
    {
        _indexs.clear();
        _arcCps.clear();
        _nodes.clear();
        _arcRadius.clear();
        _aglVar.clear();
    }
public:
    bool _isArcCW = true;
    std::vector<int>_indexs;
    std::vector<Point>_arcCps;
    std::vector<Point>_nodes;
    std::vector<double>_arcRadius;
    std::vector<double>_aglVar;
    DefArc _arc;
    DefArc _preArc;
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
#endif // DATASTRUCTURE_H
