#ifndef WIDGESTRUCT_H
#define WIDGESTRUCT_H
#include"../HearderFile.h"
using std::vector;
#include <algorithm>
#include<cmath>
#include <memory>
#include <vector>
class BndIslGeoShape
{
public:
    BndIslGeoShape() {}
    BndIslGeoShape(vector<Handle(AIS_Shape)>bndSha,
                   vector<vector<Handle(AIS_Shape)>>islSha) :
        m_vBndShape(bndSha),m_vIslShape(islSha){}
    vector<Handle(AIS_Shape)> m_vBndShape;
    vector<vector<Handle(AIS_Shape)>>m_vIslShape;
};
class LoopGeoShape
{
public:
    LoopGeoShape() {}
    LoopGeoShape(vector<Handle(AIS_Shape)> vLoopSha) :
        m_vShape(vLoopSha){}
    vector<Handle(AIS_Shape)> m_vShape;
};
class NodeGeoShape
{
public:
    NodeGeoShape() {}
    NodeGeoShape(vector<LoopGeoShape>vNodSha) : m_vLoopShape(vNodSha){}
    vector<LoopGeoShape>m_vLoopShape;
};
class ToolPathShape
{
public:
    ToolPathShape() {}
    ToolPathShape(BndIslGeoShape vProfSha,vector<NodeGeoShape>vNodeSha) :
    m_profShape(vProfSha),m_vNodeShape(vNodeSha){}
    BndIslGeoShape m_profShape;
    vector<NodeGeoShape> m_vNodeShape;
    void ClearShape()
    {
        m_profShape.m_vBndShape.clear();
        m_profShape.m_vIslShape.clear();
        m_vNodeShape.clear();
    }
};
#endif // WIDGESTRUCT_H