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
}

void Tsukuyomi::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key::Key_R)
	{
		ui.render_widget->keyReleaseEvent(event);
	}
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