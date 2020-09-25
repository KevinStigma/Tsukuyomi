#include "Triangle.h"
void Triangle::generateBoundingBox()
{
	XMVECTOR v[3];
	XMFLOAT3 world_vs[3];
	v[0] = XMLoadFloat3(&a);
	v[1] = XMLoadFloat3(&b);
	v[2] = XMLoadFloat3(&c);
	XMMATRIX world_mat = parent->getGlobalWorldMatrix();
	for (int i = 0; i < 3; i++)
	{
		v[i] = XMVector3TransformCoord(v[i], world_mat);
		XMStoreFloat3(&world_vs[i], v[i]);
	}
	boundingbox.top = boundingbox.bottom = world_vs[0];
	for (int i = 1; i <= 2; i++)
	{
		boundingbox.top = XMFLOAT3(std::max<float>(boundingbox.top.x, world_vs[i].x),
			std::max<float>(boundingbox.top.y, world_vs[i].y),
			std::max<float>(boundingbox.top.z, world_vs[i].z));
		boundingbox.bottom = XMFLOAT3(std::min<float>(boundingbox.bottom.x, world_vs[i].x),
			std::min<float>(boundingbox.bottom.y, world_vs[i].y),
			std::min<float>(boundingbox.bottom.z, world_vs[i].z));
	}
}


void Triangle::GetUVs(XMFLOAT2 uv[3]) const {
	tinyobj::mesh_t* mesh = parent->getMesh();
	if (mesh->texcoords.size()) {
		int ind1 = mesh->indices[index*3], ind2 = mesh->indices[index * 3+1], ind3= mesh->indices[index * 3+2];
		uv[0] = parent->getTriangleTexCoord(ind1);
		uv[1] = parent->getTriangleTexCoord(ind2);
		uv[2] = parent->getTriangleTexCoord(ind3);
	}
	else {
		uv[0] = XMFLOAT2(0, 0);
		uv[1] = XMFLOAT2(1, 0);
		uv[2] = XMFLOAT2(1, 1);
	}
}

bool Triangle::is_intersect(const Ray& ray, float&t, IntersectInfo& it)
{
	XMVECTOR v[3], n[3];
	XMFLOAT3 world_vs[3];
	v[0] = XMLoadFloat3(&a);
	v[1] = XMLoadFloat3(&b);
	v[2] = XMLoadFloat3(&c);

	n[0] = XMLoadFloat3(&na);
	n[1] = XMLoadFloat3(&nb);
	n[2] = XMLoadFloat3(&nc);
	XMMATRIX world_mat = parent->getGlobalWorldMatrix();
	for (int i = 0; i < 3; i++)
	{
		v[i] = XMVector3TransformCoord(v[i], world_mat);
		XMStoreFloat3(&world_vs[i], v[i]);
		n[i] = XMVector3TransformNormal(n[i], world_mat);
	}

	float beta, gama;
	if (ray.is_intersect_triangle(world_vs[0], world_vs[1], world_vs[2], t, beta, gama))
	{
		it.obj = parent;
		it.pos = ray.getExtendPos(t);
		it.wo = XMFLOAT3(-ray.direction.x, -ray.direction.y, -ray.direction.z);
		XMStoreFloat3(&it.normal, XMVector3Normalize(n[0] + (n[1] - n[0])*beta + (n[2] - n[0])*gama));

		XMFLOAT3 dpdu, dpdv;
		MathHelper::CoordinateSystem(MathHelper::NormalizeFloat3(it.normal), &dpdu, &dpdv);
		it.dpdu = dpdu;
		return true;
	}
	return false;
}
