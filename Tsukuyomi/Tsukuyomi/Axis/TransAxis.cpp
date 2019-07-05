#include "TransAxis.h"
#include "intersection.h"
#include <assert.h>

TransAxis::TransAxis():cylinder_length(0.8), cone_length(0.2), scale(1.5), cylinder_radius(0.025), cone_radius(0.05)
{

}

XMMATRIX TransAxis::computeWorldMatrix(Object * obj, AXIS axis_type)
{
	assert(axis_type >= 0 && axis_type <= 2);
	const BoundingBox &bb = obj->getBoundingBox();
	XMFLOAT3 center = bb.center;
	XMMATRIX trans_center_mat = XMMatrixTranslationFromVector(XMVector3TransformCoord(XMVectorSet(center.x, center.y, center.z, 1.0), obj->getWorldMatrix()));
	XMMATRIX scale_mat = XMMatrixScaling(scale, scale, scale);
	XMMATRIX axisTrans = getAxisLocalTransform(axis_type);
	return scale_mat * axisTrans * trans_center_mat;
}

XMMATRIX TransAxis::getAxisLocalTransform(AXIS axis_type)
{
	if (axis_type == AXIS::X)
		return XMMatrixIdentity();
	else if(axis_type == AXIS::Y)
		return XMMatrixRotationZ(-MathHelper::Pi * 0.5f);
	else
		return XMMatrixRotationX(MathHelper::Pi * 0.5f);
}

int TransAxis::rayIntersectDectect(const Ray& ray, Object* obj)
{
	if(!obj)
		return -1;
	XMFLOAT3 center = obj->getBoundingBox().center;
	XMMATRIX trans_center_mat = XMMatrixTranslationFromVector(XMVector3TransformCoord(XMVectorSet(center.x, center.y, center.z, 1.0), obj->getWorldMatrix()));
	XMVECTOR v;
	float min_t = -1.0;
	int axis_index = -1;
	for (int i = 0; i < 3; i++)
	{
		XMMATRIX axisTrans = getAxisLocalTransform(AXIS(i));
		XMMATRIX world_mat = axisTrans * trans_center_mat;
		XMMATRIX inv_world_mat = XMMatrixInverse(&v, world_mat);
		Ray trans_ray = ray.transform(inv_world_mat);
		float t = rayCylinderIntersection(trans_ray, cylinder_radius * scale, cylinder_length * scale);
		if (t > 0.0 && (t < min_t||min_t<0.0))
		{
			min_t = t;
			axis_index = i;
		}
	}
	return axis_index;
}