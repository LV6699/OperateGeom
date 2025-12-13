#ifndef OPERVIEWDATA_H
#define OPERVIEWDATA_H

#include "../../ViewWindow/OccHeader.h"

namespace ViewObj {

enum class ItemType{
    Model = 0,
    ModTris,
    ClTris,
    PtProt,
    EdgeProt,
    XEdge,
    YEdge,
    ClPts,
    SelPoint,
    SelEdge,
    SelTria
};
/**
    ViewItem _model;       ///原始模型
    ViewItem _modTris;     ///模型离散三角形
    ViewItem _trisCl;      ///偏置三角形
    ViewItem _ptProt;      ///点保护面
    ViewItem _edgeProt;    ///边保护面
    ViewItem _xEdges;      ///平行x轴边
    ViewItem _yEdges;      ///平行y轴边
    ViewItem _clPts;       ///刀位点
    ViewItem _sel_ts;      ///选择的三角形
    ViewItem _sel_clts;    ///选择的偏置三角形
    ViewItem _int_clts;    ///相交三角形
    ViewItem _selPt;       ///选择的点
    std::vector<ViewItem> _clRelItems;  ///刀位点相关来源对象
*/
class ViewItem{
public:
    ViewItem(){}
    ViewItem(const TopoDS_Shape& s,Quantity_Color c =
            Quantity_Color (0.1,0.1,0.1,Quantity_TOC_RGB),
             double w = 1) : _shape(s){
        if(s.IsNull()){return;}
        _ashape = new AIS_Shape(s);
        _ashape->SetColor(c);
        _ashape->SetWidth(w);
        _hasInitial = true;
    }
    ViewItem(const TopoDS_Shape& s,ItemType type,Quantity_Color c =
            Quantity_Color (0.1,0.1,0.1,Quantity_TOC_RGB),
             double w = 1) : _shape(s),_type(type){
        if(s.IsNull()){return;}
        _ashape = new AIS_Shape(s);
        _ashape->SetColor(c);
        _ashape->SetWidth(w);
        _hasInitial = true;
    }
    void SetHasDisplay(){_hasDisplay = !_hasDisplay;}

public:
    TopoDS_Shape _shape;
    Handle(AIS_Shape) _ashape;
    bool _hasDisplay = false;
    bool _hasInitial = false;
    Handle(AIS_Shape) _aShape = nullptr;
    Handle(AIS_TextLabel) _texAspe = nullptr;
    ItemType _type;
};

class OpeViewItem
{
public:
    OpeViewItem(){}

    void IniModelItem(const TopoDS_Shape& s,Quantity_Color c =
            Quantity_Color (0.1,0.1,0.1,Quantity_TOC_RGB),
                      double w = 1){
        _model = ViewItem(s,ItemType::Model,c,w);
    }
    void IniTrisItem(const TopoDS_Shape& s,bool iscl = false,
                     Quantity_Color c = Quantity_Color (
                0.1,0.1,0.1,Quantity_TOC_RGB),double w = 1){
        if(!iscl){_modTris = ViewItem(s,ItemType::ModTris,c,w);}
        else{_trisCl = ViewItem(s,ItemType::ClTris,c,w);}
    }
    void IniEdgesItem(const TopoDS_Shape& s,bool isXEdge = true,
                      Quantity_Color c = Quantity_Color (
                0.1,0.1,0.1,Quantity_TOC_RGB),double w = 1){
        if(isXEdge){_xEdges = ViewItem(s,ItemType::XEdge,c,w);}
        else{_yEdges = ViewItem(s,ItemType::YEdge,c,w);}
    }
    void IniPointsItem(const TopoDS_Shape& s,
                       Quantity_Color c = Quantity_Color (
                0.1,0.1,0.1,Quantity_TOC_RGB)){
        _clPts = ViewItem(s,ItemType::SelPoint,c);
    }

public:
    ViewItem _model;       ///原始模型
    ViewItem _modTris;     ///模型离散三角形
    ViewItem _trisCl;      ///偏置三角形
    ViewItem _ptProt;      ///点保护面
    ViewItem _edgeProt;    ///边保护面
    ViewItem _xEdges;      ///平行x轴边
    ViewItem _yEdges;      ///平行y轴边
    ViewItem _clPts;       ///刀位点
    ViewItem _sel_ts;      ///选择的三角形
    ViewItem _sel_clts;    ///选择的偏置三角形
    ViewItem _int_clts;    ///相交三角形
    ViewItem _selPt;       ///选择的点
    ViewItem _oriEdge;     ///离散三角形边
    std::vector<ViewItem> _clRelItems;  ///刀位点相关来源对象

};

}

extern ViewObj::ViewItem _idTria;
extern ViewObj::OpeViewItem _opeItem;

#endif // OPERVIEWDATA_H



























