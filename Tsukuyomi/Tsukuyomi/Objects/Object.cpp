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
	/*
	if (g_pGlobalSys->objectManager.getCurSelObject() == this)
	{
		renderer->renderBoundingBox(this);
		renderer->renderAxis(this);
	}
	*/
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
	scale_mat = XMMatrixScaling(scale.x, scale.y, scale.z);
	world_mat = scale_mat * rot_mat * trans_mat;
}
