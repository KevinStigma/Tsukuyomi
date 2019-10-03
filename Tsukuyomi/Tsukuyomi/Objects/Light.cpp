#include "Light.h"

Light::Light(std::string name, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, XMFLOAT3 c):Object(name, t, s, r)
{
	color = c;
}

Light::~Light()
{

}