#include "TransAxis.h"
#include "intersection.h"
#include "GlobalSys.h"
#include <assert.h>

TransAxis::TransAxis():cylinder_length(0.8), cone_length(0.2), scale(1.5), cylinder_radius(0.025), cone_radius(0.05)
{

}

XMMATRIX TransAxis::computeWorldMatrix(Object * obj, AXIS axis_type)
{
	assert(axis_type >= 0 && axis_type <= 2);
	XMMATRIX scale_mat = XMMatrixScaling(scale, scale, scale);
	XMMATRIX axisTrans = getAxisLocalTransform(axis_type);
	return scale_mat * axisTrans*obj->getBoundingBox().getTransMatrix() * obj->getGlobalWorldMatrix();
}

XMMATRIX TransAxis::getAxisLocalTransform(AXIS axis_type)
{
	if (axis_type == AXIS::X)
		return XMMatrixRotationZ(-MathHelper::Pi * 0.5f);
	else if(axis_type == AXIS::Y)
		return XMMatrixIdentity();
	else
		return XMMatrixRotationX(MathHelper::Pi * 0.5f);
}

void TransAxis::translateSelObj(XMFLOAT2 mouse_move_dir, AXIS curSelAxis)
{
	float proj_length = MathHelper::projectVector2D(mouse_move_dir, curSelAxisProjDir);
	Object* selObj = g_pGlobalSys->objectManager.getCurSelObject();
	XMFLOAT3 t = selObj->getTranslation();
	if (curSelAxis == AXIS::X)
		selObj->setTranslation(XMFLOAT3(t.x + proj_length, t.y, t.z));
	else if (curSelAxis == AXIS::Y)
		selObj->setTranslation(XMFLOAT3(t.x, t.y + proj_length, t.z));
	else if (curSelAxis == AXIS::Z)
		selObj->setTranslation(XMFLOAT3(t.x, t.y, t.z + proj_length));
}

int TransAxis::rayIntersectDectect(const Ray& ray, Object* obj)
{
	if(!obj)
		return -1;
	XMVECTOR v;
	float min_t = -1.0;
	int axis_index = -1;
	for (int i = 0; i < 3; i++)
	{
		XMMATRIX axisTrans = getAxisLocalTransform(AXIS(i));
		XMMATRIX world_mat = axisTrans * obj->getBoundingBox().getTransMatrix()* obj->getGlobalWorldMatrix();
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

void TransAxis::computeAxisDirectionProj(const Camera& cam, AXIS axis_type)
{
	if (axis_type == AXIS::NO)
	{
		curSelAxisProjDir = XMFLOAT2(0.0, 0.0);
		return;
	}
	XMVECTOR local_dir = XMVectorSet(0.0f, cylinder_length * scale, 0.0f, 0.0f);
	XMMATRIX axis_trans = getAxisLocalTransform(axis_type);
	XMVECTOR proj_dir = XMVector3TransformNormal(local_dir, axis_trans*cam.getViewProjMatrix());
	if (XMVectorGetX(XMVector2Length(proj_dir)) < 1e-6)
	{
		curSelAxisProjDir = XMFLOAT2(0.0, 0.0);
		return;
	}
	XMFLOAT2 normalized_dir;
	XMStoreFloat2(&normalized_dir, XMVector2Normalize(proj_dir));
	curSelAxisProjDir = normalized_dir;
}