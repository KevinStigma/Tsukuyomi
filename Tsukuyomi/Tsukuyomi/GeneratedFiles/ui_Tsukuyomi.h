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
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "objectslistwidget.h"
#include "renderwidget.h"

QT_BEGIN_NAMESPACE

class Ui_TsukuyomiClass
{
public:
    QAction *actionLoad_Mesh;
    QAction *actionLoad_Project;
    QWidget *centralWidget;
    RenderWidget *render_widget;
    ObjectsListWidget *objectsListView;
    QWidget *propertyWidget;
    QLabel *label;
    QLineEdit *sx_lineEdit;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *sy_lineEdit;
    QLineEdit *sz_lineEdit;
    QLineEdit *ty_lineEdit;
    QLineEdit *tx_lineEdit;
    QLineEdit *tz_lineEdit;
    QLineEdit *rz_lineEdit;
    QLineEdit *ry_lineEdit;
    QLineEdit *rx_lineEdit;
    QLineEdit *rw_lineEdit;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TsukuyomiClass)
    {
        if (TsukuyomiClass->objectName().isEmpty())
            TsukuyomiClass->setObjectName(QStringLiteral("TsukuyomiClass"));
        TsukuyomiClass->resize(1192, 754);
        QIcon icon;
        icon.addFile(QStringLiteral("logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        TsukuyomiClass->setWindowIcon(icon);
        actionLoad_Mesh = new QAction(TsukuyomiClass);
        actionLoad_Mesh->setObjectName(QStringLiteral("actionLoad_Mesh"));
        actionLoad_Project = new QAction(TsukuyomiClass);
        actionLoad_Project->setObjectName(QStringLiteral("actionLoad_Project"));
        centralWidget = new QWidget(TsukuyomiClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        render_widget = new RenderWidget(centralWidget);
        render_widget->setObjectName(QStringLiteral("render_widget"));
        render_widget->setGeometry(QRect(180, 30, 640, 480));
        objectsListView = new ObjectsListWidget(centralWidget);
        objectsListView->setObjectName(QStringLiteral("objectsListView"));
        objectsListView->setGeometry(QRect(10, 30, 161, 471));
        propertyWidget = new QWidget(centralWidget);
        propertyWidget->setObjectName(QStringLiteral("propertyWidget"));
        propertyWidget->setGeometry(QRect(840, 30, 321, 481));
        label = new QLabel(propertyWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 61, 16));
        QFont font;
        font.setFamily(QStringLiteral("Consolas"));
        font.setPointSize(12);
        label->setFont(font);
        sx_lineEdit = new QLineEdit(propertyWidget);
        sx_lineEdit->setObjectName(QStringLiteral("sx_lineEdit"));
        sx_lineEdit->setGeometry(QRect(110, 10, 41, 20));
        label_2 = new QLabel(propertyWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(0, 40, 111, 16));
        label_2->setFont(font);
        label_3 = new QLabel(propertyWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 70, 54, 11));
        label_3->setFont(font);
        sy_lineEdit = new QLineEdit(propertyWidget);
        sy_lineEdit->setObjectName(QStringLiteral("sy_lineEdit"));
        sy_lineEdit->setGeometry(QRect(160, 10, 41, 20));
        sz_lineEdit = new QLineEdit(propertyWidget);
        sz_lineEdit->setObjectName(QStringLiteral("sz_lineEdit"));
        sz_lineEdit->setGeometry(QRect(210, 10, 41, 20));
        ty_lineEdit = new QLineEdit(propertyWidget);
        ty_lineEdit->setObjectName(QStringLiteral("ty_lineEdit"));
        ty_lineEdit->setGeometry(QRect(160, 40, 41, 20));
        tx_lineEdit = new QLineEdit(propertyWidget);
        tx_lineEdit->setObjectName(QStringLiteral("tx_lineEdit"));
        tx_lineEdit->setGeometry(QRect(110, 40, 41, 20));
        tz_lineEdit = new QLineEdit(propertyWidget);
        tz_lineEdit->setObjectName(QStringLiteral("tz_lineEdit"));
        tz_lineEdit->setGeometry(QRect(210, 40, 41, 20));
        rz_lineEdit = new QLineEdit(propertyWidget);
        rz_lineEdit->setObjectName(QStringLiteral("rz_lineEdit"));
        rz_lineEdit->setGeometry(QRect(200, 70, 41, 20));
        ry_lineEdit = new QLineEdit(propertyWidget);
        ry_lineEdit->setObjectName(QStringLiteral("ry_lineEdit"));
        ry_lineEdit->setGeometry(QRect(140, 70, 41, 20));
        rx_lineEdit = new QLineEdit(propertyWidget);
        rx_lineEdit->setObjectName(QStringLiteral("rx_lineEdit"));
        rx_lineEdit->setGeometry(QRect(90, 70, 41, 20));
        rw_lineEdit = new QLineEdit(propertyWidget);
        rw_lineEdit->setObjectName(QStringLiteral("rw_lineEdit"));
        rw_lineEdit->setGeometry(QRect(250, 70, 41, 20));
        TsukuyomiClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TsukuyomiClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1192, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        TsukuyomiClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TsukuyomiClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TsukuyomiClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TsukuyomiClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TsukuyomiClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(actionLoad_Mesh);
        menu->addAction(actionLoad_Project);

        retranslateUi(TsukuyomiClass);

        QMetaObject::connectSlotsByName(TsukuyomiClass);
    } // setupUi

    void retranslateUi(QMainWindow *TsukuyomiClass)
    {
        TsukuyomiClass->setWindowTitle(QApplication::translate("TsukuyomiClass", "Tsukuyomi", Q_NULLPTR));
        actionLoad_Mesh->setText(QApplication::translate("TsukuyomiClass", "Load Mesh", Q_NULLPTR));
        actionLoad_Project->setText(QApplication::translate("TsukuyomiClass", "Load Project", Q_NULLPTR));
        label->setText(QApplication::translate("TsukuyomiClass", "Scale", Q_NULLPTR));
        label_2->setText(QApplication::translate("TsukuyomiClass", "Translation", Q_NULLPTR));
        label_3->setText(QApplication::translate("TsukuyomiClass", "Rot", Q_NULLPTR));
        menu->setTitle(QApplication::translate("TsukuyomiClass", "Files", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TsukuyomiClass: public Ui_TsukuyomiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TSUKUYOMI_H
