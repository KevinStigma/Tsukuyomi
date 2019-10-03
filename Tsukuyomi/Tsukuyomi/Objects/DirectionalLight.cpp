#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(std::string name, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, XMFLOAT3 color) :Light(name, t, s, r, color)
{
	defualt_dir = XMFLOAT3(1.0, 0.0, 0.0);
	type = DIR_LIGHT;
}

DirectionalLight::~DirectionalLight()
{

}