#include "PointLight.h"

PointLight::PointLight(std::string name, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, XMFLOAT3 color):Light(name, t, s, r, color)
{
	type = POINT_LIGHT;
}

PointLight::~PointLight()
{
}

Spectrum PointLight::sample_li(const IntersectInfo & ref, XMFLOAT2 uSample, XMFLOAT3* wi, float* pdf, VisibilityTester& vt)
{
	XMFLOAT3 pos = getTranslation();
	XMFLOAT3 diff = XMFLOAT3(pos.x - ref.pos.x, pos.y - ref.pos.y, pos.z - ref.pos.z);
	*wi = MathHelper::NormalizeFloat3(diff);
	*pdf = 1.0f;
	return Spectrum(color.x, color.y, color.z) / MathHelper::DotFloat3(diff, diff);
}