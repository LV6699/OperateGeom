/********************************************************************************
** Form generated from reading UI file 'WidgetTool.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGETTOOL_H
#define UI_WIDGETTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WidgetTool
{
public:
    QFrame *frame;
    QCheckBox *cheOriModel;
    QCheckBox *cheDiscTria;
    QCheckBox *chePtProtect;
    QCheckBox *cheEdgProtect;
    QFrame *frame_2;
    QLabel *label;
    QDoubleSpinBox *douPtXCoord;
    QDoubleSpinBox *douPtYCoord;
    QPushButton *btCalInt;

    void setupUi(QWidget *WidgetTool)
    {
        if (WidgetTool->objectName().isEmpty())
            WidgetTool->setObjectName(QString::fromUtf8("WidgetTool"));
        WidgetTool->resize(353, 298);
        frame = new QFrame(WidgetTool);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 341, 201));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(200, 210, 220);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        cheOriModel = new QCheckBox(frame);
        cheOriModel->setObjectName(QString::fromUtf8("cheOriModel"));
        cheOriModel->setGeometry(QRect(5, 5, 87, 25));
        cheDiscTria = new QCheckBox(frame);
        cheDiscTria->setObjectName(QString::fromUtf8("cheDiscTria"));
        cheDiscTria->setGeometry(QRect(100, 5, 102, 25));
        chePtProtect = new QCheckBox(frame);
        chePtProtect->setObjectName(QString::fromUtf8("chePtProtect"));
        chePtProtect->setGeometry(QRect(5, 35, 87, 19));
        cheEdgProtect = new QCheckBox(frame);
        cheEdgProtect->setObjectName(QString::fromUtf8("cheEdgProtect"));
        cheEdgProtect->setGeometry(QRect(103, 35, 102, 19));
        frame_2 = new QFrame(WidgetTool);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(0, 220, 341, 71));
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(200, 210, 220);"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(5, 5, 55, 25));
        douPtXCoord = new QDoubleSpinBox(frame_2);
        douPtXCoord->setObjectName(QString::fromUtf8("douPtXCoord"));
        douPtXCoord->setGeometry(QRect(65, 5, 120, 25));
        douPtXCoord->setStyleSheet(QString::fromUtf8("background-color: rgb(220, 220, 220);"));
        douPtXCoord->setDecimals(3);
        douPtXCoord->setMinimum(-1000.000000000000000);
        douPtXCoord->setMaximum(1000.000000000000000);
        douPtYCoord = new QDoubleSpinBox(frame_2);
        douPtYCoord->setObjectName(QString::fromUtf8("douPtYCoord"));
        douPtYCoord->setGeometry(QRect(190, 5, 120, 25));
        douPtYCoord->setStyleSheet(QString::fromUtf8("background-color: rgb(220, 220, 220);"));
        douPtYCoord->setDecimals(3);
        douPtYCoord->setMinimum(-1000.000000000000000);
        douPtYCoord->setMaximum(1000.000000000000000);
        btCalInt = new QPushButton(frame_2);
        btCalInt->setObjectName(QString::fromUtf8("btCalInt"));
        btCalInt->setGeometry(QRect(5, 35, 93, 30));

        retranslateUi(WidgetTool);

        QMetaObject::connectSlotsByName(WidgetTool);
    } // setupUi

    void retranslateUi(QWidget *WidgetTool)
    {
        WidgetTool->setWindowTitle(QApplication::translate("WidgetTool", "Form", nullptr));
        cheOriModel->setText(QApplication::translate("WidgetTool", "\345\216\237\345\247\213\346\250\241\345\236\213", nullptr));
        cheDiscTria->setText(QApplication::translate("WidgetTool", "\347\246\273\346\225\243\344\270\211\350\247\222\345\275\242", nullptr));
        chePtProtect->setText(QApplication::translate("WidgetTool", "\347\202\271\344\277\235\346\212\244\351\235\242", nullptr));
        cheEdgProtect->setText(QApplication::translate("WidgetTool", "\350\276\271\344\277\235\346\212\244\351\235\242", nullptr));
        label->setText(QApplication::translate("WidgetTool", "\347\202\271\345\235\220\346\240\207:", nullptr));
        btCalInt->setText(QApplication::translate("WidgetTool", "\350\256\241\347\256\227", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WidgetTool: public Ui_WidgetTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGETTOOL_H
