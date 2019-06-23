#pragma once
#include <string>
#include <QListWidget>
#include "ObjectsManager.h"

class D3DRenderer;
class RenderWidget;

class GlobalSys
{
public:
	GlobalSys();
	~GlobalSys();
	ObjectManager objectManager;
	D3DRenderer * renderer = nullptr;
	RenderWidget* renderWidget = nullptr;
	QListWidget*  objectsList = nullptr;
};

extern GlobalSys *g_pGlobalSys;