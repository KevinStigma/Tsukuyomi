#include "TransAxis.h"
#include <assert.h>

TransAxis::TransAxis():cylinder_length(0.8), cone_length(0.2), scale(1.5)
{

}

XMMATRIX TransAxis::computeWorldMatrix(Object * obj, int axis_type)
{
	assert(axis_type >= 0 && axis_type <= 2);
	const BoundingBox &bb = obj->getBoundingBox();
	XMFLOAT3 center = bb.center;
	XMMATRIX trans_center_mat = XMMatrixTranslationFromVector(XMVector3TransformCoord(XMVectorSet(center.x, center.y, center.z, 1.0), obj->getWorldMatrix()));
	XMMATRIX scale_mat = XMMatrixScaling(scale, scale, scale);
	XMMATRIX axisTrans = XMMatrixIdentity();
	if(axis_type == 1)
		axisTrans = XMMatrixRotationZ(-MathHelper::Pi * 0.5f);
	else if(axis_type == 2)
		axisTrans = XMMatrixRotationX(MathHelper::Pi * 0.5f);
	return scale_mat * axisTrans * obj->getRotMatrix() * trans_center_mat;
}