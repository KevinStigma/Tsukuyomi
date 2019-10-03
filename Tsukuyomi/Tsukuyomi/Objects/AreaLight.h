#pragma once
#include "Light.h"
#include "Mesh.h"
class AreaLight :public Light
{
public:
	AreaLight(std::string name, std::string mesh_path, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 color = XMFLOAT3(1.0, 1.0, 1.0));
	~AreaLight();
protected:
	Mesh* mesh = nullptr;
	XMFLOAT3 color;
};