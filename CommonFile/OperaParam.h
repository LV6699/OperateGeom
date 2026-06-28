#ifndef OPERAPARAM_H
#define OPERAPARAM_H

#include "TranStruct.h"

namespace grm {
enum class ItemType{
    Model = 0,
    TrisEdge,
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

enum class ToolType {
  PlaneEnd = 0,  /// 平底刀
  BallNoseEnd,   /// 球刀
  RoundNoseEnd,  /// 圆笔刀
  TaperEnd,      /// 锥度平底刀
  TaperBall,     /// 锥度球铣刀
  V_Cutter       /// v刀
};
class DefTool {
 public:
  DefTool() {}
  DefTool(ToolType t, double R, double cr) : _type(t), _R(R), _cr(cr) {
    if (t == ToolType::PlaneEnd) {
      cr = 0;
    } else if (t == ToolType::BallNoseEnd) {
      cr = R;
    } else if (t == ToolType::V_Cutter) {
      cr = 0;
    }
  }
  void SetR(double r) { _R = r; }
  void SetCR(double r) { _cr = r; }
  void CalRR() { _RR = _R * _R; }
  void CalDiff() { _R_r = _R - _cr; }

  double R() const { return _R; }
  double CR() const { return _cr; }
  double RadSub() const { return _R - _cr; }
  double RR() const { return _RR; }
  
  ToolType _type;
  double _R = 0;         //最大半径
  double _cr = 0;        //边角半径
  double _br = 0;        //底边半径
  
  double _h = 0;         //有效高度
  double _full_h = 0;    //计算高度(有效高度加延伸高度)
  double _br_h  = 0;
  double _taper_l = 0;       //斜边有效长度
  double _full_taper_l = 0;  //斜边计算长度(有效长度加延伸长度)
                              
  double _RR = 0;          //R*R
  double _cr2 = 0;          //r*r
  double _br2 = 0;         //br*br
  double _R_r = 0;         //R-r
  double _R_br = 0;        //R-br
  double _taper_agl = 45;  //斜边角度,轴线与母线夹角
  double _taper_k = 0;     //斜边斜率
  
  double _br_up_h = 0;     //底边半径对应上部高度
  double _br_down_h = 0;   //底边半径对应下部高度
  double _lenth = 20;
  
  std::shared_ptr<DefTool>_subTool = nullptr;
  ViewItem _item;

};


}  // namespace grm

#endif  // OPERAPARAM_H
