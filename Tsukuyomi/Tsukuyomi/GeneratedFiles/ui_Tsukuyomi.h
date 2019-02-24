/********************************************************************************
** Form generated from reading UI file 'Tsukuyomi.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TSUKUYOMI_H
#define UI_TSUKUYOMI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "renderwidget.h"

QT_BEGIN_NAMESPACE

class Ui_TsukuyomiClass
{
public:
    QWidget *centralWidget;
    RenderWidget *render_widget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TsukuyomiClass)
    {
        if (TsukuyomiClass->objectName().isEmpty())
            TsukuyomiClass->setObjectName(QStringLiteral("TsukuyomiClass"));
        TsukuyomiClass->resize(1051, 754);
        QIcon icon;
        icon.addFile(QStringLiteral("logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        TsukuyomiClass->setWindowIcon(icon);
        centralWidget = new QWidget(TsukuyomiClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        render_widget = new RenderWidget(centralWidget);
        render_widget->setObjectName(QStringLiteral("render_widget"));
        render_widget->setGeometry(QRect(30, 50, 640, 480));
        TsukuyomiClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TsukuyomiClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1051, 23));
        TsukuyomiClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TsukuyomiClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TsukuyomiClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TsukuyomiClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TsukuyomiClass->setStatusBar(statusBar);

        retranslateUi(TsukuyomiClass);

        QMetaObject::connectSlotsByName(TsukuyomiClass);
    } // setupUi

    void retranslateUi(QMainWindow *TsukuyomiClass)
    {
        TsukuyomiClass->setWindowTitle(QApplication::translate("TsukuyomiClass", "Tsukuyomi", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TsukuyomiClass: public Ui_TsukuyomiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TSUKUYOMI_H
