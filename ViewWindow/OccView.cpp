/*
* Copyright (c) 2018 Shing Liu All Rights Reserved.
*
* File : OccView.cpp
* Author : Shing Liu(eryar@163.com)
* Date : 2018-01-08 21:00
* Version : OpenCASCADE7.2.0 & Qt5.7.1
*
* Description : Qt widget for OpenCASCADE viewer.
*/
#include <OpenGl_GraphicDriver.hxx>
#include "OccView.h"
#include <QMenu>
#include <QMouseEvent>
#include <QRubberBand>
#include <QStyleFactory>
#include <V3d_View.hxx>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#pragma optimize("", off)
#ifdef WNT
#include <WNT_Window.hxx>
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
#include <Cocoa_Window.hxx>
#else
#undef Bool
#undef CursorShape
#undef None
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef Expose
#include <Xw_Window.hxx>
#endif
static Handle(Graphic3d_GraphicDriver)& GetGraphicDriver()
{
    static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
    return aGraphicDriver;
}
OccView::OccView(QWidget* parent )
    : QWidget(parent),
      myXmin(0),
      myYmin(0),
      myXmax(0),
      myYmax(0),
      ///myCurrentMode(CurAction3d_DynamicRotation),
      ///myCurrentMode(CurAction3d_GlobalPanning),
      myCurrentMode(CurAction3d_DynamicPanning),
      myDegenerateModeIsOn(Standard_True),
      myRectBand(NULL)
{
    // No Background
    setBackgroundRole( QPalette::NoRole );
    // set focus policy to threat QContextMenuEvent from keyboard
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    // Enable the mouse tracking, by default the mouse tracking is disabled.
    setMouseTracking( true );
    init();
}
void OccView::init()
{
    Handle(Aspect_DisplayConnection) aDisplayConnection = new
            Aspect_DisplayConnection();
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
    myView->SetBgGradientColors(defColor, Quantity_NOC_WHITE, Aspect_GFM_VER);
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.01,
                            V3d_ZBUFFER);
    myContext->SetDisplayMode(AIS_Shaded, Standard_True);
    myContext->SetAutomaticHilight(Standard_True);
    myView->SetZoom(1);
    Qt::Corner m_viewTrihedronCorner = Qt::TopRightCorner;
    opencascade::handle<AIS_ViewCube> aisViewCube = new AIS_ViewCube;
    aisViewCube->SetBoxColor(Quantity_NOC_GRAY75);
    // aisViewCube->SetFixedAnimationLoop(false);
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
}
Aspect_TypeOfTriedronPosition OccView::toOccCorner(Qt::Corner corner)
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
    return Aspect_TOTP_LEFT_UPPER;
}
const Handle(AIS_InteractiveContext)& OccView::getContext() const
{
    return myContext;
}
/// 清理所有的模型，仅保留 axiscube
void OccView::CreateViewDir()
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
void OccView::myRemoveall()
{
    myContext->RemoveAll(Standard_True);
    myContext->EraseAll(Standard_True);
    CreateViewDir();
}
QPaintEngine* OccView::paintEngine() const
{
    return 0;
}
void OccView::paintEvent( QPaintEvent* /*theEvent*/ )
{
    myView->Redraw();
}
void OccView::resizeEvent( QResizeEvent* /*theEvent*/ )
{
    if( !myView.IsNull()){
        myView->MustBeResized();}
}
void OccView::fitAll( void )
{
    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw();
}
void OccView::reset( void ){myView->Reset();}
void OccView::pan( void )
{myCurrentMode = CurAction3d_DynamicPanning;}
void OccView::zoom( void )
{ myCurrentMode = CurAction3d_DynamicZooming;}
void OccView::rotate( void )
{ myCurrentMode = CurAction3d_DynamicRotation;}
//#include"../ViewHeader.h"
void OccView::mousePressEvent( QMouseEvent* theEvent )
{
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    else if (theEvent->button() == Qt::MidButton)
    {
        onMButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton)
    {
        onRButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
        emit selectionChanged();
    }
    ///myContext->InitSelected();
}
void OccView::mouseReleaseEvent( QMouseEvent* theEvent )
{
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
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
void OccView::mouseMoveEvent( QMouseEvent * theEvent )
{
    onMouseMove(theEvent->buttons(), theEvent->pos());
}
void OccView::wheelEvent( QWheelEvent * theEvent )
{
    onMouseWheel(theEvent->buttons(), theEvent->delta(), theEvent->pos());
}
void OccView::onLButtonDown( const int /*theFlags*/, const QPoint thePoint )
{
    // Save the current mouse coordinate in min.
    myXmin = thePoint.x();
    myYmin = thePoint.y();
    myXmax = thePoint.x();
    myYmax = thePoint.y();
}
void OccView::onMButtonDown( const int /*theFlags*/, const QPoint thePoint )
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
void OccView::onRButtonDown( const int /*theFlags*/, const QPoint /*thePoint*/ )
{
    //ViewSelContent().FindSelLinePoint();
}
void OccView::onMouseWheel( const int /*theFlags*/, const int theDelta, const 
                            QPoint thePoint )
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
void OccView::addItemInPopup( QMenu* /*theMenu*/ )
{
}
void OccView::popup( const int /*x*/, const int /*y*/ )
{
}
void OccView::onLButtonUp( const int theFlags, const QPoint thePoint )
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
void OccView::onMButtonUp( const int /*theFlags*/, const QPoint thePoint )
{
    if (thePoint.x() == myXmin && thePoint.y() == myYmin)
    {
        panByMiddleButton(thePoint);
    }
}
void OccView::onRButtonUp( const int /*theFlags*/, const QPoint thePoint )
{
    popup(thePoint.x(), thePoint.y());
}
void OccView::onMouseMove( const int theFlags, const QPoint thePoint )
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
void OccView::dragEvent( const int x, const int y )
{
    myContext->Select(myXmin, myYmin, x, y, myView, Standard_True);
    emit selectionChanged();
}
void OccView::multiDragEvent( const int x, const int y )
{
    myContext->ShiftSelect(myXmin, myYmin, x, y, myView, Standard_True);
    emit selectionChanged();
}
void OccView::inputEvent( const int x, const int y )
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    myContext->Select(Standard_True);
    emit selectionChanged();
}
void OccView::multiInputEvent( const int x, const int y )
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    myContext->ShiftSelect(Standard_True);
    emit selectionChanged();
}
void OccView::moveEvent( const int x, const int y )
{
    myContext->MoveTo(x, y, myView, Standard_True);
}
void OccView::multiMoveEvent( const int x, const int y )
{
    myContext->MoveTo(x, y, myView, Standard_True);
}
void OccView::drawRubberBand( const int minX, const int minY, const int maxX, 
                              const int maxY )
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
void OccView::panByMiddleButton( const QPoint& thePoint )
{
    Standard_Integer aCenterX = 0;
    Standard_Integer aCenterY = 0;
    QSize aSize = size();
    aCenterX = aSize.width() / 2;
    aCenterY = aSize.height() / 2;
    myView->Pan(aCenterX - thePoint.x(), thePoint.y() - aCenterY);
}
