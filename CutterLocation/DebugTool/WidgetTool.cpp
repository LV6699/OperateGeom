#include "WidgetTool.h"
#include "ui_WidgetTool.h"
#include <QMouseEvent>

WidgetTool::WidgetTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTool)
{
    ui->setupUi(this);
}

WidgetTool::~WidgetTool()
{
    delete ui;
}
void WidgetTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPos = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void WidgetTool::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPos);
        event->accept();
    }
}
