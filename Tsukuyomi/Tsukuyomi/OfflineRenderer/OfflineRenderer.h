#pragma once
#include <omp.h>
#include "../Objects/Camera.h"
#include "../intersect_info.h"
class OfflineRenderer
{
public:
	OfflineRenderer() {};
	virtual ~OfflineRenderer() {};
	virtual void start_render(Camera* camera, int height) = 0;
	void cast_ray_to_get_intersection(Ray ray, IntersectInfo& info);
};