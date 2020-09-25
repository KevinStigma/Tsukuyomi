#define TINYOBJLOADER_IMPLEMENTATION
#include "Object.h"
#include "../D3DRenderer.h"
#include "../GlobalSys.h"
#include "Mesh.h"
#include <iostream>

BoundingBox::BoundingBox()
{
	const float minF = (std::numeric_limits<float>::lowest)();
	const float maxF = (std::numeric_limits<float>::max)();
	bottom = XMFLOAT3(maxF, maxF, maxF);
	top = XMFLOAT3(minF, minF, minF);
}

float BoundingBox::surfaceArea()
{
	float x_extend = top.x - bottom.x;
	float y_extend = top.y - bottom.y;
	float z_extend = top.z - bottom.z;
	return (x_extend * y_extend + y_extend * z_extend + x_extend * z_extend)*2.0;
}

int BoundingBox::maximumExtent()
{
	float x_extend = top.x - bottom.x;
	float y_extend = top.y - bottom.y;
	float z_extend = top.z - bottom.z;
	float max_extend = std::max<float>(x_extend, std::max<float>(y_extend, z_extend));
	if (max_extend == x_extend)
		return 0;
	else if (max_extend == y_extend)
		return 1;
	else
		return 2;
}

float BoundingBox::getTopFromDim(int dim)const
{
	if (dim == 0)
		return top.x;
	else if (dim == 1)
		return top.y;
	else
		return top.z;
}

float BoundingBox::getBottomFromDim(int dim)const
{
	if (dim == 0)
		return bottom.x;
	else if (dim == 1)
		return bottom.y;
	else
		return bottom.z;
}

bool BoundingBox::isIntersect(const Ray&ray, XMMATRIX world_mat)
{
	XMVECTOR v;
	Ray transed_ray = ray.transform(XMMatrixInverse(&v, world_mat));
	if (inBox(transed_ray.origin))
		return true;
	float t1, t2;
	XMFLOAT3 p1, p2;
	if (abs(transed_ray.direction.x) > 0.0f)
	{
		t1 = (bottom.x - transed_ray.origin.x) / transed_ray.direction.x;
		t2 = (top.x - transed_ray.origin.x) / transed_ray.direction.x;
		p1 = transed_ray.getExtendPos(t1);
		p2 = transed_ray.getExtendPos(t2);
		p1.x = bottom.x;
		p2.x = top.x;
		if ((t1 > 0.0 &&inBox(p1)) || (t2 > 0.0 && inBox(p2)))
		{
			return true;
		}
	}

	if (abs(transed_ray.direction.y) > 0.0f)
	{
		t1 = (bottom.y - transed_ray.origin.y) / transed_ray.direction.y;
		t2 = (top.y - transed_ray.origin.y) / transed_ray.direction.y;
		p1 = transed_ray.getExtendPos(t1);
		p2 = transed_ray.getExtendPos(t2);
		p1.y = bottom.y;
		p2.y = top.y;
		if ((t1 > 0.0 &&inBox(p1)) || (t2 > 0.0 && inBox(p2)))
		{
			return true;
		}
	}

	if (abs(transed_ray.direction.z) > 0.0f)
	{
		t1 = (bottom.z - transed_ray.origin.z) / transed_ray.direction.z;
		t2 = (top.z - transed_ray.origin.z) / transed_ray.direction.z;
		p1 = transed_ray.getExtendPos(t1);
		p2 = transed_ray.getExtendPos(t2);
		p1.z = bottom.z;
		p2.z = top.z;
		if ((t1 > 0.0 &&inBox(p1)) || (t2 > 0.0 && inBox(p2)))
		{
			return true;
		}
	}
	return false;
}

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

void Object::setParent(Object* p)
{
	if (parent == p)
		return;
	if (parent)
		if (parent->childs.find(name) != childs.end())
			parent->childs.erase(name);
	parent = p;
	if(p)
		p->childs.insert(std::pair<std::string, Object*>(name, this));
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

XMMATRIX Object::getGlobalWorldMatrix()const
{
	XMMATRIX parent_matrix = XMMatrixIdentity();
	if (parent)
		parent_matrix = parent->getGlobalWorldMatrix();

	return parent_matrix * getWorldMatrix();
}

bool Object::is_intersect_bounding_box(const Ray&ray)
{
	XMMATRIX world_mat = getWorldMatrix();
	return boundingBox.isIntersect(ray, world_mat);
}
