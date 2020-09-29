#pragma once
#include <set>
#include <QListWidget>
#include <qtreewidget.h>
#include <unordered_map>
#include "Objects/Object.h"
#include "Objects/Mesh.h"
#include "Objects/Sphere.h"
#include "Objects/Camera.h"
#include "Objects/PointLight.h"
#include "Objects/DirectionalLight.h"
#include "Objects/AreaLight.h"
#include "ObjectsListWidget.h"
#include "tinyxml2/tinyxml2.h"
#include "Accelerate/BVHManager.h"
#include "PbrMat/PbrMat.h"

class EnvironmentMap;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	void clear();
	void renderAllObjects(ID3D11DeviceContext * context, D3DRenderer* renderer);
	Object* createNewObjectOfMesh(std::string name, std::string obj_path, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), RenderMats* mats = nullptr);
	Object* createNewObjectOfCamera(std::string name, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0));
	Object* createNewObjectOfPointLight(std::string name, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 c=XMFLOAT3(1.0, 1.0, 1.0));
	Object* createNewObjectOfDirectionalLight(std::string name, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 c = XMFLOAT3(1.0, 1.0, 1.0));
	Object* createNewObjectOfAreaLight(std::string name, std::string mesh_path, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 c = XMFLOAT3(1.0, 1.0, 1.0), RenderMats* mats = nullptr);
	Object* getObjectFromName(std::string name);
	void loadEnvMap(std::string path);
	std::vector<Light*> getAllLights();
	int getLightsCountParameter();
	bool removeObject(std::string name);
	bool changeObjectName(std::string old_name, std::string new_name);
	void setListView(ObjectsListWidget* qlistview) { listview = qlistview; }
	void setTreeWidget(QTreeWidget* widget) { treeWidget = widget; }
	Object* getCurSelObject() { return curSelObject; }
	void setCurSelObject(std::string name);
	void setCurShadowLight(Light* light);
	Light* getCurSelShadowLight() { return curShadowLight; }
	void exportProject(std::string file_path);
	void outputSphereMesh();
	void exportMaterial(PbrMat* pbr_mat, RenderLightHelper::Material realtime_mat, tinyxml2::XMLElement* parent, tinyxml2::XMLDocument& doc);
	void updateFromProject(std::string file_path);
	std::vector<Object*> getAllObjects();
	Camera* getRenderCamera();
	BVHManager* getBVHManager() { return &bvhManager; }
	EnvironmentMap* getEnvironmentMap() { return environmentMap; }

	Object* curEditingObj = nullptr;

protected:
	void updateTreeWidget();
	QTreeWidgetItem* addTreeFromRoot(QTreeWidgetItem*p, Object* root);
	BVHManager bvhManager;
	std::string genNewObjectName();
	std::unordered_map<std::string, Object*> objects;
	ObjectsListWidget* listview = nullptr;
	QTreeWidget* treeWidget = nullptr;
	Object* curSelObject = nullptr;
	Light* curShadowLight = nullptr;
	EnvironmentMap* environmentMap = nullptr;
};
