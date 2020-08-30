#include "Tsukuyomi.h"
#include "Globalsys.h"
#include "D3DRenderer.h"
#include "intersection.h"
#include "../OfflineRenderer/OfflineRenderUtility.h"
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
	g_pGlobalSys->exposureSlider = ui.moveSpeedSlider_hdr;
	g_pGlobalSys->mainWindow = this;
	srand(time(0));
}

Tsukuyomi::~Tsukuyomi()
{
	g_pGlobalSys->objectManager.setListView(nullptr);
	ui.objectsListView->setPropertyWidget(nullptr);
	g_pGlobalSys->objectsList = nullptr;
	g_pGlobalSys->renderWidget = nullptr;
	g_pGlobalSys->moveSpeedSlider = nullptr;
	g_pGlobalSys->objectPropertyWidget = nullptr;
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
	ui.render_widget->getRenderer()->buildShadowTransform();
}

void Tsukuyomi::on_actionLoadEnvMap_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load EnvMap"), "./Data/IBLs", "hdr files(*.hdr)", 0);
	if (!filename.size())
		return;
	ObjectManager& object_mgr = g_pGlobalSys->objectManager;
	object_mgr.loadEnvMap(filename.toStdString());
}

void Tsukuyomi::on_actionBack_Irradiance_triggered()
{
	g_pGlobalSys->renderer->bakeIrradiance();
}

void Tsukuyomi::on_actionBake_PreFilterEnvMpas_triggered()
{

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

void Tsukuyomi::on_actionScreenShot_triggered()
{
	ui.render_widget->getRenderer()->GragScreenShot();
}

void Tsukuyomi::on_actionExit_triggered()
{
	qApp->quit();
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
	if (event->key() == Qt::Key::Key_Delete)
	{
		QListWidget* object_list = g_pGlobalSys->objectsList;
		QListWidgetItem* item = object_list->currentItem();
		if (!item)
			return;
		ObjectManager& obj_mgr = g_pGlobalSys->objectManager;
		obj_mgr.removeObject(item->text().toStdString());
	}
	else if (event->key() == Qt::Key::Key_0)
	{
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

void Tsukuyomi::on_genBVHButton_clicked()
{
	g_pGlobalSys->generateBVH();
}

void Tsukuyomi::on_showBVHcheckBox_stateChanged(int val)
{
	g_pGlobalSys->render_paras.renderBVH = bool(val);
}

void Tsukuyomi::on_RulerLine_checkBox_stateChanged(int val)
{
	g_pGlobalSys->render_paras.showRulerLines = bool(val);
}

void Tsukuyomi::on_MSAA_checkBox_stateChanged(int val)
{
	ui.render_widget->getRenderer()->enableMSAA(bool(val));
}

void Tsukuyomi::on_HDR_checkBox_stateChanged(int val)
{
	g_pGlobalSys->render_paras.enableHDR = bool(val);
}

void Tsukuyomi::on_SSAO_checkBox_stateChanged(int val)
{
	g_pGlobalSys->render_paras.enableSSAO = bool(val);
}

void Tsukuyomi::on_Gamma_checkBox_stateChanged(int val)
{
	g_pGlobalSys->render_paras.gammaCorrect = bool(val);
}

void Tsukuyomi::on_DebugTex_checkBox_stateChanged(int val)
{
	g_pGlobalSys->render_paras.showDebugTex = bool(val);
}

void Tsukuyomi::on_sampleComboBox_currentIndexChanged(int ind)
{
	g_pGlobalSys->render_paras.sampleCount = atoi(ui.sampleComboBox->currentText().toStdString().c_str());
	std::cout << "sample count " << g_pGlobalSys->render_paras.sampleCount << std::endl;
}

void Tsukuyomi::on_depthcomboBox_currentIndexChanged(int id)
{
	g_pGlobalSys->render_paras.depth = atoi(ui.depthcomboBox->currentText().toStdString().c_str());
	std::cout << "depth " << g_pGlobalSys->render_paras.depth << std::endl;
}

void Tsukuyomi::on_renderMethodcomboBox_currentIndexChanged(int id)
{
	g_pGlobalSys->render_paras.offline_render_type = OfflineRenderType(id);
}

void Tsukuyomi::on_writeFilecheckBox_stateChanged(int state)
{
	g_pGlobalSys->render_paras.writeFile = state;
	std::cout << g_pGlobalSys->render_paras.writeFile << std::endl;
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

void Tsukuyomi::on_targetCamButton_clicked()
{
	Object* sel_obj = g_pGlobalSys->objectManager.getCurSelObject();
	if (!sel_obj)
		return;
	if (sel_obj->getType() != CAM)
	{
		std::cout << "You must select a camera!" << std::endl;
		return;
	}
	Camera* camera = dynamic_cast<Camera*>(sel_obj);
	g_pGlobalSys->renderer->resetCameraTransform(camera);
}

void Tsukuyomi::on_resetViewButton_clicked()
{
	if (ui.objectsListView->isEditing)
		return;
	Camera& camera = ui.render_widget->getRenderer()->getCamera();
	camera.init();
}

void Tsukuyomi::on_setShadowLightButton_clicked()
{
	Object* sel_obj = g_pGlobalSys->objectManager.getCurSelObject();
	g_pGlobalSys->objectManager.setCurShadowLight(dynamic_cast<Light*>(sel_obj));
	ui.render_widget->getRenderer()->buildShadowTransform();
}