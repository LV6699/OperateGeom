#ifndef OCCHEADER_H
#define OCCHEADER_H

#include <QtWidgets/QMenu>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QRubberBand>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QMenu>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QRubberBand>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMenu>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QRubberBand>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QWidget>
#include <QtCore/QDebug>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFileDialog>
//***************************occ*******************************

#include <gp_Pnt.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_ViewCube.hxx>
#include <V3d_View.hxx>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Prs3d_DatumAspect.hxx>
#include <Graphic3d_MaterialAspect.hxx>
#include <WNT_Window.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <AIS_ViewCube.hxx>
#include <V3d_View.hxx>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Prs3d_DatumAspect.hxx>
#include <Graphic3d_MaterialAspect.hxx>
#include <WNT_Window.hxx>
#include <Xw_Window.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>
#include <BRepBndLib.hxx>
//#include <X11/Xlib.h>
//#include <X11/Xatom.h>
#include <iostream>
#include <list>
#include <stdlib.h>
//#include <unistd.h>
#include <OSD_Environment.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Xw_Window.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <AIS_ViewCube.hxx>
#include <V3d_View.hxx>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Prs3d_DatumAspect.hxx>
#include <Graphic3d_MaterialAspect.hxx>
#include <WNT_Window.hxx>
#include"OcctWindow.h"
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>
#include <gp_Lin2d.hxx>
#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <TopoDS.hxx>
#include <TopTools.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TColgp_Array1OfPnt2d.hxx>
#include <BRepLib.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <AIS_Shape.hxx>
#include <IGESControl_Reader.hxx>
#include <STEPControl_Reader.hxx>
#include <Draw_Circle2D.hxx>
#include<BRepBuilderAPI_MakeEdge.hxx>
#include<GCE2d_MakeLine.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_Wire.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <Geom_Plane.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <BRepLib.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include<Geom_Line.hxx>
#include<Geom_Circle.hxx>
#include<Geom_Ellipse.hxx>
#include<Geom_Parabola.hxx>
#include<Geom_Hyperbola.hxx>
#include<Geom_BezierCurve.hxx>
#include<Geom_BSplineCurve.hxx>
#include<Geom_TrimmedCurve.hxx>
#include<Geom_OffsetCurve.hxx>
#include<TColgp_Array1OfPnt.hxx>
#include<TColStd_Array1OfReal.hxx>
#include<TColStd_Array1OfInteger.hxx>
#include<GeomTools.hxx>
#include<GeomTools_CurveSet.hxx>
#include<StlAPI_Reader.hxx>
#include <RWStl.hxx>
#include<Geom2d_Circle.hxx>
#include<STEPCAFControl_ActorWrite.hxx>
#include<STEPCAFControl_Controller.hxx>
#include<STEPControl_Writer.hxx>
#include <BRep_Tool.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Graphic3d_ArrayOfTriangles.hxx>
#include <TopExp_Explorer.hxx>
#include <Poly_Triangulation.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <AIS_TextLabel.hxx>

#endif // OCCHEADER_H






