#pragma once
#include "Light.h"
class DirectionalLight :public Light
{
public:
	DirectionalLight(std::string name, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 color = XMFLOAT3(1.0, 1.0, 1.0));
	~DirectionalLight();
	XMFLOAT3 getWorldDir();
	virtual Spectrum sample_li(const IntersectInfo & ref, XMFLOAT2 uSample, XMFLOAT3* wi, float* pdf, VisibilityTester&  vt);
	bool isDelta() { return true; }
protected:
	XMFLOAT3 defualt_dir;
};