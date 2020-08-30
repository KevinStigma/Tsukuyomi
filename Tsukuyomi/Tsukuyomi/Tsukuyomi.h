#pragma once

#include <QtWidgets/QMainWindow>
#include <qkeyevent.h>
#include "ui_Tsukuyomi.h"

class Tsukuyomi : public QMainWindow
{
	Q_OBJECT

public:
	Tsukuyomi(QWidget *parent = Q_NULLPTR);
	~Tsukuyomi();

protected:
	void keyReleaseEvent(QKeyEvent *event);

public slots:
	void on_actionLoad_Mesh_triggered();
	void on_actionLoad_Project_triggered();
	void on_actionLoadEnvMap_triggered();
	void on_actionBack_Irradiance_triggered();
	void on_actionBake_PreFilterEnvMpas_triggered();
	void on_actionSave_Project_triggered();
	void on_actionClear_Scene_triggered();
	void on_actionScreenShot_triggered();
	void on_actionExit_triggered();
	void on_actionCamera_triggered();
	void on_actionPointLight_triggered();
	void on_actionDirectionalLight_triggered();
	void on_actionAreaLight_triggered();
	void on_renderButton_clicked();
	void on_genBVHButton_clicked();
	void on_showBVHcheckBox_stateChanged(int);
	void on_RulerLine_checkBox_stateChanged(int);
	void on_MSAA_checkBox_stateChanged(int);
	void on_HDR_checkBox_stateChanged(int);
	void on_SSAO_checkBox_stateChanged(int);
	void on_Gamma_checkBox_stateChanged(int);
	void on_DebugTex_checkBox_stateChanged(int);
	void on_sampleComboBox_currentIndexChanged(int);
	void on_depthcomboBox_currentIndexChanged(int);
	void on_renderMethodcomboBox_currentIndexChanged(int);
	void on_writeFilecheckBox_stateChanged(int state);
	void on_transButton_clicked(bool checked);
	void on_rotButton_clicked(bool checked);
	void on_targetCamButton_clicked();
	void on_resetViewButton_clicked();
	void on_setShadowLightButton_clicked();

private:
	Ui::TsukuyomiClass ui;
};
