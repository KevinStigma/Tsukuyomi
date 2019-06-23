/********************************************************************************
** Form generated from reading UI file 'Tsukuyomi.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TSUKUYOMI_H
#define UI_TSUKUYOMI_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "objectpropertywidget.h"
#include "objectslistwidget.h"
#include "renderwidget.h"

QT_BEGIN_NAMESPACE

class Ui_TsukuyomiClass
{
public:
    QAction *actionLoad_Mesh;
    QAction *actionLoad_Project;
    QAction *actionSave_Project;
    QWidget *centralWidget;
    RenderWidget *render_widget;
    ObjectsListWidget *objectsListView;
    ObjectPropertyWidget *propertyWidget;
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
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TsukuyomiClass)
    {
        if (TsukuyomiClass->objectName().isEmpty())
            TsukuyomiClass->setObjectName(QString::fromUtf8("TsukuyomiClass"));
        TsukuyomiClass->resize(1192, 754);
        QIcon icon;
        icon.addFile(QString::fromUtf8("logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        TsukuyomiClass->setWindowIcon(icon);
        actionLoad_Mesh = new QAction(TsukuyomiClass);
        actionLoad_Mesh->setObjectName(QString::fromUtf8("actionLoad_Mesh"));
        actionLoad_Project = new QAction(TsukuyomiClass);
        actionLoad_Project->setObjectName(QString::fromUtf8("actionLoad_Project"));
        actionSave_Project = new QAction(TsukuyomiClass);
        actionSave_Project->setObjectName(QString::fromUtf8("actionSave_Project"));
        centralWidget = new QWidget(TsukuyomiClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        render_widget = new RenderWidget(centralWidget);
        render_widget->setObjectName(QString::fromUtf8("render_widget"));
        render_widget->setGeometry(QRect(180, 30, 640, 480));
        objectsListView = new ObjectsListWidget(centralWidget);
        objectsListView->setObjectName(QString::fromUtf8("objectsListView"));
        objectsListView->setGeometry(QRect(10, 30, 161, 471));
        propertyWidget = new ObjectPropertyWidget(centralWidget);
        propertyWidget->setObjectName(QString::fromUtf8("propertyWidget"));
        propertyWidget->setGeometry(QRect(840, 30, 321, 481));
        label = new QLabel(propertyWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 61, 16));
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        font.setPointSize(12);
        label->setFont(font);
        sx_lineEdit = new QLineEdit(propertyWidget);
        sx_lineEdit->setObjectName(QString::fromUtf8("sx_lineEdit"));
        sx_lineEdit->setGeometry(QRect(110, 10, 41, 20));
        sx_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_2 = new QLabel(propertyWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 40, 111, 16));
        label_2->setFont(font);
        label_3 = new QLabel(propertyWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 70, 54, 11));
        label_3->setFont(font);
        sy_lineEdit = new QLineEdit(propertyWidget);
        sy_lineEdit->setObjectName(QString::fromUtf8("sy_lineEdit"));
        sy_lineEdit->setGeometry(QRect(160, 10, 41, 20));
        sy_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        sz_lineEdit = new QLineEdit(propertyWidget);
        sz_lineEdit->setObjectName(QString::fromUtf8("sz_lineEdit"));
        sz_lineEdit->setGeometry(QRect(210, 10, 41, 20));
        sz_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        ty_lineEdit = new QLineEdit(propertyWidget);
        ty_lineEdit->setObjectName(QString::fromUtf8("ty_lineEdit"));
        ty_lineEdit->setGeometry(QRect(160, 40, 41, 20));
        ty_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        tx_lineEdit = new QLineEdit(propertyWidget);
        tx_lineEdit->setObjectName(QString::fromUtf8("tx_lineEdit"));
        tx_lineEdit->setGeometry(QRect(110, 40, 41, 20));
        tx_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        tz_lineEdit = new QLineEdit(propertyWidget);
        tz_lineEdit->setObjectName(QString::fromUtf8("tz_lineEdit"));
        tz_lineEdit->setGeometry(QRect(210, 40, 41, 20));
        tz_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        rz_lineEdit = new QLineEdit(propertyWidget);
        rz_lineEdit->setObjectName(QString::fromUtf8("rz_lineEdit"));
        rz_lineEdit->setGeometry(QRect(210, 70, 41, 20));
        rz_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        ry_lineEdit = new QLineEdit(propertyWidget);
        ry_lineEdit->setObjectName(QString::fromUtf8("ry_lineEdit"));
        ry_lineEdit->setGeometry(QRect(160, 70, 41, 20));
        ry_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        rx_lineEdit = new QLineEdit(propertyWidget);
        rx_lineEdit->setObjectName(QString::fromUtf8("rx_lineEdit"));
        rx_lineEdit->setGeometry(QRect(110, 70, 41, 20));
        rx_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        TsukuyomiClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TsukuyomiClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1192, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        TsukuyomiClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TsukuyomiClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        TsukuyomiClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TsukuyomiClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        TsukuyomiClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(actionLoad_Mesh);
        menu->addAction(actionLoad_Project);
        menu->addAction(actionSave_Project);

        retranslateUi(TsukuyomiClass);

        QMetaObject::connectSlotsByName(TsukuyomiClass);
    } // setupUi

    void retranslateUi(QMainWindow *TsukuyomiClass)
    {
        TsukuyomiClass->setWindowTitle(QApplication::translate("TsukuyomiClass", "Tsukuyomi", nullptr));
        actionLoad_Mesh->setText(QApplication::translate("TsukuyomiClass", "Load Mesh", nullptr));
        actionLoad_Project->setText(QApplication::translate("TsukuyomiClass", "Load Project", nullptr));
        actionSave_Project->setText(QApplication::translate("TsukuyomiClass", "Save Project", nullptr));
        label->setText(QApplication::translate("TsukuyomiClass", "Scale", nullptr));
        sx_lineEdit->setInputMask(QString());
        sx_lineEdit->setText(QString());
        label_2->setText(QApplication::translate("TsukuyomiClass", "Translation", nullptr));
        label_3->setText(QApplication::translate("TsukuyomiClass", "Rot", nullptr));
        sy_lineEdit->setInputMask(QString());
        sy_lineEdit->setText(QString());
        sz_lineEdit->setInputMask(QString());
        sz_lineEdit->setText(QString());
        ty_lineEdit->setInputMask(QString());
        ty_lineEdit->setText(QString());
        tx_lineEdit->setInputMask(QString());
        tx_lineEdit->setText(QString());
        tz_lineEdit->setInputMask(QString());
        tz_lineEdit->setText(QString());
        rz_lineEdit->setInputMask(QString());
        rz_lineEdit->setText(QString());
        ry_lineEdit->setInputMask(QString());
        ry_lineEdit->setText(QString());
        rx_lineEdit->setInputMask(QString());
        rx_lineEdit->setText(QString());
        menu->setTitle(QApplication::translate("TsukuyomiClass", "Files", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TsukuyomiClass: public Ui_TsukuyomiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TSUKUYOMI_H
