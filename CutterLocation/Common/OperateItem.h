#ifndef OPERVIEWDATA_H
#define OPERVIEWDATA_H

#include "ToolParam.h"

namespace ViewObj {
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

class OpeViewItem
{
public:
    OpeViewItem(){}

    void IniModelItem(const TopoDS_Shape& s,Quantity_Color c =
            Quantity_Color (0.1,0.1,0.1,Quantity_TOC_RGB),
                      double w = 1){
        _model = grm::ViewItem(s,grm::ItemType::Model,c,w);
    }
    void IniTrisItem(const TopoDS_Shape& s,bool iscl = false,
                     Quantity_Color c = Quantity_Color (
                0.1,0.1,0.1,Quantity_TOC_RGB),double w = 1){
        if(!iscl){_modTris = grm::ViewItem(s,grm::ItemType::ModTris,c,w);}
        else{_trisCl = grm::ViewItem(s,grm::ItemType::ClTris,c,w);}
    }
    void IniEdgesItem(const TopoDS_Shape& s,bool isXEdge = true,
                      Quantity_Color c = Quantity_Color (
                0.1,0.1,0.1,Quantity_TOC_RGB),double w = 1){
        if(isXEdge){_xEdges = grm::ViewItem(s,grm::ItemType::XEdge,c,w);}
        else{_yEdges = grm::ViewItem(s,grm::ItemType::YEdge,c,w);}
    }
    void IniPointsItem(const TopoDS_Shape& s,
                       Quantity_Color c = Quantity_Color (
                0.1,0.1,0.1,Quantity_TOC_RGB)){
        _clPts = grm::ViewItem(s,grm::ItemType::SelPoint,c);
    }
    void IniTranglesEdge(const TopoDS_Shape& s,
                         Quantity_Color c = Quantity_Color(
                0.1, 0.1, 0.1,Quantity_TOC_RGB),double w = 1) {
        _oriEdge = grm::ViewItem(s, grm::ItemType::TrisEdge, c,w);
    }

public:
    grm::ViewItem _model;       ///原始模型
    grm::ViewItem _modTris;     ///模型离散三角形
    grm::ViewItem _trisCl;      ///偏置三角形
    grm::ViewItem _ptProt;      ///点保护面
    grm::ViewItem _edgeProt;    ///边保护面
    grm::ViewItem _xEdges;      ///平行x轴边
    grm::ViewItem _yEdges;      ///平行y轴边
    grm::ViewItem _clPts;       ///刀位点
    grm::ViewItem _sel_ts;      ///选择的三角形
    grm::ViewItem _sel_clts;    ///选择的偏置三角形
    grm::ViewItem _int_clts;    ///相交三角形
    grm::ViewItem _selPt;       ///选择的点
    grm::ViewItem _oriEdge;     ///离散三角形边
    std::vector<grm::ViewItem> _clRelItems;  ///刀位点相关来源对象

};

}

extern grm::ViewItem _idTria;
extern grm::ViewItem _id_e_ite;
extern ViewObj::OpeViewItem _opeItem;

#endif // OPERVIEWDATA_H



























