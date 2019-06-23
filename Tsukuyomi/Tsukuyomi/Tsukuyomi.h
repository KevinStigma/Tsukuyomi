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

private:
	Ui::TsukuyomiClass ui;
};
