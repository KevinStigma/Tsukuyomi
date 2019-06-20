#pragma once

#include <QtWidgets/QMainWindow>
#include <qkeyevent.h>
#include "ui_Tsukuyomi.h"

class Tsukuyomi : public QMainWindow
{
	Q_OBJECT

public:
	Tsukuyomi(QWidget *parent = Q_NULLPTR);
	void keyReleaseEvent(QKeyEvent *event);

public slots:
	void on_objectsListView_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
	void on_actionLoad_Mesh_triggered();
	void on_sx_lineEdit_editingFinished();

private:
	Ui::TsukuyomiClass ui;
};
