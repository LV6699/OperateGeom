#ifndef DEBUGINFODIALOG_H
#define DEBUGINFODIALOG_H
#include <QtWidgets/QDialog>
#include "AIS_TextLabel.hxx"
#include "AIS_Shape.hxx"
//#include"TestDataStruct.h"
#include <vector>
#include <map>
namespace Ui {
class DebugInfoDialog;
}
class DebugInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DebugInfoDialog(QWidget *parent = nullptr);
    ~DebugInfoDialog();
public slots:
    void ReciveSelectChanged();
private:
    Ui::DebugInfoDialog *ui;
    using Label = Handle(AIS_TextLabel);
    std::vector<Label> all_label;
    size_t use_label_count = 0;
    void clear_all_label();
    Label get_new_label();
    void update_label(Label& l);
};
//extern DebugInfoDialog* debug_info_dialog;
struct RecordData{
    size_t index = 0;
};
extern std::map<Handle(AIS_Shape), RecordData> record_map;
#endif // DEBUGINFODIALOG_H