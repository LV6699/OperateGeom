#include <QApplication>
#include "./ViewWindow/MainWindow.h"
#include "DiscreteModel/DiscreteModel.h"
#include "CutterLocation/Common/ViewTool.h"
#include "CutterLocation/Solution/CutLocateZ.h"

#include <string>
#include<iostream>
#include <filesystem>
#include <vector>
#include <algorithm>

using std::vector;
using namespace grm;
using namespace ViewObj;

MeshMap _meshMap;
ViewObj::ViewItem _idTria;
///vector<Quantity_Color>_colors;
ViewObj::OpeViewItem _opeItem;
vector<grm::ClRelItem>_clRelItems;


#pragma optimize("", off)

void FunTest();
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.resize(1100, 900);
    ///string path = "C:\\Users\\lvd\\Desktop\\GeomOperate\\part\\STEP2.stp";
    string path = "C:\\Users\\lvd\\Desktop\\CAD绘图\\平行刀路\\lineOffset3DSimple2.step";
    ///string path = "C:\\Users\\lvd\\Desktop\\CAD绘图\\刀位面\\刀位面案例.step";
    ///string path = "C:\\Users\\lvd\\Desktop\\CAD绘图\\平行刀路\\lineOffset3DSimple2.step";
    ///string path = "C:\\Users\\lvd\\Desktop\\CAD绘图\\平行刀路\\行切顺序simple1.step";
    _meshMap._model._shape = w.PathMode(path);
    FunTest();

    return a.exec();

}
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
void DisplayOperItem(ViewObj::ViewItem& item) {
    if(!item._hasInitial) { return; }
    _mainwind->myOccView->getContext()->Display(item._ashape, true);
    item.SetHasDisplay();
}
void RemoveOperItem(ViewObj::ViewItem& item) {
    if(!item._hasInitial) { return; }
    _mainwind->myOccView->getContext()->Erase(item._ashape, true);
    item.SetHasDisplay();
}

void FunTest() {
    CutLocateZ cl;
    TrianRes result;
    _meshMap._tool = DefTool(grm::ToolType::BallNoseEnd, 2, 2);
    _meshMap._tool.CalRR();
    /// PlaneEnd BallNoseEnd
    ///_meshMap._tool._type = grm::ToolType::BallNoseEnd;
    Quantity_Color c(0.2, 0.3, 0.4, Quantity_TOC_RGB);
    Quantity_Color c1(0.1, 0.1, 0.1, Quantity_TOC_RGB);
#if 1
    _opeItem.IniModelItem(_meshMap._model._shape, c, 1);
    ///_opeItem._model._ashape->SetTransparency(0.3);
    DisplayOperItem(_opeItem._model);///return;
#endif
    DiscreteModel().ModelTriangulation(_meshMap.Shape(), result);
    _meshMap._triRes = result;
    std::cout << "vertexs:" << result.Vertexs().size()
        << ",triangls:" << result.Triangles().size() << endl;
    _meshMap.IniTriangles();
    _meshMap.CreateModelGrid(0.5);
    _meshMap.InitialEdge();
    _meshMap.IniTrisId(false);
    _meshMap.IniTrisId(true);
#if 1
    _opeItem.IniTrisItem(ViewTool::TrianglesToShape(
        _meshMap._tris), false, c1, 1);
///DisplayOperItem(_opeItem._modTris);
#endif
    OperTriaCl ocl;
    _meshMap.IniTrisNor();
    ocl.CalTrianglesCl(_meshMap);
    _meshMap.IniClTrisNor();

    _meshMap.IniTrisLimVal(false);
    _meshMap.IniTrisLimVal(true);
#if 1
    _opeItem.IniTrisItem(ViewTool::TrianglesToShape(
        _meshMap._trisCl), true, _colors[1], 1);
///DisplayOperItem(_opeItem._trisCl);
#endif
    cl.CutterLocation(_meshMap);

#if 1
    _opeItem.IniEdgesItem(ViewTool::EdgesToShape(_meshMap._xEdges),
        true, _colors[2], 0.5);
    _opeItem.IniEdgesItem(ViewTool::EdgesToShape(_meshMap._yEdges),
        false, _colors[2], 0.5);
    DisplayOperItem(_opeItem._xEdges);
#endif
#if 1
    _opeItem.IniPointsItem(ViewTool::PointsToShape(_meshMap._clPts), _colors[4]);
#endif

    std::cout << "hhh" << _meshMap._clPts.size() << "," << _meshMap._clPts[0].size() << endl;

    ///DisplayGeom().DisplayModelDescrete(result,c1);

}























