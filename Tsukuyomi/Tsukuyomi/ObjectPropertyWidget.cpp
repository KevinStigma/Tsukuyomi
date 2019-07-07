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

	connect(scaleXLineEdit, SIGNAL(editingFinished()), this, SLOT(scaleXLineEditingFinished()));
	connect(scaleYLineEdit, SIGNAL(editingFinished()), this, SLOT(scaleYLineEditingFinished()));
	connect(scaleZLineEdit, SIGNAL(editingFinished()), this, SLOT(scaleZLineEditingFinished()));

	connect(transXLineEdit, SIGNAL(editingFinished()), this, SLOT(transXLineEditingFinished()));
	connect(transYLineEdit, SIGNAL(editingFinished()), this, SLOT(transYLineEditingFinished()));
	connect(transZLineEdit, SIGNAL(editingFinished()), this, SLOT(transZLineEditingFinished()));

	connect(rotXLineEdit, SIGNAL(editingFinished()), this, SLOT(rotXLineEditingFinished()));
	connect(rotYLineEdit, SIGNAL(editingFinished()), this, SLOT(rotYLineEditingFinished()));
	connect(rotZLineEdit, SIGNAL(editingFinished()), this, SLOT(rotZLineEditingFinished()));

	QRegExp rx("^(-?[0]|-?[1-9][0-9]{0,5})(?:\\.\\d{1,4})?$|(^\\t?$)");
	QRegExpValidator *pReg =new QRegExpValidator(rx, this);
	scaleXLineEdit->setValidator(pReg);
	scaleYLineEdit->setValidator(pReg);
	scaleZLineEdit->setValidator(pReg);

	transXLineEdit->setValidator(pReg);
	transYLineEdit->setValidator(pReg);
	transZLineEdit->setValidator(pReg);

	rotXLineEdit->setValidator(pReg);
	rotYLineEdit->setValidator(pReg);
	rotZLineEdit->setValidator(pReg);
}

void ObjectPropertyWidget::updateObjectProperty(Object* object)
{
	XMFLOAT3 scale = object->getScale();
	scaleXLineEdit->setText(QString(std::to_string(scale.x).c_str()));
	scaleYLineEdit->setText(QString(std::to_string(scale.y).c_str()));
	scaleZLineEdit->setText(QString(std::to_string(scale.z).c_str()));

	XMFLOAT3 translation = object->getTranslation();
	transXLineEdit->setText(QString(std::to_string(translation.x).c_str()));
	transYLineEdit->setText(QString(std::to_string(translation.y).c_str()));
	transZLineEdit->setText(QString(std::to_string(translation.z).c_str()));

	XMFLOAT3 rotation = object->getRotation();
	rotXLineEdit->setText(QString(std::to_string(XMConvertToDegrees(rotation.x)).c_str()));
	rotYLineEdit->setText(QString(std::to_string(XMConvertToDegrees(rotation.y)).c_str()));
	rotZLineEdit->setText(QString(std::to_string(XMConvertToDegrees(rotation.z)).c_str()));
}

void ObjectPropertyWidget::updateObjectTranslation(Object* object)
{
	XMFLOAT3 translation = object->getTranslation();
	transXLineEdit->setText(QString(std::to_string(translation.x).c_str()));
	transYLineEdit->setText(QString(std::to_string(translation.y).c_str()));
	transZLineEdit->setText(QString(std::to_string(translation.z).c_str()));
}

void ObjectPropertyWidget::scaleXLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;
	auto scale = object->getScale();
	object->setScale(XMFLOAT3(scaleXLineEdit->text().toFloat(), scale.y, scale.z));
}

void ObjectPropertyWidget::scaleYLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;
	auto scale = object->getScale();
	object->setScale(XMFLOAT3(scale.x, scaleYLineEdit->text().toFloat(), scale.z));
}

void ObjectPropertyWidget::scaleZLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;

	auto scale = object->getScale();
	object->setScale(XMFLOAT3(scale.x, scale.y, scaleZLineEdit->text().toFloat()));
}

void ObjectPropertyWidget::transXLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;
	auto & translation = object->getTranslation();
	object->setTranslation(XMFLOAT3(transXLineEdit->text().toFloat(), translation.y, translation.z));
}

void ObjectPropertyWidget::transYLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;
	auto & translation = object->getTranslation();
	object->setTranslation(XMFLOAT3(translation.x, transYLineEdit->text().toFloat(), translation.z));
}

void ObjectPropertyWidget::transZLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;

	auto & translation = object->getTranslation();
	object->setTranslation(XMFLOAT3(translation.x, translation.y, transZLineEdit->text().toFloat()));
}

void ObjectPropertyWidget::rotXLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;
	auto & rotation = object->getRotation();
	object->setRotation(XMFLOAT3(XMConvertToRadians(rotXLineEdit->text().toFloat()), rotation.y, rotation.z));
}

void ObjectPropertyWidget::rotYLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;
	auto & rotation = object->getRotation();
	object->setRotation(XMFLOAT3(rotation.x, XMConvertToRadians(rotYLineEdit->text().toFloat()), rotation.z));
}

void ObjectPropertyWidget::rotZLineEditingFinished()
{
	ObjectManager & obj_mgr = g_pGlobalSys->objectManager;
	Object* object = obj_mgr.getCurSelObject();
	if (!object)
		return;
	auto & rotation = object->getRotation();
	object->setRotation(XMFLOAT3(rotation.x, rotation.y, XMConvertToRadians(rotZLineEdit->text().toFloat())));
}

void ObjectPropertyWidget::leaveEvent(QEvent * e)
{
	scaleXLineEdit->clearFocus();
	scaleYLineEdit->clearFocus();
	scaleZLineEdit->clearFocus();

	transXLineEdit->clearFocus();
	transYLineEdit->clearFocus();
	transZLineEdit->clearFocus();

	rotXLineEdit->clearFocus();
	rotYLineEdit->clearFocus();
	rotZLineEdit->clearFocus();
}
