#pragma once
#include <string>
#include "ObjectsManager.h"

class D3DRenderer;

class GlobalSys
{
public:
	GlobalSys();
	~GlobalSys();
	ObjectManager objectManager;
	D3DRenderer * renderer = nullptr;
};

extern GlobalSys *g_pGlobalSys;