#include "ObjectsListWidget.h"
#include "GlobalSys.h"

ObjectsListWidget::ObjectsListWidget(QWidget *parent): QListWidget(parent)
{

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

void ObjectsListWidget::setPropertyWidget(QWidget* widget)
{
	propertyWidget = widget;
	scaleXLineEdit = propertyWidget->findChild<QLineEdit*>(QString("sx_lineEdit"));
	scaleYLineEdit = propertyWidget->findChild<QLineEdit*>(QString("sy_lineEdit"));
	scaleZLineEdit = propertyWidget->findChild<QLineEdit*>(QString("sz_lineEdit"));

	transXLineEdit = propertyWidget->findChild<QLineEdit*>(QString("tx_lineEdit"));
	transYLineEdit = propertyWidget->findChild<QLineEdit*>(QString("ty_lineEdit"));
	transZLineEdit = propertyWidget->findChild<QLineEdit*>(QString("tz_lineEdit"));

	rotXLineEdit = propertyWidget->findChild<QLineEdit*>(QString("rx_lineEdit"));
	rotYLineEdit = propertyWidget->findChild<QLineEdit*>(QString("ry_lineEdit"));
	rotZLineEdit = propertyWidget->findChild<QLineEdit*>(QString("rz_lineEdit"));
	rotWLineEdit = propertyWidget->findChild<QLineEdit*>(QString("rw_lineEdit"));
}

void ObjectsListWidget::showCurItemProperty(QListWidgetItem*item)
{
	if (!item)
		return;
	Object* object = g_pGlobalSys->objectManager.getObjectFromName(item->text().toStdString());
	scaleXLineEdit->setText(QString(std::to_string(object->scale.x).c_str()));
	scaleYLineEdit->setText(QString(std::to_string(object->scale.y).c_str()));
	scaleZLineEdit->setText(QString(std::to_string(object->scale.z).c_str()));

	transXLineEdit->setText(QString(std::to_string(object->translation.x).c_str()));
	transYLineEdit->setText(QString(std::to_string(object->translation.y).c_str()));
	transZLineEdit->setText(QString(std::to_string(object->translation.z).c_str()));

	rotXLineEdit->setText(QString(std::to_string(object->rotation.x).c_str()));
	rotYLineEdit->setText(QString(std::to_string(object->rotation.y).c_str()));
	rotZLineEdit->setText(QString(std::to_string(object->rotation.z).c_str()));
	rotWLineEdit->setText(QString(std::to_string(object->rotation.w).c_str()));
}


