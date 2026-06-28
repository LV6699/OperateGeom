#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
shared_ptr<ProIOInterface> win_IO = make_shared<ProIOInterface>();
shared_ptr<GeomCalculate>mwd_Go = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate> wind_num = make_shared<ElemCalculate>();
shared_ptr<DataEdit> win_dataEdit = make_shared<DataEdit>();
shared_ptr<CutParts> win_cut = make_shared<CutParts>();
Handle(AIS_Shape) m_curBlankShape;
UiSelShape m_uiCurSelShape;
Handle(AIS_InteractiveContext) m_uiOriContext;
long double Time_offset = 0,Time_equCur = 0,Time_oftSelInt = 0,
Time_smooth = 0,Time_boolUni = 0,Time_boolSub = 0,
Time_other = 0,Time_boolClip = 0,Time_total = 0;
MainWindow* _mainwind = nullptr;
ManipuWidget* test_window = nullptr;
AutoTestDialog* autoTestDialog = nullptr;
ViewData* viewData = nullptr;
vector<NodeShape>glb_vNodeOcc;
BndIslShape glb_bndIslShape;
bool m_isNewOpenFile = false;
vector<string> m_vFilePath;
bool m_isNewOstPathOperate = false;
vector<Quantity_Color>_colors;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _mainwind = this;
    ostDlg = new ManipuWidget(this);
    ///myOccView = new OCCView(this, ostDlg);
    myOccView = new OCCView(this);
    auto settings = ini_settings();
    settings->beginGroup("MainWindow");
    resize(settings->value("size", QSize(800, 700)).toSize());
    move(settings->value("pos", QPoint(0, 0)).toPoint());
    settings->endGroup();
    //myOccView->move(50, -50);
    setCentralWidget(myOccView);
    //setCentralWidget(ui->labDisplayImage);
    createActions();
    createMenus();
    myOccView->myView->SetProj(V3d_Zpos);
    ///myOccView->myView->SetProj(V3d_XposYnegZpos);
    ///auto context = _mainwind->get_OCCView()->getContext();
    ///context->SelectionStyle()->SetColor({0, 0.2, 0,Quantity_TOC_RGB});
    ///auto dialog = new DebugInfoDialog(this);
    auto viewDat = new ViewData(this);
    Quantity_Color green(0,0.25,0,Quantity_TOC_RGB);///0
    Quantity_Color blue(0,0,0.7,Quantity_TOC_RGB);///1
    Quantity_Color red(0.7,0,0,Quantity_TOC_RGB);///2
    Quantity_Color purple(0.6,0,0.6,Quantity_TOC_RGB);///3
    Quantity_Color yellow(0.6,0.6,0,Quantity_TOC_RGB);///4
    Quantity_Color cyan(0,0.5,0.5,Quantity_TOC_RGB);///5
    Quantity_Color orange(0.9,0.1,0,Quantity_TOC_RGB);///6
    Quantity_Color brown(0.5,0.2,0.0,Quantity_TOC_RGB);///7
    Quantity_Color black(0,0,0,Quantity_TOC_RGB);///8
    vector<Quantity_Color>colors{green,blue,red,purple,
                yellow,orange,cyan,brown,black};
    _colors = colors;
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
}
MainWindow::~MainWindow(){delete ui;}
void MainWindow::createActions(void)
{
    connect(ui->actionZoom, SIGNAL(triggered()), myOccView, SLOT(zoom()));
    connect(ui->actionMove, SIGNAL(triggered()), myOccView, SLOT(pan()));
    connect(ui->actionRotate, SIGNAL(triggered()), myOccView, SLOT(rotate()));
    connect(ui->actionReset, SIGNAL(triggered()), myOccView, SLOT(reset()));
    connect(ui->actionImportFiles, SIGNAL(triggered()), this, SLOT(readStep()));
    connect(ui->actionClear, SIGNAL(triggered()), this, SLOT(ClearModel()));
}
void MainWindow::keyPressEvent(QKeyEvent*event)
{
    /*setFocusPolicy((Qt::StrongFocus));
    installEventFilter(this);
    if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
                myOccView->myContext->ShiftSelect(true);
    }*/
}
void MainWindow::createMenus(void)
{
    QToolBar* aToolBar = addToolBar(tr("主功能"));
    aToolBar->addAction(ui->actionImportFiles);
    aToolBar->addAction(ui->actionRotate);
    aToolBar->addAction(ui->actionMove);
    aToolBar->addAction(ui->actionReset);
    aToolBar->addAction(ui->actionClear);
    aToolBar = addToolBar(tr("偏置算法测试"));
    aToolBar->addAction(ui->actOstTest);
    aToolBar->addAction(ui->actionSplitShape);
}
void MainWindow::on_actionSplitShape_triggered()
{
    _mainwind->myOccView->getContext()->Activate(AIS_Shape::SelectionMode(TopAbs_EDGE));
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    auto settings = ini_settings();
    settings->beginGroup("MainWindow");
    settings->setValue("size", size());
    settings->setValue("pos", pos());
    settings->endGroup();
    if(test_window){
        test_window->close();
    }
    if(autoTestDialog){
        //autoTestDialog->close();
    }
    QMainWindow::closeEvent(event);
}
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Version 1.0"),
                       tr("<h2>Welcome to this application.</h2><p>This "
                          "application is developed based on Qt and "
                          "OpenCASCADE under Linux operating system."));
}
void MainWindow::ClearModel() //清除模型数据
{
    myOccView->myRemoveall();
    emit sendIsExistModelFile(NOTEXIST_MODELFILR);
}
void MainWindow::initialViewShape(STEPControl_Reader& reader)
{
    myOccView->m_viewShape = new AIS_Shape(reader.OneShape());
}
void MainWindow::beforeReadStep()
{
    m_vFilePath.clear();
    myOccView->myRemoveall();
    emit sendIsExistModelFile(NOTEXIST_MODELFILR);
}
void MainWindow::readStep()  ///读取文件
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(QStringLiteral("Hello"));
    fileDialog->setDirectory("1");///home/jlc/AlgTestFile
    fileDialog->setNameFilter(tr("Step file(*.step *.stp);;Stl file(*.stl *.stls);;"
                                 "Igs file(*.igs *.iges);;All file (*.*)"));
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);
    int index = 1;
    QStringList fileNames;
    beforeReadStep();
    if (fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
        myOccView->myRemoveall();
        for  (int i=0;i< fileNames.size();i++)
        {
            loadStep(fileNames[i]);
        }
    }
    afterReadStep();
}
void MainWindow::loadStep(QString fileName)
{
    myOccView->myRemoveall();
    static int FileIndex = 0;
    m_vFilePath.push_back(fileName.toStdString());
    QString var = fileName;
    if (var.right(3).toUpper() == "IGS" || var.right(4).toUpper() == "IGES")
    {
        qDebug()<<fileName<<endl;
        IGESControl_Reader reader;
        ///reader.Shape();
        auto status = reader.ReadFile(var.toStdString().c_str());
        if (status == IFSelect_RetDone)
        {
            reader.TransferRoots();
            ///initialViewShape(reader);
            Handle(AIS_Shape) anshape = new AIS_Shape(reader.OneShape());
            anshape->SetColor(Quantity_NOC_STEELBLUE);
            anshape->SetHilightMode(1);
            Graphic3d_MaterialAspect smaterial;
            smaterial.SetMaterialType(Graphic3d_MATERIAL_PHYSIC);
            smaterial.SetShininess(0.5);
            myOccView->getContext()->SetMaterial(anshape, smaterial, Standard_False);
            myOccView->getContext()->Display(anshape, Standard_True);
            Handle(Prs3d_Drawer)t_hilight_style = myOccView->getContext()->HighlightStyle(Prs3d_TypeOfHighlight_LocalSelected);
            t_hilight_style->SetMethod(Aspect_TOHM_COLOR);
            t_hilight_style->SetColor(Quantity_NOC_GREEN);
            t_hilight_style->SetDisplayMode(1);
            myOccView->getContext()->SetHighlightStyle(t_hilight_style);
            myOccView->getContext()->Activate(anshape, 4);
            //myOccView->m_viewShape->SetTransparency(0.2);
            myOccView->fitAll();
        }
    }
    else {
        QFileInfo fileInfo = QFileInfo(var);
        QString fileName = fileInfo.fileName();
        QString filePath = fileInfo.absolutePath();
        QString fileSuffix = fileInfo.suffix();
        if(fileSuffix=="step" || fileSuffix=="STEP"
                || fileSuffix=="stp"  || fileSuffix=="STP"){
            STEPControl_Reader reader;
            auto status = reader.ReadFile(var.toStdString().c_str());
            if (status == IFSelect_RetDone) {
                reader.TransferRoots();
                initialViewShape(reader);
                ///Handle(AIS_Shape) anshape = new AIS_Shape(reader.OneShape());
                myOccView->m_viewShape->SetColor(Quantity_NOC_STEELBLUE);
                myOccView->m_viewShape->SetHilightMode(1);
                Graphic3d_MaterialAspect smaterial;
                smaterial.SetMaterialType(Graphic3d_MATERIAL_PHYSIC);
                smaterial.SetShininess(0.5);
                myOccView->getContext()->SetMaterial(myOccView->m_viewShape, smaterial, Standard_False);
                myOccView->getContext()->Display(myOccView->m_viewShape, Standard_True);
                Handle(Prs3d_Drawer)t_hilight_style = myOccView->getContext()->HighlightStyle(Prs3d_TypeOfHighlight_LocalSelected);
                t_hilight_style->SetMethod(Aspect_TOHM_COLOR);
                t_hilight_style->SetColor(Quantity_NOC_GREEN);
                t_hilight_style->SetDisplayMode(1);
                myOccView->getContext()->SetHighlightStyle(t_hilight_style);
                myOccView->getContext()->Activate(myOccView->m_viewShape, 4);
                //myOccView->m_viewShape->SetTransparency(0.2);
                myOccView->fitAll();
                /////旋转实体
            }
            FileIndex ++;
            qDebug()<<fileName;
            m_asSavePath = filePath.toStdString() + "/" + fileName.toStdString();
        }
        if(fileSuffix=="stl" || fileSuffix=="STL" ||
                fileSuffix=="stls" || fileSuffix=="STLS"){
            StlAPI_Reader aReader_Stl;
            TopoDS_Shape shape_Stl;
            aReader_Stl.Read(shape_Stl, var.toStdString().c_str());
            Handle(AIS_Shape) shape_ = new AIS_Shape(shape_Stl);
            shape_->SetColor(Quantity_NOC_CHOCOLATE1);
            myOccView->getContext()->Display(shape_, Standard_True);
            FileIndex ++;
            qDebug()<<fileName;
        }
    }
}
void MainWindow::afterReadStep()
{
    m_isNewOpenFile = true;
    emit sendIsExistModelFile(EXIST_MODELFILR);
}
void MainWindow::reLeadingModelFile(string path)
{
    myOccView->myRemoveall();
    emit sendIsExistModelFile(NOTEXIST_MODELFILR);
    static int FileIndex = 0;
    ///QString var = fileNames[i];
    QString var = QString::fromStdString(path);
    if (var.right(3).toUpper() == "IGS" || var.right(4).toUpper() == "IGES"){
        qDebug()<<var<<endl;
        IGESControl_Reader reader;
        ///reader.Shape();
        auto status = reader.ReadFile(var.toStdString().c_str());
        if (status == IFSelect_RetDone)
        {
            reader.TransferRoots();
            ///initialViewShape(reader);
            Handle(AIS_Shape) anshape = new AIS_Shape(reader.OneShape());
            anshape->SetColor(Quantity_NOC_STEELBLUE);
            anshape->SetHilightMode(1);
            Graphic3d_MaterialAspect smaterial;
            smaterial.SetMaterialType(Graphic3d_MATERIAL_PHYSIC);
            smaterial.SetShininess(0.5);
            myOccView->getContext()->SetMaterial(anshape, smaterial, Standard_False);
            myOccView->getContext()->Display(anshape, Standard_True);
            Handle(Prs3d_Drawer)t_hilight_style = myOccView->getContext()->HighlightStyle(Prs3d_TypeOfHighlight_LocalSelected);
            t_hilight_style->SetMethod(Aspect_TOHM_COLOR);
            t_hilight_style->SetColor(Quantity_NOC_GREEN);
            t_hilight_style->SetDisplayMode(1);
            myOccView->getContext()->SetHighlightStyle(t_hilight_style);
            myOccView->getContext()->Activate(anshape, 4);
            //myOccView->m_viewShape->SetTransparency(0.7);
            myOccView->fitAll();
        }
    }
    else{
        QFileInfo fileInfo = QFileInfo(var);
        QString fileName = fileInfo.fileName();
        QString filePath = fileInfo.absolutePath();
        QString fileSuffix = fileInfo.suffix();
        if(fileSuffix=="step" || fileSuffix=="stp" ||
                fileSuffix=="STEP" || fileSuffix=="STP"){
            STEPControl_Reader reader;
            auto status = reader.ReadFile(var.toStdString().c_str());
            if (status == IFSelect_RetDone){
                reader.TransferRoots();
                initialViewShape(reader);
                //myOccView->m_viewShape->SetTransparency(0.7);
                ///Handle(AIS_Shape) anshape = new AIS_Shape(reader.OneShape());
                myOccView->m_viewShape->SetColor(Quantity_NOC_STEELBLUE);
                myOccView->m_viewShape->SetHilightMode(1);
                Graphic3d_MaterialAspect smaterial;
                smaterial.SetMaterialType(Graphic3d_MATERIAL_PHYSIC);
                smaterial.SetShininess(0.5);
                myOccView->getContext()->SetMaterial(myOccView->m_viewShape, smaterial, Standard_False);
                myOccView->getContext()->Display(myOccView->m_viewShape, Standard_True);
                Handle(Prs3d_Drawer)t_hilight_style = myOccView->getContext()->HighlightStyle(Prs3d_TypeOfHighlight_LocalSelected);
                t_hilight_style->SetMethod(Aspect_TOHM_COLOR);
                t_hilight_style->SetColor(Quantity_NOC_GREEN);
                t_hilight_style->SetDisplayMode(1);
                myOccView->getContext()->SetHighlightStyle(t_hilight_style);
                myOccView->getContext()->Activate(myOccView->m_viewShape, 4);
                myOccView->fitAll();
                /////旋转实体
            }
            FileIndex ++;
            qDebug()<<"num:"<<FileIndex<<" Open the stp file>>>>"
                   <<"Path:"<<filePath<<","<<"FileName:"<<fileName;
            m_asSavePath = filePath.toStdString() + "/" + fileName.toStdString();
        }
        if(fileSuffix=="stl" || fileSuffix=="STL"){
            StlAPI_Reader aReader_Stl;
            TopoDS_Shape shape_Stl;
            aReader_Stl.Read(shape_Stl, var.toStdString().c_str());
            Handle(AIS_Shape) shape_ = new AIS_Shape(shape_Stl);
            shape_->SetColor(Quantity_NOC_CHOCOLATE1);
            myOccView->getContext()->Display(shape_, Standard_True);
            FileIndex ++;
            qDebug()<<"num:"<<FileIndex<<" Open the stl file>>>>"
                   <<"Path:"<<filePath<<","<<"FileName:"<<fileName;
        }
    }
    m_isNewOpenFile = true;
    emit sendIsExistModelFile(EXIST_MODELFILR);
}
void MainWindow::DisplayOffsetNodeLoopData(shared_ptr<OffsetNode> rootNode,
                                           bool isToolPath,
                                           int curNodeIdx,
                                           bool isView)
{
    if(rootNode == nullptr){
        return;
    }
    NodeShape curNodeShape;
    GetOccShapeData(rootNode->_loops,isToolPath,curNodeIdx,
                    curNodeShape);
    glb_vNodeOcc.push_back(curNodeShape);
    for (int i = 0; i < rootNode->_nodes.size(); i++) {
        curNodeIdx ++;
        DisplayOffsetNodeLoopData(rootNode->_nodes[i],
                                  isToolPath,curNodeIdx,isView);
    }
}
///显示偏置算法计算结果,bool 值控制显示还是隐藏
void MainWindow::GetOccShapeData(vector<DefLoop>vLoopData,
                                 bool isToolPath,int nodeIdx,
                                 NodeShape& curNodeShape)
{
    vector<LoopShape>vLoopShape;
    for(int lo = 0;lo < vLoopData.size();lo++){
        LoopShape loopShape;
        GetOCCViewShape(vLoopData[lo],isToolPath,
                        nodeIdx,loopShape);
        loopShape.m_loop = vLoopData[lo];
        if(!loopShape.m_vShape.empty()){
            vLoopShape.push_back(loopShape);
        }
    }
    curNodeShape.m_nodeIdx = nodeIdx;
    curNodeShape.m_vLoopShape = vLoopShape;
}
void MainWindow::TranslateEleToAIS_Shape(const DefElem& ele,
                                         Quantity_Color color,
                                         double width,bool& isSucccess,
                                         Handle(AIS_Shape)&shape)
{
    isSucccess = false;
    if(ele._type == LINETYPE){
        bool isSame = false;
        mwd_Go->IsIdenticalPoint(ele._segment._segSp,ele._segment._segEp,PreErr_10,isSame);
        if(!wind_num->IsValidPoint(ele._segment._segSp) ||
                !wind_num->IsValidPoint(ele._segment._segEp) ||
                !wind_num->IsValidPoint(ele._segment._segEp)){
            //return;
        }
        if(isSame ){
            return;
        }
        gp_Pnt startP(ele._segment._segSp._x,ele._segment._segSp._y,ele._segment._segSp._z);
        gp_Pnt endP(ele._segment._segEp._x,ele._segment._segEp._y,ele._segment._segEp._z);
        TopoDS_Edge ost_Edge = BRepBuilderAPI_MakeEdge(startP, endP);
        shape = new AIS_Shape(ost_Edge);
        shape->SetColor(color);
        shape->SetWidth(width);
    }
    else if(ele._type == ARCTYPE){
        Point startPnt = ele._arc._arcSp;
        Point endPnt = ele._arc._arcEp;
        Point centerPnt = ele._arc._arcCp;
        bool iscw = ele._arc._isCW;
        double radius = ele._arc._arcR;
        double dis0,dis1,dis2;
        mwd_Go->TwoPointDistance(startPnt,endPnt,dis0);
        mwd_Go->TwoPointDistance(startPnt,centerPnt,dis1);
        mwd_Go->TwoPointDistance(endPnt,centerPnt,dis2);
        if(dis0 < PreErr5_7 || dis1 < PreErr5_7 || dis2 < PreErr5_7){
            return;
            gp_Pnt centerCdt (centerPnt._x,centerPnt._y,centerPnt._z);
            gp_Circ circle(gp_Ax2(centerCdt, gp_Dir(0, 0, 1)), radius);
            Handle_Geom_Circle geomCircle = new Geom_Circle(circle);
            TopoDS_Edge circleEdge = BRepBuilderAPI_MakeEdge(geomCircle);
            shape = new AIS_Shape(circleEdge);
        }
        if(!wind_num->IsValidPoint(startPnt) || !wind_num->IsValidPoint(endPnt)){
            //return;
        }
        gp_Pnt startP(startPnt._x,startPnt._y,startPnt._z);
        gp_Pnt endP(endPnt._x,endPnt._y,endPnt._z);
        gp_Pnt centerP(centerPnt._x,centerPnt._y,centerPnt._z);
        /*radius = sqrt(pow(startPnt._x - centerPnt._x, 2) +
                          pow(startPnt._y - centerPnt._y, 2));*/
        gp_Circ circle(gp_Ax2(centerP, gp_Dir(0, 0, 1)), radius);
        if(iscw){
            std::swap(startP, endP);
        }
        GC_MakeArcOfCircle arcMaker(circle, startP, endP, true);
        Handle(Geom_TrimmedCurve) trimmedCurve = arcMaker.Value();
        BRepBuilderAPI_MakeEdge edgeMaker(trimmedCurve);
        shape = new AIS_Shape(edgeMaker);
        shape->SetColor(color);
        shape->SetWidth(width);
    }
    else if(ele._type == CIRCLETYPE){
        double radius = ele._circle._cirR;
        gp_Pnt centerCdt (ele._circle._cirCp._x,
                          ele._circle._cirCp._y,
                          ele._circle._cirCp._z);
        gp_Circ circle(gp_Ax2(centerCdt, gp_Dir(0, 0, 1)), radius);
        Handle_Geom_Circle geomCircle = new Geom_Circle(circle);
        TopoDS_Edge circleEdge = BRepBuilderAPI_MakeEdge(geomCircle);
        shape = new AIS_Shape(circleEdge);
        shape->SetColor(color);
        shape->SetWidth(width);
    }
    isSucccess = true;
}
void MainWindow::GetOCCViewShape(const DefLoop& loop,bool isToolPath,
                                 int nodeIdx,LoopShape& loopShape)
{
    double r = 0.8,r1 = 0;
    double g = 0.0,g1 = 0.25;
    double b = 0.0,b1 = 0;
    double width = 1;
    double width1 = 1.5;
    Quantity_Color defColor(r, g, b, Quantity_TOC_RGB);
    Quantity_Color defColor1(r1, g1, b1, Quantity_TOC_RGB);
    /*if(loop.m_vElem.size() == 8){
        gp_Pnt sp(340.225,19.298,0);
        BRepBuilderAPI_MakeVertex vertex(sp);
        auto p_test = new AIS_Shape(vertex);
        myOccView->myContext->Display(p_test, true);
    }*/
    for(int i = 0;i < loop.m_vElem.size();i++){
        bool isSucccess = false;
        DefElem ele = loop.m_vElem[i];
        Handle(AIS_Shape) AIS_Face;
        if(isToolPath){
            TranslateEleToAIS_Shape(ele,defColor1,width1,isSucccess,AIS_Face);
        }
        else{
            TranslateEleToAIS_Shape(ele,defColor,width,isSucccess,AIS_Face);
        }
        if(!isSucccess){
            continue;
        }
        record_map[AIS_Face].index = i;
        EleShape occData(loop.m_vElem[i],AIS_Face,i,nodeIdx);
        loopShape.m_vShape.push_back(occData);
    }
}
void MainWindow::ViewOccNode(bool isUpdateViewer)
{
    if(glb_vNodeOcc.empty()){
        return;
    }
    myOccView->myView->FitAll(0.05);
    for(int n = 0;n < glb_vNodeOcc.size();n ++){
        NodeShape curNode = glb_vNodeOcc[n];
        for(int lo = 0 ;lo < curNode.m_vLoopShape.size();lo ++){
            LoopShape loopShape = curNode.m_vLoopShape[lo];
            for(int i = 0;i < loopShape.m_vShape.size();i ++){
                myOccView->getContext()->Display(loopShape.m_vShape[i].m_occShape,
                                                 isUpdateViewer);
            }
        }
        //usleep(100);
        myOccView->getContext()->UpdateCurrentViewer();
        myOccView->myView->FitAll(0.05);
    }
    myOccView->getContext()->UpdateCurrentViewer();
}
void MainWindow::ReceiveTranseformMachCdt()
{
    if(m_vFilePath.size() == 0){
        return;
    }/*
    if (access(m_asSavePath.c_str(), F_OK) != 0){
        return;
    }*/
    Handle(AIS_Shape) transeShape;
    myOccView->RotationShape(/*m_vFilePath[0],*/transeShape);
    //TopTools_HSequenceOfShape ss = transeShape->Shape();
    TopoDS_Shape shape = transeShape->Shape();
    /// 存放图形的容器
    Handle(TopTools_HSequenceOfShape) aHSequenceOfShape = new TopTools_HSequenceOfShape;
    aHSequenceOfShape->Clear();
    aHSequenceOfShape->Append(shape);
    IFSelect_ReturnStatus status;
    /// 保存文件
    STEPControl_StepModelType aValue = STEPControl_AsIs;
    STEPControl_Writer aWriter;
    for (Standard_Integer i = 1; i <= aHSequenceOfShape->Length(); i++){
        ///转换三维模型到aWriter
        status = aWriter.Transfer(aHSequenceOfShape->Value(i), aValue);
        if (status != IFSelect_RetDone)
            std::cout << "转化失败" << std::endl;
    }
    ///保存数据到磁盘
    Standard_CString cc = m_asSavePath.c_str();
    status = aWriter.Write(cc);
    myOccView->myRemoveall();
    ///readStep();
    reLeadingModelFile(m_asSavePath);
    m_asSavePath = "";
}
void MainWindow::on_actionImportFiles_triggered()
{
    //record_map.clear();
}
void MainWindow::on_actOstTest_triggered()
{
    ManipuWidget *ostDlg = new ManipuWidget(this);
    ostDlg->show();
    test_window = ostDlg;
    ostDlg->setAttribute(Qt::WA_DeleteOnClose);
    AutoTestDialog* autoTest = new AutoTestDialog();
    connect(ostDlg,SIGNAL(upDataDialUiData(const int&,const int&,
                                           const double& )),
            autoTest,SLOT(ReceiveOffsetVal(const int&,const int&,
                                           const double& )));
    connect(ostDlg,SIGNAL(SendTranseformMachCdt()),
            this,SLOT(ReceiveTranseformMachCdt()));
    connect(ostDlg,SIGNAL(SignalDisplaySelData(BndIslPair,bool,bool)),
            this,SLOT(DisplayUiSelShape(BndIslPair,bool,bool)));
    connect(ostDlg,SIGNAL(SignalFileIdxPartModel(QString)),
            this,SLOT(loadStep(QString)));
    connect(ostDlg,SIGNAL(SignalDisplayLoopData(const vector<DefLoop>&,bool,bool)),
            this,SLOT(SlotDisplayLoopData(const vector<DefLoop>&,bool,bool)));
    connect(ostDlg,SIGNAL(SignalCreateDisplayBlank(string,double,double,bool)),
            this,SLOT(SlotCreateDisplayBlank(string,double,double,bool)));
    connect(ostDlg,SIGNAL(SignalRemoveBlank()),this,SLOT(SlotRemoveBlank()));
    connect(ostDlg,SIGNAL(SignalDisplayToolPath(const shared_ptr<OffsetNode>&,
                                                const shared_ptr<OffsetNode>&)),
            this,SLOT(SlotDisplayToolPath(const shared_ptr<OffsetNode>&,
                                          const shared_ptr<OffsetNode>&)));
    connect(ostDlg,SIGNAL(SignalViewCutContour(const BndIslPair&)),
            this,SLOT(SlotViewCutContourData(const BndIslPair&)));
    connect(ostDlg,SIGNAL(SignalViewToolPath(const BndIslPair&,
                                             const shared_ptr<OffsetNode>&)),
            this,SLOT(SlotViewToolPathData(const BndIslPair&,
                                           const shared_ptr<OffsetNode>&)));
    connect(ostDlg,SIGNAL(SignalScreen(string,string)),this,
            SLOT(SlotScreen(string,string)));
    connect(ostDlg,SIGNAL(SignalSwitchWindDisplay(bool,string)),this,
            SLOT(SlotSwitchWindDisplay(bool,string)));
    connect(this->myOccView,SIGNAL(SelectOccDefEle(const DefElem&)),
            ostDlg,SLOT(SlotGetUiSelPairLoop(const DefElem&)));
    if(autoTestDialog == nullptr){
        autoTestDialog = new AutoTestDialog(this);
    }
}
void MainWindow::DisplayUiSelShape(const BndIslPair& bndIslPair,
                                   bool isChangeSel,bool isView)
{
    if(isView){
        myOccView->getContext()->EraseAll(true);
        m_uiCurSelShape.m_vSelBndShape.clear();
        m_uiCurSelShape.m_vSelIslShape.clear();
        LoopShape loopShape;
        GetOCCViewShape(bndIslPair.m_bndLoop,false,0,loopShape);
        vector<vector<Handle(AIS_Shape)>>vIslShape;
        for(int lo = 0;lo < bndIslPair.m_vIslLoop.size();lo ++){
            LoopShape loopSha;
            vector<Handle(AIS_Shape)>vCurIslShape;
            GetOCCViewShape(bndIslPair.m_vIslLoop[lo],false,0,loopSha);
            for(int i = 0;i < loopSha.m_vShape.size();i++){
                vCurIslShape.push_back(loopSha.m_vShape[i].m_occShape);
            }
            vIslShape.push_back(vCurIslShape);
        }
        for(int i = 0;i < loopShape.m_vShape.size();i++){
            m_uiCurSelShape.m_vSelBndShape.
                    push_back(loopShape.m_vShape[i].m_occShape);
        }
        ///m_uiCurSelShape.m_vSelBndShape = vBndShape;
        m_uiCurSelShape.m_vSelIslShape = vIslShape;
        for(auto& it : m_uiCurSelShape.m_vSelBndShape){
            myOccView->getContext()->Display(it,false);
        }
        for(auto& lo : m_uiCurSelShape.m_vSelIslShape){
            for(auto& it : lo){
                myOccView->getContext()->Display(it,false);
            }
        }
        myOccView->getContext()->UpdateCurrentViewer();
    }
    else{
        for(auto& it : m_uiCurSelShape.m_vSelBndShape){
            myOccView->getContext()->Remove(it,false);
        }
        for(auto& lo : m_uiCurSelShape.m_vSelIslShape){
            for(auto& it : lo){
                myOccView->getContext()->Remove(it,false);
            }
        }
        myOccView->getContext()->DisplayAll(false);
        myOccView->getContext()->UpdateCurrentViewer();
        if(myOccView != nullptr){
            ///setCentralWidget(myOccView);
        }
    }
}
void MainWindow::DisplayPairLoop(const BndIslPair &bndIslPair,
                                 bool isClear,bool isToolPath)
{
    if(isClear){
        myOccView->myRemoveall();
    }
    LoopShape loopShape;
    GetOCCViewShape(bndIslPair.m_bndLoop,
                    false,0,loopShape);
    BndIslShape temBndIslShape;
    glb_bndIslShape = temBndIslShape;
    glb_bndIslShape.m_bndShape = loopShape;
    for(int lo = 0;lo < bndIslPair.m_vIslLoop.size();lo ++){
        LoopShape loopSha;
        GetOCCViewShape(bndIslPair.m_vIslLoop[lo],false,
                        0,loopSha);
        glb_bndIslShape.m_vIslShape.push_back(loopSha);
    }
    myOccView->myView->FitAll(0.05);
    for(auto& it : glb_bndIslShape.m_bndShape.m_vShape){
        myOccView->getContext()->Display(it.m_occShape,false);
    }
    for(auto& lo : glb_bndIslShape.m_vIslShape){
        for(auto& data : lo.m_vShape){
            myOccView->getContext()->Display(data.m_occShape,false);
        }
    }
    myOccView->myView->FitAll(0.05);
    myOccView->getContext()->UpdateCurrentViewer();
}
void MainWindow::SlotDisplayLoopData(const vector<DefLoop>&vLoopData,
                                     bool isToolPath,bool isClearOther)
{
    if(isClearOther){
        myOccView->myRemoveall();
    }
    vector<Handle(AIS_Shape)>vLoopShape;
    for(int lo = 0;lo < vLoopData.size();lo++){
        LoopShape loopShape;
        GetOCCViewShape(vLoopData[lo],isToolPath,0,loopShape);
        for(int i = 0;i < loopShape.m_vShape.size();i++){
            vLoopShape.push_back(loopShape.m_vShape[i].m_occShape);
        }
    }
    for(auto& it : vLoopShape){
        myOccView->getContext()->Display(it,false);
    }
}
void MainWindow::SlotCreateDisplayBlank(string path,double xExp,
                                        double yExp,bool isClearShape)
{
    myOccView->getContext()->Remove(m_curBlankShape,Standard_True);
    TopoDS_Shape blankShape;
    win_cut->CreateBlanck(path,xExp,yExp,blankShape);
    m_curBlankShape = new AIS_Shape(blankShape);
    m_curBlankShape->SetTransparency(0.8);
    myOccView->getContext()->Display(m_curBlankShape,true);
}
void MainWindow::SlotDisplayToolPath(const shared_ptr<OffsetNode>&oriRootNode,
                                     const shared_ptr<OffsetNode>&rootNode)
{
    ///glb_vOriShape.clear();
    glb_bndIslShape.m_bndShape.m_vShape.clear();
    glb_bndIslShape.m_vIslShape.clear();
    glb_vNodeOcc.clear();
    myOccView->myRemoveall();
    myOccView->myView->SetProj(V3d_Zpos);
    DisplayOffsetNodeLoopData(oriRootNode,false,0,true);
    ViewOccNode(false);
    glb_vNodeOcc.clear();
    DisplayOffsetNodeLoopData(rootNode,true,0,true);
    ViewOccNode(false);
}
void MainWindow::SlotRemoveBlank()
{
    myOccView->getContext()->Remove(m_curBlankShape,Standard_True);
    myOccView->getContext()->UpdateCurrentViewer();
}
void MainWindow::SlotViewCutContourData(const BndIslPair& pairLoop)
{
    myOccView->myRemoveall();
    DisplayPairLoop(pairLoop,true,false);
}
void MainWindow::SlotViewToolPathData(const BndIslPair&oriPairLoop,
                                      const shared_ptr<OffsetNode>&rootNode)
{
    glb_bndIslShape.m_bndShape.m_vShape.clear();
    glb_bndIslShape.m_vIslShape.clear();
    glb_vNodeOcc.clear();
    myOccView->myView->SetProj(V3d_Zpos);
    DisplayPairLoop(oriPairLoop,true,false);
    DisplayOffsetNodeLoopData(rootNode,true,0,true);
    ViewOccNode(false);
}
///DefineDisplay
void MainWindow::SlotScreen(string saveDir, string name)
{
    QString file = QString::fromStdString(saveDir) + "/" +
            QString::fromStdString(name);
    /*screen->grabWindow(0).save(file,"jpg");*/
    //QPixmap p = this->grab(QRect(0,0,400,460));
    //QPixmap pixmap = QPixmap::grabWidget(this);
    auto p = QGuiApplication::primaryScreen()->grabWindow(this->winId(),
                                                          this->get_OCCView()->pos().x(),
                                                          this->get_OCCView()->pos().y(),
                                                          this->get_OCCView()->width(),
                                                          this->get_OCCView()->height()).toImage();
    if(!p.save(file,"jpg")){
        std::cout<<"save widget screen failed"<<endl;
    }
}
void MainWindow::SlotSwitchWindDisplay(bool isDisplayOcc,
                                       string imgPath)
{
    if(isDisplayOcc){
        setCentralWidget(myOccView);
        return;
    }/*
    if (access(imgPath.c_str(), F_OK) != 0){
        return;
    }*/
    QLabel *labImg = new QLabel;
    labImg->setGeometry(QRect(0,0,1000,800));
    QImage *image = new QImage();
    image->load(QString::fromStdString(imgPath));
    labImg->setPixmap(QPixmap::fromImage(*image).scaled(labImg->size()));
    setCentralWidget(labImg);
}
std::shared_ptr<QSettings> ini_settings() {
    return make_shared<QSettings>("jlc_software", "camAlgTest");
}