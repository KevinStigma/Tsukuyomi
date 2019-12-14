#include "intersect_info.h"

Ray IntersectInfo::spawnRay(const XMFLOAT3& d)const
{
	XMFLOAT3 nd = MathHelper::NormalizeFloat3(d);
	float offset = 0.001f;
	return Ray(XMFLOAT3(pos.x+nd.x *offset, pos.y + nd.y *offset, pos.z + nd.z *offset), nd);
}

Ray IntersectInfo::spawnRayTo(const XMFLOAT3& p1, const XMFLOAT3& p2)const
{
	XMFLOAT3 d(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
	XMFLOAT3 nd = MathHelper::NormalizeFloat3(d);
	float offset = 0.001f;
	return Ray(XMFLOAT3(p1.x + nd.x *offset, p1.y + nd.y *offset, p1.z + nd.z *offset), d);
}