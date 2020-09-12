#include "ObjectsManager.h"
#include <iostream>
#include "EnvMap.h"
#include "GlobalSys.h"
#include "PbrMat/Matte.h"
#include "PbrMat/Plastic.h"
#include "D3DRenderer.h"

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
	if(listview)
		listview->clear();

	SAFE_DELETE(environmentMap);
	curSelObject = nullptr;
	curShadowLight = nullptr;
	bvhManager.destroyBoundingVolumeHieratches();
}

Object* ObjectManager::createNewObjectOfMesh(std::string name, std::string obj_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, RenderMats* mats)
{
	if (obj_path == "")
		return nullptr;
	std::string obj_name = name;
	if (name == "" || objects.find(name) != objects.end())
		obj_name = genNewObjectName();
	int index = obj_path.rfind('/');
	Mesh*mesh = nullptr;
	if(obj_path.substr(index + 1, obj_path.size() - index) == "sphere.obj")
		mesh = new Sphere(obj_name, obj_path, t, s, r, nullptr, mats);
	else
		mesh = new Mesh(obj_name, obj_path, t, s, r, nullptr, mats);
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

Object* ObjectManager::createNewObjectOfAreaLight(std::string name, std::string mesh_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, XMFLOAT3 c, RenderMats* render_mats)
{
	std::string obj_name = name;
	if (name == "" || objects.find(name) != objects.end())
		obj_name = genNewObjectName();
	AreaLight* light = new AreaLight(obj_name, mesh_path, t, s, r, c, render_mats);
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

void ObjectManager::loadEnvMap(std::string path)
{
	if (!path.size())
		return;
	environmentMap = new EnvironmentMap(path, g_pGlobalSys->renderer->getDevice(), g_pGlobalSys->renderer->getContext());
}

void ObjectManager::setCurSelObject(std::string name)
{
	if (objects.find(name) != objects.end())
		curSelObject = objects[name];
}

void ObjectManager::setCurShadowLight(Light* light)
{
	if (light && light->getType() != DIR_LIGHT)
		return;
	curShadowLight = light;
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

void ObjectManager::exportMaterial(PbrMat* pbr_mat, RenderLightHelper::Material realtime_mat, tinyxml2::XMLElement* parent, tinyxml2::XMLDocument& doc)
{
	if (!pbr_mat)
		return;
	tinyxml2::XMLElement*mat_element = doc.NewElement("Material");
	pbr_mat->exportToXML(mat_element, realtime_mat);
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
	parent->SetAttribute("EnvMap", environmentMap? environmentMap->getEnvrionmentMap().c_str():"");
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
			exportMaterial(((Mesh*)obj)->getPbrMat(), ((Mesh*)obj)->getRenderMaterial(), obj_element, doc);
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
			if (curShadowLight == obj)
				obj_element->SetAttribute("ShadowLight", "true");
			else
				obj_element->SetAttribute("ShadowLight", "false");
		}
		else if (obj->getType() == AREA_LIGHT)
		{
			type_str = "area_light";
			obj_element->SetAttribute("MeshPath", ((AreaLight*)obj)->getMesh()->getMeshPath().c_str());
			obj_element->SetAttribute("Color", ((DirectionalLight*)obj)->getColorText().c_str());
			Mesh* mesh = ((AreaLight*)obj)->getMesh();
			exportMaterial(mesh->getPbrMat(), mesh->getRenderMaterial(), obj_element, doc);
		}
		else
			continue;
		obj_element->SetAttribute("Name", obj->getName().c_str());
		obj_element->SetAttribute("Type", type_str.c_str());

		Object* p = obj->getParent();
		obj_element->SetAttribute("Parent", p ? p->getName().c_str() : "");

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

std::pair<PbrMat*, RenderLightHelper::Material> loadPbrMat(tinyxml2::XMLElement* mat_elm)
{
	RenderLightHelper::Material material;
	if (!mat_elm)
		return RenderMats(nullptr, material);
	std::vector<std::string> strs;
	
	if (mat_elm->FindAttribute("kd"))
	{
		SplitString(std::string(mat_elm->Attribute("kd")), strs, ",");
		material.albedo = XMFLOAT3(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
	}

	if (mat_elm->FindAttribute("roughness"))
	{
		material.roughness = stringToNum<float>(std::string(mat_elm->Attribute("roughness")));
	}

	if (mat_elm->FindAttribute("metallic"))
	{
		material.metallic = stringToNum<float>(std::string(mat_elm->Attribute("metallic")));
	}
	
	PbrMat* pbr_mat;
	if (std::string(mat_elm->Attribute("type")) == "Matte")
	{
		SplitString(std::string(mat_elm->Attribute("kd")), strs, ",");
		Spectrum kd(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
		float sigma = stringToNum<float>(std::string(mat_elm->Attribute("sigma")));
		pbr_mat = new MatteMaterial(kd, sigma);
	}
	if (std::string(mat_elm->Attribute("type")) == "Plastic")
	{
		SplitString(std::string(mat_elm->Attribute("kd")), strs, ",");
		Spectrum kd(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
		float rou = stringToNum<float>(std::string(mat_elm->Attribute("roughness")));
		SplitString(std::string(mat_elm->Attribute("ks")), strs, ",");
		Spectrum ks(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
		bool remap = stringToNum<int>(std::string(mat_elm->Attribute("remapRoughness")));
		pbr_mat = new PlasticMaterial(kd, ks, rou, remap);
	}
	return std::pair<PbrMat*, RenderLightHelper::Material>(pbr_mat, material);
}

void ObjectManager::updateFromProject(std::string file_path)
{
	clear();
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	doc.LoadFile(file_path.c_str());
	XMLElement* parent = doc.FirstChildElement("TsukuyomiProject");

	std::string env_map_path = "";
	if(parent->FindAttribute("EnvMap"))
		env_map_path = parent->Attribute("EnvMap");

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
		
		RenderMats materials = loadPbrMat(mat_elm);
		if (type == "mesh")
		{
			std::string mesh_path = express->Attribute("MeshPath");
			createNewObjectOfMesh(name, mesh_path, translation, scale, rotation, &materials);
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
			Object* light = createNewObjectOfDirectionalLight(name, translation, scale, rotation, color);
			if (express->FindAttribute("ShadowLight"))
			{
				if (std::string(express->Attribute("ShadowLight")) == "true")
					setCurShadowLight((Light*)light);
			}
		}
		else if (type == "area_light")
		{
			std::string mesh_path = express->Attribute("MeshPath");
			std::string color_str = express->Attribute("Color");
			SplitString(color_str, strs, ",");
			XMFLOAT3 color(stringToNum<float>(strs[0]), stringToNum<float>(strs[1]), stringToNum<float>(strs[2]));
			createNewObjectOfAreaLight(name, mesh_path ,translation, scale, rotation, color, &materials);
		}
	}

	for (XMLElement*express = obj_node; express; express = express->NextSiblingElement())
	{
		if (express->FindAttribute("Parent"))
		{
			std::string name = express->Attribute("Name");
			std::string parent_name = express->Attribute("Parent");
			if (parent_name.size())
			{
				Object* parent = getObjectFromName(parent_name);
				Object* obj = getObjectFromName(name);
				if(parent && obj)
					obj->setParent(parent);
			}
		}
	}

	loadEnvMap(env_map_path);
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
