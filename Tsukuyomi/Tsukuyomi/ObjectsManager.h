#pragma once
#include <set>
#include <QListWidget>
#include <unordered_map>
#include "Objects\Object.h"
#include "Objects\Mesh.h"
#include "ObjectsListWidget.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	void renderAllObjects(ID3D11DeviceContext * context, D3DRenderer* renderer);
	Object* createNewObjectOfMesh(std::string name, std::string obj_path = "", XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT4 r = XMFLOAT4(0.0, 0.0, 0.0, 1.0));
	Object* getObjectFromName(std::string name);
	bool removeObject(std::string name);
	bool changeObjectName(std::string old_name, std::string new_name);
	void setListView(ObjectsListWidget* qlistview) { listview = qlistview; };
	Object* getCurSelObject() { return curSelObject; }
protected:
	std::unordered_map<std::string, Object*> objects;
	ObjectsListWidget* listview = nullptr;
	Object* curSelObject = nullptr;
};