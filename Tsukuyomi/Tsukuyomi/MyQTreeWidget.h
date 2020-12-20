#pragma once
#pragma once
#include <qtreewidget.h>
#include "D3DRenderer.h"

class MyQTreeWidget :public QTreeWidget
{
	Q_OBJECT
public:
	MyQTreeWidget(QWidget *parent = 0);
	~MyQTreeWidget();

protected:
	void dropEvent(QDropEvent *ev);
};