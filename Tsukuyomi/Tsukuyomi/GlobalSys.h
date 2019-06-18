#pragma once
#include <string>
#include "Objects/Object.h"
#include "tiny_obj_loader.h"

class GlobalSys
{
public:
	GlobalSys();
	~GlobalSys();
	void createNewObjectOfMesh(std::string name, std::string obj_path = "", bool recon_normals = false, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT4 r = XMFLOAT4(0.0, 0.0, 0.0, 1.0));
	Object* getObjectFromName(std::string name);
	std::vector<Object*> objects;
};

extern GlobalSys *g_pGlobalSys;