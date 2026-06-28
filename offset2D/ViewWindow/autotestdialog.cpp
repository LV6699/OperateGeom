#include "autotestdialog.h"
#include "ui_autotestdialog.h"
#include"ManipulateWidget/ManipuWidget.h"
#include <QFileDialog>
#include <QDebug>
#include <QStringListModel>
#include "MainWindow/mainwindow.h"/**/
int m_ereaIdx;
int m_layIdx;
double _offsetVal;
AutoTestDialog::AutoTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AutoTestDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
//    setWindowFlag(Qt::WindowStaysOnTopHint);
    ManipuWidget *ostDlg = new ManipuWidget(this);
    auto settings = ini_settings();
    settings->beginGroup("AutoTestDialog");
    move(settings->value("pos", QPoint(0, 0)).toPoint());
    ui->line_dir->setText(settings->value("select_dir", QString("./")).toString());
    ui->le_out_dir->setText(settings->value("out_dir", QString("./test")).toString());
    settings->endGroup();
    ui->list_files->setMovement(QListView::Movement::Static);
    ui->list_files->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->list_files->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->btn_select_dir, SIGNAL(clicked()), this, SLOT(select_dir()));
    connect(ui->btn_read_dir, SIGNAL(clicked()), this, SLOT(read_dir()));
    connect(ui->btn_next, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->btn_prev, SIGNAL(clicked()), this, SLOT(prev()));
    connect(ui->btn_test, SIGNAL(clicked()), this, SLOT(test()));
    connect(ui->btn_out_next_ok, SIGNAL(clicked()), this, SLOT(out_next_ok()));
    connect(ui->btn_out_next_err, SIGNAL(clicked()), this, SLOT(out_next_err()));
    connect(ostDlg,SIGNAL(upDataDialUiData(const int&,const int&,
                                           const double& )),this,
            SLOT(ReceiveOffsetVal(const int&,const int&,const double& )));
}
AutoTestDialog::~AutoTestDialog()
{
    delete ui;
}
void AutoTestDialog::select_dir()
{
    auto select_dir = QFileDialog::getExistingDirectory();
    if(select_dir.isEmpty()) {
        return;
    }
    ui->line_dir->setText(select_dir);
}
void AutoTestDialog::read_dir()
{
    QDir dir(ui->line_dir->text());
    if(!dir.exists())
    {
        qDebug() << "文件夹不存在";
        return;
    }
    auto settings = ini_settings();
    settings->beginGroup("AutoTestDialog");
//    if(settings->value("crash", false).toBool()){
        if(settings->value("last_dir", QString()).toString() == dir.path()){
            curr_index = settings->value("last_index", 0).toUInt();
        }
//    }
    else {
        curr_index = 0;
    }
    settings->setValue("crash", true);
    settings->setValue("last_dir", dir.path());
    settings->setValue("last_index", curr_index);
    settings->endGroup();
    dir_path = dir.path();
    dir.setFilter(QDir::Files);
    QStringList list = dir.entryList(QDir::Files);
    file_list = list;
    file_list.erase(std::remove_if(file_list.begin(),file_list.end(),
                                   [](const QString& str){
                                        QFileInfo info(str);
                                        auto suffix = info.suffix().toLower();
                                        return !(suffix == "stp" || suffix == "step");
                                    }),
                    file_list.end());
    auto model = new QStringListModel(file_list);
    ui->list_files->setModel(model);
    load_file();
}
void AutoTestDialog::load_file()
{
    if(has_file()){
        ui->curr_file->setText(get_file());
        _mainwind->on_actionImportFiles_triggered();
        _mainwind->beforeReadStep();
        _mainwind->loadStep(get_file_full());
        _mainwind->afterReadStep();
    }
}
void AutoTestDialog::test()
{
}
void AutoTestDialog::ReceiveOffsetVal(const int &ereaIdx,
                                      const int &layIdx,
                                      const double &offsetVal)
{
    m_ereaIdx = ereaIdx;
    m_layIdx = layIdx;
    _offsetVal = offsetVal;
}
void AutoTestDialog::out(test_result res)
{
    if(!has_file()) return;
    qDebug() << "current out dir: " << out_dir();
    if(!has_out_dir()) return;
    QDir dir(out_dir());
    if(!dir.exists()){
        if(dir.isRelative()){
            dir.makeAbsolute();
        }
        if(!dir.exists()){
            dir.mkpath(dir.path());
        }
    }
    // 無法截屏occ
    //QImage img = _mainwind->centralWidget()->grab().toImage();
    auto img = QGuiApplication::primaryScreen()->grabWindow(_mainwind->winId(),
                                                            _mainwind->get_OCCView()->pos().x(),
                                                            _mainwind->get_OCCView()->pos().y(),
                                                            _mainwind->get_OCCView()->width(),
                                                            _mainwind->get_OCCView()->height()).toImage();
    static QString enum_map[] = {
        "ok",
        "err"
    };
    QString str_OffsetVal = "_" + QString::number(_offsetVal) + "_";
    auto img_file = QString("%1/%2.%3.%4.png").arg(out_dir()).arg(get_file()).arg(str_OffsetVal).arg(enum_map[res]);
    if(!img.save(img_file)){
        qDebug() << "無法保存" << img_file;
    }
    qDebug() << "Save file " << img_file;
}
void AutoTestDialog::prev()
{
    if(curr_index!=0){
        curr_index--;
        auto settings = ini_settings();
        settings->beginGroup("AutoTestDialog");
        settings->setValue("last_index", curr_index);
        settings->endGroup();
        load_file();
    }
}
void AutoTestDialog::next()
{
    if((curr_index+1) >= file_list.size()){
        return;
    }
    curr_index++;
    auto settings = ini_settings();
    settings->beginGroup("AutoTestDialog");
    settings->setValue("last_index", curr_index);
    settings->endGroup();
    load_file();
}
void AutoTestDialog::out_next_ok()
{
    out(OK);
    next();
}
void AutoTestDialog::out_next_err()
{
    out(ERR);
    next();
}
bool AutoTestDialog::has_file() const
{
    return (curr_index < file_list.size());
}
QString AutoTestDialog::get_file() const
{
    if(has_file()) return file_list[curr_index];
    return QString();
}
QString AutoTestDialog::get_file_full() const
{
    if(has_file()) return QString("%1/%2").arg(dir_path).arg(file_list[curr_index]);
    return QString();
}
bool AutoTestDialog::has_out_dir() const
{
    return !ui->le_out_dir->text().isEmpty();
}
QString AutoTestDialog::out_dir() const
{
    return (ui->le_out_dir->text());
}
void AutoTestDialog::closeEvent(QCloseEvent *event)
{
    auto settings = ini_settings();
    settings->beginGroup("AutoTestDialog");
    settings->setValue("pos", pos());
    settings->setValue("select_dir", ui->line_dir->text());
    settings->setValue("out_dir", ui->le_out_dir->text());
    settings->setValue("crash", false);
    settings->endGroup();
    autoTestDialog = nullptr;
    QDialog::closeEvent(event);
}