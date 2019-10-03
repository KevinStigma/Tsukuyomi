#include "SpotLight.h"

SpotLight::SpotLight(std::string name, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, XMFLOAT3 color):Light(name, t, s, r, color)
{
}

SpotLight::~SpotLight()
{
}
