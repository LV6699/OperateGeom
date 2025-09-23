#ifndef ENUMTYPE_H
#define ENUMTYPE_H

namespace OffsetEnum {
enum EBlankType
{
    CUBOID,
    CYLINDERBLANK
};
enum ProfShape
{
    BLANK,
    RECTANGLE,
    CIRCLE,
    TRIANGLE,
    DIAGONAL
};
enum ElemType
{
    LINETYPE,
    ARCTYPE,
    CIRCLETYPE
};
enum ProfType  // contoure type
{
    OUTSIDEPROFILE,
    WITHINPROFILE,
};
enum ContourClass{
    BLANK_CT_CLASS = 0,  // blank
    OFFSET_CT_CLASS  //offset loop
};
enum ProfElemErrorType{
    PROFELEMNUMERROR = 0,
    PROFELEMTYPEERROR
};
enum GetProfType{
    ORIGINPROF = 0,
    ITEROFFSETPROF,
    ITERSELFITEROFFSETPROF,
    OFFSETLOOPCHECK
};
enum RotDir {
    CLOCKWISE = 0,
    ANTICLOCKWISE
};
enum FindPointDir{
    CLOSERPOINT = 0,
    FARTHERAWAYPOINT
};
enum IncreaseSEPoint{
    STARTPOINTINCREAS = 0,
    ENDPOINTINCREASE
};
enum SetWhichCircle{
    PREVIOUSCIECLE = 0,
    LATTERCIRCLE
};
enum IsExistedFrontPoint{
    EXISTETFRONTPOINT = 0,
    NONEXISTFRONTPOINT
};
enum IsExistedStartPointOnCircle{
    EXISTETVALIDSTARRPOINTONCIRCLE = 0,
    NONEXISTVALIDSTARRPOINTONCIRCLE
};
enum SelfInterLCReferPoint{
    LINEINARCFRONT = 0,
    ARCINLINEFRONT
};
enum SigCirRelativePos{
    SIGINCIR_FRONT = 0,
    CIRINLINE_FRONT
};
enum DefineXYZDir{
    XDIR = 0,
    YDIR ,
    ZDIR
};
enum ProfElemNoneInterHandleWay{
    LCLNONEINTER = 0,
    LCCNONEINTER,
    CCLNONEINTER,
    CCCNONEINTER
};
enum CalTrigFunType{
    SINCALType = 0,
    COSCALType ,
    TANCALType
};
enum AngleWhichQuadrant{
    FIRSTQUADRANT = 0,
    SECONDQUADRANT,
    THIRDQUADRANT,
    FOURTHTQUADRANT,
    XPOSITIVE_AXIS,
    XNEGATIVE_AXIS,
    YPOSITIVE_AXIS,
    YNEGATIVE_AXIS
};
enum OffsetResultCheckType{
    OFFSETPRELATCHECK = 0,
    SELFINTERHANDLECHECK,
    GETCLOSEPROFCHECK
};
enum SegmentType{
    HORIZONTALLINE = 0,
    VERTICALLINE,
    DIAGONALLINE
};
enum RelativeLocationType{
    ABOVELOCATED = 0,
    BELOWLOCATED,
    LEFTLOCATED,
    RIGHTLOCATED,
    LEFTABOVELOCATED,
    LEFTBELOWLOCATED,
    RIGHTABOVELOCATED ,
    RIGHTBELOWLOCATED
};
enum ProfElemTrendDir{
    HORIZONTAL_DIR_TREND = 0,
    VERTICAL_DIR_TREND,
    DIAGONAL_DIR_TREND
};
enum WhichProfElem{
    FRONTPROFELEM = 0,
    BACKPROFELEM,
    UNABLEJUDGE
};
enum PathProfElemType{
    OUTPUT_SIGMENT,
    OUTPUT_ARC_SIGMENT
};
enum LineStrechDir{
    LINE_STARTPOINT_DIR = 0,
    LINE_ENDPOINT_DIR
};
enum OffsetIterNumIsInt{
    INT_OFFSETITERNUM = 0,
    DECIMAL_OFFSETITERNUM
};
enum ElemsInterPType{
    NEGATIVE_INTERP = 0,
    ONELEM_INTERP,
    POSITIVE_INTERP,
};
enum AddPreNextNewArc{
    PREADDNEWARC = 0,
    NEXTADDNEWARC
};
enum EleConnectType{
    LLL_CONNECT,
    LLC_CONNECT,
    CLL_CONNECT,
    CLC_CONNECT,
    LCL_CONNECT,
    LCC_CONNECT,
    CCL_CONNECT,
    CCC_CONNECT,
};
enum LoopRelativePos{
    Mutual_Separation = 0,
    Partial_Intersecte,
    PartCoin_NoInt,
    Mutual_CoinCide,
    Mutual_Include,
};
enum EleIntPntType{
    NO_ENDPNT = 0,
    BND_ENDPNT,
    ISL_ENDPNT,
    BOTH_ENDPNT
};
enum PntOnEleType{
    NotOnEle = 0,
    OnEleStartPnt,
    OnEleEndPnt,
    WithinEleTwoPnt
};
enum OffsetStyle{
    /// 边界环偏置，岛屿环不偏置
    BndLoop_Offset,
    /// 岛屿环偏置，边界环是否偏置由m_isBndLoopOffset决定
    IslLoop_Offset,
    /// 边界环和岛屿环同时偏置，受
    /// m_IsOstFixedNum 和m_OffsetNum控制
    Simultaneously_Offset,
    ///模型轮廓加工
    ModelContour_Offset,
    ///平面样式铣削
    Plane_Style_Milling
};
enum EleCntType{
    CreateConnectArc = 0,
    CalculateIntersect,
    ChangeEndpoint,
    FontEleInvalid,
    BackEleInvalid,
    DoNothing
};
enum MC_Strategy{
    ModelAreaClearing = 0,
    ModelContour,
    ContourFinishing
};
enum SuitableCutEle{
    JudgeCurCutEle = 0,
    JudgeNexCutEle,
    JudgeAnoCurCutEle,
    JudgeAnoNexCutEle
};
enum EleLoopDisType{
    EntiretyOutside = 0,
    PartOutside,
    EntiretyInside
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
enum LoopLimitRange{
    BoundaryLimitRange = 0,
    IslandLimitRange,
    BlockLimitRange
};
enum ElePntPosition
{
    ElementStartPnt = 0,
    ElementMidPnt,
    ElementEndPnt,
    UseDefinePoint
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
}

#endif // ENUMTYPE_H


