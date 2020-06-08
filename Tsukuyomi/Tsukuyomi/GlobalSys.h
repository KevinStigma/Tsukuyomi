#pragma once
#include <string>
#include <QListWidget>
#include <QSlider>
#include <QMainWindow.h>
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
	float getHDRExposure();
	OfflineRenderer* generateOfflineRenderer();
	float cast_ray_to_get_intersection(const Ray& ray, IntersectInfo& info, bool use_accel=true);
	void generateBVH();
	RenderParas render_paras;
	ObjectManager objectManager;
	QMainWindow *  mainWindow = nullptr;
	D3DRenderer * renderer = nullptr;
	RenderWidget* renderWidget = nullptr;
	QListWidget*  objectsList = nullptr;
	QSlider*      moveSpeedSlider = nullptr;
	QSlider*      exposureSlider = nullptr;
	ObjectPropertyWidget* objectPropertyWidget = nullptr;
};

extern GlobalSys *g_pGlobalSys;