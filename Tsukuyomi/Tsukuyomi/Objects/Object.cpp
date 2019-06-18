#define TINYOBJLOADER_IMPLEMENTATION
#include "Object.h"
#include <iostream>

Object::Object(std::string obj_name, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT4 r):translation(t), scale(s), rotation(r)
{
	name = obj_name;
	type = ObjectType::EMPTY;
}

Object::~Object()
{

}

bool Object::isEmpty()
{
	return true;
}

void Object::setTranslation(XMFLOAT3 t)
{
	translation = t;
}

void Object::setScale(XMFLOAT3 s)
{
	scale = s;
}

void Object::setRotation(XMFLOAT4 r)
{
	rotation = r;
}

void Object::render(ID3D11DeviceContext * context, D3DRenderer* renderer)
{

}

void Object::updateTransform(XMFLOAT3 t, XMFLOAT3 s, XMFLOAT4 r)
{
	setTranslation(t);
	setRotation(r);
	setScale(s);
}

XMMATRIX Object::genereateWorldMatrix()
{
	XMMATRIX world_mat = XMMatrixTranslation(translation.x, translation.y, translation.z);
	XMMATRIX rot_mat = XMMatrixIdentity();
	XMMATRIX scale_mat = XMMatrixScaling(scale.x, scale.y, scale.z);
	return world_mat * rot_mat * scale_mat;
}
