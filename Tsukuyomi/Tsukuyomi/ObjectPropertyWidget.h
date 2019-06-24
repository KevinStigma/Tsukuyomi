#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QEvent>
#include "GlobalSys.h"

class ObjectPropertyWidget :public QWidget
{
	Q_OBJECT
public:
	ObjectPropertyWidget(QWidget *parent = 0);
	~ObjectPropertyWidget();
	void updateObjectProperty(Object* object);
	void init();

public slots:
	void scaleXLineEditingFinished();
	void scaleYLineEditingFinished();
	void scaleZLineEditingFinished();
	void transXLineEditingFinished();
	void transYLineEditingFinished();
	void transZLineEditingFinished();
	void rotXLineEditingFinished();
	void rotYLineEditingFinished();
	void rotZLineEditingFinished();

protected:
	void leaveEvent(QEvent *);
	QLineEdit * scaleXLineEdit = nullptr, *scaleYLineEdit = nullptr, *scaleZLineEdit = nullptr;
	QLineEdit * transXLineEdit = nullptr, *transYLineEdit = nullptr, *transZLineEdit = nullptr;
	QLineEdit * rotXLineEdit = nullptr, *rotYLineEdit = nullptr, *rotZLineEdit = nullptr;
};