#include "GlobalSys.h"
#include "common.h"
#include <iostream>

GlobalSys *g_pGlobalSys = NULL;

GlobalSys::GlobalSys()
{

}

GlobalSys::~GlobalSys()
{

}

void GlobalSys::createNewObject(std::string name, std::string obj_path, bool recon_normals, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT4 r)
{
	Object obj = Object(name, obj_path, t, s, r);
	if (recon_normals)
		obj.constructNormals();
	objects.push_back(obj);
}

Object* GlobalSys::getObjectFromName(std::string name)
{
	for (int i = 0; i < objects.size(); i++)
	{
		Object& object = objects[i];
		if (object.getName() == name)
			return &object;
	}
	return nullptr;
}

