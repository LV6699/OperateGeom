//
// Created by jlc on 2024/7/2.
//
#ifndef SLICES_SLICE_H
#define SLICES_SLICE_H/*
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopoDS.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <vector>
#include <gp_Pnt.hxx>
namespace slice{
class Edge
{
public:
    Edge(){}
    TopoDS_Shape edge;
    GeomAbs_CurveType type;
    gp_Pnt startPnt, midPnt, endPnt;
    int a, b;
};
//获得指定的环路径
std::vector<std::vector<Edge>> GetCurrentLevelLoop(TopoDS_Shape shape, double level);
}*/
#endif //SLICES_SLICE_H