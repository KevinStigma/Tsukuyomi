#include "ObjectsManager.h"
#include <iostream>
#include "PbrMat/Matte.h"

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
	bvhManager.destroyBoundingVolumeHieratches();
}

Object* ObjectManager::createNewObjectOfMesh(std::string name, std::string obj_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, PbrMat* pbr_mat)
{
	if (obj_path == "")
		return nullptr;
	std::string obj_name = name;
	if (name == "" || objects.find(name) != objects.end())
		obj_name = genNewObjectName();
	int index = obj_path.rfind('/');
	Mesh*mesh = nullptr;
	if(obj_path.substr(index + 1, obj_path.size() - index) == "sphere.obj")
		mesh = new Sphere(obj_name, obj_path, t, s, r, nullptr, pbr_mat);
	else
		mesh = new Mesh(obj_name, obj_path, t, s, r, nullptr, pbr_mat);
	objects.insert(std::pair<std::string, Object*>(obj_name, mesh));
	listview->addItem(QString(obj_name.c_str()));
	QListWidgetItem* item = listview->item(listview->count()-1);
	listview->setCurrentItem(item);
	return mesh;
}

Object* ObjectManager::createNewObjectOfCamera(std::string name, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r)
{
	std::string obj_name = name;
	if (name == "" || objects.find(name) != objects.end())
		obj_name = genNewObjectName();
	Camera* cam = new Camera(obj_name, t, s, r);
	objects.insert(std::pair<std::string, Object*>(obj_name, cam));
	listview->addItem(QString(obj_name.c_str()));
	QListWidgetItem* item = listview->item(listview->count() - 1);
	listview->setCurrentItem(item);
	return cam;
}

Object* ObjectManager::createNewObjectOfPointLight(std::string name, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, XMFLOAT3 c)
{
	std::string obj_name = name;
	if (name == "" || objects.find(name) != objects.end())
		obj_name = genNewObjectName();
	PointLight* light = new PointLight(obj_name, t, s, r, c);
	objects.insert(std::pair<std::string, Object*>(obj_name, light));
	listview->addItem(QString(obj_name.c_str()));
	QListWidgetItem* item = listview->item(listview->count() - 1);
	listview->setCurrentItem(item);
	return light;
}

std::string ObjectManager::genNewObjectName()
{
	int i = 0;
	std::string base_name = "object";
	while (objects.find(base_name + std::to_string(i)) != objects.end())
		i++;
	return base_name + std::to_string(i);
}

Object* ObjectManager::createNewObjectOfDirectionalLight(std::string name, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, XMFLOAT3 c)
{
	std::string obj_name = name;
	if (name == "" || objects.find(name) != objects.end())
		obj_name = genNewObjectName();
	DirectionalLight* light = new DirectionalLight(obj_name, t, s, r, c);
	objects.insert(std::pair<std::string, Object*>(obj_name, light));
	listview->addItem(QString(obj_name.c_str()));
	QListWidgetItem* item = listview->item(listview->count() - 1);
	listview->setCurrentItem(item);
	return light;
}

Object* ObjectManager::createNewObjectOfAreaLight(std::string name, std::string mesh_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, XMFLOAT3 c, PbrMat*pbr_mat)
{
	std::string obj_name = name;
	if (name == "" || objects.find(name) != objects.end())
		obj_name = genNewObjectName();
	AreaLight* light = new AreaLight(obj_name, mesh_path, t, s, r, c, pbr_mat);
	objects.insert(std::pair<std::string, Object*>(obj_name, light));
	listview->addItem(QString(obj_name.c_str()));
	QListWidgetItem* item = listview->item(listview->count() - 1);
	listview->setCurrentItem(item);
	return light;
}

std::vector<Light*> ObjectManager::getAllLights()
{
	std::vector<Light*> lights;
	for each (auto item in objects)
	{
		if (item.second->isLight())
			lights.push_back(dynamic_cast<Light*>(item.second));
	}
	return lights;
}

int ObjectManager::getLightsCountParameter()
{
	std::vector<Light*> lights = getAllLights();
	int count = 0;
	for (int i = 0; i < lights.size(); i++)
	{
		Light* light = lights[i];
		if (light->getType() == AREA_LIGHT)
			count += dynamic_cast<AreaLight*>(light)->getMesh()->getComponentNum();
		else
			count++;
	}
	return count;
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
		obj->setName(new_name);
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

void ObjectManager::exportMaterial(PbrMat*mat, tinyxml2::XMLElement* parent, tinyxml2::XMLDocument& doc)
{
	if (!mat)
		return;
	tinyxml2::XMLElement*mat_element = doc.NewElement("Material");
	mat->exportToXML(mat_element);
	parent->InsertEndChild(mat_element);
}

void ObjectManager::outputSphereMesh()
{
	Sphere* s = dynamic_cast<Sphere*>(getObjectFromName("s"));
	s->outputSphereMesh();
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
			exportMaterial(((Mesh*)obj)->getPbrMat(), obj_element, doc);
		}
		else if (obj->getType() == CAM)
		{
			type_str = "cam";
		}
		else if (obj->getType() == POINT_LIGHT)
		{
			type_str = "point_light";
			obj_element->SetAttribute("Color", ((PointLight*)obj)->getColorText().c_str());
		}
		else if (obj->getType() == DIR_LIGHT)
		{
			type_str = "dir_light";
			obj_element->SetAttribute("Color", ((DirectionalLight*)obj)->getColorText().c_str());
		}
		else if (obj->getType() == AREA_LIGHT)
		{
			type_str = "area_light";
			obj_element->SetAttribute("MeshPath", ((AreaLight*)obj)->getMesh()->getMeshPath().c_str());
			obj_element->SetAttribute("Color", ((DirectionalLight*)obj)->getColorText().c_str());
			exportMaterial(((AreaLight*)obj)->getMesh()->getPbrMat(), obj_element, doc);
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

PbrMat* loadPbrMat(tinyxml2::XMLElement* mat_elm)
{
	if (!mat_elm)
		return nullptr;
	std::vector<std::string> strs;
	if (std::string(mat_elm->Attribute("type")) == "Matte")
	{
		SplitString(std::string(mat_elm->Attribute("kd")), strs, ",");
		Spectrum kd(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
		float sigma = stringToNum<float>(std::string(mat_elm->Attribute("sigma")));
		MatteMaterial* matte = new MatteMaterial(kd, sigma);
		return matte;
	}
	return nullptr;
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

		XMLElement* trans_elm,* scale_elm,* rot_elm, * mat_elm=nullptr;
		
		for (XMLElement* child_elm = express->FirstChildElement(); child_elm; child_elm =child_elm->NextSiblingElement())
		{
			std::string name = child_elm->Name();
			if (name == "Translation")
				trans_elm = child_elm;
			else if (name == "Rotation")
				rot_elm = child_elm;
			else if (name == "Scale")
				scale_elm = child_elm;
			else if (name == "Material")
				mat_elm = child_elm;
		}
		std::string trans_str = trans_elm->GetText();
		std::string scale_str = scale_elm->GetText();
		std::string rot_str = rot_elm->GetText();

		SplitString(trans_str, strs, ",");
		XMFLOAT3 translation(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
		SplitString(scale_str, strs, ",");
		XMFLOAT3 scale(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
		SplitString(rot_str, strs, ",");
		XMFLOAT3 rotation(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
		
		PbrMat* mat = loadPbrMat(mat_elm);
		if (type == "mesh")
		{
			std::string mesh_path = express->Attribute("MeshPath");
			createNewObjectOfMesh(name, mesh_path, translation, scale, rotation, mat);
		}
		else if (type == "cam")
		{
			createNewObjectOfCamera(name, translation, scale, rotation);
		}
		else if (type == "point_light")
		{
			std::string color_str = express->Attribute("Color");
			SplitString(color_str, strs, ",");
			XMFLOAT3 color(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
			createNewObjectOfPointLight(name, translation, scale, rotation, color);
		}
		else if (type == "dir_light")
		{
			std::string color_str = express->Attribute("Color");
			SplitString(color_str, strs, ",");
			XMFLOAT3 color(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
			createNewObjectOfDirectionalLight(name, translation, scale, rotation, color);
		}
		else if (type == "area_light")
		{
			std::string mesh_path = express->Attribute("MeshPath");
			std::string color_str = express->Attribute("Color");
			SplitString(color_str, strs, ",");
			XMFLOAT3 color(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
			createNewObjectOfAreaLight(name, mesh_path ,translation, scale, rotation, color, mat);
		}
	}
	bvhManager.generateBoundingVolumeHieratchies();
}

Camera* ObjectManager::getRenderCamera()
{
	for each (auto item in objects)
	{
		if (item.second->getType() == CAM)
			return dynamic_cast<Camera*>(item.second);
	}
	return nullptr;
}

std::vector<Object*> ObjectManager::getAllObjects()
{
	std::vector<Object*> objs;
	for each (auto item in objects)
	{
		objs.push_back(item.second);
	}
	return objs;
}
