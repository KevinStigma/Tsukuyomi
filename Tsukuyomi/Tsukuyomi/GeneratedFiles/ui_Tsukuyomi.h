/********************************************************************************
** Form generated from reading UI file 'Tsukuyomi.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
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
    QAction *actionClear_Scene;
    QAction *actionExit;
    QAction *actionCamera;
    QAction *actionPointLight;
    QAction *actionDirectionalLight;
    QAction *actionAreaLight;
    QAction *actionNormalDebug;
    QAction *actionPathTracing;
    QAction *actionWhitted;
    QAction *actiongenBVH;
    QAction *actionshowBVH;
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
    QSlider *moveSpeedSlider;
    QLabel *label_4;
    QWidget *widget;
    QPushButton *transButton;
    QPushButton *rotButton;
    QPushButton *renderButton;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menuObjects;
    QMenu *menuLights;
    QMenu *menuRenderOption;
    QMenu *menuOfflineRender;
    QMenu *menuType;
    QMenu *menuAccel;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TsukuyomiClass)
    {
        if (TsukuyomiClass->objectName().isEmpty())
            TsukuyomiClass->setObjectName(QString::fromUtf8("TsukuyomiClass"));
        TsukuyomiClass->resize(1170, 754);
        QIcon icon;
        icon.addFile(QString::fromUtf8("C:/Users/linyimap/.designer/backup/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        TsukuyomiClass->setWindowIcon(icon);
        actionLoad_Mesh = new QAction(TsukuyomiClass);
        actionLoad_Mesh->setObjectName(QString::fromUtf8("actionLoad_Mesh"));
        actionLoad_Project = new QAction(TsukuyomiClass);
        actionLoad_Project->setObjectName(QString::fromUtf8("actionLoad_Project"));
        actionSave_Project = new QAction(TsukuyomiClass);
        actionSave_Project->setObjectName(QString::fromUtf8("actionSave_Project"));
        actionClear_Scene = new QAction(TsukuyomiClass);
        actionClear_Scene->setObjectName(QString::fromUtf8("actionClear_Scene"));
        actionExit = new QAction(TsukuyomiClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionCamera = new QAction(TsukuyomiClass);
        actionCamera->setObjectName(QString::fromUtf8("actionCamera"));
        actionPointLight = new QAction(TsukuyomiClass);
        actionPointLight->setObjectName(QString::fromUtf8("actionPointLight"));
        actionDirectionalLight = new QAction(TsukuyomiClass);
        actionDirectionalLight->setObjectName(QString::fromUtf8("actionDirectionalLight"));
        actionAreaLight = new QAction(TsukuyomiClass);
        actionAreaLight->setObjectName(QString::fromUtf8("actionAreaLight"));
        actionNormalDebug = new QAction(TsukuyomiClass);
        actionNormalDebug->setObjectName(QString::fromUtf8("actionNormalDebug"));
        actionNormalDebug->setCheckable(true);
        actionNormalDebug->setChecked(true);
        actionPathTracing = new QAction(TsukuyomiClass);
        actionPathTracing->setObjectName(QString::fromUtf8("actionPathTracing"));
        actionPathTracing->setCheckable(true);
        actionWhitted = new QAction(TsukuyomiClass);
        actionWhitted->setObjectName(QString::fromUtf8("actionWhitted"));
        actionWhitted->setCheckable(true);
        actionWhitted->setChecked(false);
        actiongenBVH = new QAction(TsukuyomiClass);
        actiongenBVH->setObjectName(QString::fromUtf8("actiongenBVH"));
        actionshowBVH = new QAction(TsukuyomiClass);
        actionshowBVH->setObjectName(QString::fromUtf8("actionshowBVH"));
        actionshowBVH->setCheckable(true);
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
        propertyWidget->setGeometry(QRect(840, 30, 261, 101));
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
        moveSpeedSlider = new QSlider(centralWidget);
        moveSpeedSlider->setObjectName(QString::fromUtf8("moveSpeedSlider"));
        moveSpeedSlider->setGeometry(QRect(940, 140, 160, 22));
        moveSpeedSlider->setMinimum(0);
        moveSpeedSlider->setMaximum(100);
        moveSpeedSlider->setValue(20);
        moveSpeedSlider->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(840, 140, 91, 16));
        label_4->setFont(font);
        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(840, 180, 151, 61));
        transButton = new QPushButton(widget);
        transButton->setObjectName(QString::fromUtf8("transButton"));
        transButton->setGeometry(QRect(10, 10, 51, 41));
        transButton->setCheckable(true);
        transButton->setChecked(true);
        rotButton = new QPushButton(widget);
        rotButton->setObjectName(QString::fromUtf8("rotButton"));
        rotButton->setGeometry(QRect(80, 10, 51, 41));
        rotButton->setIconSize(QSize(16, 16));
        rotButton->setCheckable(true);
        renderButton = new QPushButton(centralWidget);
        renderButton->setObjectName(QString::fromUtf8("renderButton"));
        renderButton->setGeometry(QRect(850, 260, 111, 41));
        TsukuyomiClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TsukuyomiClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1170, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menuObjects = new QMenu(menuBar);
        menuObjects->setObjectName(QString::fromUtf8("menuObjects"));
        menuLights = new QMenu(menuObjects);
        menuLights->setObjectName(QString::fromUtf8("menuLights"));
        menuRenderOption = new QMenu(menuBar);
        menuRenderOption->setObjectName(QString::fromUtf8("menuRenderOption"));
        menuOfflineRender = new QMenu(menuRenderOption);
        menuOfflineRender->setObjectName(QString::fromUtf8("menuOfflineRender"));
        menuType = new QMenu(menuOfflineRender);
        menuType->setObjectName(QString::fromUtf8("menuType"));
        menuAccel = new QMenu(menuBar);
        menuAccel->setObjectName(QString::fromUtf8("menuAccel"));
        TsukuyomiClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TsukuyomiClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        TsukuyomiClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TsukuyomiClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        TsukuyomiClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menuObjects->menuAction());
        menuBar->addAction(menuAccel->menuAction());
        menuBar->addAction(menuRenderOption->menuAction());
        menu->addAction(actionLoad_Mesh);
        menu->addAction(actionLoad_Project);
        menu->addAction(actionSave_Project);
        menu->addAction(actionClear_Scene);
        menu->addAction(actionExit);
        menuObjects->addAction(actionCamera);
        menuObjects->addAction(menuLights->menuAction());
        menuLights->addAction(actionPointLight);
        menuLights->addAction(actionDirectionalLight);
        menuLights->addAction(actionAreaLight);
        menuRenderOption->addAction(menuOfflineRender->menuAction());
        menuOfflineRender->addAction(menuType->menuAction());
        menuType->addAction(actionNormalDebug);
        menuType->addAction(actionWhitted);
        menuType->addAction(actionPathTracing);
        menuAccel->addAction(actiongenBVH);
        menuAccel->addAction(actionshowBVH);

        retranslateUi(TsukuyomiClass);

        QMetaObject::connectSlotsByName(TsukuyomiClass);
    } // setupUi

    void retranslateUi(QMainWindow *TsukuyomiClass)
    {
        TsukuyomiClass->setWindowTitle(QApplication::translate("TsukuyomiClass", "Tsukuyomi", nullptr));
        actionLoad_Mesh->setText(QApplication::translate("TsukuyomiClass", "Load Mesh", nullptr));
        actionLoad_Project->setText(QApplication::translate("TsukuyomiClass", "Load Project", nullptr));
        actionSave_Project->setText(QApplication::translate("TsukuyomiClass", "Save Project", nullptr));
        actionClear_Scene->setText(QApplication::translate("TsukuyomiClass", "Clear Scene", nullptr));
        actionExit->setText(QApplication::translate("TsukuyomiClass", "Exit", nullptr));
        actionCamera->setText(QApplication::translate("TsukuyomiClass", "Camera", nullptr));
        actionPointLight->setText(QApplication::translate("TsukuyomiClass", "PointLight", nullptr));
        actionDirectionalLight->setText(QApplication::translate("TsukuyomiClass", "DirectionalLight", nullptr));
        actionAreaLight->setText(QApplication::translate("TsukuyomiClass", "AreaLight", nullptr));
        actionNormalDebug->setText(QApplication::translate("TsukuyomiClass", "NormalDebug", nullptr));
        actionPathTracing->setText(QApplication::translate("TsukuyomiClass", "PathTracing", nullptr));
        actionWhitted->setText(QApplication::translate("TsukuyomiClass", "Whitted", nullptr));
        actiongenBVH->setText(QApplication::translate("TsukuyomiClass", "genBVH", nullptr));
        actionshowBVH->setText(QApplication::translate("TsukuyomiClass", "showBVH", nullptr));
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
        label_4->setText(QApplication::translate("TsukuyomiClass", "Move speed", nullptr));
        transButton->setText(QApplication::translate("TsukuyomiClass", "Trans", nullptr));
        rotButton->setText(QApplication::translate("TsukuyomiClass", "Rot", nullptr));
        renderButton->setText(QApplication::translate("TsukuyomiClass", "Render", nullptr));
        menu->setTitle(QApplication::translate("TsukuyomiClass", "Files", nullptr));
        menuObjects->setTitle(QApplication::translate("TsukuyomiClass", "Objects", nullptr));
        menuLights->setTitle(QApplication::translate("TsukuyomiClass", "Lights", nullptr));
        menuRenderOption->setTitle(QApplication::translate("TsukuyomiClass", "Options", nullptr));
        menuOfflineRender->setTitle(QApplication::translate("TsukuyomiClass", "OfflineRender", nullptr));
        menuType->setTitle(QApplication::translate("TsukuyomiClass", "Type", nullptr));
        menuAccel->setTitle(QApplication::translate("TsukuyomiClass", "Accel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TsukuyomiClass: public Ui_TsukuyomiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TSUKUYOMI_H
