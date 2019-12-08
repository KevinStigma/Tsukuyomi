#pragma once
#include <string>
#include <QListWidget>
#include <QSlider>
#include "RenderParas.h"
#include "ObjectsManager.h"
#include "OfflineRenderer/OfflineRenderer.h"
#include "ObjectPropertyWidget.h"

class D3DRenderer;
class RenderWidget;

class GlobalSys
{
public:
	GlobalSys();
	~GlobalSys();
	float getMoveSpeed();
	OfflineRenderer* generateOfflineRenderer();
	void cast_ray_to_get_intersection(Ray ray, IntersectInfo& info);
	RenderParas render_paras;
	ObjectManager objectManager;
	D3DRenderer * renderer = nullptr;
	RenderWidget* renderWidget = nullptr;
	QListWidget*  objectsList = nullptr;
	QSlider*      moveSpeedSlider = nullptr;
	ObjectPropertyWidget* objectPropertyWidget = nullptr;
};

extern GlobalSys *g_pGlobalSys;