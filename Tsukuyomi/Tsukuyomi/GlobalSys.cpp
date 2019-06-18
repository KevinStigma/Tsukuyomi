#include "GlobalSys.h"
#include "Objects/Mesh.h"
#include "common.h"
#include <iostream>

GlobalSys *g_pGlobalSys = NULL;

GlobalSys::GlobalSys()
{

}

GlobalSys::~GlobalSys()
{
	for each (Object* obj in objects)
	{
		SAFE_DELETE(obj);
	}
}

void GlobalSys::createNewObjectOfMesh(std::string name, std::string obj_path, bool recon_normals, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT4 r)
{
	Mesh* mesh = new Mesh(name, obj_path, t, s, r);
	if (recon_normals)
		mesh->constructNormals();
	objects.push_back(mesh);
}

Object* GlobalSys::getObjectFromName(std::string name)
{
	for (int i = 0; i < objects.size(); i++)
	{
		Object* object = objects[i];
		if (object->getName() == name)
			return object;
	}
	return nullptr;
}

