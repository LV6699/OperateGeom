#ifndef WIDGETTOOL_H
#define WIDGETTOOL_H

#include <QDialog>
#include "../Common/OperateItem.h"

namespace Ui {
class WidgetTool;
}

class WidgetTool : public QDialog
{
    Q_OBJECT

public:
    explicit WidgetTool(QWidget *parent = nullptr);
    ~WidgetTool();
    void ReDrawPosLine();

    void DisplayOperItem(ViewObj::ViewItem& item);
    void RemoveOperItem(ViewObj::ViewItem& item);
    void FindSelItem();

private  slots:
    void on_douPtXCoord_valueChanged();
    void on_douPtYCoord_valueChanged();

    void on_btLineInt_clicked();

    void on_cheOriModel_clicked();

    void on_cheDiscTria_clicked();

    void on_chePtProtect_clicked();

    void on_cheEdgProtect_clicked();

    void on_cheTrisCl_clicked();

    void on_cheClPt_clicked();

    void on_che_xEdge_clicked();

    void on_che_yEdge_clicked();

    void on_btSelPtAllZ_clicked();

    void on_btVieIdTria_clicked();

    void on_btVieIdEdge_clicked();

public:
    bool isIniL = false;
    Handle(AIS_Shape) _posLine = nullptr;

public:
    Ui::WidgetTool *sub_ui;
};

#endif // WIDGETTOOL_H






















