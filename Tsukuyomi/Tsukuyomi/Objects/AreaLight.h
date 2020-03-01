#pragma once
#include "Light.h"
#include "Mesh.h"
#include "Sphere.h"
#include <LightHelper.h>

class IntersectInfo;

class AreaLight :public Light
{
public:
	AreaLight(std::string name, std::string mesh_path, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 color = XMFLOAT3(1.0, 1.0, 1.0));
	~AreaLight();
	void render(ID3D11DeviceContext * context, D3DRenderer* renderer);
	virtual Spectrum L(const IntersectInfo &it, const XMFLOAT3& w)const;
	virtual Spectrum sample_li(const IntersectInfo & ref, XMFLOAT2 uSample, XMFLOAT3* wi, float* pdf, VisibilityTester& vt);
	float Pdf_Li(const IntersectInfo & ref, const XMFLOAT3& wi);
	Mesh* getMesh() { return mesh; }
	void setScale(XMFLOAT3 s);
	void setRotation(XMFLOAT3 r);
	void setTranslation(XMFLOAT3 t);
	void genereateWorldMatrix();
protected:
	Mesh* mesh = nullptr;
};