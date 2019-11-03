#define TINYOBJLOADER_IMPLEMENTATION
#include "Object.h"
#include "../D3DRenderer.h"
#include "../GlobalSys.h"
#include <iostream>

Object::Object(std::string obj_name, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r):translation(t), scale(s), rotation(r)
{
	name = obj_name;
	type = ObjectType::EMPTY;
	genereateWorldMatrix();
}

Object::~Object()
{

}

bool Object::isEmpty()
{
	return true;
}


void Object::render(ID3D11DeviceContext * context, D3DRenderer* renderer)
{
}

void Object::updateTransform(XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r)
{
	translation = t;
	scale = s;
	rotation = r;
	genereateWorldMatrix();
}

void Object::setScale(XMFLOAT3 s)
{
	scale = s;
	genereateWorldMatrix();
}

void Object::setRotation(XMFLOAT3 r)
{
	rotation = r;
	genereateWorldMatrix();
}

void Object::setTranslation(XMFLOAT3 t)
{
	translation = t;
	genereateWorldMatrix();
}

std::string Object::getTranslationText()
{
	return std::to_string(translation.x) + "," + std::to_string(translation.y) + "," + std::to_string(translation.z);
}

std::string Object::getScaleText()
{
	return std::to_string(scale.x) + "," + std::to_string(scale.y) + "," + std::to_string(scale.z);
}

std::string Object::getRotationText()
{
	return std::to_string(rotation.x) + "," + std::to_string(rotation.y) + "," + std::to_string(rotation.z);
}

void Object::genereateWorldMatrix()
{
	trans_mat = XMMatrixTranslation(translation.x, translation.y, translation.z);
	rot_mat = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMFLOAT4X4 r;
	XMStoreFloat4x4(&r, rot_mat);
	scale_mat = XMMatrixScaling(scale.x, scale.y, scale.z);
	world_mat = scale_mat * rot_mat * trans_mat;
}

bool Object::is_intersect_bounding_box(const Ray&ray)
{
	XMMATRIX world_mat = getWorldMatrix();
	XMVECTOR v;
	Ray transed_ray = ray.transform(XMMatrixInverse(&v, world_mat));
	if (boundingBox.inBox(transed_ray.origin))
		return true;
	float t1, t2;
	XMFLOAT3 p1, p2;
	if (abs(transed_ray.direction.x) > 0.0f)
	{
		t1 = (boundingBox.bottom.x - transed_ray.origin.x) / transed_ray.direction.x;
		t2 = (boundingBox.top.x - transed_ray.origin.x) / transed_ray.direction.x;
		p1 = transed_ray.getExtendPos(t1);
		p2 = transed_ray.getExtendPos(t2);
		p1.x = boundingBox.bottom.x;
		p2.x = boundingBox.top.x;
		if ((t1 > 0.0 &&boundingBox.inBox(p1)) || (t2 > 0.0 && boundingBox.inBox(p2)))
			return true;
	}

	if (abs(transed_ray.direction.y) > 0.0f)
	{
		t1 = (boundingBox.bottom.y - transed_ray.origin.y) / transed_ray.direction.y;
		t2 = (boundingBox.top.y - transed_ray.origin.y) / transed_ray.direction.y;
		p1 = transed_ray.getExtendPos(t1);
		p2 = transed_ray.getExtendPos(t2);
		p1.y = boundingBox.bottom.y;
		p2.y = boundingBox.top.y;
		if ((t1 > 0.0 &&boundingBox.inBox(p1)) || (t2 > 0.0 && boundingBox.inBox(p2)))
			return true;
	}

	if (abs(transed_ray.direction.z) > 0.0f)
	{
		t1 = (boundingBox.bottom.z - transed_ray.origin.z) / transed_ray.direction.z;
		t2 = (boundingBox.top.z - transed_ray.origin.z) / transed_ray.direction.z;
		p1 = transed_ray.getExtendPos(t1);
		p2 = transed_ray.getExtendPos(t2);
		p1.z = boundingBox.bottom.z;
		p2.z = boundingBox.top.z;
		if ((t1 > 0.0 &&boundingBox.inBox(p1)) || (t2 > 0.0 && boundingBox.inBox(p2)))
			return true;
	}
	return false;
}
