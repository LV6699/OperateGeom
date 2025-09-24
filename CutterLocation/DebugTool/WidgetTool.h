#ifndef WIDGETTOOL_H
#define WIDGETTOOL_H

#include <QWidget>

namespace Ui {
class WidgetTool;
}

class WidgetTool : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTool(QWidget *parent = nullptr);
    ~WidgetTool();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint m_dragPos;
    Ui::WidgetTool *ui;
};

#endif // WIDGETTOOL_H
