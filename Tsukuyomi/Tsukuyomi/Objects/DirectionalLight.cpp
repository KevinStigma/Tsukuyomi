#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(std::string name, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, XMFLOAT3 color) :Light(name, t, s, r, color)
{
	defualt_dir = XMFLOAT3(1.0, 0.0, 0.0);
	type = DIR_LIGHT;
}

DirectionalLight::~DirectionalLight()
{

}

XMFLOAT3 DirectionalLight::getWorldDir()
{
	XMMATRIX rot_mat = getLocalRotMatrix();
	XMVECTOR vec = XMVector3TransformNormal(XMVectorSet(defualt_dir.x, defualt_dir.y, defualt_dir.z, 0.0), rot_mat);
	XMFLOAT3 world_dir;
	XMStoreFloat3(&world_dir, vec);
	return world_dir;
}

Spectrum DirectionalLight::sample_li(const IntersectInfo & ref, XMFLOAT2 uSample, XMFLOAT3* wi, float* pdf, VisibilityTester& vt)
{
	*wi = MathHelper::NegativeFloat3(getWorldDir());
	*pdf = 1.0f;
	XMFLOAT3 pOutside(ref.pos.x + world_radius * 2.0 * wi->x, 
		ref.pos.y + world_radius * 2.0 * wi->y,
		ref.pos.z + world_radius * 2.0 * wi->z);
	vt.setup(ref.pos, pOutside);
	return Spectrum(color.x, color.y, color.z);
}