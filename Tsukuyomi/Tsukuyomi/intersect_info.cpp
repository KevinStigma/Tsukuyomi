#include "intersect_info.h"

Ray IntersectInfo::spawnRay(const XMFLOAT3& d)
{
	return Ray(pos, d);
}