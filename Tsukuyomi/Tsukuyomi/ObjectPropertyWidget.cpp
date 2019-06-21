#include "ObjectPropertyWidget.h"
#include "Objects/Object.h"
#include <iostream>

ObjectPropertyWidget::ObjectPropertyWidget(QWidget *parent):QWidget(parent)
{
	
}

ObjectPropertyWidget::~ObjectPropertyWidget()
{

}

void ObjectPropertyWidget::init()
{
	scaleXLineEdit = findChild<QLineEdit*>(QString("sx_lineEdit"));
	scaleYLineEdit = findChild<QLineEdit*>(QString("sy_lineEdit"));
	scaleZLineEdit = findChild<QLineEdit*>(QString("sz_lineEdit"));

	transXLineEdit = findChild<QLineEdit*>(QString("tx_lineEdit"));
	transYLineEdit = findChild<QLineEdit*>(QString("ty_lineEdit"));
	transZLineEdit = findChild<QLineEdit*>(QString("tz_lineEdit"));

	rotXLineEdit = findChild<QLineEdit*>(QString("rx_lineEdit"));
	rotYLineEdit = findChild<QLineEdit*>(QString("ry_lineEdit"));
	rotZLineEdit = findChild<QLineEdit*>(QString("rz_lineEdit"));
	rotWLineEdit = findChild<QLineEdit*>(QString("rw_lineEdit"));

	connect(scaleXLineEdit, SIGNAL(editingFinished()), this, SLOT(scaleXLineEditingFinished()));
	connect(scaleYLineEdit, SIGNAL(editingFinished()), this, SLOT(scaleYLineEditingFinished()));
	connect(scaleZLineEdit, SIGNAL(editingFinished()), this, SLOT(scaleZLineEditingFinished()));

	connect(transXLineEdit, SIGNAL(editingFinished()), this, SLOT(transXLineEditingFinished()));
	connect(transYLineEdit, SIGNAL(editingFinished()), this, SLOT(transYLineEditingFinished()));
	connect(transZLineEdit, SIGNAL(editingFinished()), this, SLOT(transZLineEditingFinished()));
}

void ObjectPropertyWidget::updateObjectProperty(Object* object)
{
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

void ObjectPropertyWidget::scaleXLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;
	auto & scale = object->scale;
	scale.x = scaleXLineEdit->text().toFloat();
}

void ObjectPropertyWidget::scaleYLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;
	auto & scale = object->scale;
	scale.y = scaleYLineEdit->text().toFloat();
}

void ObjectPropertyWidget::scaleZLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;
	auto & scale = object->scale;
	scale.z = scaleZLineEdit->text().toFloat();
}

void ObjectPropertyWidget::transXLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;
	auto & translation = object->translation;
	translation.x = transXLineEdit->text().toFloat();
}

void ObjectPropertyWidget::transYLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;
	auto & translation = object->translation;
	translation.y = transYLineEdit->text().toFloat();
}

void ObjectPropertyWidget::transZLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;
	auto & translation = object->translation;
	translation.z = transZLineEdit->text().toFloat();
}