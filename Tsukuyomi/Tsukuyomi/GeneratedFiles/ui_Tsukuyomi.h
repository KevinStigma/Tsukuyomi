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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include "objectpropertywidget.h"
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
    QAction *actionScreenShot;
    QAction *actionLoadEnvMap;
    QAction *actionBack_Irradiance;
    QAction *actionBake_PreFilterEnvMpas;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *objsTreeWidget;
    RenderWidget *render_widget;
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QLabel *label_4;
    QWidget *widget;
    QPushButton *transButton;
    QPushButton *rotButton;
    QSlider *moveSpeedSlider;
    QCheckBox *MSAA_checkBox;
    QCheckBox *Gamma_checkBox;
    QPushButton *targetCamButton;
    QPushButton *resetViewButton;
    QPushButton *setShadowLightButton;
    QCheckBox *HDR_checkBox;
    QLabel *label_8;
    QSlider *moveSpeedSlider_hdr;
    QCheckBox *SSAO_checkBox;
    QCheckBox *DebugTex_checkBox;
    QCheckBox *RulerLine_checkBox;
    QWidget *tab;
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
    QWidget *tab_2;
    QComboBox *depthcomboBox;
    QCheckBox *writeFilecheckBox;
    QPushButton *renderButton;
    QLabel *label_5;
    QComboBox *sampleComboBox;
    QLabel *label_6;
    QPushButton *genBVHButton;
    QCheckBox *showBVHcheckBox;
    QComboBox *renderMethodcomboBox;
    QLabel *label_7;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menuObjects;
    QMenu *menuLights;
    QMenu *menuRenderOption;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TsukuyomiClass)
    {
        if (TsukuyomiClass->objectName().isEmpty())
            TsukuyomiClass->setObjectName(QString::fromUtf8("TsukuyomiClass"));
        TsukuyomiClass->resize(1200, 780);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TsukuyomiClass->sizePolicy().hasHeightForWidth());
        TsukuyomiClass->setSizePolicy(sizePolicy);
        TsukuyomiClass->setMinimumSize(QSize(1200, 780));
        QIcon icon;
        icon.addFile(QString::fromUtf8("C:/Users/linyimap/.designer/backup/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8("logo.png"), QSize(), QIcon::Normal, QIcon::On);
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
        actionScreenShot = new QAction(TsukuyomiClass);
        actionScreenShot->setObjectName(QString::fromUtf8("actionScreenShot"));
        actionLoadEnvMap = new QAction(TsukuyomiClass);
        actionLoadEnvMap->setObjectName(QString::fromUtf8("actionLoadEnvMap"));
        actionBack_Irradiance = new QAction(TsukuyomiClass);
        actionBack_Irradiance->setObjectName(QString::fromUtf8("actionBack_Irradiance"));
        actionBake_PreFilterEnvMpas = new QAction(TsukuyomiClass);
        actionBake_PreFilterEnvMpas->setObjectName(QString::fromUtf8("actionBake_PreFilterEnvMpas"));
        centralWidget = new QWidget(TsukuyomiClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        objsTreeWidget = new QTreeWidget(centralWidget);
        objsTreeWidget->setObjectName(QString::fromUtf8("objsTreeWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(objsTreeWidget->sizePolicy().hasHeightForWidth());
        objsTreeWidget->setSizePolicy(sizePolicy1);
        objsTreeWidget->setLineWidth(1);
        objsTreeWidget->setColumnCount(1);

        horizontalLayout->addWidget(objsTreeWidget);

        render_widget = new RenderWidget(centralWidget);
        render_widget->setObjectName(QString::fromUtf8("render_widget"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(render_widget->sizePolicy().hasHeightForWidth());
        render_widget->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(render_widget);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setMinimumSize(QSize(250, 0));
        tabWidget->setMaximumSize(QSize(300, 16777215));
        tabWidget->setMovable(false);
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        label_4 = new QLabel(tab_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 80, 91, 16));
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        font.setPointSize(12);
        label_4->setFont(font);
        widget = new QWidget(tab_3);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 10, 151, 61));
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
        moveSpeedSlider = new QSlider(tab_3);
        moveSpeedSlider->setObjectName(QString::fromUtf8("moveSpeedSlider"));
        moveSpeedSlider->setGeometry(QRect(110, 80, 120, 22));
        moveSpeedSlider->setMinimum(0);
        moveSpeedSlider->setMaximum(100);
        moveSpeedSlider->setValue(20);
        moveSpeedSlider->setOrientation(Qt::Horizontal);
        MSAA_checkBox = new QCheckBox(tab_3);
        MSAA_checkBox->setObjectName(QString::fromUtf8("MSAA_checkBox"));
        MSAA_checkBox->setGeometry(QRect(20, 190, 71, 15));
        QFont font1;
        font1.setPointSize(12);
        MSAA_checkBox->setFont(font1);
        Gamma_checkBox = new QCheckBox(tab_3);
        Gamma_checkBox->setObjectName(QString::fromUtf8("Gamma_checkBox"));
        Gamma_checkBox->setGeometry(QRect(20, 230, 151, 16));
        Gamma_checkBox->setFont(font1);
        targetCamButton = new QPushButton(tab_3);
        targetCamButton->setObjectName(QString::fromUtf8("targetCamButton"));
        targetCamButton->setGeometry(QRect(60, 310, 141, 31));
        targetCamButton->setFont(font1);
        resetViewButton = new QPushButton(tab_3);
        resetViewButton->setObjectName(QString::fromUtf8("resetViewButton"));
        resetViewButton->setGeometry(QRect(60, 350, 141, 31));
        resetViewButton->setFont(font1);
        setShadowLightButton = new QPushButton(tab_3);
        setShadowLightButton->setObjectName(QString::fromUtf8("setShadowLightButton"));
        setShadowLightButton->setGeometry(QRect(60, 390, 141, 31));
        setShadowLightButton->setFont(font1);
        HDR_checkBox = new QCheckBox(tab_3);
        HDR_checkBox->setObjectName(QString::fromUtf8("HDR_checkBox"));
        HDR_checkBox->setGeometry(QRect(20, 170, 71, 15));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(false);
        font2.setWeight(50);
        HDR_checkBox->setFont(font2);
        label_8 = new QLabel(tab_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 120, 91, 16));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Consolas"));
        font3.setPointSize(10);
        label_8->setFont(font3);
        moveSpeedSlider_hdr = new QSlider(tab_3);
        moveSpeedSlider_hdr->setObjectName(QString::fromUtf8("moveSpeedSlider_hdr"));
        moveSpeedSlider_hdr->setGeometry(QRect(110, 120, 120, 22));
        moveSpeedSlider_hdr->setMinimum(0);
        moveSpeedSlider_hdr->setMaximum(100);
        moveSpeedSlider_hdr->setValue(10);
        moveSpeedSlider_hdr->setOrientation(Qt::Horizontal);
        SSAO_checkBox = new QCheckBox(tab_3);
        SSAO_checkBox->setObjectName(QString::fromUtf8("SSAO_checkBox"));
        SSAO_checkBox->setGeometry(QRect(20, 210, 71, 15));
        SSAO_checkBox->setFont(font1);
        DebugTex_checkBox = new QCheckBox(tab_3);
        DebugTex_checkBox->setObjectName(QString::fromUtf8("DebugTex_checkBox"));
        DebugTex_checkBox->setGeometry(QRect(20, 250, 101, 16));
        DebugTex_checkBox->setFont(font1);
        RulerLine_checkBox = new QCheckBox(tab_3);
        RulerLine_checkBox->setObjectName(QString::fromUtf8("RulerLine_checkBox"));
        RulerLine_checkBox->setGeometry(QRect(20, 270, 101, 16));
        RulerLine_checkBox->setFont(font1);
        RulerLine_checkBox->setChecked(true);
        tabWidget->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        propertyWidget = new ObjectPropertyWidget(tab);
        propertyWidget->setObjectName(QString::fromUtf8("propertyWidget"));
        propertyWidget->setGeometry(QRect(10, 20, 261, 101));
        label = new QLabel(propertyWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 61, 16));
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
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        depthcomboBox = new QComboBox(tab_2);
        depthcomboBox->addItem(QString());
        depthcomboBox->addItem(QString());
        depthcomboBox->addItem(QString());
        depthcomboBox->setObjectName(QString::fromUtf8("depthcomboBox"));
        depthcomboBox->setGeometry(QRect(90, 60, 69, 22));
        writeFilecheckBox = new QCheckBox(tab_2);
        writeFilecheckBox->setObjectName(QString::fromUtf8("writeFilecheckBox"));
        writeFilecheckBox->setGeometry(QRect(160, 150, 101, 21));
        writeFilecheckBox->setFont(font1);
        renderButton = new QPushButton(tab_2);
        renderButton->setObjectName(QString::fromUtf8("renderButton"));
        renderButton->setGeometry(QRect(60, 240, 111, 41));
        renderButton->setFont(font);
        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 20, 54, 21));
        label_5->setFont(font1);
        sampleComboBox = new QComboBox(tab_2);
        sampleComboBox->addItem(QString());
        sampleComboBox->addItem(QString());
        sampleComboBox->addItem(QString());
        sampleComboBox->addItem(QString());
        sampleComboBox->addItem(QString());
        sampleComboBox->addItem(QString());
        sampleComboBox->addItem(QString());
        sampleComboBox->setObjectName(QString::fromUtf8("sampleComboBox"));
        sampleComboBox->setGeometry(QRect(90, 20, 69, 22));
        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 60, 54, 21));
        label_6->setFont(font1);
        genBVHButton = new QPushButton(tab_2);
        genBVHButton->setObjectName(QString::fromUtf8("genBVHButton"));
        genBVHButton->setGeometry(QRect(60, 190, 111, 41));
        genBVHButton->setFont(font);
        showBVHcheckBox = new QCheckBox(tab_2);
        showBVHcheckBox->setObjectName(QString::fromUtf8("showBVHcheckBox"));
        showBVHcheckBox->setGeometry(QRect(20, 150, 91, 21));
        showBVHcheckBox->setFont(font1);
        renderMethodcomboBox = new QComboBox(tab_2);
        renderMethodcomboBox->addItem(QString());
        renderMethodcomboBox->addItem(QString());
        renderMethodcomboBox->addItem(QString());
        renderMethodcomboBox->setObjectName(QString::fromUtf8("renderMethodcomboBox"));
        renderMethodcomboBox->setGeometry(QRect(90, 100, 111, 22));
        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 100, 54, 21));
        label_7->setFont(font1);
        tabWidget->addTab(tab_2, QString());

        horizontalLayout->addWidget(tabWidget);

        TsukuyomiClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TsukuyomiClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1200, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menuObjects = new QMenu(menuBar);
        menuObjects->setObjectName(QString::fromUtf8("menuObjects"));
        menuLights = new QMenu(menuObjects);
        menuLights->setObjectName(QString::fromUtf8("menuLights"));
        menuRenderOption = new QMenu(menuBar);
        menuRenderOption->setObjectName(QString::fromUtf8("menuRenderOption"));
        TsukuyomiClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TsukuyomiClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        TsukuyomiClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TsukuyomiClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        TsukuyomiClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menuObjects->menuAction());
        menuBar->addAction(menuRenderOption->menuAction());
        menu->addAction(actionLoad_Mesh);
        menu->addAction(actionLoadEnvMap);
        menu->addAction(actionLoad_Project);
        menu->addAction(actionSave_Project);
        menu->addAction(actionClear_Scene);
        menu->addAction(actionExit);
        menuObjects->addAction(actionCamera);
        menuObjects->addAction(menuLights->menuAction());
        menuLights->addAction(actionPointLight);
        menuLights->addAction(actionDirectionalLight);
        menuLights->addAction(actionAreaLight);
        menuRenderOption->addAction(actionScreenShot);
        menuRenderOption->addAction(actionBack_Irradiance);
        menuRenderOption->addAction(actionBake_PreFilterEnvMpas);

        retranslateUi(TsukuyomiClass);

        tabWidget->setCurrentIndex(0);
        sampleComboBox->setCurrentIndex(0);
        renderMethodcomboBox->setCurrentIndex(2);


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
        actionScreenShot->setText(QApplication::translate("TsukuyomiClass", "ScreenShot", nullptr));
        actionLoadEnvMap->setText(QApplication::translate("TsukuyomiClass", "Load EnvMap", nullptr));
        actionBack_Irradiance->setText(QApplication::translate("TsukuyomiClass", "Back Irradiance", nullptr));
        actionBake_PreFilterEnvMpas->setText(QApplication::translate("TsukuyomiClass", "Bake PreFilterEnvMaps", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = objsTreeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("TsukuyomiClass", "scene", nullptr));
        label_4->setText(QApplication::translate("TsukuyomiClass", "Move speed", nullptr));
        transButton->setText(QApplication::translate("TsukuyomiClass", "Trans", nullptr));
        rotButton->setText(QApplication::translate("TsukuyomiClass", "Rot", nullptr));
        MSAA_checkBox->setText(QApplication::translate("TsukuyomiClass", "MSAA", nullptr));
        Gamma_checkBox->setText(QApplication::translate("TsukuyomiClass", "Gamma Correction", nullptr));
        targetCamButton->setText(QApplication::translate("TsukuyomiClass", "Target To Cam", nullptr));
        resetViewButton->setText(QApplication::translate("TsukuyomiClass", "Reset View", nullptr));
        setShadowLightButton->setText(QApplication::translate("TsukuyomiClass", "Set Shadow Light", nullptr));
        HDR_checkBox->setText(QApplication::translate("TsukuyomiClass", "HDR", nullptr));
        label_8->setText(QApplication::translate("TsukuyomiClass", "HDR Exposure", nullptr));
        SSAO_checkBox->setText(QApplication::translate("TsukuyomiClass", "SSAO", nullptr));
        DebugTex_checkBox->setText(QApplication::translate("TsukuyomiClass", "Debug Tex", nullptr));
        RulerLine_checkBox->setText(QApplication::translate("TsukuyomiClass", "RulerLine", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("TsukuyomiClass", "view", nullptr));
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
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("TsukuyomiClass", "Object", nullptr));
        depthcomboBox->setItemText(0, QApplication::translate("TsukuyomiClass", "1", nullptr));
        depthcomboBox->setItemText(1, QApplication::translate("TsukuyomiClass", "3", nullptr));
        depthcomboBox->setItemText(2, QApplication::translate("TsukuyomiClass", "5", nullptr));

        writeFilecheckBox->setText(QApplication::translate("TsukuyomiClass", "WriteFile", nullptr));
        renderButton->setText(QApplication::translate("TsukuyomiClass", "Render", nullptr));
        label_5->setText(QApplication::translate("TsukuyomiClass", "sample", nullptr));
        sampleComboBox->setItemText(0, QApplication::translate("TsukuyomiClass", "1", nullptr));
        sampleComboBox->setItemText(1, QApplication::translate("TsukuyomiClass", "16", nullptr));
        sampleComboBox->setItemText(2, QApplication::translate("TsukuyomiClass", "64", nullptr));
        sampleComboBox->setItemText(3, QApplication::translate("TsukuyomiClass", "126", nullptr));
        sampleComboBox->setItemText(4, QApplication::translate("TsukuyomiClass", "512", nullptr));
        sampleComboBox->setItemText(5, QApplication::translate("TsukuyomiClass", "1024", nullptr));
        sampleComboBox->setItemText(6, QApplication::translate("TsukuyomiClass", "2048", nullptr));

        label_6->setText(QApplication::translate("TsukuyomiClass", "depth", nullptr));
        genBVHButton->setText(QApplication::translate("TsukuyomiClass", "genBVH", nullptr));
        showBVHcheckBox->setText(QApplication::translate("TsukuyomiClass", "showBVH", nullptr));
        renderMethodcomboBox->setItemText(0, QApplication::translate("TsukuyomiClass", "Normal", nullptr));
        renderMethodcomboBox->setItemText(1, QApplication::translate("TsukuyomiClass", "Whitted", nullptr));
        renderMethodcomboBox->setItemText(2, QApplication::translate("TsukuyomiClass", "PathTracing", nullptr));

        label_7->setText(QApplication::translate("TsukuyomiClass", "method", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("TsukuyomiClass", "OfflineRender", nullptr));
        menu->setTitle(QApplication::translate("TsukuyomiClass", "Files", nullptr));
        menuObjects->setTitle(QApplication::translate("TsukuyomiClass", "Objects", nullptr));
        menuLights->setTitle(QApplication::translate("TsukuyomiClass", "Lights", nullptr));
        menuRenderOption->setTitle(QApplication::translate("TsukuyomiClass", "Options", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TsukuyomiClass: public Ui_TsukuyomiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TSUKUYOMI_H
