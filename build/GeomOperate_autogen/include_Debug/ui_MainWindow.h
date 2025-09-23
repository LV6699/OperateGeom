/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actPan;
    QAction *actRotate;
    QAction *actReset;
    QAction *actFitall;
    QAction *actImport;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuView;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Resources/lamp.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actPan = new QAction(MainWindow);
        actPan->setObjectName(QString::fromUtf8("actPan"));
        QIcon icon1;
        QString iconThemeName = QString::fromUtf8("\345\271\263\347\247\273");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8("Icon/Pan.png"), QSize(), QIcon::Normal, QIcon::Off);
        }
        actPan->setIcon(icon1);
        actRotate = new QAction(MainWindow);
        actRotate->setObjectName(QString::fromUtf8("actRotate"));
        QIcon icon2;
        iconThemeName = QString::fromUtf8("\346\227\213\350\275\254");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon2 = QIcon::fromTheme(iconThemeName);
        } else {
            icon2.addFile(QString::fromUtf8("Icon/rotate.png"), QSize(), QIcon::Normal, QIcon::Off);
        }
        actRotate->setIcon(icon2);
        actReset = new QAction(MainWindow);
        actReset->setObjectName(QString::fromUtf8("actReset"));
        QIcon icon3;
        iconThemeName = QString::fromUtf8("\351\207\215\347\275\256");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon3 = QIcon::fromTheme(iconThemeName);
        } else {
            icon3.addFile(QString::fromUtf8("Icon/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        }
        actReset->setIcon(icon3);
        actFitall = new QAction(MainWindow);
        actFitall->setObjectName(QString::fromUtf8("actFitall"));
        QIcon icon4;
        iconThemeName = QString::fromUtf8("\345\205\250\345\261\200");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon4 = QIcon::fromTheme(iconThemeName);
        } else {
            icon4.addFile(QString::fromUtf8("Icon/fitAll.png"), QSize(), QIcon::Normal, QIcon::Off);
        }
        actFitall->setIcon(icon4);
        actImport = new QAction(MainWindow);
        actImport->setObjectName(QString::fromUtf8("actImport"));
        QIcon icon5;
        iconThemeName = QString::fromUtf8("\345\257\274\345\205\245");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon5 = QIcon::fromTheme(iconThemeName);
        } else {
            icon5.addFile(QString::fromUtf8("Icon/import.png"), QSize(), QIcon::Normal, QIcon::Off);
        }
        actImport->setIcon(icon5);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 26));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuView->menuAction());
        menuView->addSeparator();
        menuView->addAction(actPan);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actPan->setText(QApplication::translate("MainWindow", "\345\271\263\347\247\273", nullptr));
#ifndef QT_NO_TOOLTIP
        actPan->setToolTip(QApplication::translate("MainWindow", "\345\271\263\347\247\273", nullptr));
#endif // QT_NO_TOOLTIP
        actRotate->setText(QApplication::translate("MainWindow", "\346\227\213\350\275\254", nullptr));
#ifndef QT_NO_TOOLTIP
        actRotate->setToolTip(QApplication::translate("MainWindow", "\346\227\213\350\275\254", nullptr));
#endif // QT_NO_TOOLTIP
        actReset->setText(QApplication::translate("MainWindow", "\351\207\215\347\275\256", nullptr));
#ifndef QT_NO_TOOLTIP
        actReset->setToolTip(QApplication::translate("MainWindow", "\351\207\215\347\275\256", nullptr));
#endif // QT_NO_TOOLTIP
        actFitall->setText(QApplication::translate("MainWindow", "\345\205\250\345\261\200", nullptr));
#ifndef QT_NO_TOOLTIP
        actFitall->setToolTip(QApplication::translate("MainWindow", "\345\205\250\345\261\200", nullptr));
#endif // QT_NO_TOOLTIP
        actImport->setText(QApplication::translate("MainWindow", "\345\257\274\345\205\245", nullptr));
#ifndef QT_NO_TOOLTIP
        actImport->setToolTip(QApplication::translate("MainWindow", "\345\257\274\345\205\245", nullptr));
#endif // QT_NO_TOOLTIP
        menuView->setTitle(QApplication::translate("MainWindow", "View", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
