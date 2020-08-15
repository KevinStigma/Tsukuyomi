#include "AreaLight.h"
#include "../intersect_info.h"

AreaLight::AreaLight(std::string name, std::string mesh_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, XMFLOAT3 color, RenderMats* mats):Light(name, t, s, r, color)
{
	int index = mesh_path.rfind('/');
	if (mesh_path.substr(index + 1, mesh_path.size() - index) == "sphere.obj")
		mesh = new Sphere("", mesh_path, t, s, r, this, mats);
	else
		mesh = new Mesh("", mesh_path, t, s, r, this, mats);
	type = AREA_LIGHT;
	RenderLightHelper::Material mat;
	mat.albedo = XMFLOAT3(color.x, color.y, color.z);
	mat.roughness = 1.0;
	mat.metallic = 0.0;
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
	IntersectInfo pShape = mesh->sample(ref, uSample, pdf);
	if (*pdf == 0 || (MathHelper::DistanceSquared(pShape.pos, ref.pos) == 0.0)) {
		*pdf = 0;
		return 0.f;
	}
	*wi = MathHelper::NormalizeFloat3(XMFLOAT3(pShape.pos.x - ref.pos.x, pShape.pos.y - ref.pos.y, pShape.pos.z - ref.pos.z));
	vt.setup(ref.pos, pShape.pos);
	return L(pShape, XMFLOAT3(-wi->x, -wi->y, -wi->z));
}

float AreaLight::Pdf_Li(const IntersectInfo & ref, const XMFLOAT3& wi)
{
	return mesh->Pdf(ref, wi);
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
