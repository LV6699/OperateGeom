#ifndef CLSTRUCT_H
#define CLSTRUCT_H

#include "PureNumStruct.h"
#include "OperateItem.h"
#include "../../CommonFile/ModelStruct.h"

namespace grm{
struct  TId{
    TId(){}
    size_t _id = -1;
    size_t _oid = -1;
};

class LimVal{
public:
    LimVal(){}
    static double ThreeMin(double v1,double v2,double v3){
        return std::min(v1,v2) < v3 ? std::min(v1,v2) : v3;
    }
    static double ThreeMax(double v1,double v2,double v3){
        return std::max(v1,v2) > v3 ? std::max(v1,v2) : v3;
    }
    LimVal(double x,double x1,double y,double y1){
        if(x < x1){_minx = x;_maxx = x1;}
        else{_minx = x1;_maxx = x;}
        if(y < y1){_miny = y;_maxy = y1;}
        else{_miny = y1;_maxy = y;}
    }
    double _minx,_maxx,_miny,_maxy;
};
class Triangle{
public:
    Triangle(){}
    Triangle(const oft::Point& p0,const oft::Point& p1,
             const oft::Point& p2):_p0(p0),_p1(p1),_p2(p2){}

    void CalNormal(){
        auto v1 = _p1 - _p0,v2 = _p2 - _p0;
        _n = v1.Cross(v2);
        _n = _n.Normalized();
    }
    void CalNorProj(){
        _nxy = _n;
        _nxy.SetZ(0);
        _nxy.Normalize();///_nxy.Normalize();
    }
    void IniOrigin(const Triangle& t){
        _ot = std::make_shared<Triangle>(t._p0,t._p1,t._p2);
        _ot->SetOriId(t.Id());
    }
    void IniLimtVal(){
        double x = LimVal::ThreeMin(_p0.X(),_p1.X(),_p2.X());
        double x1 = LimVal::ThreeMax(_p0.X(),_p1.X(),_p2.X());
        double y = LimVal::ThreeMin(_p0.Y(),_p1.Y(),_p2.Y());
        double y1 = LimVal::ThreeMax(_p0.Y(),_p1.Y(),_p2.Y());
        _limVal._minx = x;_limVal._maxx = x1;
        _limVal._miny = y;_limVal._maxy = y1;
        _eLim0 = LimVal(_p0.X(),_p1.X(),_p0.Y(),_p1.Y());
        _eLim1 = LimVal(_p1.X(),_p2.X(),_p1.Y(),_p2.Y());
        _eLim2 = LimVal(_p2.X(),_p0.X(),_p2.Y(),_p0.Y());
    }
    void SetId(size_t i){_id._id = i;}
    void SetOriId(size_t i){_id._oid = i;}

    const oft::Point& P0()const{return _p0;}
    const oft::Point& P1()const{return _p1;}
    const oft::Point& P2()const{return _p2;}
    oft::Point Centroid()const{
        return oft::Point((_p0.X()+_p1.X()+_p2.X())/3,
                          (_p0.Y()+_p1.Y()+_p2.Y())/3,
                          (_p0.Z()+_p1.Z()+_p2.Z())/3);
    }
    const oft::Point& N()const{return _n;}
    const oft::Point& Nxy()const{return _nxy;}
    double MinX()const{return _limVal._minx;}
    double MaxX()const{return _limVal._maxx;}
    double MinY()const{return _limVal._miny;}
    double MaxY()const{return _limVal._maxy;}
    bool IsVertical(double e = PreErr_8)const{
        return std::abs(_p0.Z()-_p1.Z()) < e &&
                std::abs(_p0.Z()-_p2.Z()) < e;
    }
    bool IsInRange(const oft::Point& p,double e = PreErr_12)const{
        ///auto x = MinX(),x1 = MaxX(),y = MinY(),y1 = MaxY();
        if(MinX() > p.X() || MaxX() < p.X() ||
                MinY() > p.Y() || MaxY() < p.Y()){return false;}
        double denom = ((_p1.Y() - _p2.Y()) * (_p0.X() - _p2.X()) +
                        (_p2.X() - _p1.X()) * (_p0.Y() - _p2.Y()));
        if (std::abs(denom) < e) {return false;}
        double alpha = ((_p1.Y() - _p2.Y()) * (p.X() - _p2.X()) +
                        (_p2.X() - _p1.X()) * (p.Y() - _p2.Y())) / denom;
        double beta = ((_p2.Y() - _p0.Y()) * (p.X() - _p2.X()) +
                       (_p0.X() - _p2.X()) * (p.Y() - _p2.Y())) / denom;
        double gamma = 1.0 - alpha - beta;
        return (alpha >= -e) && (beta >= -e) && (gamma >= -e);
    }
    bool IsVertex(const oft::Point& p,double e = PreErr_8)const{
        return p.IsSameCoord3D(_p0,e) || p.IsSameCoord3D(_p1,e) ||
                p.IsSameCoord3D(_p2,e);
    }
    bool IsInE0Range(const oft::Point& p,double e)const{
        return pnum::IsInRange(p.X(),_eLim0._minx,_eLim0._maxx,e) &&
                pnum::IsInRange(p.Y(),_eLim0._miny,_eLim0._maxy,e);
    }
    bool IsInE1Range(const oft::Point& p,double e)const{
        return pnum::IsInRange(p.X(),_eLim1._minx,_eLim1._maxx,e) &&
                pnum::IsInRange(p.Y(),_eLim1._miny,_eLim1._maxy,e);
    }
    bool IsInE2Range(const oft::Point& p,double e)const{
        return pnum::IsInRange(p.X(),_eLim2._minx,_eLim2._maxx,e) &&
                pnum::IsInRange(p.Y(),_eLim2._miny,_eLim2._maxy,e);
    }
    size_t Id()const{return _id._id;}
    size_t OriId()const{return _id._oid;}

public:
    TId _id;
    LimVal _limVal;
    oft::Point _n,_nxy;
    oft::Point _p0,_p1,_p2;
    LimVal _eLim0,_eLim1,_eLim2;
    std::shared_ptr<Triangle>_ot = nullptr;
};

class ProjRes{
public:
    ProjRes(){}
    ProjRes(const oft::Point& p,double v):_p(p),_val(v){}
    const oft::Point& P()const{return _p;}
    const double Val()const{return _val;}
    void SetProjPt(const oft::Point& p){_p = p;}
    void SetVal(double v){_val = v;}
public:
    oft::Point _p;
    double _val;
};
class ClEdge{
public:
    ClEdge(){}
    ///~ClEdge(){delete _p0;delete _p1;_p0 = nullptr;_p1 = nullptr;}
    ClEdge(oft::Point& p0,oft::Point& p1) : _sp(&p0),_ep(&p1) {}
public:
    oft::Point* _sp = nullptr;
    oft::Point* _ep = nullptr;
};

class MeshMap{
public:
    MeshMap(){}
    void InitialEdge();
    void IniTriangles();
    void CreateModelGrid(double step);
    void IniTrisLimVal(bool iscl){
        if(iscl){for(auto& t : _trisCl){t.IniLimtVal();}}
        else{for(auto& t : _tris){t.IniLimtVal();}}
    }
    void IniTrisNor(){/**
        for(size_t i = 0;i < _tris.size();++i){
            auto& t = _tris[i];
            if(i == 408){
                int tem = 1;
            }
            t.CalNormal();t.CalNorProj();
            int tem1 = 1;
        }*/
        for(auto& t : _tris){t.CalNormal();t.CalNorProj();}
    }
    void IniClTrisNor(){/**
        for(size_t i = 0;i < _trisCl.size();++i){
            if(i == 588){
                int tem = 0;
            }
            _trisCl[i].CalNormal();_trisCl[i].CalNorProj();
        }*/
        for(auto& t : _trisCl){t.CalNormal();}
    }
    void IniTrisId(bool iscl){
        if(iscl){for(size_t i = 0;i < _trisCl.size();++i){_trisCl[i].SetId(i);}}
        else{for(size_t i = 0;i < _tris.size();++i){_tris[i].SetId(i);}}
    }
    const DefTool& Tool()const{return _tool;}
    const TrianRes& TriangleRes()const{return _triRes;}
    const std::vector<Triangle>& Triangles()const{return _tris;}
    const std::vector<Triangle>& TrianglesCl()const{return _trisCl;}
    const TopoDS_Shape& Shape()const{return _model.Shape();}
    const std::vector<std::vector<oft::Point>>& ClPoints()const{return _clPts;}
    const std::vector<std::vector<ClEdge>>& ClEdgesX()const{return _xEdges;}
    const std::vector<std::vector<ClEdge>>& ClEdgesY()const{return _yEdges;}


public:
    DefTool _tool;
    ClModel _model;
    TrianRes _triRes;
    std::vector<Triangle>_tris,_trisCl;
    std::vector<std::vector<double>>_clgrid;
    std::vector<std::vector<oft::Point>>_clPts;
    std::vector<std::vector<ClEdge>>_xEdges,_yEdges;


};

enum class ClRelType{
    OftTriangle = 0,
    PtProtect,
    EdgeProtecct
};
class ClRelItem{
public:
    ClRelItem(){}
    ClRelItem(ClRelType ty,const Triangle& t,size_t i,double z) :
        _type(ty),_t(t),_id(i),_z(z){}
    ClRelItem(ClRelType ty,const oft::Point& p,size_t i,double z) :
        _type(ty),_pt(p),_id(i),_z(z){}
    ClRelItem(ClRelType ty,const oft::Point& sp,
              const oft::Point& ep,size_t i,double z) :
        _type(ty),_sp(sp),_ep(ep),_id(i),_z(z){}
    const Triangle& Tria()const{return _t;}
    const oft::Point& P()const{return _pt;}
    const oft::Point& SP()const{return _sp;}
    const oft::Point& EP()const{return _ep;}
    const ClRelType& Type()const{return _type;}
    const size_t& Id()const{return _id;}
    const double& Z()const{return _z;}
    const ViewObj::ViewItem& ViewObj()const{return _vieItem;}

public:
    Triangle _t;     ///偏置三角形
    oft::Point _pt;  ///计算点保护面的点
    oft::Point _sp;  ///计算边保护面的边起点
    oft::Point _ep;  ///计算边保护面的边终点
    ClRelType _type; ///刀位点来源类型
    size_t _id = -1;
    double _z = Min_Val;
    ViewObj::ViewItem _vieItem;

};

}
extern grm::MeshMap _meshMap;
extern std::vector<grm::ClRelItem>_clRelItems;
extern std::vector<Quantity_Color>_colors;

#endif // CLSTRUCT_H





















