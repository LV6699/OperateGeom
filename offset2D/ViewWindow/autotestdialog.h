#ifndef AUTOTESTDIALOG_H
#define AUTOTESTDIALOG_H
#include <QtWidgets/QDialog>
namespace Ui {
class AutoTestDialog;
}
enum test_result{
    OK,
    ERR
};
class AutoTestDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AutoTestDialog(QWidget *parent = nullptr);
    ~AutoTestDialog();
public slots:
    void select_dir();
    void read_dir();
    void load_file();
    void test();
    void out(test_result res);
    void prev();
    void next();
    void out_next_ok();
    void out_next_err();
    bool has_file() const;
    QString get_file() const;
    QString get_file_full() const;
    bool has_out_dir() const;
    QString out_dir() const;
    void ReceiveOffsetVal(const int&, const int&layIdx,
                          const double& offsetVal);
protected:
    void closeEvent(QCloseEvent *) override;
private:
    Ui::AutoTestDialog *ui;
    QString dir_path;
    int curr_index;
    QStringList file_list;
};
#endif // AUTOTESTDIALOG_H