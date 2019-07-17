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

int RotAxis::rayIntersectDectect(const Ray& ray, Object* obj, float &t)
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
	t = min_t;
	return axis_index;
}

XMFLOAT3 RotAxis::computeNormalSpherePoint(XMFLOAT2 normal_pos2d)
{
	float x = normal_pos2d.x;
	float y = normal_pos2d.y;
	float length = sqrt(x * x + y * y);
	if (length > 1.0f)
	{
		x = x / length;
		y = y / length;
	}
	float z = -sqrt(MathHelper::Clamp<float>(1.0f - x * x - y * y, 0.0f, 1.0f));
	return XMFLOAT3(x, y, z);
}

void RotAxis::rotateSelObj(const Camera& cam, XMFLOAT2 cur_normalized_pos, XMFLOAT2 last_normalized_pos, AXIS curSelAxis, Object* obj)
{
	XMFLOAT3 cur_pos = cam.unprojectCoord(cur_normalized_pos, cam.zNear * 10.0f);
	XMFLOAT3 last_pos = cam.unprojectCoord(last_normalized_pos, cam.zNear * 10.0f);
	XMFLOAT3 vec(cur_pos.x - last_pos.x, cur_pos.y-last_pos.y, cur_pos.z-last_pos.z);
	XMVECTOR inter_pt1 = XMVectorSet(intersection_pt.x, intersection_pt.y, intersection_pt.z, 1.0);
	XMVECTOR inter_pt2 = XMVectorSet(intersection_pt.x + vec.x, intersection_pt.y + vec.y, intersection_pt.z + vec.z, 1.0);

	XMMATRIX axisTrans = getAxisLocalTransform(curSelAxis);
	XMMATRIX world_mat = axisTrans * obj->getRotMatrix() * obj->getTransMatrix();
	XMVECTOR v;
	XMMATRIX inv_world_mat = XMMatrixInverse(&v, world_mat);
	inter_pt1 = XMVector3TransformCoord(inter_pt1, inv_world_mat);
	inter_pt2 = XMVector3TransformCoord(inter_pt2, inv_world_mat);
	float x = XMVectorGetX(inter_pt2);
	float z = XMVectorGetZ(inter_pt2);
 	float cur_radian = atan2(x, z);
	if (cur_radian < 0.0)
		cur_radian += MathHelper::Pi;
	x = XMVectorGetX(inter_pt1);
	z = XMVectorGetZ(inter_pt1);
	float last_radian = atan2(x, z);
	if (last_radian < 0.0)
		last_radian += MathHelper::Pi;

	XMVECTOR rot_axis = XMVector3TransformNormal(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), axisTrans * obj->getRotMatrix());

	XMFLOAT4X4 new_rot_mat;
	XMStoreFloat4x4(&new_rot_mat, obj->getRotMatrix() * XMMatrixRotationAxis(rot_axis, (cur_radian - last_radian) * 8.0f));
	XMFLOAT3 rot = MathHelper::transRotationMatrixToEulerAngles(new_rot_mat);
	obj->setRotation(rot);
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