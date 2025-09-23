#include<iostream>
#include "ClStruct.h"

#pragma optimize("", off)

namespace grm{

void grm::MeshMap::InitialEdge()
{
    _xEdges.reserve(_clPts.size());
    for(auto& d : _clPts){
        std::vector<ClEdge>v;
        v.reserve(d.size());
        for(size_t i = 0;i < d.size()-1;++i){
            v.emplace_back(ClEdge(d[i],d[i+1]));}
        _xEdges.emplace_back(v);
    }
}

void grm::MeshMap::CreateModelGrid(double step)
{
    auto& m = _model;
    m.BoundaryPoint();
    double x1 = m.MaxXPt().X(),y1 = m.MaxYPt().Y();
    double cy = m.MinYPt().Y();
    while (cy < y1) {
        double cx = m.MinXPt().X();
        vector<oft::Point>xs;
        while (cx < x1) {
            xs.emplace_back(oft::Point(cx,cy));
            cx += step;
        }
        _clPts.emplace_back(xs);
        cy += step;
    }
}

void grm::MeshMap::IniTriangles()
{
    auto& vs = _triRes._vertexs;
    auto& ts = _triRes.Triangles();
    for(auto& d : ts){
        auto& p0 = vs[d[0]],p1 = vs[d[1]],p2 = vs[d[2]];
        _tris.emplace_back(Triangle(p0,p1,p2));
    }
}



}


























