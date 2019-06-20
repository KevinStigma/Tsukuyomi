#include "Tsukuyomi.h"
#include "Globalsys.h"
#include <QFileDialog>
#include <iostream>

Tsukuyomi::Tsukuyomi(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	g_pGlobalSys = new GlobalSys;
	g_pGlobalSys->objectManager.setListView(ui.objectsListView);
	ui.objectsListView->setPropertyWidget(ui.propertyWidget);
}

void Tsukuyomi::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key::Key_R)
	{
		ui.render_widget->keyReleaseEvent(event);
	}
}

void Tsukuyomi::on_objectsListView_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	std::cout << current << " " << previous << std::endl;
	ui.objectsListView->showCurItemProperty(current);
}

void Tsukuyomi::on_actionLoad_Mesh_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load Mesh"), "./Data/Meshes", "obj files(*.obj)", 0);
	if (!filename.size())
		return;
	ObjectManager& objectMgr = g_pGlobalSys->objectManager;
	Object* obj = objectMgr.createNewObjectOfMesh("", filename.toStdString());
	if (!obj)
		return;
	if (obj->getType() == MESH)
		((Mesh*)obj)->generateBuffers(ui.render_widget->getRenderer()->getDevice());
}
