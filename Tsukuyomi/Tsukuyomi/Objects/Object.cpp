#define TINYOBJLOADER_IMPLEMENTATION
#include "Object.h"
#include "../D3DRenderer.h"
#include "../GlobalSys.h"
#include <iostream>

Object::Object(std::string obj_name, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r):translation(t), scale(s), rotation(r)
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


void Object::render(ID3D11DeviceContext * context, D3DRenderer* renderer)
{
	if (g_pGlobalSys->objectManager.getCurSelObject() == this)
	{
		renderBoundingBox(renderer);
	}
}

void Object::renderBoundingBox(D3DRenderer* renderer)
{
	renderer->renderBoundingBox(this);
}

void Object::updateTransform(XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r)
{
	translation = t;
	scale = s;
	rotation = r;
}

void Object::setScale(XMFLOAT3 s)
{
	scale = s;
}

void Object::setRotation(XMFLOAT3 r)
{
	rotation = r;
}

void Object::setTranslation(XMFLOAT3 t)
{
	translation = t;
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

XMMATRIX Object::genereateWorldMatrix()
{
	XMMATRIX world_mat = XMMatrixTranslation(translation.x, translation.y, translation.z);
	XMMATRIX rot_mat = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMMATRIX scale_mat = XMMatrixScaling(scale.x, scale.y, scale.z);
	return world_mat * rot_mat * scale_mat;
}
