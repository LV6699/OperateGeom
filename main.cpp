#include "./ViewWindow/MainWindow.h"
#include "CutterLocation/Common/ViewTool.h"
#include "CutterLocation/Solution/CutLocateZ.h"
#include "DiscreteModel/DiscreteModel.h"
#include <QApplication>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using std::vector;
using namespace grm;
using namespace ViewObj;

MeshMap _meshMap;
ViewObj::ViewItem _idTria;
ViewObj::ViewItem _id_e_ite;
/// vector<Quantity_Color>_colors;
ViewObj::OpeViewItem _opeItem;
vector<grm::ClRelItem> _clRelItems;

#pragma optimize("", off)

void FunTest();
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  w.resize(1100, 900);
  /// string path = "C:\\Users\\lvd\\Desktop\\GeomOperate\\part\\STEP2.stp";
  string path = "C:\\Users\\lvd\\Desktop\\CAD绘图\\平行刀路\\lineOffset3DSimple2.step";
  /// string path = "C:\\Users\\lvd\\Desktop\\CAD绘图\\刀位面\\刀位面案例.step";
  /// string path =
  /// "C:\\Users\\lvd\\Desktop\\CAD绘图\\平行刀路\\lineOffset3DSimple2.step";
  /// string path =
  /// "C:\\Users\\lvd\\Desktop\\CAD绘图\\平行刀路\\行切顺序simple1.step";
  _meshMap._model._shape = w.PathMode(path);
  FunTest();

  return a.exec();
}
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

void DisplayOperItem(ViewObj::ViewItem &item) {
  if (!item._hasInitial) {
    return;
  }
  _mainwind->myOccView->getContext()->Display(item._ashape, true);
  item.SetHasDisplay();
}
void RemoveOperItem(ViewObj::ViewItem &item) {
  if (!item._hasInitial) {
    return;
  }
  _mainwind->myOccView->getContext()->Erase(item._ashape, true);
  item.SetHasDisplay();
}
void FunTest() {
  CutLocateZ cl;
  TrianRes result;
  DefTool tool1(grm::ToolType::PlaneEnd, 2,0);
  DefTool tool2(grm::ToolType::BallNoseEnd, 2, 2);
  DefTool tool3(grm::ToolType::RoundNoseEnd, 2, 1);
  DefTool tool4(grm::ToolType::TaperEnd, 2, 0);
  tool4._taper_agl = 45;
  tool4._br = 1;
  auto& tool = _meshMap._tool;
  tool = tool4;
  grm::ToolParam::SetToolBaseParam(2,1,45,_meshMap._tool);
  _meshMap._tool._rr = 1;
  Quantity_Color c(0.2, 0.3, 0.4, Quantity_TOC_RGB);
  Quantity_Color c1(0.1, 0.1, 0.1, Quantity_TOC_RGB);
#if 1
  ofts::Point op(-41.000000000000000, 7.9999999999999858, 0.0000000000000000);
  ofts::Point p0(-20.000000000000000, 15.391376188718372, 18.000000000000004);
  ofts::Point p1(20.000000000000000, 25.997977906516525, 28.606601717798270);
  ofts::DefSeg seg(p0, p1);

#endif

#if 1
  _opeItem.IniModelItem(_meshMap._model._shape, c, 1);
  ///_opeItem._model._ashape->SetTransparency(0.3);
  DisplayOperItem(_opeItem._model);  //return;
#endif
  DiscreteModel().ModelTriangulation(_meshMap.Shape(), result);
  _meshMap._triRes = result;
  std::cout << "vertexs:" << result.Vertexs().size()
            << ",triangls:" << result.Triangles().size() << endl;
  _meshMap.IniTriangles();
  _meshMap.CreateModelGrid(1);
  _meshMap.InitialEdge();
  _meshMap.IniTrisId(false);
  _meshMap.IniTrisId(true);

  OperTriaCl ocl;
  _meshMap.IniTrisNor();
  ocl.CalTrianglesProtFace(_meshMap);
  _meshMap.IniClTrisNor();

  _meshMap.IniTrisLimVal(false);
  _meshMap.IniTrisLimVal(true);
  _meshMap.IniTVerts();
  _meshMap.IniTEdges();
#if 1
  _opeItem.IniTranglesEdge(ViewTool::EdgesToShape(_meshMap._tedges), c1,1);
  /// DisplayOperItem(_opeItem._modTris);
#endif
#if 1
  _opeItem.IniTrisItem(ViewTool::TrianglesToShape(_meshMap._trisCl), true,
                       _colors[1], 1);
/// DisplayOperItem(_opeItem._trisCl);
#endif
  cl.CutterLocation(_meshMap);

#if 1
  _opeItem.IniEdgesItem(ViewTool::EdgesToShape(_meshMap._xEdges), true,
                        _colors[2], 0.5);
  _opeItem.IniEdgesItem(ViewTool::EdgesToShape(_meshMap._yEdges), false,
                        _colors[2], 0.5);
  DisplayOperItem(_opeItem._xEdges);
#endif
#if 1
  _opeItem.IniPointsItem(ViewTool::PointsToShape(_meshMap._clPts), _colors[4]);
#endif

  std::cout << "hhh" << _meshMap._clPts.size() << ","
            << _meshMap._clPts[0].size() << endl;

  /// DisplayGeom().DisplayModelDescrete(result,c1);
}
