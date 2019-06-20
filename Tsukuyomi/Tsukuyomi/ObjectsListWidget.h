#pragma once
#include "Objects\Object.h"
#include <QListWidget>
#include <QLineEdit>
#include <unordered_map>

class ObjectListWidgetItem : public QListWidgetItem
{
	//Q_OBJECT
public:
	ObjectListWidgetItem(std::string content);
	~ObjectListWidgetItem();
	QString curText = "";
};


class ObjectsListWidget :public QListWidget
{
	Q_OBJECT
public:
	ObjectsListWidget(QWidget *parent);
	~ObjectsListWidget();
	void addItem(const QString &label);
	void addItem(QListWidgetItem *item);
	void updateObjects(std::unordered_map<std::string, Object*>& objects);
	void showCurItemProperty(QListWidgetItem*item);
	void setPropertyWidget(QWidget* widget);

public slots:
	void updateItemName(QListWidgetItem * item);

protected:
	QWidget* propertyWidget = nullptr;
	QLineEdit * scaleXLineEdit = nullptr, *scaleYLineEdit = nullptr, *scaleZLineEdit = nullptr;
	QLineEdit * transXLineEdit = nullptr, *transYLineEdit = nullptr, *transZLineEdit = nullptr;
	QLineEdit * rotXLineEdit = nullptr, *rotYLineEdit = nullptr, *rotZLineEdit = nullptr, *rotWLineEdit = nullptr;
};