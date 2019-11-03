#pragma once
#include <string>
#include <QListWidget>
#include <QSlider>
#include "RenderParas.h"
#include "ObjectsManager.h"
#include "ObjectPropertyWidget.h"

class D3DRenderer;
class RenderWidget;

class GlobalSys
{
public:
	GlobalSys();
	~GlobalSys();
	float getMoveSpeed();
	RenderParas render_paras;
	ObjectManager objectManager;
	D3DRenderer * renderer = nullptr;
	RenderWidget* renderWidget = nullptr;
	QListWidget*  objectsList = nullptr;
	QSlider*      moveSpeedSlider = nullptr;
	ObjectPropertyWidget* objectPropertyWidget = nullptr;
};

extern GlobalSys *g_pGlobalSys;