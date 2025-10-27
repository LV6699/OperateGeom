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
using namespace ViewObj;

MeshMap _meshMap;
OpeViewItem _opeItem;

#pragma optimize("", off)

void FunTest();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.resize(1100,900);
    ///string path = "C:\\Users\\lvd\\Desktop\\GeomOperate\\part\\STEP2.stp";
    string path = "C:\\Users\\lvd\\Desktop\\CAD绘图\\平行刀路\\lineOffset3DSimple2.step";
    _meshMap._model._shape = w.PathMode(path);
    FunTest();

    return a.exec();

}
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
void DisplayOperItem(ViewObj::ViewItem& item)
{
    if(!item._hasInitial){return;}
    _mainwind->myOccView->getContext()->Display(item._ashape,true);
    item.SetHasDisplay();
}
void RemoveOperItem(ViewObj::ViewItem& item)
{
    if(!item._hasInitial){return;}
    _mainwind->myOccView->getContext()->Erase(item._ashape,true);
    item.SetHasDisplay();
}

void FunTest()
{
    CutterLocationZ cl;
    TrianRes result ;
    _meshMap._tool = DefTool(4,0);
    _meshMap._tool._type = grm::ToolType::PlaneEnd;
    Quantity_Color c(0.3,0.35,0.35,Quantity_TOC_RGB);
    Quantity_Color c1(0.1,0.1,0.1,Quantity_TOC_RGB);
#if 1
    _opeItem.IniModelItem(_meshMap._model._shape,c,1);
    _opeItem._model._ashape->SetTransparency(0.3);
    DisplayOperItem(_opeItem._model);
#endif
    DiscreteModel().ModelTriangulation(_meshMap.Shape(),result);
    _meshMap._triRes = result;
    std::cout<<"vertexs:"<<result.Vertexs().size()
            <<",triangls:"<<result.Triangles().size()<<endl;
    _meshMap.IniTriangles();
    _meshMap.IniTrisNor();
    _meshMap.CreateModelGrid(1);
    _meshMap.InitialEdge();
#if 1
    _opeItem.IniTrisItem(ToOcc::TrianglesToShape(
                             _meshMap._tris),false,c1,1);
    ///DisplayOperItem(_opeItem._modTris);
#endif
    OperTriaCl ocl;
    ocl.CalTrianglesCl(_meshMap);
    _meshMap.IniClTrisNor();
    _meshMap.IniTrisLimVal(true);
#if 1
    _opeItem.IniTrisItem(ToOcc::TrianglesToShape(
                             _meshMap._trisCl),true,_colors[1],1);
    ///DisplayOperItem(_opeItem._trisCl);
#endif
    cl.CutterLocation(_meshMap);

#if 1
    _opeItem.IniEdgesItem(ToOcc::EdgesToShape(_meshMap._xEdges),true,_colors[2],1);
    DisplayOperItem(_opeItem._xEdges);
#endif
#if 1
    _opeItem.IniPointsItem(ToOcc::PointsToShape(_meshMap._clPts),_colors[4]);
#endif

    std::cout<<"hhh"<<_meshMap._clPts.size()<<","<<_meshMap._clPts[0].size()<<endl;

    ///DisplayGeom().DisplayModelDescrete(result,c1);

}























