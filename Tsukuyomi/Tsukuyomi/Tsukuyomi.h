#pragma once

#include <QtWidgets/QMainWindow>
#include <qkeyevent.h>
#include "ui_Tsukuyomi.h"

class Tsukuyomi : public QMainWindow
{
	Q_OBJECT

public:
	Tsukuyomi(QWidget *parent = Q_NULLPTR);

protected:
	void keyReleaseEvent(QKeyEvent *event);

public slots:
	void on_actionLoad_Mesh_triggered();
	void on_actionLoad_Project_triggered();
	void on_actionSave_Project_triggered();
	void on_actionClear_Scene_triggered();
	void on_actionNormalDebug_triggered();
	void on_actionWhitted_triggered();
	void on_actionPathTracing_triggered();
	void on_actionExit_triggered();
	void on_actiongenBVH_triggered();
	void on_actionshowBVH_triggered();
	void on_actionCamera_triggered();
	void on_actionPointLight_triggered();
	void on_actionDirectionalLight_triggered();
	void on_actionAreaLight_triggered();
	void on_renderButton_clicked();
	void on_transButton_clicked(bool checked);
	void on_rotButton_clicked(bool checked);

private:
	Ui::TsukuyomiClass ui;
};
