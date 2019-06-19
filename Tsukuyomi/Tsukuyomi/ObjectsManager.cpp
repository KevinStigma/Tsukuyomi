#include "ObjectsManager.h"

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
	for each (auto iter in objects)
	{
		SAFE_DELETE(iter.second);
	}
}

void ObjectManager::createNewObjectOfMesh(std::string name, std::string obj_path, bool recon_normals, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT4 r)
{
	if (obj_path == "")
		return;
	std::string obj_name = name;
	if (name == "" || objects.find(name) != objects.end())
	{
		int i = 0;
		std::string base_name = "object";
		while (objects.find(base_name + std::to_string(i)) != objects.end())
			i++;
		obj_name = base_name + std::to_string(i);
	}
	Mesh* mesh = new Mesh(obj_name, obj_path, t, s, r);
	if (recon_normals)
		mesh->constructNormals();
	objects.insert(std::pair<std::string, Object*>(obj_name, mesh));
	listview->addItem(QString(obj_name.c_str()));
}

Object* ObjectManager::getObjectFromName(std::string name)
{
	if (objects.find(name) == objects.end())
		return nullptr;
	return objects[name];
}

bool ObjectManager::removeObject(std::string name)
{
	if (objects.find(name) == objects.end())
		return false;
	auto iter = getObjectFromName(name);
	SAFE_DELETE(iter);
	objects.erase(name);
	listview->clear();
	for each (auto iter in objects)
		listview->addItem(QString(iter.first.c_str()));

	return true;
}

void ObjectManager::renderAllObjects(ID3D11DeviceContext * context, D3DRenderer* renderer)
{
	for each (auto iter in objects)
	{
		iter.second->render(context, renderer);
	}
}