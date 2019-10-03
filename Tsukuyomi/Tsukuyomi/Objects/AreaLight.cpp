#include "AreaLight.h"

AreaLight::AreaLight(std::string name, std::string mesh_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, XMFLOAT3 color):Light(name, t, s, r, color)
{
	mesh = new Mesh("", mesh_path, t, s, r);
}

AreaLight::~AreaLight()
{
	SAFE_DELETE(mesh);
}
