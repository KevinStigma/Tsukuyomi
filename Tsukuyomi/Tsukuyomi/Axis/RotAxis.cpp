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
	XMMATRIX scale_mat = XMMatrixScaling(scale, scale, scale);
	XMMATRIX axisTrans = getAxisLocalTransform(axis_type);
	return scale_mat * axisTrans * obj->getRotMatrix() * obj->getTransMatrix();
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
	XMMATRIX rot_mat = obj->getRotMatrix();
	XMMATRIX local_trans_mat = XMMatrixTranslation(0.0f, -diff_radius * scale, 0.0f);
	XMVECTOR v;
	float min_t = -1.0;
	int axis_index = -1;
	for (int i = 0; i < 3; i++)
	{
		XMMATRIX axisTrans = getAxisLocalTransform(AXIS(i));
		XMMATRIX world_mat = axisTrans * rot_mat * obj->getTransMatrix();
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

XMFLOAT2 RotAxis::getAxisDirectionProj(const Camera& cam, Object*sel_obj, AXIS axis_type)
{
	XMVECTOR p1 = XMVectorSet(-large_radius * scale, 0.0f, 0.0f, 1.0f);
	XMVECTOR p2 = XMVectorSet(large_radius * scale, 0.0f, 0.0f, 1.0f);
	XMVECTOR p3 = XMVectorSet(0.0f, 0.0f, -large_radius * scale, 1.0f);
	XMVECTOR p4 = XMVectorSet(0.0f, 0.0f, large_radius * scale, 1.0f);

	XMMATRIX axis_trans = getAxisLocalTransform(axis_type);
	XMVECTOR local_dir1 = XMVectorSubtract(p1, p2);
	XMVECTOR local_dir2 = XMVectorSubtract(p3, p4);
	XMVECTOR proj_dir1 = XMVector3TransformNormal(local_dir1, axis_trans* sel_obj->getRotMatrix() * sel_obj->getTransMatrix() * cam.getViewProjMatrix());
	XMVECTOR proj_dir2 = XMVector3TransformNormal(local_dir2, axis_trans* sel_obj->getRotMatrix() * sel_obj->getTransMatrix() * cam.getViewProjMatrix());

	XMFLOAT2 normalized_dir;
	XMVECTOR long_dir;
	if (XMVectorGetX(XMVector2Length(proj_dir1)) >= XMVectorGetX(XMVector2Length(proj_dir1)))
		long_dir = proj_dir1;
	else
		long_dir = proj_dir2;
	if ((XMVectorGetX(long_dir) > XMVectorGetY(long_dir)&&XMVectorGetX(long_dir)<0.0)|| (XMVectorGetX(long_dir) <= XMVectorGetY(long_dir) && XMVectorGetY(long_dir) < 0.0))
		long_dir = XMVectorNegate(long_dir);
	XMStoreFloat2(&normalized_dir, XMVector2Normalize(long_dir));
	return normalized_dir;
}