#ifndef OPERAPARAM_H
#define OPERAPARAM_H

namespace grm {

enum class ToolType {
  PlaneEnd = 0,  /// 平底刀
  BallNoseEnd,   /// 圆笔刀
  RoundNoseEnd,  /// 球刀
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
  double _full_h;        //计算高度(有效高度加延伸高度)
  double _taper_l;       //斜边有效长度
  double _full_taper_l;  //斜边计算长度(有效长度加延伸长度)
                              
  double _RR = 0;
  double _rr = 0;
  double _br2 = 0;
  double _R_r = 0;
  double _R_br = 0;
  double _taper_agl = 45;  //斜边角度
  double _taper_k = 0;     //斜边斜率

};


}  // namespace grm

#endif  // OPERAPARAM_H
