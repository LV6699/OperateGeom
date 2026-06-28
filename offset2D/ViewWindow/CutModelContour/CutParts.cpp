#include "CutParts.h"
shared_ptr<GeomCalculate>cut_go = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate>cut_num = make_shared<ElemCalculate>();
shared_ptr<CutModelSolve>cut_cutsol = make_shared<CutModelSolve>();
///零件剖分，参数为文件名，层的数量，剖分结果，剖分结果转换成点（此参数无作用）
void CutParts::CutPartIntoLoopData(const string &file,
                                   double xExp, double yExp,
                                   const int &layTotal,
                                   bool& isCutSuccess,
                                   vector<vector<BndIslPair>>&vAreaLayData)
{
    /*isCutSuccess = true;
    shared_ptr<Coordinate> wcs = make_shared<Coordinate>();
    shared_ptr<PartManager> partManager = make_shared<PartManager>(wcs);
    partManager->AddPart(file);
    ///double offsetMargin[6] = {-10, -10, 0, 10, 10, 0};
    if(xExp < 5){
        xExp = 10;
    }
    if(yExp < 5){
        yExp = 10;
    }
    double offsetMargin[6] = {-xExp, -yExp, 0, xExp, yExp, 0};
    partManager->vPartInfoList[0]->CreateBoundBox(offsetMargin);
    double z_min = partManager->vPartInfoList[0]->modelMin.Z();
    double z_max = partManager->vPartInfoList[0]->modelMax.Z();
    double stepDown = (z_max - z_min) / layTotal;
    //double stepDown = 3;
    ///设置参数
    shared_ptr<ModelAreaClearParam> pMachiningParam = make_shared<ModelAreaClearParam>();
    pMachiningParam->setCutStepDownVal(stepDown);
    ///模型区域清除策略
    shared_ptr<Rough_ModelAreaClearing> operation =
            make_shared<Rough_ModelAreaClearing>(partManager, "粗加工");
    operation->setMachiningParam(pMachiningParam);
    auto partInfo = partManager->vPartInfoList[0];
    vector<vector<shared_ptr<JLC_MachiningArea>>> vAreaData;
    try{
        vAreaData = operation->GetAllProcessAreaBySlices(partInfo->stockMin.Z(),
                                                         partInfo->stockMax.Z());
    }catch (...) {
        ///QMessageBox::warning(this,"剖分失败","剖分失败");
        isCutSuccess = false;
        return;
    }
    for(int areaIdx = 0;areaIdx < vAreaData.size();areaIdx ++){
        if(vAreaData.size() == 0){
            areaIdx ++;
            continue;
        }
        ///vector<pair<DefLoop,vector<DefLoop>>> vLayData;
        vector<BndIslPair> vLayData;
        for(int layIdx = 0;layIdx < vAreaData[areaIdx].size();layIdx ++){
            auto curLoop = vAreaData[areaIdx][layIdx];
            if(curLoop->boundaryLoop->vCurveList.size() == 0){
                continue;
            }
            DefLoop bndLoop;
            for(int i = 0;i < curLoop->boundaryLoop->vCurveList.size();i++){
                JLC_Curve* curve = curLoop->boundaryLoop->vCurveList[i];
                Point starP(curve->startPnt.X(), curve->startPnt.Y(), curve->startPnt.Z());
                Point endP(curve->endPnt.X(), curve->endPnt.Y(), curve->endPnt.Z());
                if(curve->curveType == LINE_TYPE){///直线
                    DefSeg  line(starP,endP);
                    bndLoop.m_vElem.push_back(DefElem(LINETYPE, line));
                }
                else if (curve->curveType == ARC_TYPE){
                    Point centerP(curve->circleCenter.X(),
                                  curve->circleCenter.Y(),
                                  curve->circleCenter.Z());
                    RotDir rot = curve->isClockwiseArc ? CLOCKWISE : ANTICLOCKWISE;
                    DefArc arcData(starP,endP,centerP,rot);
                    bndLoop.m_vElem.push_back(DefElem(ARCTYPE, arcData));
                }
                else if(curve->curveType == CIRCLE_TYPE){
                    Point centerPnt(curve->circleCenter.X(),
                                    curve->circleCenter.Y(),
                                    curve->circleCenter.Z());
                    double radius = curve->circleRadius;
                    DefCircle cirData(centerPnt,radius);
                    bndLoop.m_vElem.push_back(DefElem(CIRCLETYPE, cirData));
                }
            }
            vector<DefLoop> vIslLoop;
            for(int i = 0;i < curLoop->vIslandsLoop.size();i++){
                DefLoop islLoop;
                islLoop.SetLoopType(WITHINPROFILE);
                int ele = 0;
                while (ele < curLoop->vIslandsLoop[i]->vCurveList.size()) {
                    if((curLoop->vIslandsLoop[i]->vCurveList.size() == 2) &&
                            curLoop->vIslandsLoop[i]->vCurveList[0]->curveType == ARC_TYPE &&
                            curLoop->vIslandsLoop[i]->vCurveList[1]->curveType == ARC_TYPE){
                        auto curve1 = curLoop->vIslandsLoop[i]->vCurveList[ele];
                        auto curve2 = curLoop->vIslandsLoop[i]->vCurveList[ele];
                        Point startPnt1(curve1->startPnt.X(), curve1->startPnt.Y(),
                                        curve1->startPnt.Z());
                        Point endPnt1(curve1->endPnt.X(), curve1->endPnt.Y(),
                                      curve1->endPnt.Z());
                        Point cirCdt1(curve1->circleCenter.X(), curve1->circleCenter.Y(),
                                      curve1->circleCenter.Z());
                        Point startPnt2(curve2->startPnt.X(), curve2->startPnt.Y(),
                                        curve2->startPnt.Z());
                        Point endPnt2(curve2->endPnt.X(), curve2->endPnt.Y(),
                                      curve2->endPnt.Z());
                        Point cirCdt2(curve2->circleCenter.X(), curve2->circleCenter.Y(),
                                      curve2->circleCenter.Z());
                        bool isCirType = false;
                        DefCircle cirData;
                        TwoArcConvertCircle(startPnt1,endPnt1,cirCdt1,
                                            startPnt2,endPnt2,cirCdt2,
                                            isCirType,cirData);
                        if(isCirType){
                            islLoop.m_vElem.push_back(DefElem(CIRCLETYPE, cirData));
                            break;
                        }
                    }
                    auto curve = curLoop->vIslandsLoop[i]->vCurveList[ele];
                    Point starP(curve->startPnt.X(), curve->startPnt.Y(), curve->startPnt.Z());
                    Point endP(curve->endPnt.X(), curve->endPnt.Y(), curve->endPnt.Z());
                    if(curve->curveType == LINE_TYPE){///直线
                        DefSeg  line(starP,endP);
                        islLoop.m_vElem.push_back(DefElem(LINETYPE, line));
                    }
                    else if (curve->curveType == ARC_TYPE){
                        Point centerP(curve->circleCenter.X(),
                                      curve->circleCenter.Y(),
                                      curve->circleCenter.Z());
                        RotDir rot = curve->ArcIsClockwise() ? CLOCKWISE : ANTICLOCKWISE;
                        DefArc arcData(starP,endP,centerP,rot);
                        islLoop.m_vElem.push_back(DefElem(ARCTYPE, arcData));
                    }
                    else if(curve->curveType == CIRCLE_TYPE){
                        Point centerPnt(curve->circleCenter.X(),curve->circleCenter.Y(),
                                        curve->circleCenter.Z());
                        double radius = curve->circleRadius;
                        DefCircle cirData(centerPnt,radius);
                        islLoop.m_vElem.push_back(DefElem(CIRCLETYPE, cirData));
                        break;
                    }
                    ele ++;
                }
                if(!islLoop.m_vElem.empty()){
                    vIslLoop.push_back(islLoop);
                }
            }
            ///pair<DefLoop,vector<DefLoop>>bndIslPairLoop (bndLoop,vIslLoop);
            BndIslPair biLoop(bndLoop,vIslLoop);
            vLayData.push_back(biLoop);
        }
        if (vLayData.size() > 0)
            vAreaLayData.push_back(vLayData);
    }*/
}
void CutParts::TwoArcConvertCircle(const Point& arcSp1,
                                   const Point& arcEp1,
                                   const Point& cirCdt1,
                                   const Point& arcSp2,
                                   const Point& arcEp2,
                                   const Point& cirCdt2,
                                   bool& isCircle,
                                   DefCircle& cirData)
{
    isCircle = false;
    double cirDis;
    cut_go-> TwoPointDistance(cirCdt1,cirCdt2,cirDis);
    if(cirDis > PreErr_3){
        return;
    }
    double radius1_1,radius1_2,radius1,radius2_1,radius2_2,radius2;
    cut_go-> TwoPointDistance(cirCdt1,arcSp1,radius1_1);
    cut_go-> TwoPointDistance(cirCdt1,arcEp1,radius1_2);
    radius1 = radius1_1 >= radius1_2 ? radius1_1 : radius1_2;
    cut_go-> TwoPointDistance(cirCdt2,arcSp2,radius2_1);
    cut_go-> TwoPointDistance(cirCdt2,arcEp2,radius2_2);
    radius2 = radius2_1 >= radius2_2 ? radius2_1 : radius2_2;
    Point cdt((cirCdt1._x + cirCdt2._x) / 2,
              (cirCdt1._y + cirCdt2._y) / 2,
              (cirCdt1._z + cirCdt2._z) / 2);
    double R = radius1 >= radius2 ? radius1 : radius2;
    cirData._cirCp = cdt;
    cirData._cirR = R;
    isCircle = true;
}
void CutParts::HandleCircleSameCdt(vector<DefLoop>&vIslLoop)
{
    if(vIslLoop.size() < 2){
        return;
    }
    int i = 0,nexIdx = 0;
    while (i < vIslLoop.size()) {
        if(i < vIslLoop.size() - 1){
            nexIdx = i + 1;
        }
        else{
            nexIdx = 0;
        }
        if(vIslLoop[i].m_vElem.size() == 1 &&
                vIslLoop[i].m_vElem[0]._type == CIRCLETYPE &&
                vIslLoop[nexIdx].m_vElem.size() == 1 &&
                vIslLoop[nexIdx].m_vElem[0]._type == CIRCLETYPE){
            double cirDis;
            cut_go-> TwoPointDistance(vIslLoop[i].m_vElem[0]._circle._cirCp,
                    vIslLoop[nexIdx].m_vElem[0]._circle._cirCp,cirDis);
            if(cirDis < PreErr_1){
                if(vIslLoop[i].m_vElem[0]._circle._cirR >=
                        vIslLoop[nexIdx].m_vElem[0]._circle._cirR){
                    vIslLoop.erase(vIslLoop.begin() + nexIdx);
                }
                else{
                    vIslLoop.erase(vIslLoop.begin() + i);
                }
                i --;
                continue;
            }
        }
        i ++;
    }
}
void CutParts::CutPartNameIntoData(double xExp,double yExp,
                                   CutPartPara cutPara,
                                   CutPartResult& cutResultData)
{
    vector<QString>vPartFile;
    if(cutPara.m_isBatchCut){
        vPartFile = cutPara.m_vPartFile;
    }
    else{
        vPartFile.push_back(cutPara.m_partFile);
    }
    int i = 0,sucNum = 0,faiNum = 0;
    vector<PartCutData> vCutPartResult;
    vector<string> vCutFailurePart;
    while (i < vPartFile.size()) {
        string curFile = vPartFile[i].toStdString();
        ///vector<vector<pair<DefLoop,vector<DefLoop>>>> vMulIslandLoop;
        vector<vector<BndIslPair>>vAreaLayData;
        bool isCutSuc = false;
        CutPartIntoLoopData(curFile,xExp,yExp,cutPara.m_layTotal,
                            isCutSuc,vAreaLayData);
        if(!isCutSuc || vAreaLayData.empty()){
            vCutFailurePart.push_back(curFile);
            faiNum ++;
            i ++;
            continue;
        }
        sucNum ++;
        PartCut partLoop;
        for(int a = 0;a < vAreaLayData.size();a ++){
            PartAreaCut cutArea;
            for(int lay = 0;lay < vAreaLayData[a].size();lay ++){
                BndIslPair pair;
                pair.m_partName = curFile;
                PartLayCut cutLay;
                cutLay.m_vPairLoop.push_back(vAreaLayData[a][lay]);
                cutArea.m_vLayData.push_back(cutLay);
            }
            partLoop.m_vAreaData.push_back(cutArea);
        }
        PartCutData cutPart(curFile,partLoop);
        vCutPartResult.push_back(cutPart);
        i ++;
    }
    CutPartResult cutResult(sucNum,faiNum,vCutPartResult,
                            vCutFailurePart);
    cutResultData = cutResult;
}
void CutParts::CutSingleModelFile(CutModelInfo modelInfo,bool &isCutSuccess,
                                  vector<vector<BndIslPair>>&vAreLayData)
{
    isCutSuccess = false;/**
    if (access(modelInfo.m_modelPath.c_str(), F_OK) != 0){
        cout<<"err--->当前剖分模型文件不存在!->" << modelInfo.m_modelPath<<endl;
        return;
    }*/
    CutPartIntoLoopData(modelInfo.m_modelPath,modelInfo.m_xExp,modelInfo.m_yExp,
                        modelInfo.m_layNum,isCutSuccess,vAreLayData);
}
void CutParts::BatchCutModelFile(const vector<CutModelInfo>& vModelFile,
                                 vector<BndIslPair>& vModelLoop)
{
    int successNum = 0,failureNum = 0;
    vector<PartCutData> vCutPartResult;
    vector<string> vCutFailurePart;
    for (int m = 0; m < vModelFile.size(); ++m) {
        CutModelInfo modInfo = vModelFile[m];/**
        if (access(modInfo.m_modelPath.c_str(), F_OK) != 0){
            cout<<"err--->当前剖分模型文件不存在!->"<< modInfo.m_modelPath<<endl;
            continue;
        }*/
        bool isCutSuccess = false;
        vector<vector<BndIslPair>>vAreaLayData;
        CutSingleModelFile(modInfo,isCutSuccess,vAreaLayData);
        if(!isCutSuccess || vAreaLayData.empty()){
            failureNum ++;
            vCutFailurePart.push_back(modInfo.m_modelName);
            //cout<<"err--->当前剖分模型文件剖分失败!->"<< modInfo.m_modelPath<<endl;
            continue;
        }
        successNum ++;
        PartCut partLoop;
        for(int a = 0;a < vAreaLayData.size();a ++){
            PartAreaCut cutArea;
            for(int lay = 0;lay < vAreaLayData[a].size();lay ++){
                BndIslPair pair;
                pair.m_partName = modInfo.m_modelName;
                PartLayCut cutLay;
                cutLay.m_vPairLoop.push_back(vAreaLayData[a][lay]);
                cutArea.m_vLayData.push_back(cutLay);
            }
            partLoop.m_vAreaData.push_back(cutArea);
        }
        PartCutData cutPart(modInfo.m_modelName,partLoop);
        vCutPartResult.push_back(cutPart);
    }
}
void CutParts::CreateBlanck(string path,double xExp,double yExp,
                            TopoDS_Shape& blankShape)
{
/*    shared_ptr<Coordinate> wcs = make_shared<Coordinate>();
    shared_ptr<PartManager> partManager = make_shared<PartManager>(wcs);
    partManager->AddPart(path);
    double offsetMargin[6] = {-xExp,-yExp,0,xExp,yExp,0};
    auto partInfo = partManager->vPartInfoList[0];
    partInfo->CreateBoundBox(offsetMargin);
    blankShape = partInfo->stockShape;*/
}