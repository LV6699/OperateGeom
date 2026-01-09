#ifndef OPERAPARAM_H
#define OPERAPARAM_H

namespace grm {

enum class ToolType {
  PlaneEnd = 0,  /// 平底刀
  BallNoseEnd,   /// 圆笔刀
  RoundNoseEnd,  /// 球刀
  TaperEnd,      ///锥度平底刀
  V_Cutter       /// v刀
};

class DefTool {
 public:
  DefTool() {}
  DefTool(ToolType t, double R, double cr) : _type(t), _R(R), _cr(cr) {
      if (t == ToolType::PlaneEnd) {cr = 0;} 
      else if (t == ToolType::BallNoseEnd) {cr = R;} 
      else if (t == ToolType::V_Cutter) {cr = 0;}
  }
  void SetR(double r) { _R = r; }
  void SetCR(double r) { _cr = r; }
  void CalRR() { _RR = _R * _R; }
  void CalDiff() { _diff = _R - _cr; }

  double R() const { return _R; }
  double CR() const { return _cr; }
  double RadSub() const { return _R - _cr; }
  double RR() const { return _RR; }
  double Calc_Length() const {
    switch (_type) {
      case ToolType::PlaneEnd:return 0;
      case ToolType::BallNoseEnd:return _R;
      case ToolType::RoundNoseEnd:return _cr;
      case ToolType::TaperEnd:return (_R - _bottom_r) / std::tan(_taper_alg);
      default:throw "unsupported tool type";
    }
  }

 public:
    ToolType _type;
    double _R = 0;
    double _cr = 0;
    double _RR = 0;
    double _rr = 0;
    double _diff = 0;
    double _taper_alg = 45;
    double _bottom_r = 0;
    double _taper_k = 0;
    double _l = 0;
    double _taper_l = 0;
};

// void SetToolParam(DefTool& t) {
//     t._RR = t._R * t._R;
//     t._diff = t._R - t._cr;
    
//     t._l = t.Calc_Length();
//     if (t._type == ToolType::TaperEnd) {
//       t._taper_k = std::tan(t._taper_alg * _RadAlg);
//       t._taper_l = t._R / t._taper_k;
//   }
// }

}  // namespace grm

#endif  // OPERAPARAM_H
