#include "GlobalSys.h"
#include "Objects/Mesh.h"
#include "common.h"
#include "D3DRenderer.h"
#include "RenderWidget.h"
#include "RGBSpectrum.h"
#include "OfflineRenderer\NormalRenderer.h"
#include "OfflineRenderer\PathTracingRenderer.h"
#include <iostream>

GlobalSys *g_pGlobalSys = NULL;

GlobalSys::GlobalSys(){}

GlobalSys::~GlobalSys(){}

float GlobalSys::getMoveSpeed()
{
	return 0.001 / 20.0 * moveSpeedSlider->value();
}

OfflineRenderer* GlobalSys::generateOfflineRenderer()
{
	OfflineRenderer* renderer=nullptr;
	if (render_paras.offline_render_type == OfflineRenderType::NORMAL_DEBUG)
		renderer = new NormalRenderer();
	else if (render_paras.offline_render_type == OfflineRenderType::PATH_TRACING)
		renderer = new PathTracingRenderer();
	return renderer;
}

void GlobalSys::cast_ray_to_get_intersection(const Ray& ray, IntersectInfo& info)
{
	auto objs = objectManager.getAllObjects();
	float min_t = -1.0f;
	for (int i = 0; i < objs.size(); i++)
	{
		Object* obj = objs[i];
		if (obj->getType() != MESH)
			continue;
		float t;
		if (((Mesh*)obj)->is_intersect(ray, t, info) && (min_t < 0.0f || t < min_t))
		{
			min_t = t;
		}
	}
}