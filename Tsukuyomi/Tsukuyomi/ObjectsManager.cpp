#include "ObjectsManager.h"
#include <iostream>

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

Object* ObjectManager::createNewObjectOfMesh(std::string name, std::string obj_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT4 r)
{
	if (obj_path == "")
		return nullptr;
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
	if (mesh->getMesh()->normals.size() == 0)
		mesh->constructNormals();
	objects.insert(std::pair<std::string, Object*>(obj_name, mesh));
	listview->addItem(QString(obj_name.c_str()));
	return mesh;
}

Object* ObjectManager::getObjectFromName(std::string name)
{
	if (objects.find(name) == objects.end())
		return nullptr;
	return objects[name];
}

bool ObjectManager::changeObjectName(std::string old_name, std::string new_name)
{
	if (old_name == new_name)
		return true;
	else if (objects.find(old_name) == objects.end())
	{
		std::cout << "There is not the object named '" << old_name << "'!" << std::endl;
		return false;
	}

	if (objects.find(new_name) != objects.end())
	{
		std::cout << "There has existed the object named '" << new_name << "'!" << std::endl;
		return false;
	}
	else
	{
		Object* obj = objects[old_name];
		objects.erase(old_name);
		objects.insert(std::pair<std::string, Object*>(new_name, obj));
		return true;
	}
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