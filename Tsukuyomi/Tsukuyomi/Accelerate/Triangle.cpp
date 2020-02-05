#include "Triangle.h"
void Triangle::generateBoundingBox()
{
	XMVECTOR v[3];
	XMFLOAT3 world_vs[3];
	v[0] = XMLoadFloat3(&a);
	v[1] = XMLoadFloat3(&b);
	v[2] = XMLoadFloat3(&c);
	XMMATRIX world_mat = parent->getWorldMatrix();
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