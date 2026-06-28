#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#include <QtCharts>
#include<vector>
#include<QtWidgets/QMainWindow>
#include<QtCore/QSettings>
#include<QtWidgets/QLabel>
#include<QtGui/QScreen>
#include<string>
#include<cstring>
#include"OCCView.h"
#include"../HearderFile.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
struct UiSelShape
{
    UiSelShape() {}
    UiSelShape(vector<Handle(AIS_Shape)>bndShape,
               vector<vector<Handle(AIS_Shape)>>islShape)
    {
        m_vSelBndShape = bndShape;
        m_vSelIslShape = islShape;
    }
    vector<Handle(AIS_Shape)> m_vSelBndShape;
    vector<vector<Handle(AIS_Shape)>>m_vSelIslShape;
};
struct EleShape{
    EleShape(){}
    EleShape(DefElem ele,Handle(AIS_Shape) shape,
             int idx,int nodeIdx)
    {
        m_ele = ele;
        m_occShape = shape;
        m_idx = idx;
        m_nodeIdx = nodeIdx;
    }
    DefElem m_ele;
    Handle(AIS_Shape) m_occShape;
    int m_idx;
    int m_nodeIdx;
};
struct LoopShape
{
    LoopShape() {}
    LoopShape(DefLoop loop,vector<EleShape>vShape,int loIdx,int nodeIdx)
    {
        m_loop = loop;
        m_vShape = vShape;
        m_loIdx = loIdx;
        m_nodeIdx = nodeIdx;
    }
    DefLoop m_loop;
    vector<EleShape> m_vShape;
    int m_loIdx;
    int m_nodeIdx;
};
struct NodeShape
{
    NodeShape() {}
    NodeShape(vector<LoopShape>vLoopShape,int nodeIdx)
    {
        m_vLoopShape = vLoopShape;
        m_nodeIdx = nodeIdx;
    }
    vector<LoopShape> m_vLoopShape;
    int m_nodeIdx;
};
struct BndIslShape
{
    BndIslShape() {}
    BndIslShape(LoopShape lo,vector<LoopShape>vLo)
    {
        m_bndShape = lo;
        m_vIslShape = vLo;
    }
    LoopShape m_bndShape;
    vector<LoopShape>m_vIslShape;
    vector<LoopShape>m_vBlockShape;
};
extern vector<NodeShape>glb_vNodeOcc;
extern BndIslShape glb_bndIslShape;
std::shared_ptr<QSettings> ini_settings();
class AutoTestDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    //MainWindow(){}
    ~MainWindow();
    void GetOccShapeData(vector<DefLoop>vLoopData,
                         bool isToolPath, int nodeIdx,
                         NodeShape& curNodeShape);
    void ViewOccNode(bool isUpdateViewer);
    void DisplayOffsetNodeLoopData(shared_ptr<OffsetNode> rootNode,
                                   bool isToolPath, int curNodeIdx,
                                   bool isView);
    void ReceiveCutPartData(int);
    void ToolTrajectoryAlgInPutOutPutIO(DefLoop& bndLoop,
                                        vector<DefLoop>& vIslandLoop,
                                        OffsetParam &offsetPara,
                                        shared_ptr<OffsetNode> &rootNode);
    void initialViewShape(STEPControl_Reader& reader);
    void TranslateEleToAIS_Shape(const DefElem& ele,
                                 Quantity_Color color,
                                 double width,bool& isSucccess,
                                 Handle(AIS_Shape)&shape);
    void GetOCCViewShape(const DefLoop& loopData,bool isToolPath,
                                     int nodeIdx,LoopShape& loopShape);
    OCCView* get_OCCView() const { return myOccView; }
    void keyPressEvent(QKeyEvent*event);
public:
    ///选择模式，鼠标点击模型，选中对应的面
    void selectMode(Handle(AIS_Shape));
    ///获取鼠标选中的对象
    void getShape();
    ///高亮显示
    void setHightStyle();
    ///选择样式
    void setSelectStyle();
    string m_asSavePath = "";
public:
    Ui::MainWindow *ui;
    OCCView* myOccView;
    QMenuBar *menu;
    QMenu *menu1;
    QMenu *menu2;
    QMenu *menu3;
    QAction *action1;
    QAction *action2;
    QAction *action3;
    QStatusBar *status;
    ManipuWidget *ostDlg;
protected:
    /// create all the actions.
    void createActions(void);
    void createMenus(void);
    ///覆写鼠标按键按下事件
    void winMousePressEvent(QMouseEvent* event);
    ///覆写鼠标按键释放事件
    void winMouseReleaseEvent(QMouseEvent* event);
    ///覆写鼠标移动事件
    void winMouseMoveEvent(QMouseEvent* event);
    void closeEvent(QCloseEvent* event) override;
public slots:
    void about(void);
    void beforeReadStep();
    void readStep();
    void loadStep(QString fileName);
    void afterReadStep();
    void reLeadingModelFile(string path);
    void ClearModel();
    void on_actOstTest_triggered();
    void on_actionImportFiles_triggered();
    void DisplayUiSelShape(const BndIslPair &bndIslPair,
                           bool isChangeSel, bool);
    void DisplayPairLoop(const BndIslPair &bndIslPair, bool isClear,
                           bool isToolPath);
    void SlotDisplayLoopData(const vector<DefLoop>&vLoopData,
                             bool isToolPath,bool isClearOther);
    void SlotCreateDisplayBlank(string path, double xExp,
                                double yExp, bool isClearShape);
    void SlotDisplayToolPath(const shared_ptr<OffsetNode>&,
                             const shared_ptr<OffsetNode>&);
    void SlotRemoveBlank();
    void SlotViewCutContourData(const BndIslPair& pairLoop);
    void SlotViewToolPathData(const BndIslPair &oriPairLoop,
                              const shared_ptr<OffsetNode>&rootNode);
    void SlotScreen(string saveDir, string name);
    void SlotSwitchWindDisplay(bool isDisplayImg,string imgPath);
signals:
    void sendIsExistModelFile(IsExistedModelFile);
public slots:
    void ReceiveTranseformMachCdt();
private slots:
    void on_actionSplitShape_triggered();
};
extern MainWindow* _mainwind;
extern ManipuWidget* test_window;
extern AutoTestDialog* autoTestDialog;
extern long double Time_offset,Time_equCur,
Time_oftSelInt,Time_smooth,Time_boolUni,
Time_boolSub,Time_other,Time_boolClip,Time_total;
extern std::vector<Quantity_Color>_colors;
#endif // MAINWINDOW_H