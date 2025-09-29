#ifndef WIDGETTOOL_H
#define WIDGETTOOL_H

#include <QDialog>
#include "../../ViewWindow/OccHeader.h"
///#include "../Common/ClStruct.h"
#include "../Solution/CutterLocationZ.h"

namespace Ui {
class WidgetTool;
}

class WidgetTool : public QDialog
{
    Q_OBJECT

public:
    explicit WidgetTool(QWidget *parent = nullptr);
    ~WidgetTool();

    void ConnectConnect();
    void ReDrawPosLine();

private  slots:
    void on_douPtXCoord_valueChanged();
    void on_douPtYCoord_valueChanged();

    void on_btLineInt_clicked();

public:
    bool isIniL = false;
    Handle(AIS_Shape) _posLine = nullptr;

public:
    Ui::WidgetTool *sub_ui;
};

#endif // WIDGETTOOL_H
