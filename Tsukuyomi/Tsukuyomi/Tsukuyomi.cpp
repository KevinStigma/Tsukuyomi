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
	ui.propertyWidget->init();
	ui.objectsListView->setPropertyWidget(ui.propertyWidget);
	g_pGlobalSys->objectsList = ui.objectsListView;
	g_pGlobalSys->renderWidget = ui.render_widget;
}

void Tsukuyomi::on_actionLoad_Mesh_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load Mesh"), "./Data/Meshes", "obj files(*.obj)", 0);
	if (!filename.size())
		return;
	ObjectManager& objectMgr = g_pGlobalSys->objectManager;

	QString curPath = QDir::currentPath(); 
	QString relPath = filename.mid(curPath.length() + 1);
	objectMgr.createNewObjectOfMesh("", relPath.toStdString());
}

void Tsukuyomi::on_actionLoad_Project_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load Project"), "./Data/Projects", "xml files(*.xml)", 0);
	if (!filename.size())
		return;
	ObjectManager& object_mgr = g_pGlobalSys->objectManager;
	object_mgr.updateFromProject(filename.toStdString());
}

void Tsukuyomi::on_actionSave_Project_triggered()
{
	QString name = QFileDialog::getSaveFileName(this, tr("Export Project"), "./Data/Projects", "xml files(*.xml)", 0);
	if (!name.size())
		return;
	ObjectManager& object_mgr = g_pGlobalSys->objectManager;
	object_mgr.exportProject(name.toStdString());

}

void Tsukuyomi::on_actionClear_Scene_triggered()
{
	ObjectManager& object_mgr = g_pGlobalSys->objectManager;
	object_mgr.clear();
}

void Tsukuyomi::on_actionExit_triggered()
{
	qApp->quit();
}

void Tsukuyomi::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key::Key_R)
	{
		if (ui.objectsListView->isEditing)
			return;
		Camera& camera = ui.render_widget->getRenderer()->getCamera();
		camera.init();
	}
	else if (event->key() == Qt::Key::Key_Delete)
	{
		QListWidget* object_list = g_pGlobalSys->objectsList;
		QListWidgetItem* item = object_list->currentItem();
		if (!item)
			return;
		ObjectManager& obj_mgr = g_pGlobalSys->objectManager;
		obj_mgr.removeObject(item->text().toStdString());
	}
}