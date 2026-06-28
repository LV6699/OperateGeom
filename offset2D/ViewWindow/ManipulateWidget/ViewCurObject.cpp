#include "ViewCurObject.h"
#include"../DisplayShape/DisplayToolPath.h"
#include"../DisplayShape/DisplayModel.h"
shared_ptr<DataEdit>widvie_dat = make_shared<DataEdit>();
shared_ptr<CutParts>widvie_getpro = make_shared<CutParts>();
shared_ptr<TransToShape>widvie_trans = make_shared<TransToShape>();
Handle(AIS_Shape) m_modelBlank;
Handle(AIS_Shape) m_modelShape;
BndIslGeoShape m_selBndIslShape;
vector<vector<BndIslGeoShape>>m_vModelCutShape;
ToolPathShape m_rootNodeShape;
bool m_hasSetModel = false,m_hasSetBlank = false,
m_hasSetSelObj = false,m_hasSetContour = false,
m_hasSetToolPath = false;
#if DebugModel
#include<iostream>
#pragma optimize("", off)
#endif
void ViewCurObject::UpdateUiStatus()
{
    auto myOccView = _mainwind->get_OCCView();
    myOccView->getContext()->UpdateCurrentViewer();
}
void ViewCurObject::ViewCurModelShape(ObjectViewType viewType)
{
    if(!m_hasSetModel){
        return;
    }
    auto myOccView = _mainwind->get_OCCView();
    switch (viewType) {
    case DisplayObject:
        myOccView->getContext()->Display(m_modelShape,false);
        break;
    case EreaseObject:
        myOccView->getContext()->Erase(m_modelShape,false);
        break;
    case RemoveObject:
        myOccView->getContext()->Remove(m_modelShape,false);
        break;
    default:
        break;
    }
    myOccView->fitAll();
    myOccView->getContext()->UpdateCurrentViewer();
}
void ViewCurObject::ViewCurrentModelBlank(ObjectViewType viewType)
{
    if(!m_hasSetBlank){
        return;
    }
    auto myOccView = _mainwind->get_OCCView();
    switch (viewType) {
    case DisplayObject:
        myOccView->getContext()->Display(m_modelBlank,false);
        break;
    case EreaseObject:
        myOccView->getContext()->Erase(m_modelBlank,false);
        break;
    case RemoveObject:
        myOccView->getContext()->Remove(m_modelBlank,false);
        break;
    default:
        break;
    }
    myOccView->fitAll();
    myOccView->getContext()->UpdateCurrentViewer();
}
void ViewCurObject::ViewVectShape(const vector<Handle(AIS_Shape)>&vShape,
                                  ObjectViewType viewType)
{
    auto myOccView = _mainwind->get_OCCView();
    for (int i = 0; i < vShape.size(); ++i) {
        switch (viewType) {
        case DisplayObject:
            myOccView->getContext()->Display(vShape[i], false);
            break;
        case EreaseObject:
            myOccView->getContext()->Erase(vShape[i], false);
            break;
        case RemoveObject:
            myOccView->getContext()->Remove(vShape[i], false);
            break;
        default:
            break;
        }
    }
}
void ViewCurObject::View2DVectShape(const vector<vector<Handle(AIS_Shape)>>&vShape,
                                    ObjectViewType viewType)
{
    for (int i = 0; i < vShape.size(); ++i) {
        ViewVectShape(vShape[i],viewType);
    }
}
void ViewCurObject::ViewBndIslShape(const vector<Handle(AIS_Shape)>&vBndShape,
                                    const vector<vector<Handle(AIS_Shape)>>&vIslShape,
                                    ObjectViewType viewType)
{
    ViewVectShape(vBndShape,viewType);
    View2DVectShape(vIslShape,viewType);
}
void ViewCurObject::ViewCurrentContoure(ObjectViewType viewType)
{
    for (int a = 0; a < m_vModelCutShape.size(); ++a) {
        for (int l = 0; l < m_vModelCutShape[a].size(); ++l) {
            ViewBndIslShape(m_vModelCutShape[a][l].m_vBndShape,
                            m_vModelCutShape[a][l].m_vIslShape,viewType);
        }
    }
    UpdateUiStatus();
}
void ViewCurObject::ViewCurrentSelBndIslLoop(ObjectViewType viewType)
{
    ViewBndIslShape(m_selBndIslShape.m_vBndShape,
                    m_selBndIslShape.m_vIslShape,viewType);
    UpdateUiStatus();
}
void ViewCurObject::ViewCurrentToolPath(ObjectViewType viewType)
{
    for (int n = 0; n < m_rootNodeShape.m_vNodeShape.size(); ++n) {
        NodeGeoShape nodeShape = m_rootNodeShape.m_vNodeShape[n];
        for (int lo = 0; lo < nodeShape.m_vLoopShape.size(); ++lo) {
            LoopGeoShape loopShape = nodeShape.m_vLoopShape[lo];
            ViewVectShape(loopShape.m_vShape,viewType);
        }
    }
    ViewBndIslShape(m_rootNodeShape.m_profShape.m_vBndShape,
                    m_rootNodeShape.m_profShape.m_vIslShape,viewType);
    auto myOccView = _mainwind->get_OCCView();
    myOccView->myView->SetProj(V3d_Zpos);
    myOccView->myView->FitAll(0.05);
    myOccView->getContext()->UpdateCurrentViewer();
}
void ViewCurObject::HideVectShape(const vector<Handle(AIS_Shape)>&vShape)
{
    auto myOccView = _mainwind->get_OCCView();
    for (int i = 0; i < vShape.size(); ++i) {
        myOccView->getContext()->Erase(vShape[i],false);
    }
}
ObjectViewType ViewCurObject::GetViewType(bool isDisplay)
{
    if(isDisplay){
        return DisplayObject;
    }
    return EreaseObject;
}
void ViewCurObject::ControlCurrentUiModel(bool isDisplay)
{
    if(!isDisplay)
        ViewCurModelShape(RemoveObject);
    else
        ViewCurModelShape(DisplayObject);
}
void ViewCurObject::ControlCurrentUiBlank(bool isDisplay)
{
    if(!isDisplay)
        ViewCurrentModelBlank(RemoveObject);
    else
        ViewCurrentModelBlank(DisplayObject);
}
void ViewCurObject::ControlCurrentUiContoure(bool isDisplay)
{
    if(!isDisplay)
        ViewCurrentContoure(RemoveObject);
    else
        ViewCurrentContoure(DisplayObject);
    UpdateUiStatus();
}
void ViewCurObject::ControlCurrentUiSelect(bool isDisplay)
{
    if(!isDisplay)
        ViewCurrentSelBndIslLoop(RemoveObject);
    else
        ViewCurrentSelBndIslLoop(DisplayObject);
    UpdateUiStatus();
}
void ViewCurObject::ControlCurrentUiToolPath(bool isDisplay)
{
    if(isDisplay)
        ViewCurrentToolPath(DisplayObject);
    else
        ViewCurrentToolPath(RemoveObject);
    UpdateUiStatus();
}
void ViewCurObject::InitailUiStatus()
{
    ViewCurModelShape(RemoveObject);
    ViewCurrentModelBlank(RemoveObject);
    ViewCurrentSelBndIslLoop(RemoveObject);
    ViewCurrentContoure(RemoveObject);
    ViewCurrentToolPath(RemoveObject);
    ///m_modelShape = new AIS_Shape();
    ///m_modelBlank = new AIS_Shape();
    m_selBndIslShape.m_vBndShape.clear();
    m_selBndIslShape.m_vIslShape.clear();
    m_vModelCutShape.clear();
    m_rootNodeShape.ClearShape();
    m_hasSetModel = false;
    m_hasSetBlank = false;
    m_hasSetSelObj = false;
    m_hasSetContour = false;
    m_hasSetToolPath = false;
}
void ViewCurObject::ReadCurrentModelFile(QString path)
{
    InitailUiStatus();
    TransModelPathToShape(path);
    m_hasSetModel = true;
    ViewCurModelShape(DisplayObject);
}
void ViewCurObject::DisplayModelBlank(string path,double xExp,double yExp)
{
    if(m_hasSetBlank){
        ViewCurrentModelBlank(RemoveObject);
    }
    TransModelBlankToShape(path,xExp,yExp);
    m_hasSetBlank = true;
    ViewCurrentModelBlank(DisplayObject);
}
void ViewCurObject::DisplaySelBndIslLoop(const BndIslPair& bndIslLoop)
{
    if(m_hasSetSelObj)
        ViewCurrentSelBndIslLoop(RemoveObject);
    m_selBndIslShape.m_vBndShape.clear();
    m_selBndIslShape.m_vIslShape.clear();
    TransSelectObjectToShape(bndIslLoop);
    m_hasSetSelObj = true;
    ViewCurrentSelBndIslLoop(DisplayObject);
}
void ViewCurObject::DisplayCurrentContoure(const vector<vector<BndIslPair>>&
                                           modCutData)
{
    if(m_hasSetContour)
        ViewCurrentContoure(RemoveObject);
    TransCurrentContoureToShape(modCutData);
    m_hasSetContour = true;
    ViewCurrentContoure(DisplayObject);
}
void ViewCurObject::DisplayCurrentToolPath(const GeomArea& bndIslLoop,
                                           const shared_ptr<OffsetNode>& rootNode)
{
    if(m_hasSetToolPath)
        ViewCurrentToolPath(RemoveObject);
    TransCurrentToolPathToShape(bndIslLoop,rootNode);
    m_hasSetToolPath = true;
    ViewCurrentToolPath(DisplayObject);
}
void ViewCurObject::TransModelPathToShape(QString path)
{
    QFileInfo fileInfo = QFileInfo(path);
    QString fileName = fileInfo.fileName();
    QString fileSuffix = fileInfo.suffix();
    auto myOccView = _mainwind->get_OCCView();
    if(fileSuffix=="step" || fileSuffix=="STEP"
            || fileSuffix=="stp"  || fileSuffix=="STP"){
        STEPControl_Reader reader;
        auto status = reader.ReadFile(path.toStdString().c_str());
        if (status == IFSelect_RetDone) {
            reader.TransferRoots();
            m_modelShape = new AIS_Shape(reader.OneShape());
        }
        m_modelShape->SetColor(Quantity_NOC_STEELBLUE);
        m_modelShape->SetHilightMode(1);
        Graphic3d_MaterialAspect smaterial;
        smaterial.SetMaterialType(Graphic3d_MATERIAL_PHYSIC);
        smaterial.SetShininess(0.5);
        myOccView->getContext()->SetMaterial(m_modelShape,smaterial,false);
        Handle(Prs3d_Drawer)t_hilight_style = myOccView->getContext()->
                HighlightStyle(Prs3d_TypeOfHighlight_LocalSelected);
        t_hilight_style->SetMethod(Aspect_TOHM_COLOR);
        t_hilight_style->SetColor(Quantity_NOC_GREEN);
        t_hilight_style->SetDisplayMode(1);
        myOccView->getContext()->SetHighlightStyle(t_hilight_style);
        myOccView->getContext()->Activate(myOccView->m_viewShape, 4);
        qDebug()<<fileName;
    }
    else if(fileSuffix=="stl" || fileSuffix=="STL" ||
            fileSuffix=="stls" || fileSuffix=="STLS"){
        StlAPI_Reader aReader_Stl;
        TopoDS_Shape shape_Stl;
        aReader_Stl.Read(shape_Stl, path.toStdString().c_str());
        m_modelShape = new AIS_Shape(shape_Stl);
        m_modelShape->SetColor(Quantity_NOC_CHOCOLATE1);
    }
}
void ViewCurObject::TransModelBlankToShape(string path,double xExp,double yExp)
{
    TopoDS_Shape blankShape;
    widvie_getpro->CreateBlanck(path,xExp,yExp,blankShape);
    m_modelBlank = new AIS_Shape(blankShape);
    m_modelBlank->SetTransparency(0.8);
}
void ViewCurObject::TransSelectObjectToShape(const BndIslPair &bndIslLoop)
{
    GeomArea bndIslLoop_(bndIslLoop.m_bndLoop,bndIslLoop.m_vIslLoop);
    widvie_trans->GetBndIslLoopShape(bndIslLoop_,BndOffsetPath,false,
                                     true,m_selBndIslShape.m_vBndShape,
                                     m_selBndIslShape.m_vIslShape);
}
void ViewCurObject::TransCurrentContoureToShape(const vector<vector<BndIslPair>>&modCutData)
{
    m_vModelCutShape.clear();
    for (int a = 0; a < modCutData.size(); ++a) {
        vector<BndIslGeoShape> vLayShape;
        for (int l = 0; l < modCutData[a].size(); ++l) {
            BndIslPair bndIslLoop = modCutData[a][l];
            GeomArea bndIslLoop_(bndIslLoop.m_bndLoop,bndIslLoop.m_vIslLoop);
            vector<Handle(AIS_Shape)>vBndShape;
            vector<vector<Handle(AIS_Shape)>>vIslShape;
            widvie_trans->GetBndIslLoopShape(bndIslLoop_,OriginBndPath,
                                             true,true,vBndShape,vIslShape);
            BndIslGeoShape biShape(vBndShape,vIslShape);
            vLayShape.push_back(biShape);
        }
        m_vModelCutShape.push_back(vLayShape);
    }
}
void ViewCurObject::TransCurrentToolPathToShape(const GeomArea& bndIslLoop,
                                                const shared_ptr<OffsetNode>& rootNode)
{
    m_rootNodeShape.ClearShape();
    widvie_trans->GetBndIslLoopShape(bndIslLoop,BndOffsetPath,true,
                                     false,m_rootNodeShape.m_profShape.m_vBndShape,
                                     m_rootNodeShape.m_profShape.m_vIslShape);
    vector<NodeShape>vNodeShape;
    widvie_trans->GetOffsetNodeLoopShape(rootNode,true,0,vNodeShape);
    for (int n = 0; n < vNodeShape.size(); ++n) {
        NodeGeoShape nodeShape;
        for (int lo = 0; lo < vNodeShape[n].m_vLoopShape.size(); ++lo) {
            LoopGeoShape loopGoShape;
            for (int e = 0; e < vNodeShape[n].m_vLoopShape[lo].m_vShape.size(); ++e) {
                EleShape eleShape = vNodeShape[n].m_vLoopShape[lo].m_vShape[e];
                loopGoShape.m_vShape.push_back(eleShape.m_occShape);
            }
            nodeShape.m_vLoopShape.push_back(loopGoShape);
        }
        ///m_vNodeShape.push_back(nodeShape);
        m_rootNodeShape.m_vNodeShape.push_back(nodeShape);
    }
}