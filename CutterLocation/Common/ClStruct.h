#ifndef CLSTRUCT_H
#define CLSTRUCT_H

//#include "../../CommonFile/ModelStruct.h"
#include "ToolParam.h"
#include "OperateItem.h"
#include "PureNumStruct.h"

#pragma optimize("", off)

namespace grm {
struct TId {
    TId() {}
    size_t _id = -1;
    size_t _oid = -1;
};

class LimVal {
public:
    LimVal() {}
    static double ThreeMin(double v1, double v2, double v3) {
        return std::min(v1, v2) < v3 ? std::min(v1, v2) : v3;
    }
    static double ThreeMax(double v1, double v2, double v3) {
        return std::max(v1, v2) > v3 ? std::max(v1, v2) : v3;
    }
    LimVal(double x, double x1, double y, double y1) {
        if (x < x1) {
            _minx = x;
            _maxx = x1;
        } else {
            _minx = x1;
            _maxx = x;
        }
        if (y < y1) {
            _miny = y;
            _maxy = y1;
        } else {
            _miny = y1;
            _maxy = y;
        }
    }
    double _minx, _maxx, _miny, _maxy,_maxz;
};
class Triangle {
public:
    Triangle() {}
    Triangle(const ofts::Point &p0, const ofts::Point &p1, const ofts::Point &p2)
        : _p0(p0), _p1(p1), _p2(p2) {}

    void CalNormal() {
        auto v1 = _p1 - _p0, v2 = _p2 - _p0;
        _n = v1.Cross(v2);
        _n.Normalize();
    }
    void CalNorProj() {
        _nxy = _n;
        _nxy.SetZ(0);
        _nxy.Normalize();
    }
    void IniOrigin(const Triangle &t) {
        _ot = std::make_shared<Triangle>(t._p0, t._p1, t._p2);
        _ot->SetOriId(t.Id());
    }
    void IniLimtVal() {
        double x = LimVal::ThreeMin(_p0.X(), _p1.X(), _p2.X());
        double x1 = LimVal::ThreeMax(_p0.X(), _p1.X(), _p2.X());
        double y = LimVal::ThreeMin(_p0.Y(), _p1.Y(), _p2.Y());
        double y1 = LimVal::ThreeMax(_p0.Y(), _p1.Y(), _p2.Y());
        double z1 = LimVal::ThreeMax(_p0.Z(), _p1.Z(), _p2.Z());
        _limVal._minx = x;
        _limVal._maxx = x1;
        _limVal._miny = y;
        _limVal._maxy = y1;
        _limVal._maxz = z1;
        _eLim0 = LimVal(_p0.X(), _p1.X(), _p0.Y(), _p1.Y());
        _eLim1 = LimVal(_p1.X(), _p2.X(), _p1.Y(), _p2.Y());
        _eLim2 = LimVal(_p2.X(), _p0.X(), _p2.Y(), _p0.Y());
    }
    void SetId(size_t i) { _id._id = i; }
    void SetOriId(size_t i) { _id._oid = i; }

    const ofts::Point &P0() const { return _p0; }
    const ofts::Point &P1() const { return _p1; }
    const ofts::Point &P2() const { return _p2; }
    ofts::Point Centroid() const {
        return ofts::Point((_p0.X() + _p1.X() + _p2.X()) / 3,
                           (_p0.Y() + _p1.Y() + _p2.Y()) / 3,
                           (_p0.Z() + _p1.Z() + _p2.Z()) / 3);
    }
    const ofts::Point &N() const { return _n; }
    const ofts::Point &Nxy() const { return _nxy; }
    double MinX() const { return _limVal._minx; }
    double MaxX() const { return _limVal._maxx; }
    double MinY() const { return _limVal._miny; }
    double MaxY() const { return _limVal._maxy; }
    bool IsVertical(double e = PreErr_8) const {
        return std::abs(_p0.Z() - _p1.Z()) < e && std::abs(_p0.Z() - _p2.Z()) < e;
    }
    bool IsInRange(const ofts::Point &p, double e = PreErr_12) const {
        /// auto x = MinX(),x1 = MaxX(),y = MinY(),y1 = MaxY();
        if (MinX() > p.X() || MaxX() < p.X() || MinY() > p.Y() || MaxY() < p.Y()) {
            return false;
        }
        double denom = ((_p1.Y() - _p2.Y()) * (_p0.X() - _p2.X()) +
                        (_p2.X() - _p1.X()) * (_p0.Y() - _p2.Y()));
        if (std::abs(denom) < e) {
            return false;
        }
        double alpha = ((_p1.Y() - _p2.Y()) * (p.X() - _p2.X()) +
                        (_p2.X() - _p1.X()) * (p.Y() - _p2.Y())) /
                denom;
        double beta = ((_p2.Y() - _p0.Y()) * (p.X() - _p2.X()) +
                       (_p0.X() - _p2.X()) * (p.Y() - _p2.Y())) /
                denom;
        double gamma = 1.0 - alpha - beta;
        return (alpha >= -e) && (beta >= -e) && (gamma >= -e);
    }
    bool IsVertex(const ofts::Point &p, double e = PreErr_8) const {
        return p.IsSameCoord3D(_p0, e) || p.IsSameCoord3D(_p1, e) ||
                p.IsSameCoord3D(_p2, e);
    }
    bool IsInE0Range(const ofts::Point &p, double e) const {
        return pnum::IsInRange(p.X(), _eLim0._minx, _eLim0._maxx, e) &&
                pnum::IsInRange(p.Y(), _eLim0._miny, _eLim0._maxy, e);
    }
    bool IsInE1Range(const ofts::Point &p, double e) const {
        return pnum::IsInRange(p.X(), _eLim1._minx, _eLim1._maxx, e) &&
                pnum::IsInRange(p.Y(), _eLim1._miny, _eLim1._maxy, e);
    }
    bool IsInE2Range(const ofts::Point &p, double e) const {
        return pnum::IsInRange(p.X(), _eLim2._minx, _eLim2._maxx, e) &&
                pnum::IsInRange(p.Y(), _eLim2._miny, _eLim2._maxy, e);
    }
    size_t Id() const { return _id._id; }
    size_t OriId() const { return _id._oid; }

public:
    TId _id;
    LimVal _limVal;
    ofts::Point _n, _nxy;
    ofts::Point _p0, _p1, _p2;
    LimVal _eLim0, _eLim1, _eLim2;
    std::shared_ptr<Triangle> _ot = nullptr;
};

class ProjRes {
public:
    ProjRes() {}
    ProjRes(const ofts::Point &p, double v) : _p(p), _val(v) {}
    const ofts::Point &P() const { return _p; }
    const double Val() const { return _val; }
    void SetProjPt(const ofts::Point &p) { _p = p; }
    void SetVal(double v) { _val = v; }

public:
    ofts::Point _p;
    double _val;
};
class ClEdge {
public:
    ClEdge() {}
    ///~ClEdge(){delete _p0;delete _p1;_p0 = nullptr;_p1 = nullptr;}
    ClEdge(ofts::Point &p0, ofts::Point &p1) : _sp(&p0), _ep(&p1) {}

public:
    ofts::Point *_sp = nullptr;
    ofts::Point *_ep = nullptr;
};

class TEdge {
public:
    TEdge() {}
    TEdge(const ofts::Point &p0, const ofts::Point &p1){
        if (p0.Z() < p1.Z()) {_p0 = p0;_p1 = p1;}
        else {_p0 = p1;_p1 = p0;}
    }
    TEdge(const ofts::Point &p0, const ofts::Point &p1, size_t tid, size_t eid)
        : _tid(tid), _eid(eid) {
        if (p0.Z() < p1.Z()) {_p0 = p0;_p1 = p1;}
        else {_p0 = p1;_p1 = p0;}
    }
    void SetP0(const ofts::Point &p){_p0 = p;}
    void SetP1(const ofts::Point &p){_p1 = p;}
    const ofts::Point& P0() const {return _p0;}
    const ofts::Point& P1() const {return _p1;}
    size_t TId()const{return _tid;}
    size_t EId()const{return _tid;}

public:
    ofts::Point _p0;
    ofts::Point _p1;
    size_t _tid = -1;  ///所在三角形的id
    size_t _eid = -1;  ///三角形中边的id(0-1:0,1-2:1,2-0:2)
};

class TVert : public ofts::Point{
public:
    TVert(double x, double y, double z, size_t tid, size_t pid)
        : ofts::Point(x, y, z), _tid(tid), _pid(pid) {}
    TVert(const ofts::Point& p, size_t tid, size_t pid)
        : ofts::Point(p.X(), p.Y(), p.Z()), _tid(tid), _pid(pid) {}
    TVert(double x, double y, double z) : ofts::Point(x, y, z) {}
    size_t _tid = -1;  /// 所在三角形的id
    size_t _pid = -1;  /// 三角形中顶点的id(0-1:0,1-2:1,2-0:2)
};

class MeshMap {
public:
    MeshMap() {}
    void InitialEdge();
    void IniTriangles();
    void CreateModelGrid(double step);
    void IniTrisLimVal(bool iscl) {
        if (iscl) {for (auto &t : _trisCl) {t.IniLimtVal();}}
        else {for (auto &t : _tris) {t.IniLimtVal();}}
    }
    void IniTrisNor() { /**
        for(size_t i = 0;i < _tris.size();++i){
            auto& t = _tris[i];
            if(i == 408){
                int tem = 1;
            }
            t.CalNormal();t.CalNorProj();
            int tem1 = 1;
        }*/
        for (auto &t : _tris) {t.CalNormal();t.CalNorProj();}
    }
    void IniClTrisNor() { /**
        for(size_t i = 0;i < _trisCl.size();++i){
            if(i == 588){
                int tem = 0;
            }
            _trisCl[i].CalNormal();_trisCl[i].CalNorProj();
        }*/
        for (auto &t : _trisCl) {t.CalNormal();}
    }
    void IniTrisId(bool iscl) {
        if (iscl) {
            for (size_t i = 0; i < _trisCl.size(); ++i) {_trisCl[i].SetId(i);}}
        else {for (size_t i = 0; i < _tris.size(); ++i) {_tris[i].SetId(i);}}
    }
    void IniTVerts(){
        _tedges.reserve(_tris.size() * 3);
        for (size_t i = 0; i < _tris.size(); i++) {
            const auto& t = _tris[i];
            _tedges.emplace_back(TEdge(t._p0, t._p1, i, 0));
            _tedges.emplace_back(TEdge(t._p1, t._p2, i, 1));
            _tedges.emplace_back(TEdge(t._p2, t._p0, i, 2));
        }
    }
    void IniTEdges(){
        _tverts.reserve(_tris.size() * 3);
        for (size_t i = 0; i < _tris.size(); i++) {
            const auto& t = _tris[i];
            _tverts.emplace_back(TVert(t._p0, i, 0));
            _tverts.emplace_back(TVert(t._p1, i, 1));
            _tverts.emplace_back(TVert(t._p2, i, 2));
        }
    }
    const ClModel& ModelData() const{return _model;}
    const DefTool &Tool() const { return _tool; }
    const TrianRes &TriangleRes() const { return _triRes; }
    const std::vector<Triangle> &Triangles() const { return _tris; }
    const std::vector<Triangle> &TrianglesCl() const { return _trisCl; }
    const TopoDS_Shape &Shape() const { return _model.Shape(); }
    const std::vector<std::vector<ofts::Point>> &ClPoints() const {
        return _clPts;
    }
    const std::vector<std::vector<ClEdge>> &ClEdgesX() const { return _xEdges; }
    const std::vector<std::vector<ClEdge>> &ClEdgesY() const { return _yEdges; }
    const std::vector<TVert>& GetTVerts()const{return _tverts;}
    const std::vector<TEdge>& GetTEdges()const{return _tedges;}

public:
    DefTool _tool;
    ClModel _model;
    TrianRes _triRes;
    std::vector<TEdge> _tedges;
    std::vector<TVert> _tverts;
    std::vector<Triangle> _tris, _trisCl;
    std::vector<std::vector<double>> _clgrid;
    std::vector<std::vector<ofts::Point>> _clPts;
    std::vector<std::vector<ClEdge>> _xEdges, _yEdges;
};

enum class ClRelType { OftTriangle = 0, PtProtect, EdgeProtecct };
class ClRelItem {
public:
    ClRelItem() {}
    ClRelItem(ClRelType ty, const Triangle &t, size_t i, double z)
        : _type(ty), _t(t), _id(i), _z(z) {}
    ClRelItem(ClRelType ty, const ofts::Point &p, size_t i, double z)
        : _type(ty), _pt(p), _id(i), _z(z) {}
    ClRelItem(ClRelType ty, const ofts::Point &sp, const ofts::Point &ep, size_t i,
              double z)
        : _type(ty), _sp(sp), _ep(ep), _id(i), _z(z) {}
    const Triangle &Tria() const { return _t; }
    const ofts::Point &P() const { return _pt; }
    const ofts::Point &SP() const { return _sp; }
    const ofts::Point &EP() const { return _ep; }
    const ClRelType &Type() const { return _type; }
    const size_t &Id() const { return _id; }
    const double &Z() const { return _z; }
    const ViewObj::ViewItem &ViewObj() const { return _vieItem; }

public:
    Triangle _t;     /// 偏置三角形
    ofts::Point _pt;  /// 计算点保护面的点
    ofts::Point _sp;  /// 计算边保护面的边起点
    ofts::Point _ep;  /// 计算边保护面的边终点
    ClRelType _type; /// 刀位点来源类型
    size_t _id = -1;
    double _z = Min_Val;
    ViewObj::ViewItem _vieItem;
};

} // namespace grm
extern grm::MeshMap _meshMap;
extern std::vector<grm::ClRelItem> _clRelItems;
extern std::vector<Quantity_Color> _colors;

#endif // CLSTRUCT_H
