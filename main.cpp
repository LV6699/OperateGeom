#include <QApplication>
#include "./ViewWindow/MainWindow.h"
#include "DiscreteModel/DiscreteModel.h"
#include "CutterLocation/Common/ViewData.h"
#include "CutterLocation/Solution/CutterLocationZ.h"

#include <string>
#include<iostream>
#include <filesystem>
#include <vector>
#include <algorithm>

using namespace std;
using namespace grm;
MeshMap meshMap;

#pragma optimize("", off)

void FunTest();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.resize(1100,900);
    string path = "C:\\Users\\lvd\\Desktop\\GeomOperate\\part\\STEP2.stp";
    meshMap._model._shape = w.PathMode(path);
    FunTest();

    return a.exec();

}
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
void FunTest()
{
    CutterLocationZ cl;
    TrianRes result ;
    meshMap._tool = DefTool(4,1);
    Quantity_Color c(0.3,0.35,0.35,Quantity_TOC_RGB);
    Quantity_Color c1(0.1,0.1,0.1,Quantity_TOC_RGB);
    ///DisplayGeom().DisplayShape(meshMap.Shape(),c,1);
    Handle(AIS_Shape) as = DisplayGeom().ShapeToAis(meshMap.Shape(),c,1);
     as->SetTransparency(0.3);
    DisplayGeom().DisplayAShape(as);

    DiscreteModel().ModelTriangulation(meshMap.Shape(),result);
    meshMap._triRes = result;
    std::cout<<"vertexs:"<<result.Vertexs().size()
            <<",triangls:"<<result.Triangles().size()<<endl;

    meshMap.IniTriangles();
    meshMap.IniTrisNor();
    meshMap.CreateModelGrid(5);
    meshMap.InitialEdge();

    OperTriaCl ocl;
    ocl.CalTrianglesCl(meshMap);
    ///DisplayGeom().DisplayTriangles(meshMap._trisCl,_colors[1],1);
    cl.GetPointCl(meshMap);

#if 1
    TopoDS_Shape eshape;
    ViewData::EdgesToShape(meshMap._xEdges,eshape);
    DisplayGeom().DisplayShape(eshape,_colors[2],1);
#endif

    std::cout<<"hhh"<<meshMap._clPts.size()<<","<<meshMap._clPts[0].size()<<endl;

    ///DisplayGeom().DisplayModelDescrete(result,c1);

}























