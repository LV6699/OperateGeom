#ifndef OPERVIEWDATA_H
#define OPERVIEWDATA_H

#include "../../ViewWindow/OccHeader.h"

namespace ViewObj {

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
    void SetHasDisplay(){_hasDisplay = !_hasDisplay;}

public:
    TopoDS_Shape _shape;
    Handle(AIS_Shape) _ashape;
    bool _hasDisplay = false;
    bool _hasInitial = false;
};

class OpeViewItem
{
public:
    OpeViewItem(){}

    void IniModelItem(const TopoDS_Shape& s,Quantity_Color c =
            Quantity_Color (0.1,0.1,0.1,Quantity_TOC_RGB),
                      double w = 1){
        _model = ViewItem(s,c,w);
    }
    void IniTrisItem(const TopoDS_Shape& s,bool iscl = false,
                     Quantity_Color c = Quantity_Color (
                0.1,0.1,0.1,Quantity_TOC_RGB),double w = 1){
        if(!iscl){_modTris = ViewItem(s,c,w);}
        else{_trisCl = ViewItem(s,c,w);}
    }
    void IniEdgesItem(const TopoDS_Shape& s,bool isXEdge = true,
                      Quantity_Color c = Quantity_Color (
                0.1,0.1,0.1,Quantity_TOC_RGB),double w = 1){
        if(isXEdge){_xEdges = ViewItem(s,c,w);}
        else{_yEdges = ViewItem(s,c,w);}
    }
    void IniPointsItem(const TopoDS_Shape& s,
                       Quantity_Color c = Quantity_Color (
                0.1,0.1,0.1,Quantity_TOC_RGB)){
        _clPts = ViewItem(s,c);
    }

public:
    ViewItem _model;
    ViewItem _modTris;
    ViewItem _trisCl;
    ViewItem _ptProt;
    ViewItem _edgeProt;
    ViewItem _xEdges;
    ViewItem _yEdges;
    ViewItem _clPts;
    ViewItem _sel_ts;
    ViewItem _sel_clts;
    ViewItem _int_clts;

};

}

#endif // OPERVIEWDATA_H
