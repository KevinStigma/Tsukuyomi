#include "ObjectPropertyWidget.h"

ObjectPropertyWidget::ObjectPropertyWidget(QWidget *parent):QWidget(parent)
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
}

ObjectPropertyWidget::~ObjectPropertyWidget()
{

}