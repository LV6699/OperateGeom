#ifndef CLSTRUCT_H
#define CLSTRUCT_H

#include "../../OffsetStruct/CommonFile/DataStructure.h"
#include "../../CommonFile/ModelStruct.h"
#include "../../CommonFile/OperaParam.h"
#include "../../ViewWindow/OccHeader.h"

namespace grm{

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
        _nxy = _n;_n.SetZ(0);_nxy.Normalize();
    }
    const oft::Point& P0()const{return _p0;}
    const oft::Point& P1()const{return _p1;}
    const oft::Point& P2()const{return _p2;}
    const oft::Point& N()const{return _n;}
    const oft::Point& Nxy()const{return _nxy;}
    double MinX()const{
        double x = std::min(_p0.X(),_p1.X());
        return x <= _p2.X() ? x : _p2.X();
    }
    double MaxX()const{
        double x = std::max(_p0.X(),_p1.X());
        return x >= _p2.X() ? x : _p2.X();
    }
    double MinY()const{
        double y = std::min(_p0.Y(),_p1.Y());
        return y <= _p2.Y() ? y : _p2.Y();
    }
    double MaxY()const{
        double x = std::max(_p0.X(),_p1.X());
        return x >= _p2.X() ? x : _p2.X();
    }
    bool IsInXYRange(const oft::Point& p)const{
        return MinX() <= p.X() && MaxX() >= p.X() &&
                MinY() <= p.Y() && MaxY() >= p.Y();
    }

public:
    oft::Point _p0,_p1,_p2;
    oft::Point _n,_nxy;

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
    void IniTrisNor(){
        for(auto& t : _tris){t.CalNormal();t.CalNorProj();}
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
