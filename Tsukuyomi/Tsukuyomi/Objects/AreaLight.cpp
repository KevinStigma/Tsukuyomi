#include "AreaLight.h"
#include "../intersect_info.h"

AreaLight::AreaLight(std::string name, std::string mesh_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, XMFLOAT3 color):Light(name, t, s, r, color)
{
	int index = mesh_path.rfind('/');
	if (mesh_path.substr(index + 1, mesh_path.size() - index) == "sphere.obj")
		mesh = new Sphere("", mesh_path, t, s, r, this);
	else
		mesh = new Mesh("", mesh_path, t, s, r, this);
	type = AREA_LIGHT;
	Material mat;
	mat.Ambient = XMFLOAT4(color.x, color.y, color.z, 1.0);
	mat.Diffuse = XMFLOAT4(0.0, 0.0, 0.0, 1.0);
	mat.Specular = XMFLOAT4(0.0, 0.0, 0.0, 1.0);
	mesh->setMaterial(mat);
}

AreaLight::~AreaLight()
{
	SAFE_DELETE(mesh);
}

void AreaLight::render(ID3D11DeviceContext * context, D3DRenderer* renderer)
{
	mesh->render(context, renderer);
}

Spectrum AreaLight::L(const IntersectInfo &it, const XMFLOAT3& w)const
{
	float val = MathHelper::DotFloat3(it.normal, w);
	return val > 0.0f ? Spectrum(color.x, color.y, color.z) : Spectrum();
}

Spectrum AreaLight::sample_li(const IntersectInfo & ref, XMFLOAT2 uSample, XMFLOAT3* wi, float* pdf, VisibilityTester& vt)
{
	float area;
	IntersectInfo it = mesh->sample(uSample, area);
	*wi = MathHelper::NormalizeFloat3(XMFLOAT3(it.pos.x - ref.pos.x, it.pos.y - ref.pos.y, it.pos.z - ref.pos.z));
	vt.setup(ref.pos, it.pos);
	if (MathHelper::Float3Length(*wi) == 0.0)
		*pdf = 0.0f;
	else
	{
		*pdf = 1.0f / area;
		// Convert from area measure, as returned by the Sample() call
		// above, to solid angle measure.
		*pdf *= MathHelper::DistanceSquared(ref.pos, it.pos) / fabs(MathHelper::DotFloat3(it.normal, MathHelper::NegativeFloat3(*wi)));
		if (std::isinf(*pdf)) *pdf = 0.f;
	}

	return L(it, XMFLOAT3(-wi->x, -wi->y, -wi->z));
}

void AreaLight::setScale(XMFLOAT3 s)
{
	Light::setScale(s);
	mesh->setScale(s);
}

void AreaLight::setRotation(XMFLOAT3 r)
{
	Light::setRotation(r);
	mesh->setRotation(r);
}

void AreaLight::setTranslation(XMFLOAT3 t)
{
	Light::setTranslation(t);
	mesh->setTranslation(t);
}

void AreaLight::genereateWorldMatrix()
{
	Light::genereateWorldMatrix();
	mesh->genereateWorldMatrix();
}
