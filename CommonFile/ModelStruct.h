#ifndef MODELSTRUCT_H
#define MODELSTRUCT_H

#include "TranStruct.h"
//#include "DataStructure.h"
//#include "../ViewWindow/OccHeader.h"

namespace grm{

//oft::Point ToPt(const gp_Pnt& p){return oft::Point(p.X(),p.Y(),p.Z());}
//gp_Pnt ToOccPt(const oft::Point& p){return gp_Pnt(p.X(),p.Y(),p.Z());}

class TrianRes {
public:
    TrianRes(){}
    TrianRes(const std::vector<oft::Point>& vs,
             const std::vector<std::array<int, 3>>& ts) :
        _vertexs(vs),_triangles(ts){}
    const std::vector<oft::Point>&Vertexs() const{return _vertexs;}
    const std::vector<std::array<int, 3>>&Triangles() const{return _triangles;}
public:
    std::vector<oft::Point> _vertexs;
    std::vector<std::array<int, 3>> _triangles;

};
/// 顶点比较函数对象
struct VertexHash {
    std::size_t operator()(const oft::Point& p) const {
        std::size_t h1 = std::hash<double>()(p.X());
        std::size_t h2 = std::hash<double>()(p.Y());
        std::size_t h3 = std::hash<double>()(p.Z());
        ///return h1 ^ (h2 << 1) ^ (h3 << 2);
        return h1 + 0x9e3779b9 + (h2 << 6) +
                (h2 >> 2) + (h3 << 13) + (h3 >> 7);
    }
};
struct VertexEqual {
    bool operator()(const oft::Point& p1, const oft::Point& p2) const {
        return p1.DistSquare3D(p2) < 1e-12; // 使用容差比较
    }
};

class ClModel{
public:
    ClModel(){}
    ClModel(const TopoDS_Shape& s) : _shape(s){}

    void BoundaryPoint(){
        if(_shape.IsNull()) return;
        bool first = true;
        TopExp_Explorer explorer(_shape, TopAbs_VERTEX);
        while (explorer.More()) {
            TopoDS_Vertex vertex = TopoDS::Vertex(explorer.Current());
            auto p = oft::Point(BRep_Tool::Pnt(vertex).X(),
                                BRep_Tool::Pnt(vertex).Y(),BRep_Tool::Pnt(vertex).Z());
            if(first){
                _minxp = p;_maxxp = p;_minyp = p;_maxyp = p;_minzp = p;_maxzp = p;
                first = false;
            }else{
                if (p.X() < _minxp.X()){_minxp = p;}if(p.X() > _maxxp.X()){_maxxp = p;}
                if (p.Y() < _minyp.Y()){_minyp = p;}if(p.Y() > _maxyp.Y()){_maxyp = p;}
                if (p.Z() < _minzp.Z()){_minzp = p;}if(p.Z() > _maxzp.Z()){_maxzp = p;}
            }
            explorer.Next();
        }
        if(first){throw std::runtime_error("模型中未找到任何顶点");}
    }
    const TopoDS_Shape& Shape()const{return _shape;}
    double XLen()const{return _maxxp.X() - _minxp.X();}
    double YLen()const{return _maxyp.Y() - _minyp.Y();}
    double ZLen()const{return _maxzp.Z() - _minzp.Z();}
    const oft::Point& MinXPt()const{return _minxp;}
    const oft::Point& MaxXPt()const{return _maxxp;}
    const oft::Point& MinYPt()const{return _minyp;}
    const oft::Point& MaxYPt()const{return _maxyp;}
    const oft::Point& MinZPt()const{return _minzp;}
    const oft::Point& MaxZPt()const{return _maxzp;}


public:

    TopoDS_Shape _shape;
    oft::Point _minxp,_maxxp,_minyp,_maxyp,_minzp,_maxzp;

};




}











#endif // MODELSTRUCT_H
