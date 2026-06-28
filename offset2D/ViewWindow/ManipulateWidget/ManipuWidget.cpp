#include "ManipuWidget.h"
#include "ui_ManipuWidget.h"
#include"../ProgramCall/CreatePathCall.h"
#include"../TextOrganize/DataEdit.h"
#include"../CutModelContour/CutParts.h"
#include"../../AlgorithmDeve/AutomaticCheck/AutoCheckPath.h"
#include"../../AlgorithmDeve/CheckToolPath/AreaClearCheck/ToolPathCheck.h"
#include"../DisplayShape/DisplayToolPath.h"
#include <QtGui/qkeysequence.h>
//#include <unistd.h>
#include <time.h>
shared_ptr<ViewCurObject>widvieobj = make_shared<ViewCurObject>();
shared_ptr<ModelFileCut>widmodcut = make_shared<ModelFileCut>();
shared_ptr<HandleFileObject>wid_wifile = make_shared<HandleFileObject>();
shared_ptr<CreateToolPath>wid_wicrttp = make_shared<CreateToolPath>();
shared_ptr<TrajectoryCheck>wid_tpch = make_shared<TrajectoryCheck>();
shared_ptr<CurrentIndexType>wid_istyp = make_shared<CurrentIndexType>();
shared_ptr<HandleFileObject>wid_wifil = make_shared<HandleFileObject>();
shared_ptr<CreateToolPath>wid_crttp = make_shared<CreateToolPath>();
int clickTestNum = 0;
bool isCutPartData = false,isFirstClickTest = false,
m_isHasExistedFile = false,m_isExistedModelFile = false,
m_isViewPart = true;
bool m_isCurSelFind = false;
int m_curFileIdx = 0,m_curCutPartIdx,m_totalFileNum = 0;
CurViewType m_curViewType;
QStringListModel *m_listmodel = new QStringListModel();
vector<FileInfo>m_vModelFile,m_vCutFile,m_vToolPathFile,m_vToolPathImg;
vector<FileModel>m_vFileModel;
vector<BndIslPair>m_vCurOffsetPair;
QString m_curDir;
BndIslPair m_curSelPairLoop;
CutPartResult m_curPartCutData;
vector<vector<BndIslPair>>m_curModCutData;
ManipuWidget::ManipuWidget(QWidget *parent) :
    QDialog(parent),ui(new Ui::ManipuWidget)
{
    m_parent = parent;
    ui->setupUi(this);
    auto settings = ini_settings();
    settings->beginGroup("ManipuWidget");
    move(settings->value("pos", QPoint(0, 0)).toPoint());
    settings->endGroup();
    SignalSlot();
}
ManipuWidget::~ManipuWidget(){delete ui;}
void ManipuWidget::SignalSlot()
{
    connect(ui->listViewListFile, SIGNAL(doubleClicked(const QModelIndex&)),
            this,SLOT(GetFileListViewIdx(const QModelIndex&)));
}
void ManipuWidget::keyPressEvent(QKeyEvent*event)
{
    setFocusPolicy((Qt::StrongFocus));
    installEventFilter(this);
    if(event->key() == Qt::Key_Down){
        on_bntNextPartFile_clicked();
    }
    else if (event->key() == Qt::Key_Up) {
        on_bntPrePartFile_clicked();
    }
}
void ManipuWidget::GetFileListViewIdx(const QModelIndex&idx)
{
    m_curFileIdx = idx.row();
    wid_istyp->GetCurrentIndexFile(this,m_curViewType);
    wid_istyp->ViewCurrentIndexFileObject(this);
}
void ManipuWidget::closeEvent(QCloseEvent *event)
{
    auto settings = ini_settings();
    settings->beginGroup("ManipuWidget");
    settings->setValue("pos", pos());
    settings->endGroup();
    QDialog::closeEvent(event);
}
void ManipuWidget::on_btnCutModel_clicked()
{
    bool isBatchCut = ui->cheBoxIsBatchCut->isChecked();
    if(isBatchCut){
        widmodcut->CutBatchModelToContoure(this);
        return;
    }
    widmodcut->CutSingleModelToContoure(this,m_curModCutData);
}
void ManipuWidget::SlotGetUiSelPairLoop(const DefElem& ele)
{
    if(m_curViewType == PartModel)
        widmodcut->InitialUiSelBndIslLoop(this,ele);
    //widvieobj->DisplaySelBndIslLoop(m_curSelPairLoop);
}
void ManipuWidget::on_bntUiSaveSelPair_clicked()
{
    widmodcut->SaveSelectBndIslLoop(this);
}
void ManipuWidget::on_cheBoxDisplaySel_clicked()
{
    bool isDisplay = ui->cheBoxDisplaySel->isChecked();
    wid_istyp->ViewSelectContourScale(this);
    widvieobj->ControlCurrentUiSelect(isDisplay);
}
void ManipuWidget::on_buttonTranseformMachCdt_clicked()
{
    widmodcut->RotateCurrentModel(this);
    ///emit SendTranseformMachCdt();
}
void ManipuWidget::on_btnReadParts_clicked()
{
    wid_istyp->LoadeViewFileList(this,PartModel);
}
void ManipuWidget::on_btnCalBlank_clicked()
{
    if(m_curViewType != PartModel || m_vModelFile.empty()){
        return;
    }
    widmodcut->CalculateModelBlank(this);
    double xExp = ui->linEdiBlankXExp->text().toDouble();
    double yExp = ui->linEdiBlankYExp->text().toDouble();
    string path = m_vModelFile[m_curFileIdx].m_filePath.toStdString();
    widvieobj->DisplayModelBlank(path,xExp,yExp);
}
void ManipuWidget::on_bntPrePartFile_clicked()
{
    wid_istyp->GetPreNexFileIndex(this,false);
}
void ManipuWidget::on_bntNextPartFile_clicked()
{
    wid_istyp->GetPreNexFileIndex(this,true);
}
void ManipuWidget::on_bntImportProfData_clicked()
{
    widvieobj->InitailUiStatus();
    wid_istyp->LoadeViewFileList(this,CutContourData);
}
void ManipuWidget::on_btnAutoCalOffsetVal_clicked()
{
    wid_wifile->SetOffsetValFromUi(this);
}
void ManipuWidget::on_btnCalToolPath_clicked()
{
    wid_wicrttp->CalculateToolPath(this);
}
void ManipuWidget::ScreenCurPage()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    screen->grabWindow(0).save("123","jpg");
}
void ManipuWidget::on_bntScreen_clicked()
{
    emit SignalScreen("./","刀路图片");
}
void ManipuWidget::on_bntImportTBData_clicked()
{
    widvieobj->InitailUiStatus();
    wid_istyp->LoadeViewFileList(this,ToolPathData);
}
void ManipuWidget::on_bntImportTPImg_clicked()
{
    widvieobj->InitailUiStatus();
   wid_istyp->LoadeViewFileList(this,ToolPathImage);
}
void ManipuWidget::on_btnCanNotCut_clicked()
{
    wid_wifile->MoveCannotCutModelFile(this);
}
void ManipuWidget::on_btnRemoveCurFile_clicked()
{
    if(m_curViewType != PartModel || m_vModelFile.empty()){
        return;
    }
    widmodcut->MoveCurrentModelFile(this);
}
void ManipuWidget::on_bntPathSuccess_clicked()
{
    wid_wifil->SaveToolPathDataInfo(this,true,true);
}
void ManipuWidget::on_bntPathFailure_clicked()
{
    wid_wifil->SaveToolPathDataInfo(this,true,false);
}
void ManipuWidget::on_tpAutoCheck_clicked()
{
    wid_tpch->ExecuteTrajectoryCheck(this);
}
void ManipuWidget::on_cheBoxVieModel_clicked()
{
    bool isDisplay = ui->cheBoxVieModel->isChecked();
    widvieobj->ControlCurrentUiModel(isDisplay);
}
void ManipuWidget::on_cheBoxVieProf_clicked()
{
    bool isDisplay = ui->cheBoxVieProf->isChecked();
    widvieobj->ControlCurrentUiContoure(isDisplay);
}
void ManipuWidget::on_cheBoxVieToolPath_clicked()
{
    bool isDisplay = ui->cheBoxVieToolPath->isChecked();
    widvieobj->ControlCurrentUiToolPath(isDisplay);
}
void ManipuWidget::on_cheBoxVieBlank_clicked()
{
    bool isDisplay = ui->cheBoxVieBlank->isChecked();
    widvieobj->ControlCurrentUiBlank(isDisplay);
}