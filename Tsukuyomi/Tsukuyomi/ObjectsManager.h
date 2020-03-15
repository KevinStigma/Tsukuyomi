#pragma once
#include <set>
#include <QListWidget>
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

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	void clear();
	void renderAllObjects(ID3D11DeviceContext * context, D3DRenderer* renderer);
	Object* createNewObjectOfMesh(std::string name, std::string obj_path = "", XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), BxDF*bxdf=nullptr);
	Object* createNewObjectOfCamera(std::string name, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0));
	Object* createNewObjectOfPointLight(std::string name, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 c=XMFLOAT3(1.0, 1.0, 1.0));
	Object* createNewObjectOfDirectionalLight(std::string name, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 c = XMFLOAT3(1.0, 1.0, 1.0));
	Object* createNewObjectOfAreaLight(std::string name, std::string mesh_path, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 c = XMFLOAT3(1.0, 1.0, 1.0), BxDF*bxdf=nullptr);
	Object* getObjectFromName(std::string name);
	std::vector<Light*> getAllLights();
	int getLightsCountParameter();
	bool removeObject(std::string name);
	bool changeObjectName(std::string old_name, std::string new_name);
	void setListView(ObjectsListWidget* qlistview) { listview = qlistview; }
	Object* getCurSelObject() { return curSelObject; }
	void setCurSelObject(std::string name);
	void exportProject(std::string file_path);
	void outputSphereMesh();
	void exportMaterial(BxDF*bxdf, tinyxml2::XMLElement* parent, tinyxml2::XMLDocument& doc);
	void updateFromProject(std::string file_path);
	std::vector<Object*> getAllObjects();
	Camera* getRenderCamera();
	BVHManager* getBVHManager() { return &bvhManager; }
protected:
	BVHManager bvhManager;
	std::string genNewObjectName();
	std::unordered_map<std::string, Object*> objects;
	ObjectsListWidget* listview = nullptr;
	Object* curSelObject = nullptr;
};