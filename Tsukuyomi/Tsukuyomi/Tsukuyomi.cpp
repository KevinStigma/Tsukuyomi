#include "Tsukuyomi.h"
#include "Globalsys.h"
#include "D3DRenderer.h"
#include "intersection.h"
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
	g_pGlobalSys->moveSpeedSlider = ui.moveSpeedSlider;
	g_pGlobalSys->objectPropertyWidget = ui.propertyWidget;
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

void Tsukuyomi::on_actionNormalDebug_triggered()
{
	ui.actionNormalDebug->setChecked(true);
	ui.actionWhitted->setChecked(false);
	ui.actionPathTracing->setChecked(false);
	g_pGlobalSys->render_paras.offline_render_type = OfflineRenderType::NORMAL_DEBUG;
}

void Tsukuyomi::on_actionWhitted_triggered()
{
	ui.actionNormalDebug->setChecked(false);
	ui.actionWhitted->setChecked(true);
	ui.actionPathTracing->setChecked(false);
	g_pGlobalSys->render_paras.offline_render_type = OfflineRenderType::WHITTED;
}

void Tsukuyomi::on_actionPathTracing_triggered()
{
	ui.actionNormalDebug->setChecked(false);
	ui.actionWhitted->setChecked(false);
	ui.actionPathTracing->setChecked(true);
	g_pGlobalSys->render_paras.offline_render_type = OfflineRenderType::PATH_TRACING;
}

void Tsukuyomi::on_actionCamera_triggered()
{
	ObjectManager& object_mgr = g_pGlobalSys->objectManager;
	object_mgr.createNewObjectOfCamera("");
}

void Tsukuyomi::on_actionPointLight_triggered()
{
	ObjectManager& object_mgr = g_pGlobalSys->objectManager;
	object_mgr.createNewObjectOfPointLight("");
}

void Tsukuyomi::on_actionDirectionalLight_triggered()
{
	ObjectManager& object_mgr = g_pGlobalSys->objectManager;
	object_mgr.createNewObjectOfDirectionalLight("");
}

void Tsukuyomi::on_actionAreaLight_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load Mesh"), "./Data/Meshes", "obj files(*.obj)", 0);
	if (!filename.size())
		return;
	
	ObjectManager& object_mgr = g_pGlobalSys->objectManager;
	object_mgr.createNewObjectOfAreaLight("", filename.toStdString());
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

void Tsukuyomi::on_renderButton_clicked()
{
	ObjectManager& object_mgr = g_pGlobalSys->objectManager;
	Camera* cam = object_mgr.getRenderCamera();
	if (!cam)
		std::cout << "Don't have valid camera!" << std::endl;
	else
	{
		OfflineRenderer* renderer = g_pGlobalSys->generateOfflineRenderer();
		if (renderer)
		{
			renderer->start_render(cam, 720);
			SAFE_DELETE(renderer);
		}
	}
}

void Tsukuyomi::on_transButton_clicked(bool checked)
{
	if (checked)
	{
		ui.rotButton->setChecked(false);
		g_pGlobalSys->renderer->setRenderSelObjMode(RenderSelObjMode::COORD_AXIS);
	}
	else
	{
		if(!ui.rotButton->isChecked())
			g_pGlobalSys->renderer->setRenderSelObjMode(RenderSelObjMode::NONE);
	}
}

void Tsukuyomi::on_rotButton_clicked(bool checked)
{
	if (checked)
	{
		ui.transButton->setChecked(false);
		g_pGlobalSys->renderer->setRenderSelObjMode(RenderSelObjMode::ROT_AXIS);
	}
	else
	{
		if (!ui.transButton->isChecked())
			g_pGlobalSys->renderer->setRenderSelObjMode(RenderSelObjMode::NONE);
	}
}