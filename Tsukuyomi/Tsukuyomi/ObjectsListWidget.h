#pragma once
#include "Objects\Object.h"
#include <QListWidget>
#include <QLineEdit>
#include <unordered_map>

class ObjectPropertyWidget;

class ObjectListWidgetItem : public QListWidgetItem
{
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
	void setPropertyWidget(ObjectPropertyWidget* widget);

public slots:
	void updateItemName(QListWidgetItem * item);
	void changeCurrentItem(QListWidgetItem *current, QListWidgetItem *previous);

protected:
	ObjectPropertyWidget* propertyWidget = nullptr;
};