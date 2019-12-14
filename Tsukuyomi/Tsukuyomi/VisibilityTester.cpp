#include "VisibilityTester.h"
#include "GlobalSys.h"
#include "intersect_info.h"

bool VisibilityTester::unoccluded()
{
	IntersectInfo it;
	Ray ray = it.spawnRayTo(p1, p2);
	float t = MathHelper::Float3Length(ray.direction);
	ray.direction = MathHelper::NormalizeFloat3(ray.direction);
	float min_t = g_pGlobalSys->cast_ray_to_get_intersection(ray, it);
	if (it.isSurfaceInteraction() && min_t > 0.0 && min_t < t)
		return false;
	return true;
}