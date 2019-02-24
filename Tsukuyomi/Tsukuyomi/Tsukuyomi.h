#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Tsukuyomi.h"

class Tsukuyomi : public QMainWindow
{
	Q_OBJECT

public:
	Tsukuyomi(QWidget *parent = Q_NULLPTR);

private:
	Ui::TsukuyomiClass ui;
};
