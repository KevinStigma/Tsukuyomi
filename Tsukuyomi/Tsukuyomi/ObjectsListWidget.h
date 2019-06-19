#pragma once
#include "Objects\Object.h"
#include <QListWidget>
#include <QLineEdit>
#include <unordered_map>

class ObjectsListWidget :public QListWidget
{
	Q_OBJECT
public:
	ObjectsListWidget(QWidget *parent);
	~ObjectsListWidget();
	void updateObjects(std::unordered_map<std::string, Object*>& objects);
	void showCurItemProperty(QListWidgetItem*item);
	void setPropertyWidget(QWidget* widget);
protected:
	QWidget* propertyWidget = nullptr;
	QLineEdit * scaleXLineEdit = nullptr, *scaleYLineEdit = nullptr, *scaleZLineEdit = nullptr;
	QLineEdit * transXLineEdit = nullptr, *transYLineEdit = nullptr, *transZLineEdit = nullptr;
	QLineEdit * rotXLineEdit = nullptr, *rotYLineEdit = nullptr, *rotZLineEdit = nullptr, *rotWLineEdit = nullptr;
};