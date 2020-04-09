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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
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
    QCheckBox *writeFilecheckBox;
    QComboBox *sampleComboBox;
    QComboBox *depthcomboBox;
    QLabel *label_5;
    QLabel *label_6;
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
            TsukuyomiClass->setObjectName(QStringLiteral("TsukuyomiClass"));
        TsukuyomiClass->resize(1170, 754);
        QIcon icon;
        icon.addFile(QStringLiteral("C:/Users/linyimap/.designer/backup/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        TsukuyomiClass->setWindowIcon(icon);
        actionLoad_Mesh = new QAction(TsukuyomiClass);
        actionLoad_Mesh->setObjectName(QStringLiteral("actionLoad_Mesh"));
        actionLoad_Project = new QAction(TsukuyomiClass);
        actionLoad_Project->setObjectName(QStringLiteral("actionLoad_Project"));
        actionSave_Project = new QAction(TsukuyomiClass);
        actionSave_Project->setObjectName(QStringLiteral("actionSave_Project"));
        actionClear_Scene = new QAction(TsukuyomiClass);
        actionClear_Scene->setObjectName(QStringLiteral("actionClear_Scene"));
        actionExit = new QAction(TsukuyomiClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionCamera = new QAction(TsukuyomiClass);
        actionCamera->setObjectName(QStringLiteral("actionCamera"));
        actionPointLight = new QAction(TsukuyomiClass);
        actionPointLight->setObjectName(QStringLiteral("actionPointLight"));
        actionDirectionalLight = new QAction(TsukuyomiClass);
        actionDirectionalLight->setObjectName(QStringLiteral("actionDirectionalLight"));
        actionAreaLight = new QAction(TsukuyomiClass);
        actionAreaLight->setObjectName(QStringLiteral("actionAreaLight"));
        actionNormalDebug = new QAction(TsukuyomiClass);
        actionNormalDebug->setObjectName(QStringLiteral("actionNormalDebug"));
        actionNormalDebug->setCheckable(true);
        actionNormalDebug->setChecked(true);
        actionPathTracing = new QAction(TsukuyomiClass);
        actionPathTracing->setObjectName(QStringLiteral("actionPathTracing"));
        actionPathTracing->setCheckable(true);
        actionWhitted = new QAction(TsukuyomiClass);
        actionWhitted->setObjectName(QStringLiteral("actionWhitted"));
        actionWhitted->setCheckable(true);
        actionWhitted->setChecked(false);
        actiongenBVH = new QAction(TsukuyomiClass);
        actiongenBVH->setObjectName(QStringLiteral("actiongenBVH"));
        actionshowBVH = new QAction(TsukuyomiClass);
        actionshowBVH->setObjectName(QStringLiteral("actionshowBVH"));
        actionshowBVH->setCheckable(true);
        centralWidget = new QWidget(TsukuyomiClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        render_widget = new RenderWidget(centralWidget);
        render_widget->setObjectName(QStringLiteral("render_widget"));
        render_widget->setGeometry(QRect(180, 30, 640, 480));
        objectsListView = new ObjectsListWidget(centralWidget);
        objectsListView->setObjectName(QStringLiteral("objectsListView"));
        objectsListView->setGeometry(QRect(10, 30, 161, 471));
        propertyWidget = new ObjectPropertyWidget(centralWidget);
        propertyWidget->setObjectName(QStringLiteral("propertyWidget"));
        propertyWidget->setGeometry(QRect(840, 30, 261, 101));
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
        sx_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
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
        sy_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        sz_lineEdit = new QLineEdit(propertyWidget);
        sz_lineEdit->setObjectName(QStringLiteral("sz_lineEdit"));
        sz_lineEdit->setGeometry(QRect(210, 10, 41, 20));
        sz_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        ty_lineEdit = new QLineEdit(propertyWidget);
        ty_lineEdit->setObjectName(QStringLiteral("ty_lineEdit"));
        ty_lineEdit->setGeometry(QRect(160, 40, 41, 20));
        ty_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        tx_lineEdit = new QLineEdit(propertyWidget);
        tx_lineEdit->setObjectName(QStringLiteral("tx_lineEdit"));
        tx_lineEdit->setGeometry(QRect(110, 40, 41, 20));
        tx_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        tz_lineEdit = new QLineEdit(propertyWidget);
        tz_lineEdit->setObjectName(QStringLiteral("tz_lineEdit"));
        tz_lineEdit->setGeometry(QRect(210, 40, 41, 20));
        tz_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        rz_lineEdit = new QLineEdit(propertyWidget);
        rz_lineEdit->setObjectName(QStringLiteral("rz_lineEdit"));
        rz_lineEdit->setGeometry(QRect(210, 70, 41, 20));
        rz_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        ry_lineEdit = new QLineEdit(propertyWidget);
        ry_lineEdit->setObjectName(QStringLiteral("ry_lineEdit"));
        ry_lineEdit->setGeometry(QRect(160, 70, 41, 20));
        ry_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        rx_lineEdit = new QLineEdit(propertyWidget);
        rx_lineEdit->setObjectName(QStringLiteral("rx_lineEdit"));
        rx_lineEdit->setGeometry(QRect(110, 70, 41, 20));
        rx_lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        moveSpeedSlider = new QSlider(centralWidget);
        moveSpeedSlider->setObjectName(QStringLiteral("moveSpeedSlider"));
        moveSpeedSlider->setGeometry(QRect(940, 140, 160, 22));
        moveSpeedSlider->setMinimum(0);
        moveSpeedSlider->setMaximum(100);
        moveSpeedSlider->setValue(20);
        moveSpeedSlider->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(840, 140, 91, 16));
        label_4->setFont(font);
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(840, 180, 151, 61));
        transButton = new QPushButton(widget);
        transButton->setObjectName(QStringLiteral("transButton"));
        transButton->setGeometry(QRect(10, 10, 51, 41));
        transButton->setCheckable(true);
        transButton->setChecked(true);
        rotButton = new QPushButton(widget);
        rotButton->setObjectName(QStringLiteral("rotButton"));
        rotButton->setGeometry(QRect(80, 10, 51, 41));
        rotButton->setIconSize(QSize(16, 16));
        rotButton->setCheckable(true);
        renderButton = new QPushButton(centralWidget);
        renderButton->setObjectName(QStringLiteral("renderButton"));
        renderButton->setGeometry(QRect(850, 260, 111, 41));
        writeFilecheckBox = new QCheckBox(centralWidget);
        writeFilecheckBox->setObjectName(QStringLiteral("writeFilecheckBox"));
        writeFilecheckBox->setGeometry(QRect(1050, 260, 91, 21));
        sampleComboBox = new QComboBox(centralWidget);
        sampleComboBox->setObjectName(QStringLiteral("sampleComboBox"));
        sampleComboBox->setGeometry(QRect(1080, 180, 69, 22));
        depthcomboBox = new QComboBox(centralWidget);
        depthcomboBox->setObjectName(QStringLiteral("depthcomboBox"));
        depthcomboBox->setGeometry(QRect(1080, 220, 69, 22));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(1010, 180, 54, 21));
        QFont font1;
        font1.setPointSize(12);
        label_5->setFont(font1);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(1010, 220, 54, 21));
        label_6->setFont(font1);
        TsukuyomiClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TsukuyomiClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1170, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menuObjects = new QMenu(menuBar);
        menuObjects->setObjectName(QStringLiteral("menuObjects"));
        menuLights = new QMenu(menuObjects);
        menuLights->setObjectName(QStringLiteral("menuLights"));
        menuRenderOption = new QMenu(menuBar);
        menuRenderOption->setObjectName(QStringLiteral("menuRenderOption"));
        menuOfflineRender = new QMenu(menuRenderOption);
        menuOfflineRender->setObjectName(QStringLiteral("menuOfflineRender"));
        menuType = new QMenu(menuOfflineRender);
        menuType->setObjectName(QStringLiteral("menuType"));
        menuAccel = new QMenu(menuBar);
        menuAccel->setObjectName(QStringLiteral("menuAccel"));
        TsukuyomiClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TsukuyomiClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TsukuyomiClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TsukuyomiClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
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

        sampleComboBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TsukuyomiClass);
    } // setupUi

    void retranslateUi(QMainWindow *TsukuyomiClass)
    {
        TsukuyomiClass->setWindowTitle(QApplication::translate("TsukuyomiClass", "Tsukuyomi", Q_NULLPTR));
        actionLoad_Mesh->setText(QApplication::translate("TsukuyomiClass", "Load Mesh", Q_NULLPTR));
        actionLoad_Project->setText(QApplication::translate("TsukuyomiClass", "Load Project", Q_NULLPTR));
        actionSave_Project->setText(QApplication::translate("TsukuyomiClass", "Save Project", Q_NULLPTR));
        actionClear_Scene->setText(QApplication::translate("TsukuyomiClass", "Clear Scene", Q_NULLPTR));
        actionExit->setText(QApplication::translate("TsukuyomiClass", "Exit", Q_NULLPTR));
        actionCamera->setText(QApplication::translate("TsukuyomiClass", "Camera", Q_NULLPTR));
        actionPointLight->setText(QApplication::translate("TsukuyomiClass", "PointLight", Q_NULLPTR));
        actionDirectionalLight->setText(QApplication::translate("TsukuyomiClass", "DirectionalLight", Q_NULLPTR));
        actionAreaLight->setText(QApplication::translate("TsukuyomiClass", "AreaLight", Q_NULLPTR));
        actionNormalDebug->setText(QApplication::translate("TsukuyomiClass", "NormalDebug", Q_NULLPTR));
        actionPathTracing->setText(QApplication::translate("TsukuyomiClass", "PathTracing", Q_NULLPTR));
        actionWhitted->setText(QApplication::translate("TsukuyomiClass", "Whitted", Q_NULLPTR));
        actiongenBVH->setText(QApplication::translate("TsukuyomiClass", "genBVH", Q_NULLPTR));
        actionshowBVH->setText(QApplication::translate("TsukuyomiClass", "showBVH", Q_NULLPTR));
        label->setText(QApplication::translate("TsukuyomiClass", "Scale", Q_NULLPTR));
        sx_lineEdit->setInputMask(QString());
        sx_lineEdit->setText(QString());
        label_2->setText(QApplication::translate("TsukuyomiClass", "Translation", Q_NULLPTR));
        label_3->setText(QApplication::translate("TsukuyomiClass", "Rot", Q_NULLPTR));
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
        label_4->setText(QApplication::translate("TsukuyomiClass", "Move speed", Q_NULLPTR));
        transButton->setText(QApplication::translate("TsukuyomiClass", "Trans", Q_NULLPTR));
        rotButton->setText(QApplication::translate("TsukuyomiClass", "Rot", Q_NULLPTR));
        renderButton->setText(QApplication::translate("TsukuyomiClass", "Render", Q_NULLPTR));
        writeFilecheckBox->setText(QApplication::translate("TsukuyomiClass", "WriteFile", Q_NULLPTR));
        sampleComboBox->clear();
        sampleComboBox->insertItems(0, QStringList()
         << QApplication::translate("TsukuyomiClass", "1", Q_NULLPTR)
         << QApplication::translate("TsukuyomiClass", "16", Q_NULLPTR)
         << QApplication::translate("TsukuyomiClass", "64", Q_NULLPTR)
         << QApplication::translate("TsukuyomiClass", "126", Q_NULLPTR)
         << QApplication::translate("TsukuyomiClass", "512", Q_NULLPTR)
         << QApplication::translate("TsukuyomiClass", "1024", Q_NULLPTR)
         << QApplication::translate("TsukuyomiClass", "2048", Q_NULLPTR)
        );
        depthcomboBox->clear();
        depthcomboBox->insertItems(0, QStringList()
         << QApplication::translate("TsukuyomiClass", "1", Q_NULLPTR)
         << QApplication::translate("TsukuyomiClass", "3", Q_NULLPTR)
         << QApplication::translate("TsukuyomiClass", "5", Q_NULLPTR)
        );
        label_5->setText(QApplication::translate("TsukuyomiClass", "sample", Q_NULLPTR));
        label_6->setText(QApplication::translate("TsukuyomiClass", "depth", Q_NULLPTR));
        menu->setTitle(QApplication::translate("TsukuyomiClass", "Files", Q_NULLPTR));
        menuObjects->setTitle(QApplication::translate("TsukuyomiClass", "Objects", Q_NULLPTR));
        menuLights->setTitle(QApplication::translate("TsukuyomiClass", "Lights", Q_NULLPTR));
        menuRenderOption->setTitle(QApplication::translate("TsukuyomiClass", "Options", Q_NULLPTR));
        menuOfflineRender->setTitle(QApplication::translate("TsukuyomiClass", "OfflineRender", Q_NULLPTR));
        menuType->setTitle(QApplication::translate("TsukuyomiClass", "Type", Q_NULLPTR));
        menuAccel->setTitle(QApplication::translate("TsukuyomiClass", "Accel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TsukuyomiClass: public Ui_TsukuyomiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TSUKUYOMI_H
