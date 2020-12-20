#include "MyQTreeWidget.h"
#include "GlobalSys.h"
#include <qevent.h>
#include <qmimedata.h>
#include <QMouseEvent>>

MyQTreeWidget::MyQTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
	setDragEnabled(true);
	setAcceptDrops(true);
	setDragDropMode(QAbstractItemView::InternalMove);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setDropIndicatorShown(true);
}

MyQTreeWidget::~MyQTreeWidget()
{

}

void MyQTreeWidget::dropEvent(QDropEvent *ev)
{
	QTreeWidgetItem* item = currentItem();
	QTreeWidget::dropEvent(ev);

	std::string cur_obj_name = item->text(0).toStdString();
	Object* cur_obj = g_pGlobalSys->objectManager.getObjectFromName(cur_obj_name);
	Object* new_parent = nullptr;
	if (item->parent())
	{
		new_parent = g_pGlobalSys->objectManager.getObjectFromName(item->parent()->text(0).toStdString());
	}
	cur_obj->changeNewParent(new_parent);
}
