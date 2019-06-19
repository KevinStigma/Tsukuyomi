#pragma once
#include <string>
#include "ObjectsManager.h"

class GlobalSys
{
public:
	GlobalSys();
	~GlobalSys();
	ObjectManager objectManager;
};

extern GlobalSys *g_pGlobalSys;