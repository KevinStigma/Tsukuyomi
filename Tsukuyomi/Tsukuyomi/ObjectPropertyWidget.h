#pragma once
#include <QWidget>
#include <QLineEdit>

class ObjectPropertyWidget :public QWidget
{
	Q_OBJECT
public:
	ObjectPropertyWidget(QWidget *parent = 0);
	~ObjectPropertyWidget();
protected:
	QLineEdit * scaleXLineEdit = nullptr, *scaleYLineEdit = nullptr, *scaleZLineEdit = nullptr;
	QLineEdit * transXLineEdit = nullptr, *transYLineEdit = nullptr, *transZLineEdit = nullptr;
	QLineEdit * rotXLineEdit = nullptr, *rotYLineEdit = nullptr, *rotZLineEdit = nullptr, *rotWLineEdit = nullptr;
};