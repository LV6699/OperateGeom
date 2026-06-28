#ifndef OCCVIEW_H
#define OCCVIEW_H
#include<QtWidgets/QWidget>
#include<string>
#include<QtWidgets/QApplication>
#include"OccHeader.h"
#include"../HearderFile.h"
class QMenu;
class QRubberBand;
//! Adapted a QWidget for OpenCASCADE viewer.
class OCCView : public QWidget
{
    Q_OBJECT
public:
    enum CurrentAction3d
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicZooming, 
        CurAction3d_WindowZooming,
        CurAction3d_DynamicPanning, 
        CurAction3d_GlobalPanning,
        CurAction3d_DynamicRotation 
    };
public:
    OCCView(QWidget* parent);
    const Handle(AIS_InteractiveContext)& getContext() const;
Aspect_TypeOfTriedronPosition toOccCorner(Qt::Corner corner);
void myRemoveall();
signals:
    void selectionChanged(void);
    void SelectOccDefEle(const DefElem&);
    void SignalPrintSelLoop(AIS_Shape);
public slots:
    void pan(void);
    void fitAll(void);
    void reset(void);
    void zoom(void);
    void rotate(void);
protected:
    virtual QPaintEngine* paintEngine() const;
    virtual void paintEvent(QPaintEvent* theEvent);
    virtual void resizeEvent(QResizeEvent* theEvent);
    virtual void mousePressEvent(QMouseEvent* theEvent);
    virtual void mouseReleaseEvent(QMouseEvent* theEvent);
    virtual void mouseMoveEvent(QMouseEvent * theEvent);
    virtual void wheelEvent(QWheelEvent * theEvent);
    virtual void onLButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onRButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMouseWheel(const int theFlags, const int theDelta, const QPoint thePoint);
    virtual void onLButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onRButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMouseMove(const int theFlags, const QPoint thePoint);
    virtual void addItemInPopup(QMenu* theMenu);
    ///选择模式，鼠标点击模型，选中对应的面
    void selectMode(Handle(AIS_Shape) );
    ///获取鼠标选中的对象
    void getShape();
    ///高亮显示
    void setHightStyle();
    ///选择样式
    void setSelectStyle();
    Standard_Boolean SurfaceCenterNormal(TopoDS_Shape &aShape, gp_Vec& aCNormal,
                                         gp_Pnt& aCenterPoint);
public:
    void RotationShape(opencascade::handle<AIS_Shape> &transeShape);
    void GetRotationParameters();
    Handle(AIS_Shape) m_viewShape;
    gp_Vec m_vecNormal;
    gp_Pnt m_normalCenterPnt;
    void TransUiSelShapeToEle();
    void CreateViewDir();
protected:
    void init(void);
    void popup(const int x, const int y);
    void dragEvent(const int x, const int y);
    void inputEvent(const int x, const int y);
    void moveEvent(const int x, const int y);
    void multiMoveEvent(const int x, const int y);
    void multiDragEvent(const int x, const int y);
    void multiInputEvent(const int x, const int y);
    void drawRubberBand(const int minX, const int minY, const int maxX, const int maxY);
    void panByMiddleButton(const QPoint& thePoint);
    //*****************************
public:
    Handle(V3d_View) myView;
    Handle(AIS_InteractiveContext) myContext;
    DefElem m_occCurSelectEle;
    bool m_isIniSelEle = false;
private:
    Handle(V3d_Viewer) myViewer;
    //Handle(V3d_View) myView;
    //Handle(AIS_InteractiveContext) myContext;
    Handle(AIS_Shape) a_sel;
    Standard_Integer myXmin;
    Standard_Integer myYmin;
    Standard_Integer myXmax;
    Standard_Integer myYmax;
    CurrentAction3d myCurrentMode;
    Standard_Boolean myDegenerateModeIsOn;
    QRubberBand* myRectBand;
    Standard_Integer m_x_max; 
    Standard_Integer m_y_max; 
    CurrentAction3d m_current_mode; 
    ManipuWidget *testDialog;
};
#endif // OCCVIEW_H