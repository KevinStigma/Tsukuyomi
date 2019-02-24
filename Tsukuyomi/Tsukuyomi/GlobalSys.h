#pragma once
#include <string>
#include "tiny_obj_loader.h"

class GlobalSys
{
public:
	GlobalSys();
	~GlobalSys();
	bool loadObjMesh(const std::string& path);
	void reconNormals(tinyobj::shape_t& shape);
	std::vector<tinyobj::shape_t> objects;
};

extern GlobalSys *g_pGlobalSys;