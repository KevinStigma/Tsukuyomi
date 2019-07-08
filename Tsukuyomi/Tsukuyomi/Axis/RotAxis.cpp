#include "RotAxis.h"
#include "../intersection.h"
#include <iostream>

RotAxis::RotAxis()
{
	small_radius = 0.95f;
	large_radius = 1.0f;
	scale = 1.2f;
}


XMMATRIX RotAxis::computeWorldMatrix(Object * obj, AXIS axis_type)
{
	XMVECTOR v;
	const BoundingBox &bb = obj->getBoundingBox();
	XMFLOAT3 center = bb.center;
	XMMATRIX trans_center_mat = XMMatrixTranslationFromVector(XMVector3TransformCoord(XMVectorSet(center.x, center.y, center.z, 1.0), obj->getWorldMatrix()));
	XMMATRIX scale_mat = XMMatrixScaling(scale, scale, scale);
	XMMATRIX axisTrans = getAxisLocalTransform(axis_type);
	return scale_mat * axisTrans * obj->getRotMatrix() * trans_center_mat;
}

XMMATRIX RotAxis::getAxisLocalTransform(AXIS axis_type)
{
	if (axis_type == AXIS::X)
		return XMMatrixRotationZ(-MathHelper::Pi * 0.5f);
	else if (axis_type == AXIS::Y)
		return XMMatrixIdentity();
	else
		return XMMatrixRotationX(MathHelper::Pi * 0.5f);
}

int RotAxis::rayIntersectDectect(const Ray& ray, Object* obj)
{
	if (!obj)
		return -1;
	float diff_radius = large_radius - small_radius;
	XMFLOAT3 center = obj->getBoundingBox().center;
	XMMATRIX trans_center_mat = XMMatrixTranslationFromVector(XMVector3TransformCoord(XMVectorSet(center.x, center.y, center.z, 1.0), obj->getWorldMatrix()));
	XMMATRIX rot_mat = obj->getRotMatrix();
	XMMATRIX local_trans_mat = XMMatrixTranslation(0.0f, -diff_radius * scale, 0.0f);
	XMVECTOR v;
	float min_t = -1.0;
	int axis_index = -1;
	for (int i = 0; i < 3; i++)
	{
		XMMATRIX axisTrans = getAxisLocalTransform(AXIS(i));
		XMMATRIX world_mat = axisTrans * rot_mat * trans_center_mat;
		XMMATRIX inv_world_mat = XMMatrixInverse(&v, world_mat);
		Ray trans_ray = ray.transform(inv_world_mat);
		float t1 = rayCircleIntersection(trans_ray, large_radius * scale);
		float t2 = rayCircleIntersection(trans_ray, small_radius * scale);
		if (t1 > 0.0 && (t1 < min_t || min_t < 0.0) && (t1 > 0.0 && t2 < 0.0))
		{
			min_t = t1;
			axis_index = i;
		}

		world_mat = local_trans_mat * world_mat;
		inv_world_mat = XMMatrixInverse(&v, world_mat);
		trans_ray = ray.transform(inv_world_mat);
		t1 = rayCylinderIntersection(trans_ray, (diff_radius * 0.5f + small_radius) * scale , diff_radius*scale);
		if (t1 > 0.0 && (t1 < min_t || min_t < 0.0))
		{
			min_t = t1;
			axis_index = i;
		}
	}
	return axis_index;
}