#include "./ViewWindow/MainWindow.h"
#include "CutterLocation/Common/ViewTool.h"
#include "CutterLocation/Solution/CutLocateZ.h"
#include "DiscreteModel/DiscreteModel.h"
#include <QApplication>
#include "CutterLocation/DebugTool/CreateTool.h"
#include "OperateView/DisplayGeom.h"
#include "OctreeNode/OctStruct.h"
#include<TopOpeBRepBuild_Area1dBuilder.hxx>
#include "offset2D/ToolTrajectoryAlg/ProIOInterface.h"
#include "OffsetData/CloseLoopData.h"
#include "ParallelFinishing/LineCutPath/CreateLineCut.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

/*
#include<json.h>

#ifdef emit
#undef emit
#include <tbb/tbb.h>
#define emit Q_EMIT
#else
#include <tbb/tbb.h>
#endif*/

using std::vector;
using namespace grm;
using namespace ViewObj;

MeshMap _meshMap;
grm::ViewItem _idTria;
grm::ViewItem _id_e_ite;
/// vector<Quantity_Color>_colors;
ViewObj::OpeViewItem _opeItem;
vector<grm::ClRelItem> _clRelItems;

#pragma optimize("", off)

void OffsetTest();
void FunTest();
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.resize(1100, 900);
    // string path = "C:/Users/lvd/Desktop/GeomOperate/part/STEP2.stp";
    //string path = "C:/Users/lvd/Desktop/CAD绘图/平行刀路/lineOffset3DSimple2.step";
    string path = "C:/Users/Administrator/Desktop/Deve/part/刀位面案例.step";
    // string path =
    // "C:/Users/lvd/Desktop/CAD绘图/平行刀路/lineOffset3DSimple2.step";
    // string path =
    // "C:/Users/lvd/Desktop/CAD绘图/平行刀路/行切顺序simple1.step";
    _meshMap._model._shape = w.PathMode(path);
    FunTest();
    //OffsetTest();

    return a.exec();
}
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

void DisplayOperItem(grm::ViewItem &item) {
    if (!item._hasInitial) {
        return;
    }
    _mainwind->myOccView->getContext()->Display(item._ashape, false);
    item.SetHasDisplay();
}
void RemoveOperItem(grm::ViewItem &item) {
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
    DefTool tool4(grm::ToolType::TaperEnd, 2, 1);
    DefTool tool5(grm::ToolType::TaperBall, 2, 1);
    tool4._taper_agl = 45;tool4._br = 1;
    tool5._taper_agl = 45;
    _meshMap._tool = tool2;
    auto& tool = _meshMap._tool;

    grm::ToolParam::SetToolBaseParam(2,1,45,_meshMap._tool);
    //grm::ToolParam::SetToolBaseParam(2,1.4142135623731,45,_meshMap._tool);
    auto tshape = CreateTool().GetToolShape(ofts::Point(0,0,0),tool);
    tool._item = grm::ViewItem(tshape,_colors[4],1);

    //_meshMap._tool._cr2 = 1;
    Quantity_Color c(0.2, 0.3, 0.4, Quantity_TOC_RGB);
    Quantity_Color c1(0.1, 0.1, 0.1, Quantity_TOC_RGB);
#if 0
    auto shape = CreateTool().GetToolShape(ofts::Point(0,0,0),tool);
    DisplayGeom().DisplayShape(shape,_colors[4],1);
    return;
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


void OffsetTest()
{
    ofts::GeomArea region;

#if 0
    ofts::OffsetParam param;
    CloseLoopData().OutputStruct(region);
    DisplayGeom().DisplayGeomArea(region);
    param._offsetUnit.SetFirstOffsetValue(2);
    param._offsetUnit.SetOffsetValue(2);
    param._offsetUnit.SetToolRadius(10);
    std::shared_ptr<ofts::OffsetNode> pathNode;
    ProIOInterface().InPutOutPutIO(region,param,pathNode);
    DisplayGeom().DisplayPathNode(pathNode,false,true);
#else
    std::vector<DefLoop>loops;
    //SingleDirection,DoubleDirection,AloneContour
    ofts::ParalParam param1(4,45,AloneContour);
    CloseLoopData().ParallelData(region);
    DisplayGeom().DisplayLoop(region.BLoop(),_colors[8],1,true);
    for (size_t i = 0; i < region.ILoops().size(); i++){
      DisplayGeom().DisplayLoop(region._vIslLoop[i],_colors[8],1.5,true);
    }
    //DisplayGeom().DisplayGeomArea(region);
    CreateLineCut().GeomAreaLineCutPath(region,param1,loops);

    size_t cid = 0;
    for (size_t i = 0; i < loops.size(); i++){
        if(cid > _colors.size() - 1){cid = 0;}
        DisplayGeom().DisplayLoop(loops[i],_colors[cid++],1.5);
        std::cout<<"";
    }
#endif

}






















