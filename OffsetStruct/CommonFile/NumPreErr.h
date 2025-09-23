#ifndef DEFNUMPRE_H
#define DEFNUMPRE_H

#define PreErr_100_ 10.0
#define PreErr_90_ 9.0
#define PreErr_80_ 8.0
#define PreErr_70_ 7.0
#define PreErr_60_ 6.0
#define PreErr_50_ 5.0
#define PreErr_40_ 4.0
#define PreErr_30_ 3.0
#define PreErr_20_ 2.0
#define PreErr_10_ 1.0
#define PreErr_9_ 0.9
#define PreErr_8_ 0.8
#define PreErr_7_ 0.7
#define PreErr_6_ 0.6
#define PreErr_5_ 0.5
#define PreErr_4_ 0.4
#define PreErr_3_ 0.3
#define PreErr_2_ 0.2
#define PreErr_1 1e-1
#define PreErr_2 1e-2
#define PreErr_3 1e-3
#define PreErr_4 1e-4
#define PreErr_5 1e-5
#define PreErr_6 1e-6
#define PreErr_7 1e-7
#define PreErr_8 1e-8
#define PreErr_9 1e-9
#define PreErr_10 1e-10
#define PreErr_11 1e-11
#define PreErr_12 1e-12
#define PreErr_13 1e-13
#define PreErr_14 1e-14
#define PreErr_15 1e-15
#define PreErr_16 1e-16
#define PreErr_17 1e-17
#define PreErr_18 1e-18
#define PreErr_19 1e-19
#define PreErr_20 1e-20
#define PreErr5_1 5e-1
#define PreErr5_2 5e-2
#define PreErr5_3 5e-3
#define PreErr5_4 5e-4
#define PreErr5_5 5e-5
#define PreErr5_6 5e-6
#define PreErr5_7 5e-7
#define PreErr5_8 5e-8
#define PreErr5_9 5e-9
#define PreErr5_10 5e-10
#define PreErr5_11 5e-11
#define PreErr5_12 5e-12
#define PreErr5_13 5e-13
#define PreErr5_14 5e-14
#define PreErr5_15 5e-15
#define PreErr5_16 5e-16
#define PreErr5_17 5e-17
#define PreErr5_18 5e-18
#define PreErr5_19 5e-19
#define PreErr5_20 5e-20
#define PreErr_30 1e-30
#define PreErr_80 1e-80
#define PreErr_100 1e-100
#define ErrTimes_5 5
#define ErrTimes_10 10
#define ErrTimes_20 20
#define ErrTimes_30 30
#define ErrTimes_40 40
#define ErrTimes_50 50
#define ErrTimes_60 60
#define ErrTimes_70 70
#define ErrTimes_80 80
#define ErrTimes_90 90
#define ErrTimes_100 100
#define Max_Val 1e10
#define Min_Val -1e10
#define PreprocValLim PreErr_8_
#define OftValLimit PreErr5_4
#define _AlgRad 57.29577951308232087679  ///180/PI
#define _RadAlg 0.017453292519943295769  ///PI/180
///Smooth
#define SmoothAngle 170
#define SmoothTimes 2.5
#define SmoothExtrOnOri PreErr5_3
#define SmoothValLimit PreErr5_3
#define SmoothExtrDistance PreErr5_3
#define OftValMultiplier 1.01
#define OftValCornorMultiplier 0.75
#define OftValDiamMultiplier 0.75
#define LastOftValTimes 0.85
///几何相交,直线-圆,圆-圆之间是否存在相交
#define LC_ExiIn_Err PreErr_11
#define LC_InOnePnt_Err PreErr_10
#define CC_ExiIn_Err PreErr_11
#define CC_InOnePnt_Err PreErr_10
#define ArcArc_SameExtr_Err PreErr_10
#define ArcLin_SameExtr_Err PreErr_10
#define LimCoorIniVal -1e5
///基础计算
#define Base_AngBetweenAng_Err PreErr_6
#define Base_LineHor_Err PreErr_6
#define Base_LineVer_Err PreErr_6
#define Base_SameSlope_Err PreErr_6
#define Base_Extr_Err PreErr5_10
#define Base_LoInLo_Err PreErr5_8
#define Fitting_LenLim 3
#define Fitting_DisErr PreErr5_1
#define Fitting_AngVarErr 2
///自相交
#define SeIn_Dis_Err PreErr_5
#define SeIn_Extr_Err PreErr5_7
#define SeIn_IntExtr_Err PreErr5_8
#define SeIn_EleLen_Lim PreErr5_3
///点是否在线段上
#define OnSeg_Err PreErr_10
///点是否在圆弧上
#define OnArc_AngErr PreErr5_9   ///角度,这里角度的变化往往接近端点误差
#define OnArc_DisErr PreErr_11   ///点与圆心距离
#define OnArc_IsExtrPt_Err PreErr_10  ///点是否是圆弧端点
///点是否在环内
#define InPol_HorOrdi_Err PreErr_6 ///线段是否水平
#define InPol_OnSig_Err PreErr_10  ///是否在线段上
#define InPol_IsArcExtrPt_Err PreErr_6  ///是否是圆弧端点
#define InPol_IsSegExtrPt_Err PreErr_6  ///是否是线段端点
#define InPol_ArcExtrStep_Err PreErr_6_   ///靠近圆弧端点角度
#define InPol_PntIsMaxOrdi_Err PreErr_4   ///是否是最大y座标点
///输入输出处理
#define Pre_ParallSeg_Err PreErr_4  ///线段是否平行
#define Pre_SameCirPt_Err PreErr5_6  ///是否相同圆心
#define Pre_SameCirR_Err PreErr5_6  ///是否相同半径
#define Pre_HorVerSig_Err PreErr_4  ///是否是垂直水平线段
#define Pre_SigCoin_Err PreErr_10  ///线段是否存在重合
///等距曲线
#define Oft_EleTan_Err PreErr5_10
#define Oft_CircleVal_Err PreErr5_8  ///圆有效性
#define Oft_ArcAngVal_Err PreErr_6  ///圆弧角度有效性
#define Oft_LineVal_Err PreErr_6  ///线段长度有效性
#define Oft_CntArcAngVal_Err PreErr_4 ///连接圆弧角度限制
#define Oft_CntArcVal_Err PreErr_4  ///连接圆弧端点距离
#define Oft_Emb_ProjPtOnSig_Err PreErr_4
#define Oft_CntType_ExtrDis_Err PreErr_4
#define Oft_EleExtrDis_Err PreErr_10
#define Oft_CalIntExtDis_Err PreErr5_2
///多边形裁剪
#define PolCli_ValCutEleLen_Err PreErr5_10
#define PolyCli_IntPtIsExtr_Err PreErr5_9 ///交点是元素端点
#define PolyClip_PtInVecPt_Err PreErr_6
#define PolyClip_TwoIntPtExtrPt_Err PreErr_6 ///两个交点是元素端点
#define PolyClip_TanIntExtrPt_Err PreErr_6  ///相切时是元素端点
#define PolyCli_IsTanIn_Err PreErr5_8  ///是否相切相交
#define PolyClip_CutEleVality_Err PreErr5_6  ///是否相切相交
///两相交元素进行裁剪时,如果两相交元素长度低于该值,则相交无效,即作不相交处理
#define PolyClip_IntCutEleLenLim_Err PreErr5_5
///判断裁剪元素长度限制
#define PolCli_CutEleLen_Err PreErr_8_
///处理相交点是元素端点时交点有效性判断
#define PolCli_IntPntExtr_Err PreErr_6
///相交元素长度有效性判断
#define PolCli_IntEleLneVal_Err PreErr5_6
#define PolCli_CutValSigLen_Err PreErr_8
#define PolCli_CutValArcLen_Err PreErr_8


#endif // DEFNUMPRE_H
