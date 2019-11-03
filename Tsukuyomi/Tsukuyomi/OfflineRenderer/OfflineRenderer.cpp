#include "OfflineRenderer.h"
#include "../GlobalSys.h"
#include "../Objects/Mesh.h"

void OfflineRenderer::cast_ray_to_get_intersection(Ray ray, IntersectInfo& info)
{
	auto objs = g_pGlobalSys->objectManager.getAllObjects();
	float min_t = -1.0f;
	for (int i = 0; i < objs.size(); i++)
	{
		Object* obj = objs[i];
		if (obj->getType() != MESH)
			continue;
		float t;
		if (((Mesh*)obj)->is_intersect(ray, t) && (min_t < 0.0f || t < min_t))
		{
			min_t = t;
		}
	}
}