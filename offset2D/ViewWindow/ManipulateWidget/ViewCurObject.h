#ifndef VIEWCUROBJECT_H
#define VIEWCUROBJECT_H
#include"WidgeStruct.h"
//#include"../DisplayShape/DisplayToolPath.h"
//#include"../DisplayShape/DisplayModel.h"
/*class BndIslGeoShape
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
};*/
enum ObjectViewType{
    DisplayObject = 0,
    EreaseObject,
    RemoveObject
};
class ViewCurObject
{
public:
    ViewCurObject(){}
    void UpdateUiStatus();
    void ViewVectShape(const vector<Handle(AIS_Shape)>&vShape,
                       ObjectViewType viewType);
    void View2DVectShape(const vector<vector<Handle(AIS_Shape)>>&vShape,
                         ObjectViewType viewType);
    void ViewBndIslShape(const vector<Handle(AIS_Shape)>&vBndShape,
                         const vector<vector<Handle(AIS_Shape)>>&vIslShape,
                         ObjectViewType viewType);
    void ViewCurModelShape(ObjectViewType viewType);
    void ViewCurrentContoure(ObjectViewType viewType);
    void ViewCurrentSelBndIslLoop(ObjectViewType viewType);
    void ViewCurrentToolPath(ObjectViewType viewType);
    void ViewCurrentModelBlank(ObjectViewType viewType);
    void HideVectShape(const vector<Handle(AIS_Shape)>&vShape);
    ObjectViewType GetViewType(bool isDisplay);
    void ControlCurrentUiModel(bool isDisplay);
    void ControlCurrentUiBlank(bool isDisplay);
    void ControlCurrentUiContoure(bool isDisplay);
    void ControlCurrentUiSelect(bool isDisplay);
    void ControlCurrentUiToolPath(bool isDisplay);
    void InitailUiStatus();
    void ReadCurrentModelFile(QString path);
    void DisplaySelBndIslLoop(const BndIslPair &bndIslLoop);
    void DisplayModelBlank(string path,double xExp,double yExp);
    void DisplayCurrentContoure(const vector<vector<BndIslPair>>&modCutData);
    void DisplayCurrentToolPath(const GeomArea &bndIslLoop,
                                const shared_ptr<OffsetNode>& rootNode);
    void TransModelPathToShape(QString path);
    void TransModelBlankToShape(string path,double xExp,double yExp);
    void TransSelectObjectToShape(const BndIslPair &bndIslLoop);
    void TransCurrentContoureToShape(const vector<vector<BndIslPair>>&modCutData);
    void TransCurrentToolPathToShape(const GeomArea &bndIslLoop,
                                     const shared_ptr<OffsetNode>& rootNode);
private:
    /**static Handle(AIS_Shape) m_ModelShape;
    static vector<vector<BndIslGeoShape>>m_vModelCutShape;
    static vector<NodeGeoShape>m_vNodeShape;*/
};
#endif // VIEWCUROBJECT_H