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
#include <QtWidgets/QSpinBox>
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
    QCheckBox *cheTrisCl;
    QCheckBox *cheClPt;
    QCheckBox *che_xEdge;
    QCheckBox *che_yEdge;
    QFrame *frame_2;
    QLabel *label;
    QDoubleSpinBox *douPtXCoord;
    QDoubleSpinBox *douPtYCoord;
    QCheckBox *cheFindInt;
    QCheckBox *cheExeSel;
    QPushButton *btLineInt;
    QPushButton *btSelPtAllZ;
    QSpinBox *spinTriaId;
    QPushButton *btVieIdTria;

    void setupUi(QWidget *WidgetTool)
    {
        if (WidgetTool->objectName().isEmpty())
            WidgetTool->setObjectName(QString::fromUtf8("WidgetTool"));
        WidgetTool->resize(349, 348);
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
        cheEdgProtect->setGeometry(QRect(100, 35, 102, 19));
        cheTrisCl = new QCheckBox(frame);
        cheTrisCl->setObjectName(QString::fromUtf8("cheTrisCl"));
        cheTrisCl->setGeometry(QRect(205, 5, 101, 25));
        cheClPt = new QCheckBox(frame);
        cheClPt->setObjectName(QString::fromUtf8("cheClPt"));
        cheClPt->setGeometry(QRect(5, 90, 71, 19));
        che_xEdge = new QCheckBox(frame);
        che_xEdge->setObjectName(QString::fromUtf8("che_xEdge"));
        che_xEdge->setGeometry(QRect(5, 60, 71, 19));
        QFont font;
        font.setPointSize(9);
        che_xEdge->setFont(font);
        che_yEdge = new QCheckBox(frame);
        che_yEdge->setObjectName(QString::fromUtf8("che_yEdge"));
        che_yEdge->setGeometry(QRect(100, 60, 71, 19));
        che_yEdge->setFont(font);
        frame_2 = new QFrame(WidgetTool);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(0, 220, 341, 121));
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
        cheFindInt = new QCheckBox(frame_2);
        cheFindInt->setObjectName(QString::fromUtf8("cheFindInt"));
        cheFindInt->setGeometry(QRect(5, 38, 85, 30));
        cheExeSel = new QCheckBox(frame_2);
        cheExeSel->setObjectName(QString::fromUtf8("cheExeSel"));
        cheExeSel->setGeometry(QRect(100, 38, 90, 30));
        btLineInt = new QPushButton(frame_2);
        btLineInt->setObjectName(QString::fromUtf8("btLineInt"));
        btLineInt->setGeometry(QRect(200, 38, 110, 30));
        btSelPtAllZ = new QPushButton(frame_2);
        btSelPtAllZ->setObjectName(QString::fromUtf8("btSelPtAllZ"));
        btSelPtAllZ->setGeometry(QRect(200, 70, 110, 30));
        spinTriaId = new QSpinBox(frame_2);
        spinTriaId->setObjectName(QString::fromUtf8("spinTriaId"));
        spinTriaId->setGeometry(QRect(5, 70, 71, 25));
        spinTriaId->setStyleSheet(QString::fromUtf8("background-color: rgb(220, 220, 220);"));
        spinTriaId->setMaximum(99999);
        btVieIdTria = new QPushButton(frame_2);
        btVieIdTria->setObjectName(QString::fromUtf8("btVieIdTria"));
        btVieIdTria->setGeometry(QRect(80, 70, 110, 30));

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
        cheTrisCl->setText(QApplication::translate("WidgetTool", "\345\201\217\347\247\273\344\270\211\350\247\222\345\275\242", nullptr));
        cheClPt->setText(QApplication::translate("WidgetTool", "\345\210\200\344\275\215\347\202\271", nullptr));
        che_xEdge->setText(QApplication::translate("WidgetTool", "x_edge", nullptr));
        che_yEdge->setText(QApplication::translate("WidgetTool", "y_edge", nullptr));
        label->setText(QApplication::translate("WidgetTool", "\347\202\271\345\235\220\346\240\207:", nullptr));
        cheFindInt->setText(QApplication::translate("WidgetTool", "\346\237\245\346\211\276\347\233\270\344\272\244", nullptr));
        cheExeSel->setText(QApplication::translate("WidgetTool", "\346\211\247\350\241\214\351\200\211\346\213\251", nullptr));
        btLineInt->setText(QApplication::translate("WidgetTool", "\350\256\241\347\256\227\345\201\217\347\275\256Z", nullptr));
        btSelPtAllZ->setText(QApplication::translate("WidgetTool", "\350\256\241\347\256\227Z", nullptr));
        btVieIdTria->setText(QApplication::translate("WidgetTool", "\347\264\242\345\274\225\344\270\211\350\247\222\345\275\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WidgetTool: public Ui_WidgetTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGETTOOL_H
