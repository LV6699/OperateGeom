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
MeshMap _meshMap;

#pragma optimize("", off)

void FunTest();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.resize(1100,900);
    string path = "C:\\Users\\lvd\\Desktop\\GeomOperate\\part\\STEP2.stp";
    _meshMap._model._shape = w.PathMode(path);
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
    _meshMap._tool = DefTool(4,1);
    Quantity_Color c(0.3,0.35,0.35,Quantity_TOC_RGB);
    Quantity_Color c1(0.1,0.1,0.1,Quantity_TOC_RGB);
    ///DisplayGeom().DisplayShape(_meshMap.Shape(),c,1);
    Handle(AIS_Shape) as = DisplayGeom().ShapeToAis(_meshMap.Shape(),c,1);
     as->SetTransparency(0.3);
    DisplayGeom().DisplayAShape(as);

    DiscreteModel().ModelTriangulation(_meshMap.Shape(),result);
    _meshMap._triRes = result;
    std::cout<<"vertexs:"<<result.Vertexs().size()
            <<",triangls:"<<result.Triangles().size()<<endl;

    _meshMap.IniTriangles();
    _meshMap.IniTrisNor();
    _meshMap.CreateModelGrid(5);
    _meshMap.InitialEdge();

    OperTriaCl ocl;
    ocl.CalTrianglesCl(_meshMap);
    ///DisplayGeom().DisplayTriangles(_meshMap._trisCl,_colors[1],1);
    cl.CutterLocation(_meshMap);

#if 1
    TopoDS_Shape eshape;
    ViewData::EdgesToShape(_meshMap._xEdges,eshape);
    DisplayGeom().DisplayShape(eshape,_colors[2],1);
#endif

    std::cout<<"hhh"<<_meshMap._clPts.size()<<","<<_meshMap._clPts[0].size()<<endl;

    ///DisplayGeom().DisplayModelDescrete(result,c1);

}























