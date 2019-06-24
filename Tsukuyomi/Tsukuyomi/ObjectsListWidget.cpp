#include "ObjectsListWidget.h"
#include "GlobalSys.h"
#include "ObjectPropertyWidget.h"
#include "RenderWidget.h"
#include <iostream>

ObjectListWidgetItem::ObjectListWidgetItem(std::string content):QListWidgetItem(QString(content.c_str()))
{
	curText = text();
}

ObjectListWidgetItem::~ObjectListWidgetItem()
{

}


ObjectsListWidget::ObjectsListWidget(QWidget *parent): QListWidget(parent)
{
	connect(this, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(updateItemName(QListWidgetItem *)));
	connect(this, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(changeCurrentItem(QListWidgetItem *, QListWidgetItem *)));
	connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(doubleClickItem(QListWidgetItem *)));
}

ObjectsListWidget::~ObjectsListWidget()
{

}

void ObjectsListWidget::updateObjects(std::unordered_map<std::string, Object*>& objects)
{
	clear();
	for each (auto iter in objects)
		addItem(QString(iter.first.c_str()));
}

void ObjectsListWidget::setPropertyWidget(ObjectPropertyWidget* widget)
{
	propertyWidget = widget;
}

void ObjectsListWidget::showCurItemProperty(QListWidgetItem*item)
{
	if (!item)
		return;
	Object* object = g_pGlobalSys->objectManager.getObjectFromName(item->text().toStdString());
	propertyWidget->updateObjectProperty(object);
}

void ObjectsListWidget::updateItemName(QListWidgetItem * item)
{
	std::string new_name = item->text().toStdString();
	std::string cur_name = (((ObjectListWidgetItem*)item)->curText).toStdString();
	isEditing = false;
	if (new_name == cur_name)
		return;
	ObjectManager& object_mgr = g_pGlobalSys->objectManager;
	if (object_mgr.changeObjectName(cur_name, new_name))
		((ObjectListWidgetItem*)item)->curText = new_name.c_str();
	else
		item->setText(cur_name.c_str());
}

void ObjectsListWidget::changeCurrentItem(QListWidgetItem *current, QListWidgetItem *previous)
{
	if (!current)
		return;
	showCurItemProperty(current);
	ObjectManager& object_mgr = g_pGlobalSys->objectManager;
	object_mgr.setCurSelObject(current->text().toStdString());
}

void ObjectsListWidget::doubleClickItem(QListWidgetItem * item)
{
	isEditing = true;
}

void ObjectsListWidget::addItem(const QString &label)
{
	ObjectListWidgetItem* item = new ObjectListWidgetItem(label.toStdString());
	addItem(item);
}

void ObjectsListWidget::addItem(QListWidgetItem *item)
{
	QListWidget::addItem(item);
	item->setFlags(item->flags() | Qt::ItemIsEditable);
}
