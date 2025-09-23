#ifndef GEOMUNITSTRUCT_H
#define GEOMUNITSTRUCT_H
#include<TopoDS_Shape.hxx>
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
#define _DevModel 0
///#include"../../../../offset2D/ToolTrajectoryAlg/CommonFile/DataStructure.h"
#include "../../../../offset2D/ToolTrajectoryAlg/CommonFile/Header.h"
#define AglErr PreErr5_1
namespace OftStr = oft;
namespace GeomStruct {
class PntSet
{
public:
    PntSet() {}
    PntSet(const std::vector<oft::Point>&v) : _vPnt(v) {}
public:
    std::vector<oft::Point>_vPnt;
};
}
namespace GridArea {
class LinePnt{
public:
    LinePnt(){}
    LinePnt(const std::vector<oft::Point> &v){_vPnt = v;}
    void PushBack(const oft::Point &v){_vPnt.push_back(v);}
    oft::Point IndexPt(int i) const{return _vPnt[i];}
    oft::Point Back() const{return _vPnt.back();}
    oft::Point Front() const{return _vPnt.front();}
    oft::Point * IndexNormal(int i) const{return 
_vPnt[i]._lable._normal;}
    oft::Point TailSecond() const{
        return _vPnt[_vPnt.size() - 2];
    }
    void SetOnBorder(int i){_vPnt[i]._lable.SetOnBorder(true);}
    void SetTailOnBorder(){
        _vPnt[_vPnt.size() - 1]._lable.SetOnBorder(true);
    }
    void SetHeadOnBorder(){_vPnt[0]._lable.SetOnBorder(true);}
    void SetTailOnSteep(){
        _vPnt[_vPnt.size() - 1]._lable._isOnSteep = true;
    }
    void SetHeadOnSteep(){_vPnt[0]._lable._isOnSteep = true;}
    void Reverse(){std::reverse(_vPnt.begin(),_vPnt.end());}
public:
    std::vector<oft::Point>_vPnt;
};
/**
m,0|--|--|--|--|--|--|m,n
   |--|--|--|--|--|--|   |--|--|--|--|--|--|
   |--|--|--|--|--|--|
   |--|--|--|--|--|--|
   |--|--|--|--|--|--|
0,0|--|--|--|--|--|--|0,n
*/
class AreaLine
{
public:
    AreaLine() {}
    AreaLine(std::vector<std::vector<LinePnt>> &h,
             std::vector<std::vector<LinePnt>> &v) :
        _vArrHorSeg(h),_vArrVerSeg(v){}
    void HorPushBack(const std::vector<LinePnt> &v){
        _vArrHorSeg.push_back(v);}
    void VerPushBack(const std::vector<LinePnt> &v){
        _vArrVerSeg.push_back(v);}
    void HorRowPushBack(int i,const LinePnt &v){
        _vArrHorSeg[i].push_back(v);}
    void VerColPushBack(int i,const LinePnt &v){
        _vArrVerSeg[i].push_back(v);}
    void SetHorizontalSeg(const std::vector<std::vector<LinePnt>>&h){
        _vArrHorSeg = h;}
    void SetVerticalSeg(const std::vector<std::vector<LinePnt>>&v){
        _vArrVerSeg = v;}
    std::vector<std::vector<LinePnt>> HorizontalSeg() const{
        return _vArrHorSeg;}
    std::vector<std::vector<LinePnt>> VerticalSeg() const{
        return _vArrVerSeg;}
    LinePnt RowLinePt(int r,int i) const{
        return _vArrHorSeg[r][i];}
    LinePnt VerLinePt(int c,int i) const{
        return _vArrVerSeg[c][i];}
public:
    std::vector<std::vector<LinePnt>>_vArrHorSeg;
    std::vector<std::vector<LinePnt>>_vArrVerSeg;
};
}
namespace GeomUnitStruct{
class DefEdge
{
public:
    DefEdge() {}
    DefEdge(int i,int j) {
        if(i < j){_a = i;_b = j;}
        else{_a = j;_b = i;}
    }
    DefEdge(const OftStr::Point& s,
            const OftStr::Point& e) : _sp(s),_ep(e){}
    DefEdge(int i,int j,const OftStr::Point& s,
            const OftStr::Point& e) : _sp(s),_ep(e){
        if(i < j){_a = i;_b = j;}
        else{_a = j;_b = i;}
    }    DefEdge(int i,int j,const OftStr::Point& s,
            const OftStr::Point& e,
            const OftStr::Point& n) : _sp(s),_ep(e),_nor(n){
        if(i < j){_a = i;_b = j;}
        else{_a = j;_b = i;}
        ///_sp.SetNormal(n);
        ///_ep.SetNormal(n);
    }
    bool operator==(const DefEdge& other) const {
        return _a == other._a && _b == other._b;
    }
    bool HasVisit() const{return _hasVisit;}
    bool IsEndpoint(const OftStr::Point& p,double e) const{
        return p.IsSameCoord3D(_sp,e) || p.IsSameCoord3D(_ep,e);
    }
    int _id = -1;
    int _a = -1,_b = -1;
    bool _hasVisit = false;
    OftStr::Point _sp,_ep,_nor;
    std::vector<int>_adjEdgs;
};
struct Polyline {
    std::vector<int> segmentIds;
    std::vector<OftStr::Point> points;
};
class AdjTri {
public:
     AdjTri(){}
    int _id,_eId;
    std::vector<int>_vAdjTri;
};
class EdgeHash {
public:
    size_t operator()(const DefEdge& e) const {
        return hash<int>()(e._a) ^ (hash<int>()(e._b) << 1);
    }
};
class TriIndex{
public:
    TriIndex() {}
    int ids[3];
};
class Triangle;
class TriLable{
public:
    TriLable(){}
    double _angle;
    bool _hasVisited = false;
    bool _isE0hasVisited = false;
    bool _isE1hasVisited = false;
    bool _isE2hasVisited = false;
};
class TriBorder{
public:
    TriBorder(){}
    bool _isSteep = false;
    bool _isBorder = false;
    bool _isBorder0 = false,_isBorder1 = false,_isBorder2 = false;    bool _isE0Border = false,_isE1Border = false,_isE2Border = false;
};
class TriInfo{
public:
    struct AdjPoint{
        AdjPoint() {}
        AdjPoint(int t,int p) :_triId(t),_ptId(p) {}
        int _triId = -1,_ptId = -1;
    };
    TriInfo(){}
    void AddP0AdjPt(int t,int p){_vAdjPt0.emplace_back(AdjPoint(t,p));}
    void AddP1AdjPt(int t,int p){_vAdjPt1.emplace_back(AdjPoint(t,p));}
    void AddP2AdjPt(int t,int p){_vAdjPt2.emplace_back(AdjPoint(t,p));}
public:
    std::vector<Triangle>_vAdjTri;
    std::vector<AdjPoint>_vAdjPt0,_vAdjPt1,_vAdjPt2;
};
class Triangle{
public:
    Triangle(){}
    Triangle(const OftStr::Point& p0,const OftStr::Point& p1,
             const OftStr::Point& p2) : _p0(p0),_p1(p1),_p2(p2){}
    Triangle(int a,int b,int c){
        _ids[0] = a,_ids[1] = b,_ids[2] = c;}
    void SetP0(const OftStr::Point &p){_p0 = p;}
    void SetP1(const OftStr::Point &p){_p1 = p;}
    void SetP2(const OftStr::Point &p){_p2 = p;}
    void SetNormal(const OftStr::Point &p){_normal = p;}
    void CalculateAngel(){
        oft::Point ab = _p0 - _p1,ac = _p0 - _p2,
                _normal = ab.Cross(ac);
        double l = _normal.Length2D();
        double a = std::atan2(std::abs(_normal._z),l);
        _lable._angle = a * _AlgRad;
    }
    void OnSteepCase(double ref){
        if(_lable._angle > ref){_border._isSteep = true;}
    }
    void AddAdjoin(Triangle& t){
        _relInfo._vAdjTri.emplace_back(t);
    }
    void SetP0OnBorder(){_border._isBorder0 = true;}
    void SetP1OnBorder(){_border._isBorder1 = true;}
    void SetP2OnBorder(){_border._isBorder2 = true;}
    void SetE0HasVisited(){_lable._isE0hasVisited = true;}
    void SetE1HasVisited(){_lable._isE0hasVisited = true;}
    void SetE2HasVisited(){_lable._isE0hasVisited = true;}
    void SetBorderCase(){
        _border._isE0Border = _border._isBorder0 && _border._isBorder1;
        _border._isE1Border = _border._isBorder1 && _border._isBorder2;
        _border._isE2Border = _border._isBorder2 && _border._isBorder0;
        _border._isBorder = _border._isE0Border ||
                _border._isE1Border || _border._isE2Border;
    }
    void SetE0OnBorder(){_border._isE0Border = true;}
    void SetE1OnBorder(){_border._isE1Border = true;}
    void SetE2OnBorder(){_border._isE2Border = true;}
    void SetOnBorder(){_border._isBorder = true;}
    OftStr::Point P0() const{return _p0;}    OftStr::Point P1() const{return _p1;}
    OftStr::Point P2() const{return _p2;}
    OftStr::Point Normal() const{return _normal;}
    bool IsBorder() const{return _border._isBorder;}
    bool IsSteep() const{return _border._isSteep;}
    double Angle() const{return _lable._angle;}
    bool IsP0Border() const{return _border._isBorder0;}
    bool IsP1Border() const{return _border._isBorder1;}
    bool IsP2Border() const{return _border._isBorder2;}
    bool IsE0Border() const{return _border._isE0Border;}
    bool IsE1Border() const{return _border._isE1Border;}
    bool IsE2Border() const{return _border._isE2Border;}
    bool IsCoordVertex(const OftStr::Point& p,double e) const{
        return p.IsSameCoord3D(_p0,e) || p.IsSameCoord3D(_p1,e)
                || p.IsSameCoord3D(_p2,e);
    }
    std::vector<TriInfo::AdjPoint>Adjoin0() const{return _relInfo._vAdjPt0;}
    std::vector<TriInfo::AdjPoint>Adjoin1() const{return _relInfo._vAdjPt1;}
    std::vector<TriInfo::AdjPoint>Adjoin2() const{return _relInfo._vAdjPt2;}
    bool IsE0HasVisited() const{return _lable._isE0hasVisited;}
    bool IsE1HasVisited() const{return _lable._isE0hasVisited;}
    bool IsE2HasVisited() const{return _lable._isE0hasVisited;}
    DefEdge IndexEdge(int i) const {
        return DefEdge(_ids[i],_ids[(i + 1) % 3]);
    }
    DefEdge Edge0(){return DefEdge(_ids[0],_ids[1],_p0,_p1,_normal);}
    DefEdge Edge1(){return DefEdge(_ids[1],_ids[2],_p1,_p2,_normal);}
    DefEdge Edge2(){return DefEdge(_ids[2],_ids[0],_p2,_p0,_normal);}
    std::vector<DefEdge>Edges() const {
        return {DefEdge(_ids[0], _ids[1]),
                    DefEdge(_ids[1], _ids[2]),DefEdge(_ids[2], _ids[0])
        };
    }
public:
    int _ids[3];
    OftStr::Point _p0;
    OftStr::Point _p1;
    OftStr::Point _p2;
public:
    int _id;
    TriInfo _relInfo;
    TriLable _lable;
    TriBorder _border;
    oft::Point _normal;
    std::vector<int>_adjs[3];
};
class TrianRes{
public:
    TrianRes(){}
    TrianRes(const std::vector<OftStr::Point>&v,
              const std::vector<Triangle>&v1) : _vNode(v),_vTriId(v1){}
    std::vector<OftStr::Point>_vNode;
    std::vector<Triangle>_vTriId;
};
class RecArcEdge{public:
    RecArcEdge(){}
public:
    bool _isExiHorArc = false;
    bool _isExiVerArc = false;
    double _horY,verX;
    std::vector<oft::DefArc> _vHorArc,_vVerArc;
};
class RecAglBor{
public:
    RecAglBor(){}
public:
    bool _isInsBord = false;
    bool _isLineBord = false;
    std::vector<oft::Point>_vBordPt;
};
class RecLable{
public:
    RecLable(){}
public:
    bool _isVisited = false;
};
class RecVertex{
public:
    RecVertex(){}
public:
    GridArea::LinePnt _vSeg[4];
    GridArea::LinePnt* _seg[4];/// 指针数组.
    oft::Point _p0,_p1,_p2,_p3;
    oft::Point _mp0,_mp1,_mp2,_mp3,_cp;
    oft::Point _maxZPt,_minZPt;
};
/**
p1------p2    ----l2---->
 |      |    |          |
 |      |  l1|          |l3
 |      |    |          |
p0------p3    ----l0---->*/
class Rectangle{
public:
    Rectangle(){}
    Rectangle(const OftStr::Point& p0,const OftStr::Point& p1,
              const OftStr::Point& p2,const OftStr::Point& p3){
        _vertex._p0 = p0;_vertex._p1 = p1;_vertex._p2 = p2;_vertex._p3 = p3;
    }
    Rectangle(const OftStr::Point& p0,const OftStr::Point& p1,
              const OftStr::Point& p2,const OftStr::Point& p3,
              const std::initializer_list<GridArea::LinePnt> &l){
        _vertex._p0 = p0;_vertex._p1 = p1;_vertex._p2 = p2;
        _vertex._p3 = p3;SetArraySegment(l);
    }
public:
    void SetVertex(){
        _vertex._p0 = _vertex._seg[0]->_vPnt.front();
        _vertex._p1 = _vertex._seg[1]->_vPnt.back();
        _vertex._p2 = _vertex._seg[2]->_vPnt.back();
        _vertex._p3 = _vertex._seg[0]->_vPnt.back();
        _vertex._mp0 = EdgeMidpoint(0);        _vertex._mp1 = EdgeMidpoint(1);
        _vertex._mp2 = EdgeMidpoint(2);
        _vertex._mp3 = EdgeMidpoint(3);
        _vertex._cp._x = (_vertex._p0._x + _vertex._p1._x + _vertex._p2._x + 
_vertex._p3._x) / 4;
        _vertex._cp._y = (_vertex._p0._y + _vertex._p1._y + _vertex._p2._y + 
_vertex._p3._y) / 4;
        _vertex._cp._z = (_vertex._p0._z + _vertex._p1._z + _vertex._p2._z + 
_vertex._p3._z) / 4;
    }
    void SetIsSteep(bool v){_isSteep = v;}
    void CalAngleWithXY(){
        double theta;
#if 0
#else
        SetExtremeZPoint();
        double d = _vertex._maxZPt.Distance2D(_vertex._minZPt),
                z = _vertex._maxZPt.Z() - _vertex._minZPt.Z();
        theta = std::atan2(z,d);
#endif
        _angleWithXY = theta * 180 / M_PI;
    }
    void SetLocatedBorder() {
        _isLocBorder = _vertex._p0._lable.IsOnBorder() ||
                _vertex._p1._lable.IsOnBorder() ||
                _vertex._p2._lable.IsOnBorder() ||
                _vertex._p3._lable.IsOnBorder();}
    void SetExtremeZPoint() {
        _vertex._maxZPt = _vertex._p0.Z() > _vertex._p1.Z() ? _vertex._p0 : 
_vertex._p1,
                _vertex._minZPt = _vertex._p0.Z() < _vertex._p1.Z() ? 
_vertex._p0 : _vertex._p1;
        if(_vertex._p0.Z() > _vertex._p1.Z()){
            _vertex._maxZPt = _vertex._p0;
            _vertex._minZPt = _vertex. _p1;}
        else{_vertex._maxZPt = _vertex._p1;
            _vertex._minZPt = _vertex._p0;}
        if(_vertex._p2.Z() > _vertex._maxZPt.Z())
            _vertex._maxZPt = _vertex._p2;
        if(_vertex._p3.Z() > _vertex._maxZPt.Z())
            _vertex._maxZPt = _vertex._p3;
        if(_vertex._p2.Z() < _vertex._minZPt.Z())
            _vertex._minZPt = _vertex._p2;
        if(_vertex._p3.Z() < _vertex._minZPt.Z())
            _vertex._minZPt = _vertex._p3;
    }
    void SetArrRectangle(const std::vector<std::shared_ptr<Rectangle>> &v){
        _vRect = v;}
    void SetArraySegment(const std::initializer_list<GridArea::LinePnt> &l){
        int i = 0;
        for(auto& d : l) {
            _vertex._vSeg[i] = d;
            i ++;}}
    void SetArraySegment(GridArea::LinePnt vSeg[4]){
        for (int i = 0; i < 4; ++i)
            _vertex._vSeg[i] = vSeg[i];}
    void SetOnBorder(int s,int p){
        _vertex._vSeg[s]._vPnt[p]._lable.SetOnBorder(true);}
    void SetHeadOnBorder(int s){_vertex._vSeg[s].SetHeadOnBorder();}
    void SetTailOnBorder(int s){_vertex._vSeg[s].SetTailOnBorder();}
    void ReverseSegment(int i){_vertex._vSeg[i].Reverse();}    bool IsSteep() const{return _isSteep;}
    double AngleWithXY() const{return _angleWithXY;}
    bool IsLocatedBorder() const{return _isLocBorder;}
    std::vector<std::shared_ptr<Rectangle>> ArrayRectangle() {
        return _vRect;}
    oft::Point IndexPt(int s,int p) const{
        return _vertex._vSeg[s]._vPnt[p];}
    std::vector<oft::Point>IndexPtSet(
            int i,bool isReverse = false) const{
        return _vertex._vSeg[i]._vPnt;}
    GridArea::LinePnt IndexSeg(int i) const{
        return _vertex._vSeg[i];}
    oft::Point LinePt(
            int i,int p) const{
        return _vertex._vSeg[i]._vPnt[p];}
    GridArea::LinePnt ExceptFirst(int i) const{
        return GridArea::LinePnt({_vertex._vSeg[i]._vPnt.begin() + 1,
                                  _vertex._vSeg[i]._vPnt.end()});}
    GridArea::LinePnt ExceptLast(int i) const{
        return GridArea::LinePnt({_vertex._vSeg[i]._vPnt.begin(),
                                  _vertex._vSeg[i]._vPnt.end() - 1});}
    GridArea::LinePnt ExceptFirstEnd(int i) const{
        return GridArea::LinePnt({_vertex._vSeg[i]._vPnt.begin() + 1,
                                  _vertex._vSeg[i]._vPnt.end() - 1});}
    std::vector<OftStr::Point>EdgePoint(int id) const{
        std::vector<OftStr::Point>v;
        for(auto& p : _vertex._seg[id]->_vPnt)
            v.push_back(p);
        return v;
    }
    OftStr::Point EdgeMidpoint(int i) const{
        return _vertex._seg[i]->Front().Midpoint(_vertex._seg[i]->Back());
    }
    OftStr::Point P0()const {return _vertex._p0;}
    OftStr::Point P1()const {return _vertex._p1;}
    OftStr::Point P2()const {return _vertex._p2;}
    OftStr::Point P3()const {return _vertex._p3;}
    OftStr::Point mp0()const {return _vertex._mp0;}
    OftStr::Point mp1()const {return _vertex._mp1;}
    OftStr::Point mp2()const {return _vertex._mp2;}
    OftStr::Point mp3()const {return _vertex._mp3;}
    OftStr::Point cp()const {return _vertex._cp;}
    void SetP0(const OftStr::Point& p){_vertex._p0 = p;}
    void SetP1(const OftStr::Point& p){_vertex._p1 = p;}
    void SetP2(const OftStr::Point& p){_vertex._p2 = p;}
    void SetP3(const OftStr::Point& p){_vertex._p3 = p;}
    void Setmp0(const OftStr::Point& p){_vertex._mp0 = p;}
    void Setmp1(const OftStr::Point& p){_vertex._mp1 = p;}
    void Setmp2(const OftStr::Point& p){_vertex._mp2 = p;}
    void Setmp3(const OftStr::Point& p){_vertex._mp3 = p;}
    void Setcp(const OftStr::Point& p){_vertex._cp = p;}
public:
    RecVertex _vertex;
    RecLable _lable;
    RecAglBor _aglBord;
    std::vector<std::shared_ptr<Rectangle>> _vRect;
public:
    int _index = 0;
    double _angleWithXY = 0;
    bool _isSteep = false;    bool _isLocBorder = false;
    RecArcEdge _arcEdge;
};
class RectPlane{
public:
    RectPlane(){}
    RectPlane(const vector<vector<Rectangle>>&v){_vArrRect = v;}
    void SetArrRectangle(const vector<vector<Rectangle>>&v){_vArrRect = v;}
    void CalRectNumber(){
        if(_vArrRect.empty())
            return;
        _rectNum = _vArrRect.size() * _vArrRect[0].size();}
    vector<vector<Rectangle>> ArrRectangle() const{return _vArrRect;}
    int RectangleNumber() const{return _rectNum;}
    Rectangle IndexRect(int r,int c) const{
        return _vArrRect[r][c];}
    void SetOnBorder(int r,int c,int s,int p){
        _vArrRect[r][c]._vertex._vSeg[s]._vPnt[p]._lable.SetOnBorder(true);
    }
public:
    int _rectNum = 0;
    std::vector<vector<Rectangle>>_vArrRect;
};
class GridPoint{
public:
    GridPoint(){}
    GridPoint(const vector<vector<OftStr::Point>>&v){_vArrPoint = v;}
    void SetPlanePoint(const vector<vector<OftStr::Point>>&v){_vArrPoint = v;}
    void CalPlanePointNum(){
        if(_vArrPoint.empty())
            return;
        _pntNum = _vArrPoint.size() * _vArrPoint[0].size();}
    vector<vector<oft::Point>> GetPlanePoint() const{return 
_vArrPoint;}
    int PlanePointNum() const{return _pntNum;}
public:
    int _pntNum = 0;
    vector<vector<oft::Point>>_vArrPoint;
};
class ModelStruct{
public:
    ModelStruct(){}
    ModelStruct(const std::vector<std::vector<GridArea::LinePnt>>&h,
                const std::vector<std::vector<GridArea::LinePnt>>&v) :
        _vHorLine(h),_vVerLine(v){}
    ModelStruct(const std::vector<std::vector<GeomUnitStruct::Rectangle>>&r) :
        _vArrRec(r){}
    ModelStruct(const std::vector<std::vector<GridArea::LinePnt>>&h,
                const std::vector<std::vector<GridArea::LinePnt>>&v,
                const std::vector<std::vector<GeomUnitStruct::Rectangle>>&r) :
        _vHorLine(h),_vVerLine(v),_vArrRec(r){}
    void JudgeTriangleSteep(double v){
        for(auto& d : _vTriangle){
            if(d._lable._angle < v){d._border._isSteep = true;}        }
    }
    void Clear(){
        _vHorLine.clear();_vVerLine.clear();
        _vArrRec.clear();_vModBorPt.clear();
        _vBorderPt.clear();_vModStePt.clear();
        _vSteepPt.clear();_vModFlaPt.clear();
        _vFlatPt.clear();_vTriangle.clear();}
    void SetHorizontal(const std::vector<std::vector<GridArea::LinePnt>>&h){
        _vHorLine = h;}
    void SetVertical(const std::vector<std::vector<GridArea::LinePnt>>&v){
        _vVerLine = v;}
    void SetRectangle(const 
std::vector<std::vector<GeomUnitStruct::Rectangle>>&r){
        _vArrRec = r;}
    void IniTriIndex(){
        for (int i = 0; i < _vTriangle.size(); ++i) {
            _vTriangle[i]._id = i;}
    }
    Triangle IndexTriangle(int i) const{return _vTriangle[i];}
    std::vector<std::vector<GridArea::LinePnt>> Horizontal() const{
        return _vHorLine;}
    std::vector<std::vector<GridArea::LinePnt>> Vertical() const{
        return _vVerLine;}
    std::vector<std::vector<GeomUnitStruct::Rectangle>> Rectangle() const{
        return _vArrRec;}
    void CalTriangleAngle(){
        for(auto& d : _vTriangle){d.CalculateAngel();}
    }
    void IniTrisIndex(){
        for (int i = 0; i < _vTriangle.size(); ++i) {
            _vTriangle[i]._id = i;}
    }
    /**
    vArrHor.size() = vArrVer.size()+1;
    vArrHor[0].size() = vArrVer[0].size()-1;
    vArrRec.size() = vArrHor.size()-1 = vArrVer.size();
    vArrRec[0].size() = vArrHor[0].size() = vArrVer[0].size()-1;*/
public:
    TopoDS_Shape _modShape;
    std::vector<std::vector<GridArea::LinePnt>>_vHorLine;
    std::vector<std::vector<GridArea::LinePnt>>_vVerLine;
    std::vector<std::vector<GeomUnitStruct::Rectangle>>_vArrRec;
    std::vector<OftStr::Point>_vModBorPt,_vBorderPt;
    std::vector<OftStr::Point>_vModStePt,_vSteepPt;
    std::vector<OftStr::Point>_vModFlaPt,_vFlatPt;
    std::vector<Triangle>_vTriangle;
    vector<OftStr::DefLoop>_vModBorLoop;
    vector<OftStr::GeomArea>_vModBorArea;
    vector<OftStr::GeomArea>_vBorArea;
    std::vector<OftStr::DefLoop>_vLineLoop;
    std::vector<OftStr::DefLoop>_vLineLoopZ;
    vector<DefEdge>_vModEdge;
    TrianRes _triangles;
};
class IntPnt{
public:
     IntPnt(){}
     IntPnt(int e,bool isEndpt,const OftStr::Point& p) :        _eId(e),_isEndpt(isEndpt),_p(p){}
     IntPnt(int e,int line,bool isEndpt,const OftStr::Point& p) :
        _eId(e),_lineId(line),_isEndpt(isEndpt),_p(p){}
     IntPnt(int lId,int e,int line,bool isEndpt,const OftStr::Point& p) :
        _lId(lId),_eId(e),_lineId(line),_isEndpt(isEndpt),_p(p){}
     OftStr::Point P() const{return _p;}
public:
    int _id = -1;              /// 交点索引
    int _eId = -1;             /// 交点所在曲线元素索引
    int _lId = -1;             /// 交点所在环索引
    int _lineId = -1;          /// 交点所在扫掠线索引
    bool _isEndpt = false;     /// 交点所在曲线元素索引
    bool _hasVisit = false;    /// 交点是否已经访问
    OftStr::Point _p;          /// 交点
    IntPnt *pre = nullptr;     /// 交点所在曲线相对该交点的前一个交点
    IntPnt *nex = nullptr;     /// 交点所在曲线相对该交点的后一个交点
    IntPnt *cnt = nullptr;     /// 与交点在同一扫掠线上的连接点
};
class IntLine{
public:
    IntLine(){}
    void CalExistValidPt(){
        for(auto& d : _vIntPt){if(!d->_hasVisit){
                _hasVisit =  true;}}
    }
    void CalExistEndpoint(){
        for(auto& d : _vIntPt){
            if(d->_isEndpt){_isExiEndpt = true;break;}}
    }
    int Index() const{return _id;}
    IntPnt* IdPt(int i) const{return _vIntPt[i];}
    bool IsValid() const{return _hasVisit;}
    std::vector<IntPnt*> Points() const{return _vIntPt;}
    bool IsExistEndpoint() const{return _isExiEndpt;}
public:
    int _id = -1;
    bool _hasVisit = false;
    bool _isExiEndpt = false;
    std::vector<IntPnt*>_vIntPt;
};
class IntStruct{
public:
    IntStruct(){}
public:
    std::vector<IntPnt>_bndPts;
    std::vector<IntLine>_bndIntLs;
    std::vector<std::vector<IntPnt>>_islPts;
    std::vector<std::vector<IntLine>>_islIntLs;
};
///*************************Triangle*************************
///*************************Triangle*************************}
namespace GeomUnit = GeomUnitStruct;
#endif // GEOMUNITSTRUCT_H