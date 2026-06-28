#include"OCCView.h"
#include<string>
///shared_ptr<ViewData> occ_vie = make_shared<ViewData>();
//DefElem m_occCurSelectEle;
static Handle(Graphic3d_GraphicDriver) & GetGraphicDriver()
{
    static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
    return aGraphicDriver;
}
OCCView::OCCView(QWidget *parent)
    : QWidget(parent),
      myXmin(0),
      myYmin(0),
      myXmax(0),
      myYmax(0),
      //myCurrentMode(CurAction3d_DynamicRotation),
      myCurrentMode(CurAction3d_DynamicPanning),
      myDegenerateModeIsOn(Standard_True),
      myRectBand(NULL)
{
    setBackgroundRole(QPalette::NoRole);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setMouseTracking(true);
    init();
}
void OCCView::init()
{
    Handle(Aspect_DisplayConnection) aDisplayConnection = new Aspect_DisplayConnection();
    // Get graphic driver if it exists, otherwise initialise it
    if (GetGraphicDriver().IsNull())
    {
        GetGraphicDriver() = new OpenGl_GraphicDriver(aDisplayConnection);
    }
    // Get window handle. This returns something suitable for all platforms.
    WId window_handle = (WId)winId();
#ifdef WNT
    Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle) window_handle);
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
    Handle(Cocoa_Window) wind = new Cocoa_Window((NSView *) window_handle);
#else
    Handle(OcctWindow) wind = new OcctWindow(this);
#endif
    Aspect_Handle aWindowHandle = (Aspect_Handle)winId();
    myViewer = new V3d_Viewer(GetGraphicDriver());
    myView = myViewer->CreateView();
    myView->SetWindow(wind);
    if (!wind->IsMapped()){
        wind->Map();
    }
    myContext = new AIS_InteractiveContext(myViewer);
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();
    Quantity_Color defColor(0.1, 0.2, 0.3, Quantity_TOC_RGB);
    //Quantity_Color blue = Quantity_Color(40 / 255.0, 90 / 255.0, 100 / 255.0, Quantity_TOC_RGB);
    //Quantity_Color blue = Quantity_Color(0.4, 0.5, 0.6, Quantity_TOC_RGB);
    myView->SetBgGradientColors(defColor, Quantity_NOC_WHITE, Aspect_GFM_VER);
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.1, V3d_ZBUFFER);
    myContext->SetDisplayMode(AIS_Shaded, Standard_True);
    myContext->SetAutomaticHilight(Standard_True);
    myView->SetZoom(1);
    Qt::Corner m_viewTrihedronCorner = Qt::TopRightCorner;
    opencascade::handle<AIS_ViewCube> aisViewCube = new AIS_ViewCube;
    aisViewCube->SetBoxColor(Quantity_NOC_GRAY75);
    // aisViewCube->SetFixedAnimationLoop(false);
    aisViewCube->SetSize(55);
    aisViewCube->SetFontHeight(12);
    aisViewCube->SetTransformPersistence(
                new Graphic3d_TransformPers(
                    Graphic3d_TMF_TriedronPers,
                    toOccCorner(m_viewTrihedronCorner),
                    Graphic3d_Vec2i(85, 85)));
    myContext->Display(aisViewCube, false);
    const Handle_Prs3d_DatumAspect &datumAspect = aisViewCube->Attributes()->DatumAspect();
    datumAspect->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED2);
    datumAspect->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN2);
    datumAspect->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE2);
}
Aspect_TypeOfTriedronPosition OCCView::toOccCorner(Qt::Corner corner)
{
    switch (corner)
    {
    case Qt::TopLeftCorner:
        return Aspect_TOTP_LEFT_UPPER;
    case Qt::TopRightCorner:
        return Aspect_TOTP_RIGHT_UPPER;
    case Qt::BottomLeftCorner:
        return Aspect_TOTP_LEFT_LOWER;
    case Qt::BottomRightCorner:
        return Aspect_TOTP_RIGHT_LOWER;
    }
    return Aspect_TOTP_LEFT_UPPER; // Fallback
}
const Handle(AIS_InteractiveContext) & OCCView::getContext() const
{
    return myContext;
}
// 清理所有的模型，仅保留axiscube
void OCCView::myRemoveall()
{
    myContext->RemoveAll(Standard_True);
    Qt::Corner m_viewTrihedronCorner = Qt::TopRightCorner;
    opencascade::handle<AIS_ViewCube> aisViewCube = new AIS_ViewCube;
    aisViewCube->SetBoxColor(Quantity_NOC_GRAY75);
    // aisViewCube->SetFixedAnimationLoop(false);
    aisViewCube->SetSize(55);
    aisViewCube->SetFontHeight(12);
    aisViewCube->SetTransformPersistence(
                new Graphic3d_TransformPers(
                    Graphic3d_TMF_TriedronPers,
                    toOccCorner(m_viewTrihedronCorner),
                    Graphic3d_Vec2i(85, 85)));
    myContext->Display(aisViewCube, false);
    const Handle_Prs3d_DatumAspect &datumAspect = aisViewCube->Attributes()->DatumAspect();
    datumAspect->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED2);
    datumAspect->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN2);
    datumAspect->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE2);
    myView->Redraw();
    myContext->ShiftSelect(true);
}
void OCCView::CreateViewDir()
{
    Qt::Corner m_viewTrihedronCorner = Qt::TopRightCorner;
    opencascade::handle<AIS_ViewCube> aisViewCube = new AIS_ViewCube;
    aisViewCube->SetBoxColor(Quantity_NOC_GRAY75);
    /// aisViewCube->SetFixedAnimationLoop(false);
    aisViewCube->SetSize(35);
    aisViewCube->SetFontHeight(8);
    aisViewCube->SetTransformPersistence(
                new Graphic3d_TransformPers(
                    Graphic3d_TMF_TriedronPers,
                    toOccCorner(m_viewTrihedronCorner),
                    Graphic3d_Vec2i(85, 85)));
    myContext->Display(aisViewCube, false);
    const Handle_Prs3d_DatumAspect &datumAspect = aisViewCube->Attributes()->DatumAspect();
    datumAspect->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED2);
    datumAspect->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN2);
    datumAspect->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE2);
    myView->Redraw();
    myContext->ShiftSelect(true);
}
/*!
Get paint engine for the OpenGL viewer. [ virtual public ]
*/
QPaintEngine *OCCView::paintEngine() const{return 0;}
void OCCView::paintEvent(QPaintEvent * /*theEvent*/)
{
    myView->Redraw();
}
void OCCView::resizeEvent(QResizeEvent * /*theEvent*/)
{
    if (!myView.IsNull())
    {
        myView->MustBeResized();
    }
}
void OCCView::fitAll(void)
{
    myView->FitAll(0.05);
  
}
void OCCView::reset(void){myView->Reset();}
void OCCView::pan(void)
{
    myCurrentMode = CurAction3d_DynamicPanning;
}
void OCCView::zoom(void)
{
    myCurrentMode = CurAction3d_DynamicZooming;
    //myCurrentMode == myCurrentMode?
}
void OCCView::rotate(void)
{
    myCurrentMode = CurAction3d_DynamicRotation;
}
///选择模式，鼠标点击模型，选中对应的面
void OCCView::selectMode(Handle(AIS_Shape) ){}
///获取鼠标选中的对象
void OCCView::getShape()
{
    /* if(myContext.IsNull()){
        return;
    }
    ///myContext->Deactivate();///将模型激活分裂
    ///myContext->InitSelected();
    TopoDS_Shape abc = myContext->DetectedShape();
    TopAbs_ShapeEnum bba = abc.ShapeType();
    Handle(AIS_Shape)  tt = new AIS_Shape(abc);
    qDebug() << bba;*/
}
Standard_Boolean OCCView::SurfaceCenterNormal(TopoDS_Shape& aShape,
                                              gp_Vec& aCNormal,
                                              gp_Pnt& aCenterPoint)
{
    if(myContext.IsNull()){
        return Standard_False;
    }
    ///myContext->Deactivate();///将模型激活分裂
    ///myContext->InitSelected();
    TopoDS_Face  aFace = TopoDS::Face(aShape);
    ///TopAbs_ShapeEnum bba = aFace.ShapeType();
    ///Handle(AIS_Shape)  tt = new AIS_Shape(aFace);
    ///qDebug() << bba;
    if (aFace.IsNull()){
        return Standard_False;
    }
    BRepAdaptor_Surface aSurface(aFace);
    ///获取曲面最大最小UV 值
    Standard_Real aUmin = aSurface.FirstUParameter();
    Standard_Real aUmax = aSurface.LastUParameter();
    Standard_Real aVmin = aSurface.FirstVParameter();
    Standard_Real aVmax = aSurface.LastVParameter();
    ///曲面中心点UV 值
    Standard_Real aMidU = (aUmax + aUmin) / 2;
    Standard_Real aMidV = (aVmax + aVmin) / 2;
    gp_Vec aV1, aV2;
    ///面中心点坐标和此点切线向量
    aSurface.D1(aMidU, aMidV, aCenterPoint, aV1, aV2);
    ///向量叉乘，即中心点法向，也是逻辑方向
    aCNormal = aV1.Crossed(aV2);
    ///法相量的模
    double dMagnitude = aCNormal.Magnitude();
    ///无效相量
    if (dMagnitude <= 0.0){
        return Standard_False;
    }
    ///Orientation 获取曲面相对性实体模型的方向，指向实体模型外部
    ///TopAbs_REVERSED 逻辑方向与曲面方向相反
    ///TopAbs_FORWARD 逻辑方向与曲面方向相同
    if (aFace.Orientation() == TopAbs_REVERSED){
        aCNormal.Reverse();///相量反向
    }
    return Standard_True;
}
void OCCView::GetRotationParameters()
{
    TopoDS_Shape aShape = myContext->DetectedShape();
    AIS_Shape shape = AIS_Shape(aShape);
    //Handle(AIS_Shape) hShape = new AIS_Shape(shape);
    AIS_StatusOfPick pickStatus = myContext->SelectDetected();
    if(pickStatus == AIS_SOP_Error ||
            pickStatus == AIS_SOP_NothingSelected){
        emit SignalPrintSelLoop(shape);
        return;
    }
    else if(myContext->DetectedShape().IsNull()){
        emit SignalPrintSelLoop(shape);
        return;
    }
    else if(aShape.ShapeType() == TopAbs_EDGE){
        emit SignalPrintSelLoop(shape);
        return;
    }
    SurfaceCenterNormal(aShape,m_vecNormal,m_normalCenterPnt);
}
void OCCView::RotationShape(Handle(AIS_Shape)& transeShape)
{
    gp_Trsf theTransformation;
    ///指定旋转轴
    gp_Dir dir(m_vecNormal);
    gp_Ax1 axe(m_normalCenterPnt, dir);
    ///旋转角度（单位：弧度）
    theTransformation.SetRotation(axe, 90.0 * M_PI / 180);
    TopoDS_Shape toShape = m_viewShape->Shape();
    BRepBuilderAPI_Transform myBRepTrans(toShape/*anshape*/,
                                        theTransformation);
    ///获取旋转转换的新的拓扑
    TopoDS_Shape S2 = myBRepTrans.Shape();
    m_viewShape = new AIS_Shape(S2);
    transeShape = m_viewShape;
}
///高亮显示
void OCCView::setHightStyle(){}
///选择样式
void OCCView::setSelectStyle(){}
void OCCView::mousePressEvent(QMouseEvent *theEvent)
{
    if (theEvent->button() == Qt::LeftButton){
        onLButtonDown((theEvent->buttons() | theEvent->modifiers()),
                      theEvent->pos());
        if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
            //myContext->ShiftSelect(true);
            //occ_vie->SignalViewSelShapeInfo();
            //emit selectionChanged();
        }
        else if (QApplication::keyboardModifiers() == Qt::ShiftModifier) {
        }
        else{
            myContext->Select(true);
        }
       myContext->InitSelected();
    }
    else if (theEvent->button() == Qt::MidButton)
    {
        onMButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton)
    {
        onRButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
}
void OCCView::mouseReleaseEvent(QMouseEvent *theEvent)
{
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
        if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
            //myContext->ShiftSelect(true);
            myContext->ShiftSelect(false);
            emit selectionChanged();
        }
        getShape();
    }
    else if (theEvent->button() == Qt::MidButton)
    {
        onMButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton)
    {
        onRButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
}
#include <AIS_Axis.hxx>
void OCCView::TransUiSelShapeToEle()
{
    ///m_isIniSelEle = false;
    TopoDS_Shape occSelectShape = TopoDS_Shape();
    try {
        AIS_StatusOfPick pickStatus = myContext->SelectDetected();
        if (pickStatus <= 1) {
            return;
        }
        Handle_SelectMgr_SelectionManager aSelectionManager = myContext->SelectionManager();
        Handle(AIS_InteractiveObject) selectedObject;
        for (myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected()) {
            selectedObject = myContext->SelectedInteractive();
            if (selectedObject->IsKind(STANDARD_TYPE(AIS_Shape))) {
                Handle(AIS_Shape) aShape = Handle(AIS_Shape)::DownCast(selectedObject);
                //a_sel = Handle(AIS_Shape)::DownCast(selectedObject);
                occSelectShape = aShape->Shape();
                ///aShape->SetColor(Quantity_NOC_YELLOW);
            } else if (selectedObject->IsKind(STANDARD_TYPE(AIS_Axis))) {
                Handle(AIS_Axis) anAxis = Handle(AIS_Axis)::DownCast(selectedObject);
            }
            else {
            }
        }
    } catch (Standard_Failure& e) {
        std::cout << e.GetMessageString() << std::endl;
    }
    if(occSelectShape.ShapeType() != TopAbs_EDGE){
        return;
    }
    TopoDS_Edge selectEdge = TopoDS::Edge(occSelectShape);
    Standard_Real first = 0;
    Standard_Real last = 0;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(selectEdge,first,last);
    gp_Pnt p0 = curve->Value(first);
    gp_Pnt pMid = curve->Value((first+last)/2);
    gp_Pnt p1 = curve->Value(last);
    Point startPnt(p0.X(),p0.Y(),p0.Z()),
            endPnt(p1.X(),p1.Y(),p1.Z());
    bool iscw = true;
    if(curve->IsKind(STANDARD_TYPE(Geom_Line))){
        m_occCurSelectEle._type = LINETYPE;
        DefSeg sel_line(startPnt,endPnt);
        m_occCurSelectEle._segment = sel_line;
    }
    else if(curve->IsKind(STANDARD_TYPE(Geom_Circle))){
        auto circle = Handle(Geom_Circle)::DownCast(curve);
        gp_Pnt centerPnt = circle->Circ().Location();
        Point centerPnt_1(centerPnt.X(),centerPnt.Y(),centerPnt.Z());
        double dis = std::sqrt(pow(endPnt._x - startPnt._x,2) +
                               pow(endPnt._y - startPnt._y,2));
        if(dis < PreErr_2){
            m_occCurSelectEle._type = CIRCLETYPE;
            double radius = std::sqrt(pow(endPnt._x - centerPnt_1._x,2) +
                                      pow(endPnt._y - centerPnt_1._y,2));
            DefCircle cir(centerPnt_1,radius);
            m_occCurSelectEle._circle = cir;
        }
        else{
            DefArc arc(startPnt,endPnt,centerPnt_1,iscw);
            m_occCurSelectEle._type = ARCTYPE;
            m_occCurSelectEle._arc = arc;
        }
    }
    if(this->testDialog != nullptr) {
        //this->testDialog->isValid = true;
        //this->testDialog->selEleData = m_occCurSelectEle;
    }
    m_isIniSelEle = true;
    ///selEle = m_occCurSelectEle;
    emit SelectOccDefEle(m_occCurSelectEle);
}
void OCCView::mouseMoveEvent(QMouseEvent *theEvent)
{
    onMouseMove(theEvent->buttons(), theEvent->pos());
    //if((theEvent->buttons()&Qt::LeftButton) && (theEvent->buttons()&Qt::RightButton))
    if(theEvent->buttons()&Qt::RightButton)
    {
        myView->Pan(theEvent->pos().x()-m_x_max,m_y_max-theEvent->pos().y());
        m_x_max=theEvent->x();
        m_y_max=theEvent->y();
    }
}
void OCCView::wheelEvent(QWheelEvent *theEvent)
{
    onMouseWheel(theEvent->buttons(), theEvent->delta(), theEvent->pos());
}
void OCCView::onLButtonDown(const int theFlags, const QPoint thePoint)
{
    ///GetSelectShape();
    //DefElem selElel;
    TransUiSelShapeToEle();
    // Save the current mouse coordinate in min.
    myXmin = thePoint.x();
    myYmin = thePoint.y();
    myXmax = thePoint.x();
    myYmax = thePoint.y();
}
void OCCView::onMButtonDown(const int /*theFlags*/, const QPoint thePoint)
{
    // Save the current mouse coordinate in min.
    myXmin = thePoint.x();
    myYmin = thePoint.y();
    myXmax = thePoint.x();
    myYmax = thePoint.y();
    if (myCurrentMode == CurAction3d_DynamicRotation)
    {
        myView->StartRotation(thePoint.x(), thePoint.y());
    }
}
void OCCView::onRButtonDown(const int /*theFlags*/, const QPoint /*thePoint*/)
{
    GetRotationParameters();
}
void OCCView::onMouseWheel(const int /*theFlags*/, const int theDelta, const QPoint thePoint)
{
    Standard_Integer aFactor = 16;
    Standard_Integer aX = thePoint.x();
    Standard_Integer aY = thePoint.y();
    if (theDelta > 0)
    {
        aX += aFactor;
        aY += aFactor;
    }
    else
    {
        aX -= aFactor;
        aY -= aFactor;
    }
    myView->Zoom(thePoint.x(), thePoint.y(), aX, aY);
}
void OCCView::addItemInPopup(QMenu * /*theMenu*/){}
void OCCView::popup(const int /*x*/, const int /*y*/){}
void OCCView::onLButtonUp(const int theFlags, const QPoint thePoint)
{
    // Hide the QRubberBand
    if (myRectBand)
    {
        myRectBand->hide();
    }
    // Ctrl for multi selection.
    if (thePoint.x() == myXmin && thePoint.y() == myYmin)
    {
        if (theFlags & Qt::ControlModifier)
        {
            multiInputEvent(thePoint.x(), thePoint.y());
        }
        else
        {
            inputEvent(thePoint.x(), thePoint.y());
        }
    }
}
void OCCView::onMButtonUp(const int /*theFlags*/, const QPoint thePoint)
{
    if (thePoint.x() == myXmin && thePoint.y() == myYmin)
    {
        panByMiddleButton(thePoint);
    }
}
void OCCView::onRButtonUp(const int /*theFlags*/, const QPoint thePoint)
{
    popup(thePoint.x(), thePoint.y());
}
void OCCView::onMouseMove(const int theFlags, const QPoint thePoint)
{
    // Draw the rubber band.
    if (theFlags & Qt::LeftButton)
    {
        drawRubberBand(myXmin, myYmin, thePoint.x(), thePoint.y());
        dragEvent(thePoint.x(), thePoint.y());
    }
    // Ctrl for multi selection.
    if (theFlags & Qt::ControlModifier)
    {
        multiMoveEvent(thePoint.x(), thePoint.y());
    }
    else
    {
        moveEvent(thePoint.x(), thePoint.y());
    }
    // Middle button.
    if (theFlags & Qt::MidButton)
    {
        switch (myCurrentMode)
        {
        case CurAction3d_DynamicRotation:
            myView->Rotation(thePoint.x(), thePoint.y());
            break;
        case CurAction3d_DynamicZooming:
            myView->Zoom(myXmin, myYmin, thePoint.x(), thePoint.y());
            break;
        case CurAction3d_DynamicPanning:
            myView->Pan(thePoint.x() - myXmax, myYmax - thePoint.y());
            myXmax = thePoint.x();
            myYmax = thePoint.y();
            break;
        default:
            break;
        }
    }
}
void OCCView::dragEvent(const int x, const int y)
{
    myContext->Select(myXmin, myYmin, x, y, myView, Standard_True);
    emit selectionChanged();
}
void OCCView::multiDragEvent(const int x, const int y)
{
    myContext->ShiftSelect(myXmin, myYmin, x, y, myView, Standard_True);
    emit selectionChanged();
}
void OCCView::inputEvent(const int x, const int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    myContext->Select(Standard_True);
    emit selectionChanged();
}
void OCCView::multiInputEvent(const int x, const int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    myContext->ShiftSelect(Standard_True);
    emit selectionChanged();
}
void OCCView::moveEvent(const int x, const int y)
{
    myContext->MoveTo(x, y, myView, Standard_True);
}
void OCCView::multiMoveEvent(const int x, const int y)
{
    myContext->MoveTo(x, y, myView, Standard_True);
}
void OCCView::drawRubberBand(const int minX, const int minY, const int maxX, const int maxY)
{
    QRect aRect;
    // Set the rectangle correctly.
    (minX < maxX) ? (aRect.setX(minX)) : (aRect.setX(maxX));
    (minY < maxY) ? (aRect.setY(minY)) : (aRect.setY(maxY));
    aRect.setWidth(abs(maxX - minX));
    aRect.setHeight(abs(maxY - minY));
    if (!myRectBand)
    {
        myRectBand = new QRubberBand(QRubberBand::Rectangle, this);
        // setStyle is important, set to windows style will just draw
        // rectangle frame, otherwise will draw a solid rectangle.
        myRectBand->setStyle(QStyleFactory::create("windows"));
    }
    myRectBand->setGeometry(aRect);
    myRectBand->show();
}
void OCCView::panByMiddleButton(const QPoint &thePoint)
{
    Standard_Integer aCenterX = 0;
    Standard_Integer aCenterY = 0;
    QSize aSize = size();
    aCenterX = aSize.width() / 2;
    aCenterY = aSize.height() / 2;
    myView->Pan(aCenterX - thePoint.x(), thePoint.y() - aCenterY);
}