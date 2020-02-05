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

