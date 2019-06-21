#include "ObjectsManager.h"
#include "tinyxml2/tinyxml2.h"
#include <iostream>

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
	clear();
}

void ObjectManager::clear()
{
	for each (auto iter in objects)
	{
		SAFE_DELETE(iter.second);
	}
	objects.clear();
	listview->clear();
	curSelObject = nullptr;
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
	QListWidgetItem* item = listview->item(listview->count()-1);
	listview->setCurrentItem(item);
	return mesh;
}

Object* ObjectManager::getObjectFromName(std::string name)
{
	if (objects.find(name) == objects.end())
		return nullptr;
	return objects[name];
}

void ObjectManager::setCurSelObject(std::string name)
{
	if (objects.find(name) != objects.end())
		curSelObject = objects[name];
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

void ObjectManager::exportProject(std::string file_path)
{
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	XMLElement* parent = doc.NewElement("TsukuyomiProject");
	doc.InsertFirstChild(parent);
	for (auto iter = objects.begin(); iter != objects.end(); iter++)
	{
		Object* obj = iter->second;
		XMLElement* obj_element = doc.NewElement("Object");
		std::string type_str = "";
		if (obj->getType() == EMPTY)
			type_str = "empty";
		else if (obj->getType() == MESH)
		{
			type_str = "mesh";
			obj_element->SetAttribute("MeshPath", ((Mesh*)obj)->getMeshPath().c_str());
		}
		else
			continue;
		obj_element->SetAttribute("Name", obj->getName().c_str());
		obj_element->SetAttribute("Type", type_str.c_str());
		XMLElement*trans_element = doc.NewElement("Translation");
		XMLElement*scale_element = doc.NewElement("Scale");
		XMLElement*rot_element = doc.NewElement("Rotation");
		trans_element->SetText(obj->getTranslationText().c_str());
		scale_element->SetText(obj->getScaleText().c_str());
		rot_element->SetText(obj->getRotationText().c_str());

		obj_element->InsertEndChild(trans_element);
		obj_element->InsertEndChild(scale_element);
		obj_element->InsertEndChild(rot_element);
		parent->InsertEndChild(obj_element);
	}

	doc.SaveFile(file_path.c_str());
	std::cout << "Export " << file_path << " successfully!" << std::endl;
}

void ObjectManager::updateFromProject(std::string file_path)
{
	clear();
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	doc.LoadFile(file_path.c_str());
	XMLElement* parent = doc.FirstChildElement("TsukuyomiProject");
	XMLElement * obj_node = parent->FirstChildElement();
	std::vector<std::string> strs;
	for (XMLElement*express = obj_node; express; express = express->NextSiblingElement())
	{
		std::string name = express->Attribute("Name");
		std::string type = express->Attribute("Type");

		std::string trans_str = express->FirstChildElement()->GetText();
		std::string scale_str = express->FirstChildElement()->NextSiblingElement()->GetText();
		std::string rot_str = express->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->GetText();
		
		SplitString(trans_str, strs, ",");
		XMFLOAT3 translation(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
		SplitString(scale_str, strs, ",");
		XMFLOAT3 scale(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
		SplitString(rot_str, strs, ",");
		XMFLOAT4 rotation(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]), stringToNum<float>(strs[3]));
		if (type == "mesh")
		{
			std::string mesh_path = express->Attribute("MeshPath");
			createNewObjectOfMesh(name, mesh_path, translation, scale, rotation);
		}
	}
}