#ifndef CLSTRUCT_H
#define CLSTRUCT_H

#include "PureNumStruct.h"
#include "../../OffsetStruct/CommonFile/DataStructure.h"
#include "../../CommonFile/ModelStruct.h"
#include "../../CommonFile/OperaParam.h"
#include "../../ViewWindow/OccHeader.h"

namespace grm{

class LimVal{
public:
    LimVal(){}
    static double ThreeMin(double v1,double v2,double v3){
        return std::min(v1,v2) < v3 ? std::min(v1,v2) : v3;
    }
    static double ThreeMax(double v1,double v2,double v3){
        return std::max(v1,v2) > v3 ? std::max(v1,v2) : v3;
    }
    LimVal(double x,double x1,double y,double y1) :
        _minx(x),_maxx(x1),_miny(y),_maxy(y1){}
    double _minx,_maxx,_miny,_maxy;
};
class Triangle{
public:
    Triangle(){}
    Triangle(oft::Point& p0,oft::Point& p1,
             oft::Point& p2):_p0(p0),_p1(p1),_p2(p2){}

    void CalNormal(){
        auto v1 = _p1 - _p0,v2 = _p2 - _p0;
        _n = v1.Cross(v2);
        _n = _n.Normalize();
    }
    void CalNorProj(){
        _nxy = _n;_nxy.SetZ(0);///_nxy.Normalize();
    }
    void IniOrigin(Triangle& t){
        _op0 = &t._p0;_op1 = &t._p1;_op2 = &t._p2;
    }
    void IniLimtVal(){
        double x = LimVal::ThreeMin(_p0.X(),_p1.X(),_p2.X());
        double x1 = LimVal::ThreeMax(_p0.X(),_p1.X(),_p2.X());
        double y = LimVal::ThreeMin(_p0.Y(),_p1.Y(),_p2.Y());
        double y1 = LimVal::ThreeMax(_p0.Y(),_p1.Y(),_p2.Y());
        _limVal = LimVal(x,x1,y,y1);
    }
    const oft::Point& P0()const{return _p0;}
    const oft::Point& P1()const{return _p1;}
    const oft::Point& P2()const{return _p2;}
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

public:
    LimVal _limVal;
    oft::Point _n,_nxy;
    oft::Point _p0,_p1,_p2;
    oft::Point* _op0 = nullptr;
    oft::Point* _op1 = nullptr;
    oft::Point* _op2 = nullptr;

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
            if(i == 997){
                int tem = 1;
            }
            _tris[i].CalNormal();_tris[i].CalNorProj();
        }*/
        for(auto& t : _tris){t.CalNormal();t.CalNorProj();}
    }
    void IniClTrisNor(){
        for(auto& t : _trisCl){t.CalNormal();t.CalNorProj();}/**
        for(size_t i = 0;i < _trisCl.size();++i){
            if(i == 588){
                int tem = 0;
            }
            _trisCl[i].CalNormal();_trisCl[i].CalNorProj();
        }*/
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

}

#endif // CLSTRUCT_H
