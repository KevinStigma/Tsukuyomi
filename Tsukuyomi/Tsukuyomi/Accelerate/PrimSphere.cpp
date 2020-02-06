#include "PrimSphere.h"
#include "../Objects/Sphere.h"

void PrimSphere::generateBoundingBox()
{
	Sphere* sphere = dynamic_cast<Sphere*>(parent);
	float radius = sphere->Radius();
	XMFLOAT3 center = sphere->getTranslation();
	boundingbox.bottom = XMFLOAT3(center.x-radius, center.y-radius, center.z-radius);
	boundingbox.top = XMFLOAT3(center.x + radius, center.y + radius, center.z + radius);
}

bool PrimSphere::is_intersect(const Ray& ray, float&t, IntersectInfo& it)
{
	return parent->is_intersect(ray, t, it);
}
