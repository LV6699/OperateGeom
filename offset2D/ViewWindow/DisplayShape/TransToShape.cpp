#include "TransToShape.h"
shared_ptr<ProIOInterface> tpsha_IO = make_shared<ProIOInterface>();
shared_ptr<GeomCalculate>tpsha_Go = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate> tpsha_num = make_shared<ElemCalculate>();
shared_ptr<CheckGeomInt> tpsha_chIn = make_shared<CheckGeomInt>();
shared_ptr<DataEdit> tpsha_dataEdit = make_shared<DataEdit>();
shared_ptr<CutParts> tpsha_cut = make_shared<CutParts>();
#if DebugModel
#pragma optimize("", off)
#endif
void TransToShape::GetShapeColor(bool isClassify,bool isOriProf,
                                 ToolPathType pathType,double &width,
                                 Quantity_Color& color)
{
    Quantity_Color blue(0,0,0.7,Quantity_TOC_RGB);
    Quantity_Color red(0.7,0,0,Quantity_TOC_RGB);
    Quantity_Color purple(0.6,0,0.6,Quantity_TOC_RGB);
    Quantity_Color green(0,0.25,0,Quantity_TOC_RGB);
    Quantity_Color orange(0.9,0.1,0,Quantity_TOC_RGB);
    Quantity_Color black(0,0,0,Quantity_TOC_RGB);
    Quantity_Color brown(0.5,0.2,0.0,Quantity_TOC_RGB);
    Quantity_Color cyan(0.,1,1,Quantity_TOC_RGB);
    Quantity_Color grey(0.02,0.06,0.06,Quantity_TOC_RGB);
    Quantity_Color grey_(0.1,0.2,0.2,Quantity_TOC_RGB);
    if(isOriProf){
        color = grey;
        width = 1;
        switch (pathType) {
        case OriginBndPath:{
            break;
        }
        case OriginIslPath:{
            width = 1.5;
            break;
        }
        case BlockPath:{
            width = 1.5;
            color = grey_;
            ///width = 1.5;
            break;
        }
        default:
            color = brown;
            width = 1.5;
            break;
        }/**
        if(pathType == OriginBndPath ||
                pathType == OriginIslPath){
            color = red;
        }
        else if(pathType == BlockPath){
            color = orange;
        }*/
        return;
    }
    width = 1.5;
    if(!isClassify){
        color = green;
        return;
    }
    switch (pathType) {
    case BndOffsetPath:
        color = blue;
        break;
    case BndOftLocalPath:
        color = red;
        break;
    case IslOffsetPath:
        color = purple;
        break;
    case IslOftLocalPath:
        color = red;
        break;
    case BndIslMerPath:
        color = green;
        break;
    case IslIslMerPath:
        color = orange;
        break;
    case BlockPath:
        color = orange;
        break;
    case SimulateTool:
        color = green;
        break;
    case UnknowPath:
        color = cyan;
        width = 1;
        break;
    default:
        break;
    }
}
void LoopToTopShape(const DefLoop& loop,vector<TopoDS_Shape>&vShape)
{
    Quantity_Color color(0.8,0,0.0,Quantity_TOC_RGB);
    for (int i = 0; i < loop.Size(); ++i) {
        DefElem ele = loop.IndexElem(i);
        if(ele.Type() == LINETYPE){
            Point sp = ele._segment._segSp,ep = ele._segment._segEp;
            gp_Pnt s(sp.X(),sp.Y(),sp.Z()),e(ep.X(),ep.Y(),ep.Z());
            TopoDS_Edge edg = BRepBuilderAPI_MakeEdge(s, e);
            Handle(AIS_Shape) shape = new AIS_Shape(edg);
            shape->SetColor(color);
            shape->SetWidth(1);
        }
        gp_Pnt startPoint(0.0, 0.0, 0.0);
        gp_Pnt endPoint(10.0, 10.0, 0.0);
        TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(startPoint,endPoint);
        TopoDS_Shape shape = edge;
        ///post2win::post(shape);
    }
}
void TransToShape::TranslatePointToShape(const Point& P,Quantity_Color color,
                                         Handle(AIS_Shape)&shape)
{
    BRepBuilderAPI_MakeVertex vertex(gp_Pnt (P.X(),P.Y(),P.Z()));
    shape = new AIS_Shape(vertex);
    shape->SetColor(color);
    shape->SetWidth(2);
}
void TransToShape::TranslateEleToAIS_Shape(DefElem ele,
                                           Quantity_Color color,
                                           double width,bool& isSucccess,
                                           Handle(AIS_Shape)&shape)
{
    isSucccess = false;
    if(ele._type == LINETYPE){
        bool isSame = false;
        tpsha_Go->IsIdenticalPoint(ele._segment._segSp,
                                   ele._segment._segEp,
                                   PreErr5_10,isSame);
        if(!tpsha_num->IsValidPoint(ele._segment._segSp) ||
                !tpsha_num->IsValidPoint(ele._segment._segEp) ||
                !tpsha_num->IsValidPoint(ele._segment._segEp)){
            //return;
        }
        if(isSame ){
            return;
        }
        gp_Pnt startP(ele._segment._segSp._x,ele._segment._segSp._y,ele._segment._segSp._z);
        gp_Pnt endP(ele._segment._segEp._x,ele._segment._segEp._y,ele._segment._segEp._z);
        TopoDS_Edge ost_Edge = BRepBuilderAPI_MakeEdge(startP, endP);
        shape = new AIS_Shape(ost_Edge);
        shape->SetColor(color);
        shape->SetWidth(width);
    }
    else if(ele._type == ARCTYPE){
        tpsha_chIn->JudgeArcRadius(ele._arc);
        Point startPnt = ele._arc._arcSp;
        Point endPnt = ele._arc._arcEp;
        Point centerPnt = ele._arc._arcCp;
        bool iscw = ele._arc._isCW;
        double radius = ele._arc._arcR;
        double dis0,dis1,dis2;
        tpsha_Go->TwoPointDistance(startPnt,endPnt,dis0);
        tpsha_Go->TwoPointDistance(startPnt,centerPnt,dis1);
        tpsha_Go->TwoPointDistance(endPnt,centerPnt,dis2);
        if(dis0 < PreErr5_10 || dis1 < PreErr5_10 || dis2 < PreErr5_10){
            return;
            gp_Pnt centerCdt (centerPnt._x,centerPnt._y,centerPnt._z);
            gp_Circ circle(gp_Ax2(centerCdt, gp_Dir(0, 0, 1)), radius);
            Handle_Geom_Circle geomCircle = new Geom_Circle(circle);
            TopoDS_Edge circleEdge = BRepBuilderAPI_MakeEdge(geomCircle);
            shape = new AIS_Shape(circleEdge);
        }
        if(!tpsha_num->IsValidPoint(startPnt) || !tpsha_num->IsValidPoint(endPnt)){
            //return;
        }
        gp_Pnt startP(startPnt._x,startPnt._y,startPnt._z);
        gp_Pnt endP(endPnt._x,endPnt._y,endPnt._z);
        gp_Pnt centerP(centerPnt._x,centerPnt._y,centerPnt._z);/**
        BRepBuilderAPI_MakeVertex vertex0(startP);
        auto shape0 = new AIS_Shape(vertex0);
        shape0->SetColor(color);
        BRepBuilderAPI_MakeVertex vertex1(centerP);
        auto shape1 = new AIS_Shape(vertex1);
        shape1->SetColor(color);
        BRepBuilderAPI_MakeVertex vertex2(endP);
        auto shape2 = new AIS_Shape(vertex2);
        shape2->SetColor(color);
        auto myOccView = _mainwind->get_OCCView();
        myOccView->getContext()->Display(shape0,true);
        myOccView->getContext()->Display(shape1,true);
        myOccView->getContext()->Display(shape2,true);
        return;*/
        /*radius = sqrt(pow(startPnt._x - centerPnt._x, 2) +
                          pow(startPnt._y - centerPnt._y, 2));*/
        gp_Circ circle(gp_Ax2(centerP, gp_Dir(0, 0, 1)), radius);
        if(iscw){
            std::swap(startP, endP);
        }
        GC_MakeArcOfCircle arcMaker(circle, startP, endP, true);
        Handle(Geom_TrimmedCurve) trimmedCurve = arcMaker.Value();
        BRepBuilderAPI_MakeEdge edgeMaker(trimmedCurve);
        shape = new AIS_Shape(edgeMaker);
        shape->SetColor(color);
        shape->SetWidth(width);/**
        auto myOccView = _mainwind->get_OCCView();
        myOccView->getContext()->Display(shape,true);
        return;*/
    }
    else if(ele._type == CIRCLETYPE){
        double radius = ele._circle._cirR;
        gp_Pnt centerCdt (ele._circle._cirCp._x,
                          ele._circle._cirCp._y,
                          ele._circle._cirCp._z);
        gp_Circ circle(gp_Ax2(centerCdt, gp_Dir(0, 0, 1)), radius);
        Handle_Geom_Circle geomCircle = new Geom_Circle(circle);
        TopoDS_Edge circleEdge = BRepBuilderAPI_MakeEdge(geomCircle);
        shape = new AIS_Shape(circleEdge);
        shape->SetColor(color);
        shape->SetWidth(width);
    }
    isSucccess = true;
}
void TransToShape::TranslateLoopToShape(const DefLoop& loop,Quantity_Color color,
                                        vector<Handle(AIS_Shape)>&vShape)
{
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        double width = 1.5;
        Handle(AIS_Shape) shape;
        bool isSucccess = false;
        ///Quantity_Color color(0.,0,1,Quantity_TOC_RGB);
        TranslateEleToAIS_Shape(loop.m_vElem[i],color,width,
                                isSucccess,shape);
        if(!isSucccess){
            continue;
        }
        vShape.push_back(shape);
    }
}
void TransToShape::GetLoopDisplayShape(const DefLoop& loop,bool isClassify,
                                       bool isOriProf,ToolPathType pathType,
                                       int nodeIdx,LoopShape& loopShape)
{
    double width;
    Quantity_Color color,brown(0.5,0.2,0.0,Quantity_TOC_RGB);
    GetShapeColor(isClassify,isOriProf,pathType,width,color);
    for(int i = 0;i < loop.m_vElem.size(); ++i){
        bool isSucccess = false;
        DefElem ele = loop.m_vElem[i];
        Handle(AIS_Shape) AIS_Face;
        /*if(loop._loopLabel._isMixLoop && ele._elemLabel._isIslElem)
            TranslateEleToAIS_Shape(ele,brown,1.5,isSucccess,AIS_Face);
        else*/
            TranslateEleToAIS_Shape(ele,color,width,isSucccess,AIS_Face);
        if(!isSucccess){
            continue;
        }/**
        ///******************************
        ElemCalculate elecal;
        Point sp = elecal.ElementEndpoint(ele,false);
        Point ep = elecal.ElementEndpoint(ele,true);
        Quantity_Color red(0.9,0.0,0.0,Quantity_TOC_RGB);
        Handle(AIS_Shape)sSha,eSha;
        TranslatePointToShape(sp,red,sSha);
        TranslatePointToShape(ep,red,eSha);
        EleShape se(loop.m_vElem[i],sSha,i,nodeIdx);
        //EleShape ee(loop.m_vElem[i],eSha,i,nodeIdx);
        loopShape.m_vShape.push_back(se);
        //loopShape.m_vShape.push_back(ee);
        ///******************************
        */
        record_map[AIS_Face].index = i;
        EleShape occData(loop.m_vElem[i],AIS_Face,i,nodeIdx);
        loopShape.m_vShape.push_back(occData);
    }
}
void TransToShape::GetBndIslLoopShape(const GeomArea &bndIslLoop,
                                      ToolPathType pathType,bool isOriLoop,
                                      bool isClassify,
                                      vector<Handle(AIS_Shape)>&vBndShape,
                                      vector<vector<Handle(AIS_Shape)>>&vIslShape)
{
    vBndShape.clear();
    vIslShape.clear();
    LoopShape bndShape;
    GetLoopDisplayShape(bndIslLoop._bndLoop,isClassify,isOriLoop,
                        pathType,0,bndShape);
    for (int i = 0; i < bndShape.m_vShape.size(); ++i) {
        vBndShape.push_back(bndShape.m_vShape[i].m_occShape);
    }
    for (int lo = 0; lo < bndIslLoop._vIslLoop.size(); ++lo) {
        LoopShape loopShape;
        DefLoop loop = bndIslLoop._vIslLoop[lo];
        GetLoopDisplayShape(loop,isClassify,isOriLoop,
                            pathType,0,loopShape);
        vector<Handle(AIS_Shape)>vLoopShape;
        for (int i = 0; i < loopShape.m_vShape.size(); ++i) {
            vLoopShape.push_back(loopShape.m_vShape[i].m_occShape);
        }
        vIslShape.push_back(vLoopShape);
    }
}
void TransToShape::GetPairLoopDisplay(const GeomArea &bndIslLoop,
                                      bool isOriLoop,bool isClear)
{
    auto myOccView = _mainwind->get_OCCView();
    if(isClear){
        myOccView->myRemoveall();
    }
    LoopShape loopShape;
    GetLoopDisplayShape(bndIslLoop._bndLoop,false,isOriLoop,
                        OriginBndPath,0,loopShape);
    BndIslShape temBndIslShape;
    glb_bndIslShape = temBndIslShape;
    glb_bndIslShape.m_bndShape = loopShape;
    for(int lo = 0;lo < bndIslLoop._vIslLoop.size();lo ++){
        LoopShape loopSha;
        GetLoopDisplayShape(bndIslLoop._vIslLoop[lo],false,isOriLoop,
                            OriginIslPath,0,loopSha);
        glb_bndIslShape.m_vIslShape.push_back(loopSha);
    }
    /*for (int lo = 0; lo < bndIslLoop._vBlockLoop.size(); ++lo) {
        LoopShape loopSha;
        GetLoopDisplayShape(bndIslLoop._vBlockLoop[lo],false,isOriLoop,
                            BlockPath,0,loopSha);
        glb_bndIslShape.m_vBlockShape.push_back(loopSha);
    }*/
    //myOccView->myView->FitAll(0.05);
    for(auto& it : glb_bndIslShape.m_bndShape.m_vShape){
        myOccView->getContext()->Display(it.m_occShape,false);
    }
    for(auto& lo : glb_bndIslShape.m_vIslShape){
        for(auto& data : lo.m_vShape){
            myOccView->getContext()->Display(data.m_occShape,false);
        }
    }
    for(auto& lo : glb_bndIslShape.m_vBlockShape){
        for(auto& data : lo.m_vShape){
            myOccView->getContext()->Display(data.m_occShape,false);
        }
    }
    myOccView->myView->FitAll(0.05);
    myOccView->getContext()->UpdateCurrentViewer();
}
void TransToShape::GetLoopsDisplayShape(const vector<DefLoop>&vLoop,
                                        ToolPathType pathType,
                                        int nodeIdx, bool isOriProf,
                                        bool isClassify,
                                        NodeShape& curNodeShape)
{
    vector<LoopShape>vLoopShape;
    for(int lo = 0;lo < vLoop.size();lo++){
        LoopShape loopShape;
        GetLoopDisplayShape(vLoop[lo],isClassify,isOriProf,pathType,
                            nodeIdx,loopShape);
        loopShape.m_loop = vLoop[lo];
        if(!loopShape.m_vShape.empty()){/**
            if(vLoop[lo].IsOriginLoop()){
                Quantity_Color color(0.5,0.2,0.0,Quantity_TOC_RGB);
                for(auto& s :loopShape.m_vShape ){
                    s.m_occShape->SetColor(color);
                }
            }*/
            vLoopShape.push_back(loopShape);
        }
    }
    curNodeShape.m_nodeIdx = nodeIdx;
    curNodeShape.m_vLoopShape = vLoopShape;
}
void TransToShape::GetOffsetNodeLoopShape(const shared_ptr<OffsetNode>&rootNode,
                                          bool isClassify,int curNodeIdx)
{
    if(rootNode == nullptr){
        return;
    }
    NodeShape curNodeShape;
    GetLoopsDisplayShape(rootNode->_loops,rootNode->_geneType,
                         curNodeIdx,false,isClassify,curNodeShape);
    glb_vNodeOcc.push_back(curNodeShape);
    for (int i = 0; i < rootNode->_nodes.size(); i++) {
        curNodeIdx ++;
        GetOffsetNodeLoopShape(rootNode->_nodes[i],
                               isClassify,curNodeIdx);
    }
}
void TransToShape::GetOffsetNodeLoopShape(const shared_ptr<OffsetNode> &rootNode,
                                          bool isClassify,int curNodeIdx,
                                          vector<NodeShape>&vNodeShape)
{
    if(rootNode == nullptr){
        return;
    }
    NodeShape curNodeShape;
    GetLoopsDisplayShape(rootNode->_loops,rootNode->_geneType,
                         curNodeIdx,false,isClassify,curNodeShape);
    vNodeShape.push_back(curNodeShape);
    for (int i = 0; i < rootNode->_nodes.size(); i++) {
        curNodeIdx ++;
        GetOffsetNodeLoopShape(rootNode->_nodes[i],
                               isClassify,curNodeIdx,vNodeShape);
    }
}