#ifndef ENUMTYPE_H
#define ENUMTYPE_H
namespace OffsetEnum {

enum ElemType{LINETYPE,ARCTYPE,CIRCLETYPE};
enum DistInOut{EntiretyOut,PartOut,EntiretyIn};
enum OffsetStyle{
    /// 边界环偏置，岛屿环不偏置
    BndLoop_Offset,
    /// 岛屿环偏置，边界环是否偏置由m_isBndLoopOffset 决定
    IslLoop_Offset,
    /// 边界环和岛屿环同时偏置，受
    /// m_IsOstFixedNum 和m_OffsetNum 控制
    Simultaneously_Offset,
    ///模型轮廓加工
    ModelContour_Offset,
    ///平面样式铣削
    Plane_Style_Milling
};
enum MC_Strategy{
    ModelAreaClearing = 0,
    ModelContour,
    ContourFinishing
};
enum ToolPathType{
    BndOffsetPath = 0,  ///边界
    BndOftLocalPath,    ///边界局部
    IslOffsetPath,      ///岛屿
    IslOftLocalPath,    ///岛屿局部
    BndIslMerPath,      ///边界-岛屿
    IslIslMerPath,      ///岛屿-岛屿
    OriginBndPath,      ///输入边界
    OriginIslPath,      ///输入岛屿
    BlockPath,          ///干涉路径
    SimulateTool,       ///模拟刀具
    UnknowPath          ///未知类型
};
enum MovePathType{
    ProfileCurve = 0,   /// 轮廓曲线
    CutPathType,        /// 切削路径
    SafeHeighType,      /// 安全高度
    FastMoveType,       /// 快速路径
    SteerEntryType,      /// 切入路径
    SteerExitType,       /// 切出路径
    SkimPathType,       /// 跳过曲线
    UnknownPathType     /// 未知类型曲线
};
enum ParalCntType{SingleDirection = 0,DoubleDirection,AloneContour};
enum class JoinType{Round = 0,Square,Bevel,Miter};
enum class EndType{Polygon = 0,Joined,Butt,Square,Round,Open};
}
namespace Ofte = OffsetEnum;
#endif // ENUMTYPE_H
