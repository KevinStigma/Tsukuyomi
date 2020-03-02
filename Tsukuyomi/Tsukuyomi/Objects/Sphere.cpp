#include "Sphere.h"
#include "../Accelerate/PrimSphere.h"
#include <algorithm>

XMFLOAT3 UniformSampleSphere(const XMFLOAT2 &u) {
	float z = 1 - 2 * u.x;
	float r = sqrtf(std::max<float>(0.0f, 1.0f - z * z));
	float phi = 2 * MathHelper::Pi * u.y;
	return XMFLOAT3(r * std::cos(phi), r * std::sin(phi), z);
}

Sphere::Sphere(std::string name, std::string file_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, Object* al, BxDF*bxdf):Mesh(name,file_path,t,s,r, al, bxdf)
{
	origin_radius = (boundingBox.top.x - boundingBox.bottom.x) * 0.5;
}

Sphere::~Sphere()
{
	Mesh::~Mesh();
}

IntersectInfo Sphere::sample(XMFLOAT2 u, float&area)const
{
	XMFLOAT3 uni_sample_coord = UniformSampleSphere(u);
	float r = Radius();
	XMVECTOR offset = XMVectorSet(r * uni_sample_coord.x, r * uni_sample_coord.y, r * uni_sample_coord.z, 0.0f);
	IntersectInfo it;
	XMVECTOR world_normal = XMVector3Normalize(offset);
	XMStoreFloat3(&it.normal, world_normal);
	float ratio = r / XMVectorGetX(XMVector3Length(offset));
	offset = XMVectorMultiply(offset, XMVectorSet(ratio, ratio, ratio, ratio));
	XMFLOAT3 center = boundingBox.getCenter();
	XMVECTOR world_pos = XMVectorAdd(XMVectorSet(center.x, center.y, center.z, 1.0), offset);
	XMStoreFloat3(&it.pos, world_pos);
	area = Area();
	return it;
}

bool Sphere::is_intersect(const Ray&ray, float& t, IntersectInfo& is_info)
{
	XMFLOAT3 center = getTranslation();
	if (ray.is_intersect_sphere(center, Radius(), t))
	{
		is_info.wo = XMFLOAT3(-ray.direction.x, -ray.direction.y, -ray.direction.z);
		is_info.obj = this;
		is_info.bxdf = this->bxdf;
		is_info.pos = XMFLOAT3(ray.origin.x + t * ray.direction.x, ray.origin.y + t * ray.direction.y, ray.origin.z + t * ray.direction.z);
		is_info.normal = MathHelper::NormalizeFloat3(XMFLOAT3(is_info.pos.x - center.x, is_info.pos.y - center.y, is_info.pos.z - center.z));
		return true;
	}
	else
		return false;
}

std::vector<Primitive*> Sphere::getAllPrimitives()
{
	std::vector<Primitive*> prims;
	PrimSphere* s = new PrimSphere(this, origin_radius, XMFLOAT3(0.0f, 0.0f, 0.0f));
	prims.push_back(s);
	return prims;
}

