#include<iostream>
#include "ClStruct.h"

#pragma optimize("", off)
#pragma GCC optimize ("o0")

namespace grm{

void grm::MeshMap::InitialEdge()
{
    auto& pts = _clPts;
    auto HandleEdges = [](vector<vector<ofts::Point>>& pts,
            std::vector<std::vector<ClEdge>>& es){
        es.reserve(pts.size());
        for(auto& d : pts){
            std::vector<ClEdge>v;
            v.reserve(d.size());
            for(size_t i = 0;i < d.size()-1;++i){
                ClEdge e(d[i],d[i+1]);
                v.emplace_back(e);
            }
            es.emplace_back(v);
        }
    };
    HandleEdges(pts,_xEdges);
    ///return;
    _yEdges.reserve(pts[0].size());

    for(size_t j = 0; j < pts[0].size(); j++) {
        std::vector<ClEdge>v;
        v.reserve(pts.size());
        for(size_t i = 0; i < pts.size()-1; i++) {
            ClEdge e(pts[i][j], pts[i+1][j]);
            v.emplace_back(e);
        }
        _yEdges.emplace_back(v);
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
        vector<ofts::Point>xs;
        while (cx < x1) {
            xs.emplace_back(ofts::Point(cx,cy));
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
    _tris.reserve(ts.size());
    for(auto& d : ts){
        auto& p0 = vs[d[0]],p1 = vs[d[1]],p2 = vs[d[2]];
        _tris.emplace_back(Triangle(p0,p1,p2));
    }
}



}


























