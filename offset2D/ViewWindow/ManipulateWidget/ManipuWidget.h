#ifndef ManipuWidget_H
#define ManipuWidget_H
#include <QtWidgets/QDialog>
#include <QtCore/QStringListModel>
#include"../HearderFile.h"
using std::vector;
#include <algorithm>
#include<cmath>
#include <memory>
#include <vector>
namespace Ui {
class ManipuWidget;
}
class ManipuWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ManipuWidget(QWidget *parent = nullptr);
    ~ManipuWidget();
public:
    void SignalSlot();
    void keyPressEvent(QKeyEvent*event);
public:
    DefElem selEleData;
    bool isValid = false;
public slots:
    void on_btnCalToolPath_clicked();
    void on_buttonTranseformMachCdt_clicked();
    void GetFileListViewIdx(const QModelIndex&idx);
    void SlotGetUiSelPairLoop(const DefElem& ele);
signals:
    void upDataDialUiData(const int& layerTota,
                          const int& whichLoop,
                          const double& toolRadius);
    void SendRemoveTestData();
    void SendTranseformMachCdt();
    void SendbuttonGetCutData(int,int);
    void SendRemoveFile(int isOk);
    void SendOffsetVal(double offsetVal);
    void SignalFileIdxPartModel(QString);
    void SignalDisplaySelData(BndIslPair,bool,bool);
    void SignalDisplayLoopData(const vector<DefLoop>&,bool,bool);
    void SignalCreateDisplayBlank(string,double,double,bool);
    void SignalDisplayToolPath(const shared_ptr<OffsetNode>&,
                               const shared_ptr<OffsetNode>&);
    void SignalRemoveBlank();
    void SignalViewCutContour(const BndIslPair&);
    void SignalViewToolPath(const BndIslPair&,
                            const shared_ptr<OffsetNode>&);
    void SignalScreen(string,string);
    void SignalSwitchWindDisplay(bool isDisplayImg,
                                 string imgPath);
protected:
    void closeEvent(QCloseEvent *) override;
private slots:
    void on_btnReadParts_clicked();
    void on_bntPrePartFile_clicked();
    void on_bntNextPartFile_clicked();
    void on_bntImportProfData_clicked();
    void on_cheBoxDisplaySel_clicked();
    void on_bntUiSaveSelPair_clicked();
    void on_btnCalBlank_clicked();
    void ScreenCurPage();
    void on_bntImportTBData_clicked();
    void on_btnAutoCalOffsetVal_clicked();
    void on_btnCanNotCut_clicked();
    void on_btnRemoveCurFile_clicked();
    void on_bntScreen_clicked();
    void on_bntImportTPImg_clicked();
    void on_bntPathSuccess_clicked();
    void on_bntPathFailure_clicked();
    void on_tpAutoCheck_clicked();
    void on_btnCutModel_clicked();
    void on_cheBoxVieModel_clicked();
    void on_cheBoxVieProf_clicked();
    void on_cheBoxVieToolPath_clicked();
    void on_cheBoxVieBlank_clicked();
public:
    Ui::ManipuWidget *ui;
    QWidget* m_parent;
    static ManipuWidget *defWid;
};
extern int clickTestNum;
extern bool isCutPartData,isFirstClickTest,
m_isHasExistedFile,m_isExistedModelFile,
m_isViewPart;
extern QStringListModel *m_listmodel;
extern CurViewType m_curViewType;
extern vector<FileInfo>m_vModelFile,m_vCutFile,
m_vToolPathFile,m_vToolPathImg;
extern vector<FileModel>m_vFileModel;
extern vector<BndIslPair>m_vCurOffsetPair;
extern QString m_curDir;
extern int m_curFileIdx,m_curCutPartIdx,m_totalFileNum;
extern CutPartResult m_curPartCutData;
extern BndIslPair m_curSelPairLoop;
extern bool m_isCurSelFind;
extern vector<vector<BndIslPair>>m_curModCutData;
#endif // ManipuWidget_H