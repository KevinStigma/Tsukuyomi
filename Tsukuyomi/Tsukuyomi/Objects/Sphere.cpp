#include "Sphere.h"
#include <algorithm>

XMFLOAT3 UniformSampleSphere(const XMFLOAT2 &u) {
	float z = 1 - 2 * u.x;
	float r = sqrtf(std::max<float>(0.0f, 1.0f - z * z));
	float phi = 2 * MathHelper::Pi * u.y;
	return XMFLOAT3(r * std::cos(phi), r * std::sin(phi), z);
}


Sphere::Sphere(std::string name, std::string file_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r):Mesh(name,file_path,r,s,r)
{
	radius = (boundingBox.top.x - boundingBox.bottom.x) * 0.5;
}

Sphere::~Sphere()
{
	Mesh::~Mesh();
}

IntersectInfo Sphere::sample(XMFLOAT2 u)const
{
	XMFLOAT3 uni_sample_coord = UniformSampleSphere(u);
	XMVECTOR pObj= XMVectorSet(radius * uni_sample_coord.x, radius * uni_sample_coord.y, radius * uni_sample_coord.z, 0.0f);
	IntersectInfo it;
	XMVECTOR world_normal = XMVector3Normalize(XMVector3TransformNormal(pObj, world_mat));
	XMStoreFloat3(&it.normal, world_normal);
	float ratio = radius / XMVectorGetX(XMVector3Length(pObj));
	pObj = XMVectorMultiply(pObj, XMVectorSet(ratio, ratio, ratio, ratio));
	XMVECTOR world_pos = XMVector3TransformCoord(pObj, world_mat);
	XMStoreFloat3(&it.pos, world_pos);
	return it;
}

bool Sphere::is_intersect(const Ray&ray, float& t, IntersectInfo& is_info)
{
	t = 0;
	XMFLOAT3 center = getTranslation();
	XMFLOAT3 diff_vec(ray.origin.x - center.x, ray.origin.y - center.y, ray.origin.z - center.z);
	float tmp_val = MathHelper::DotFloat3(ray.direction, diff_vec);
	float dir_dot_dir = MathHelper::DotFloat3(ray.direction, ray.direction);
	float val = tmp_val * tmp_val - dir_dot_dir*(MathHelper::DotFloat3(diff_vec, diff_vec) - radius * radius);

	float inv_dot = 1 / dir_dot_dir;
	if (val < 0)
		return false;
	else if (fabs(val - 0.0)<1e-6)
	{
		t = -tmp_val * inv_dot;
		if (t < 0)
			return false;
	}
	else
	{
		val = sqrt(val);
		float t1 = (-tmp_val + val)*inv_dot;
		float t2 = (-tmp_val - val)*inv_dot;
		if (t1 < 0 && t2 < 0)
			return false;
		else if (t1 < 0)
			t = t2;
		else if (t2 < 0)
			t = t1;
		else
			t = std::min(t1, t2);
	}
	is_info.wo = XMFLOAT3(-ray.direction.x, -ray.direction.y, -ray.direction.z);
	is_info.obj = this;
	is_info.bxdf = this->bxdf;
	is_info.pos = XMFLOAT3(ray.origin.x + t * ray.direction.x, ray.origin.y + t * ray.direction.y, ray.origin.z + t * ray.direction.z);
	is_info.normal = MathHelper::NormalizeFloat3(XMFLOAT3(is_info.pos.x - center.x, is_info.pos.y - center.y, is_info.pos.z - center.z));
	return true;
}
